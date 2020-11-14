#include "functions.h"
#include <random>


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


void Functions::Initialize(mat &A, double &temp, double &E, double &M){
  int n = size(A)[0];
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);

  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
      if (temp < 1.5){ // Set all spins to up = 1
        A(i,j) = 1;
      }
      else { // Set spins randomly up er down
        A(i,j) =(int)(RandomNumberGenerator(gen)*2) * 2 - 1;
      }
      // Compute energy and magnetization
        E += -A(i,j)*(A(i,j+1) + A(i+1,j));
        M += A(i,j);
    }
  }

  // Deploy boundary condidtions (ghost spins)
  for (int k = 1; k < n-1; k++){
    A(k,0)    = A(k, n-2);
    A(k,n-1)  = A(k, 1);
    A(0,k)    = A(n-2,k);
    A(n-1,k)  = A(1, k);
  }
}



void Functions::Metropolis(mat &A, int L, vec &w, double &E, double &M){
  random_device rd;
  mt19937_64 gen(rd());
  //Set up the uniform distribution for x in [0,1]
  uniform_real_distribution<double> RandomNumberGenerator(0.0, 1.0);

  for (int i = 0; i < L*L; i++){
    int iy = 1 + (int) (RandomNumberGenerator(gen)*L);
    int ix = 1 + (int) (RandomNumberGenerator(gen)*L);
    //Find delta Energy
    int deltaE = 2*A(iy,ix)* (
                  A(iy+1,ix) +
                  A(iy-1,ix) +
                  A(iy,ix+1) +
                  A(iy,ix-1) );
    //Metropolis test
    if (RandomNumberGenerator(gen) <= w[deltaE+8]) {
      A(iy,ix) *= -1; //Flip spin and accept new config

      //Update boundary conditions (ghost spin)
      A(0,ix) = A(L,ix);
      A(L+1,ix) = A(1,ix);
      A(iy,0) = A(iy,L);
      A(iy,L+1) = A(iy,1);

      // if (iy == 1) A(L+1,ix) *= -1;
      // else if (iy == L) A(0,ix) *= -1;
      // if (ix == 1) A(iy,L+1) *= -1;
      // else if (ix == L) A(iy,0) *= -1;

      //update energy and magnetization
      E += (double) deltaE;
      M += (double) 2*A(iy,ix);

    }
  }

}
