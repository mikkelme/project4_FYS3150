#include <iostream>
#include <armadillo>
#include <random>
#include "functions.h"
using namespace std;
using namespace arma;

ofstream ofile;


//J = 1; k = 1
int main(int argc, char* argv[]){
  Functions func;

  string fileout;
  int NSpins, MCcycles;
  double InitialTemp, FinalTemp, TempStep;
  if (argc <= 5) {
    cout << "Bad Usage: " << argv[0] <<
      " reads: Output filename, number of spins, MC cycles, initial and final temperature and tempurate step" << endl;
    exit(1);
  }

  fileout=argv[1];
  NSpins = atoi(argv[2]);
  MCcycles = atoi(argv[3]);
  InitialTemp = atof(argv[4]);
  FinalTemp = atof(argv[5]);
  TempStep = atof(argv[6]);
  bool WriteLog = false;
  if (argc >= 6) {
    if (string(argv[7]) == "log") {
      WriteLog = true;
    }
  }

  // Declare new file name and add lattice size to file name
  string argument = to_string(NSpins);
  ofile.open(fileout + argument + "x" + argument + "_dump.txt");
  ofile << setw(15) << setprecision(8) << "Temp";
  ofile << setw(15) << setprecision(8) << "E";
  ofile << setw(15) << setprecision(8) << "EE";
  ofile << setw(15) << setprecision(8) << "M";
  ofile << setw(15) << setprecision(8) << "MM";
  ofile << setw(15) << setprecision(8) << "M_abs" << endl;


  // Initialize spin matrix
  mat spin_matrix = zeros<mat>(NSpins + 2, NSpins + 2);
  func.DefaultConfig(spin_matrix, InitialTemp);

  //Or begin from state declared by inputfile
  string filename = "bm2x2_config.txt";
  // func.LoadConfig(spin_matrix, filename);


  //

  for (double Temp = InitialTemp; Temp <= FinalTemp; Temp+=TempStep){
    vec ExpectationValues = zeros<mat>(5);
    func.MetropolisSampling(spin_matrix, MCcycles, Temp, ExpectationValues, InitialTemp, WriteLog, fileout);
    func.output(MCcycles, Temp, ExpectationValues, ofile);
  }
  // func.SaveConfig(spin_matrix, fileout);




}
