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


void Functions::MetropolisSampling(mat &spin_matrix, int MCcycles, double Temp, vec &ExpectationValues, double InitialTemp, bool WriteLog, string fileout){
  // Runs Metropolis for given number of MC cycles
  ofstream ofile;

  int NSpins = size(spin_matrix)[0] - 2;
  vec ExpectationValues_log = zeros<mat>(5);
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
  double E = 0; double M = 0;
  Compute_E_and_M(spin_matrix, E, M);

  // Precalculate possible energy differences
  vec EnergyDifference = zeros<vec>(17);
  for (int de = -8; de <= 8; de+=4) EnergyDifference[de+8] = exp(-de/Temp);

  // Initialize the seed and call the Mersenne algo for random uniform distribution
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);


  int accepted_flips = 0;
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
        accepted_flips += 1;

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
    WriteEnergyState(E, cycles, NSpins, Temp, fileout);
    ExpectationValues(0) += E;
    ExpectationValues(1) += E*E;
    ExpectationValues(2) += M;
    ExpectationValues(3) += M*M;
    ExpectationValues(4) += fabs(M);


    // Write log (values for each cycle)
    if (WriteLog || Temp == InitialTemp){
      ExpectationValues_log(0) += E;
      ExpectationValues_log(1) += E*E;
      ExpectationValues_log(2) += M;
      ExpectationValues_log(3) += M*M;
      ExpectationValues_log(4) += fabs(M);

      double norm = 1.0/((double) (cycles));  // divided by  number of cycles
      double E = ExpectationValues_log(0)*norm;
      double EE = ExpectationValues_log(1)*norm;
      double M = ExpectationValues_log(2)*norm;
      double MM = ExpectationValues_log(3)*norm;
      double M_abs = ExpectationValues_log(4)*norm;
      ofile << setw(15) << setprecision(8) << cycles;
      ofile << setw(15) << setprecision(8) << E;
      ofile << setw(15) << setprecision(8) << EE;
      ofile << setw(15) << setprecision(8) << M;
      ofile << setw(15) << setprecision(8) << MM;
      ofile << setw(15) << setprecision(8) << M_abs << endl;
    }
  }
  WriteAcceptedFlips(accepted_flips, Temp, InitialTemp, MCcycles, NSpins, fileout);
} // end of Metropolis sampling over spins


void Functions::output(int MCcycles, double Temp, vec ExpectationValues, ofstream& ofile){
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


void Functions::WriteAcceptedFlips(int accepted_flips, double Temp, double InitialTemp, int MCcycles, int NSpins, string fileout){
  ofstream outflip;
  string output_file = fileout + "_flipped.txt";
  if (Temp == InitialTemp){
    outflip.open(output_file, ios::out);}
  else{
    outflip.open(output_file, ios::out | ios::app);
  }
  outflip << setw(15) << setprecision(8) << Temp;
  outflip << setw(15) << setprecision(8) << float(accepted_flips)/MCcycles/NSpins/NSpins << endl;
  outflip.close();
}

void Functions::WriteEnergyState(double E, int cycles, int NSpins, double Temp, string fileout){
  ofstream outstate;
  string output_file = fileout + "_EnergyStates.txt";
  if (cycles == 1){
    outstate.open(output_file, ios::out);
    outstate << "Temp= " << Temp << endl;
    outstate << "NSpins= " << NSpins << endl;
  }
  else{
    outstate.open(output_file, ios::out | ios::app);
  }
  outstate << setw(15) << setprecision(8) << cycles;
  outstate << setw(15) << setprecision(8) << E << endl;
  outstate.close();
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
