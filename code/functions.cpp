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


  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
      if (Temp < 1.5){ // Set all spins to up = 1
        A(i,j) = 1;
      }
      else { // Set spins randomly up er down
        A(i,j) =(int)(RandomNumberGenerator(gen)*2) * 2 - 1;
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
  int n = size(A)[0] - 2;
  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
    E += -A(i,j)*(A(i,j+1) + A(i+1,j));
    M += A(i,j);
    }
  }
}



void Functions::MetropolisSampling(mat &spin_matrix, int MCcycles, double Temp, vec &ExpectationValues, double InitialTemp, bool WriteLog, string fileout){
  ofstream ofile;
  int NSpins = size(spin_matrix)[0] - 2;


  if (WriteLog || Temp == InitialTemp){
    string argument = to_string(NSpins);
    ofile.open(fileout + argument + "x" + argument + "_log.txt");
    ofile << "#Info_len = 3\n#MCcycles " << MCcycles << "\n#Nspins = " << NSpins << "\n#Temp = " << Temp << endl;
    ofile << setw(15) << setprecision(8) << "cycles";
    ofile << setw(15) << setprecision(8) << "E";
    ofile << setw(15) << setprecision(8) << "EE";
    ofile << setw(15) << setprecision(8) << "M";
    ofile << setw(15) << setprecision(8) << "MM";
    ofile << setw(15) << setprecision(8) << "M_abs" << endl;
  }


  // Initialize spin matrix, energy and magnetization
  // mat spin_matrix = zeros<mat>(NSpins + 2, NSpins + 2);
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

    if (WriteLog || Temp == InitialTemp){
      double norm = 1.0/((double) (cycles));  // divided by  number of cycles
      double E = ExpectationValues(0)*norm;
      double EE = ExpectationValues(1)*norm;
      double M = ExpectationValues(2)*norm;
      double MM = ExpectationValues(3)*norm;
      double M_abs = ExpectationValues(4)*norm;
      ofile << setw(15) << setprecision(8) << cycles;
      ofile << setw(15) << setprecision(8) << E;
      ofile << setw(15) << setprecision(8) << EE;
      ofile << setw(15) << setprecision(8) << M;
      ofile << setw(15) << setprecision(8) << MM;
      ofile << setw(15) << setprecision(8) << M_abs << endl;
    }

  }
} // end of Metropolis sampling over spins



void Functions::output(int NSpins, int MCcycles, double Temp, vec ExpectationValues, ofstream& ofile){
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

} // end output function


void Functions::LoadConfig(mat &A, string filename){
  string line;
  ifstream infile (filename);


  int NSpins = size(A)[0] - 2;
  for (int j = 1; j < NSpins + 1; j++){
    getline(infile, line);
    istringstream iss (line);
    vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
    for (int i = 1; i < NSpins + 1; i++){
      A(j,i) = stoi(results[i-1]);
    }
  }
  infile.close();
  BoundaryConditions(A);
}


void Functions::SaveConfig(mat &A, string fileout){
  ofstream ofile;
  int NSpins = size(A)[0] - 2;
  string argument = to_string(NSpins);
  ofile.open(fileout + argument + "x" + argument + "_config.txt");
  for (int j = 1; j < NSpins + 1; j++){
    for (int i = 1; i < NSpins + 1; i++){
      ofile << setw(2) << A(j,i);
    }
    ofile << endl;
  }
  ofile.close();
}



//
