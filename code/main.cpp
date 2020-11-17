#include <iostream>
#include <armadillo>
#include <random>
#include "functions.h"
using namespace std;
using namespace arma;

ofstream ofile;
// #include <cmath>
// #include <iostream>
// #include <fstream>
// #include <iomanip>
// #include <cstdlib>
// #include <random>
// #include <armadillo>
// #include <string>



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

  // Declare new file name and add lattice size to file name
  string argument = to_string(NSpins);
  fileout.append(argument + "x" + argument + ".data");
  ofile.open(fileout);
  ofile << setw(15) << setprecision(8) << "Temp";
  ofile << setw(15) << setprecision(8) << "E";
  ofile << setw(15) << setprecision(8) << "C_V";
  ofile << setw(15) << setprecision(8) << "M";
  ofile << setw(15) << setprecision(8) << "Susceptibility";
  ofile << setw(15) << setprecision(8) << "M_abs" << endl;


  bool WriteLog = true;
  for (double Temp = InitialTemp; Temp <= FinalTemp; Temp+=TempStep){
    vec ExpectationValues = zeros<mat>(5);
    func.MetropolisSampling(NSpins, MCcycles, Temp, ExpectationValues, InitialTemp, WriteLog);
    func.output(NSpins, MCcycles, Temp, ExpectationValues, ofile);
  }






}
