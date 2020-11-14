#include <iostream>
#include <armadillo>
#include <random>
#include "functions.h"
using namespace std;
using namespace arma;

//J = 1; k = 1




int main(int argc, char* argv[]){
  Functions func;


  //Read initial values
  //2 x 2 case:



  double temp = 1;
  int L = 2;


  mat spin_matrix = zeros<mat>(L + 2, L + 2); //include space for boundary condidtions
  double E = 0; double M = 0;
  func.Initialize(spin_matrix, temp, E, M);
  // func.ShowMatrix(spin_matrix);














  //---Metropolis using Mersienne RNG (twister)-----//
  //Initialize the seed and call the Mersienne algo
  vec w = zeros<vec>(17);
  for (int de = -8; de <= 8; de+=4) w[de+8] = exp(-de/temp);

  func.Metropolis(spin_matrix, L, w, E, M);
  func.ShowMatrix(spin_matrix);





  //Run Monte Carlo computation
  //for (int cycles = 1; cycles <= mcs; cycles++){
    //Metropolis
    //update expectation values



}
