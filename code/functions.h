#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <armadillo>
using namespace arma;
using namespace std;



class Functions
{
public:
  void ShowMatrix(mat &A);
  void Initialize(mat &A, double &temp, double &E, double &M);
  void Metropolis(mat &A, int L, vec &w, double &E, double &M);


private:
};

#endif //FUNCTIONS_H
