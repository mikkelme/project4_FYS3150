#include <iostream>
#include <armadillo>
#include <random>
using namespace std;
using namespace arma;

//J = 1; k = 1

void ShowMatrix(mat A){
  //Prints a matrix
  int nj = size(A)[0];
  int ni = size(A)[1];
  for (int j = 0; j < nj; j++){
    for (int i = 0; i < ni; i++){
        cout << setw(4) << A(j,i) << " ";
    }
    cout << endl;
  }
}


int main(int argc, char* argv[]){



  //Read initial values
  //2 x 2 case:
  double temp = 3.0;

  int L = 2;
  int n = L + 2;
  mat spin_matrix = zeros<mat>(n, n); //include space for boundary condidtions

  //Chose starting state all up
  /*
  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
      spin_matrix(i,j) = 1;
    }
  }
  */
  spin_matrix(1,1) = 1; spin_matrix(1,2) = 1;
  spin_matrix(2,1) = 1; spin_matrix(2,2) = 1;

  //Deploy ghost spins (boundary condidtions)
  for (int k = 1; k < n-1; k++){
    spin_matrix(k,0) = spin_matrix(k, n-2);
    spin_matrix(k,n-1) = spin_matrix(k, 1);
    spin_matrix(0,k) = spin_matrix(n-2,k);
    spin_matrix(n-1,k) = spin_matrix(1, k);
  }

  ShowMatrix(spin_matrix);


  //Initialize energy and magnetization
  double E = 0;
  double M = 0;
  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
      E += -spin_matrix(i,j)*(spin_matrix(i,j+1) + spin_matrix(i+1,j));
      M += spin_matrix(i,j);
    }
  }
  // cout << E << endl;
  // cout << M << endl;


  //---Metropolis using Mersienne RNG (twister)-----//
  //Initialize the seed and call the Mersienne algo

  vec w = zeros<vec>(17);
  for (int de = -8; de <= 8; de+=4) w[de+8] = exp(-de/temp);
  // for (int de = -8; de <= 8; de++) cout << w[de +8] << endl;




  random_device rd;
  mt19937_64 gen(rd());
  //Set up the uniform distribution for x in [0,1]
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);

  for (int i = 0; i < L*L; i++){
    int iy = 1 + (int) (RandomNumberGenerator(gen)*L);
    int ix = 1 + (int) (RandomNumberGenerator(gen)*L);
    //Find delta Energy
    int deltaE = 2*spin_matrix(iy,ix)* (
                  spin_matrix(iy+1,ix) +
                  spin_matrix(iy-1,ix) +
                  spin_matrix(iy,ix+1) +
                  spin_matrix(iy,ix-1) );
    //Metropolis test
    if (RandomNumberGenerator(gen) <= w[deltaE+8]) {
      spin_matrix(iy,ix) *= -1; //Flip spin and accept new config

      //Update boundary conditions (ghost spin)
      if (iy == 1) spin_matrix(L+1,ix) *= -1;
      else if (iy == L) spin_matrix(0,ix) *= -1;
      if (ix == 1) spin_matrix(iy,L+1) *= -1;
      else if (ix == L) spin_matrix(iy,0) *= -1;

      //update energy and magnetization
      E += (double) deltaE;
      M += (double) 2*spin_matrix(iy,ix);

    }
  }

  // ShowMatrix(spin_matrix);




  //Run Monte Carlo computation
  //for (int cycles = 1; cycles <= mcs; cycles++){
    //Metropolis
    //update expectation values



}
