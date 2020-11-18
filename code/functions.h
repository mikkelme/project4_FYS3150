#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <random>
#include <armadillo>
using namespace arma;
using namespace std;



class Functions
{
public:
  void ShowMatrix(mat &A);
  void DefaultConfig(mat &A, double &Temp);
  void BoundaryConditions(mat &A);
  void Compute_E_and_M(mat &A, double &E, double &M);
  void MetropolisSampling(mat &spin_matrix, int MCcycles, double Temp, vec &ExpectationValues, double InitialTemp, bool WriteLog, string fileout);
  void output(int NSpins, int MCcycles, double Temp, vec ExpectationValues, ofstream& ofile);
  void LoadConfig(mat &spin_matrix, string filename);
  void SaveConfig(mat &spin_matrix, string fileout);


private:
};

#endif //FUNCTIONS_H
