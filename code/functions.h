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
  void Initialize(mat &A, double &temp, double &E, double &M);
  void MetropolisSampling(int L, int MCcycles, double Temp, vec &ExpectationValues, double InitialTemp, bool WriteLog);
  void output(int NSpins, int MCcycles, double Temp, vec ExpectationValues, ofstream& ofile);


private:
};

#endif //FUNCTIONS_H
