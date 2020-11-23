#include "functions.h"
#include <iostream>



void Functions::ShowMatrix(mat &A){
  // Prints matrix A
  int nj = size(A)[0];
  int ni = size(A)[1];
  for (int j = 0; j < nj; j++){
    for (int i = 0; i < ni; i++){
        cout << setw(4) << A(j,i) << " ";
    }
    cout << endl;
  }
}

void Functions::DefaultConfig(mat &A, double &Temp){
  //For temp < 1.5 => all spins up
  //else => random spins
  int n = size(A)[0];
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);
  for (int j = 1; j < n-1; j++){
    for (int i = 1; i < n-1; i++){
      if (Temp < 1.5){ // Set all spins to up = 1
        A(j,i) = 1;
      }
      else { // Set spins randomly up er down
        A(j,i) =(int)(RandomNumberGenerator(gen)*2) * 2 - 1;
      }
    }
  }

  BoundaryConditions(A);

}

void Functions::BoundaryConditions(mat &A){
  // Deploy boundary condidtions (ghost spins)
  int n = size(A)[0];
  for (int k = 1; k < n-1; k++){
    A(k,0)    = A(k, n-2);
    A(k,n-1)  = A(k, 1);
    A(0,k)    = A(n-2,k);
    A(n-1,k)  = A(1, k);
  }
}

void Functions::Compute_E_and_M(mat &A, double &E, double &M){
  // Compute energy and magnetization
  int n = size(A)[0];
  for (int j = 1; j < n-1; j++){
    for (int i = 1; i < n-1; i++){
      E += -A(j,i)*(A(j,i+1) + A(j+1,i));
      M += A(j,i);
    }
  }
}

void Functions::Stabilize(mat &spin_matrix, double Temp, int NumCycles){
  int NSpins = size(spin_matrix)[0] - 2;
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);

  // Precalculate possible energy differences
  vec EnergyDifference = zeros<vec>(17);
  for (int de = -8; de <= 8; de+=4) EnergyDifference[de+8] = exp(-de/Temp);

  for (int cycles = 1; cycles <= NumCycles; cycles++){
    //Sweap over lattice for NSpins^2 random positions
    for (int i = 0; i < NSpins*NSpins; i++){
      int iy = 1 + (int) (RandomNumberGenerator(gen)*NSpins);
      int ix = 1 + (int) (RandomNumberGenerator(gen)*NSpins);
      //Find delta Energy
      int deltaE = 2*spin_matrix(iy,ix)* (
                     spin_matrix(iy+1,ix) +
                     spin_matrix(iy-1,ix) +
                     spin_matrix(iy,ix+1) +
                     spin_matrix(iy,ix-1)  );
      //Metropolis test
      if (RandomNumberGenerator(gen) <= EnergyDifference[deltaE+8]) {
        spin_matrix(iy,ix) *= -1; // Accept new config by flipping spin

        // Update boundary conditions (ghost spin)
        spin_matrix(0,ix) = spin_matrix(NSpins,ix);
        spin_matrix(NSpins+1,ix) = spin_matrix(1,ix);
        spin_matrix(iy,0) = spin_matrix(iy,NSpins);
        spin_matrix(iy,NSpins+1) = spin_matrix(iy,1);
      }
    }
  }
}

void Functions::MetropolisSampling(mat &spin_matrix, int MCcycles, double Temp, vec &ExpectationValues, double InitialTemp, double equilibrium_pct){
  // Runs Metropolis for given number of MC cycles
  int NSpins = size(spin_matrix)[0] - 2;

  // Reach equilibrium (safety measure)
  int equilibrium_cycles = MCcycles*equilibrium_pct; //thrwow away first procent of cycles
  Stabilize(spin_matrix, Temp, equilibrium_cycles);
  MCcycles -= equilibrium_cycles;

  // Initialize energy and magnetization
  double E = 0; double M = 0;
  Compute_E_and_M(spin_matrix, E, M);

  // Precalculate possible energy differences
  vec EnergyDifference = zeros<vec>(17);
  for (int de = -8; de <= 8; de+=4) EnergyDifference[de+8] = exp(-de/Temp);

  // Initialize the seed and call the Mersenne algo for random uniform distribution
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);


  // Perform Metropolis sampling
  for (int cycles = 1; cycles <= MCcycles; cycles++){
    //Sweap over lattice for NSpins^2 random positions
    for (int i = 0; i < NSpins*NSpins; i++){
      int iy = 1 + (int) (RandomNumberGenerator(gen)*NSpins);
      int ix = 1 + (int) (RandomNumberGenerator(gen)*NSpins);
      //Find delta Energy
      int deltaE = 2*spin_matrix(iy,ix)* (
                     spin_matrix(iy+1,ix) +
                     spin_matrix(iy-1,ix) +
                     spin_matrix(iy,ix+1) +
                     spin_matrix(iy,ix-1)  );
      //Metropolis test
      if (RandomNumberGenerator(gen) <= EnergyDifference[deltaE+8]) {
        spin_matrix(iy,ix) *= -1; // Accept new config by flipping spin


        // Update boundary conditions (ghost spin)
        spin_matrix(0,ix) = spin_matrix(NSpins,ix);
        spin_matrix(NSpins+1,ix) = spin_matrix(1,ix);
        spin_matrix(iy,0) = spin_matrix(iy,NSpins);
        spin_matrix(iy,NSpins+1) = spin_matrix(iy,1);

        // Update energy and magnetization
        E += (double) deltaE;
        M += (double) 2*spin_matrix(iy,ix);
      }
    }
    // update expectation values for local node
    ExpectationValues(0) += E;
    ExpectationValues(1) += E*E;
    ExpectationValues(2) += M;
    ExpectationValues(3) += M*M;
    ExpectationValues(4) += fabs(M);
  }
} // end of Metropolis sampling over spins


void Functions::output(int MCcycles, double Temp, double InitialTemp, vec ExpectationValues, int NSpins, string fileout){
  ofstream ofile;
  string argument = to_string(NSpins);
  string output_file = fileout + argument + "x" + argument + "_dump.txt";
  if (Temp == InitialTemp){
    ofile.open(output_file, ios::out);
    ofile << "MCcycles= " << MCcycles << endl;
    ofile << setw(15) << setprecision(8) << "Temp";
    ofile << setw(15) << setprecision(8) << "E";
    ofile << setw(15) << setprecision(8) << "EE";
    ofile << setw(15) << setprecision(8) << "M";
    ofile << setw(15) << setprecision(8) << "MM";
    ofile << setw(15) << setprecision(8) << "M_abs" << endl;
  }
  else {
    ofile.open(output_file, ios::out | ios::app);
  }

  double norm = 1.0/((double) (MCcycles));  // divided by  number of cycles
  double E = ExpectationValues(0)*norm;
  double EE = ExpectationValues(1)*norm;
  double M = ExpectationValues(2)*norm;
  double MM = ExpectationValues(3)*norm;
  double M_abs = ExpectationValues(4)*norm;
  ofile << setw(15) << setprecision(8) << Temp;
  ofile << setw(15) << setprecision(8) << E;
  ofile << setw(15) << setprecision(8) << EE;
  ofile << setw(15) << setprecision(8) << M;
  ofile << setw(15) << setprecision(8) << MM;
  ofile << setw(15) << setprecision(8) << M_abs << endl;
  ofile.close();
} // end output function


void Functions::WriteTime(int NSpins, int MCcyles, double Temp, double timeused, string fileout){
  ofstream ofile;
  string output_file = fileout + "_timing.txt";
  ofile.open(output_file, ios::out | ios::app);
  ofile << setw(15) << setprecision(8) << NSpins*NSpins; //Total number of spins
  ofile << setw(15) << setprecision(8) << MCcyles;
  ofile << setw(15) << setprecision(8) << Temp;
  ofile << setw(15) << setprecision(8) << timeused << endl;
  ofile.close();
}



void Functions::LoadConfig(mat &A, string filename){
  string line;
  ifstream infile (filename);

  int n = size(A)[0];
  for (int j = 1; j < n-1; j++){
    getline(infile, line);
    istringstream iss (line);
    vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
    for (int i = 1; i < n-1; i++){
      A(j,i) = stoi(results[i-1]);
    }
  }
  infile.close();
  BoundaryConditions(A);
}


void Functions::SaveConfig(mat &A, string fileout){
  ofstream ofile;
  int n = size(A)[0];
  string argument = to_string(n-2);
  ofile.open(fileout + argument + "x" + argument + "_LastConfig.txt");
  for (int j = 1; j < n-1; j++){
    for (int i = 1; i < n-1; i++){
      ofile << setw(2) << A(j,i);
    }
    ofile << endl;
  }
  ofile.close();
}



//
