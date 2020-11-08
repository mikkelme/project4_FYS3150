#include <iostream>
#include <armadillo>
using namespace std;
using namespace arma;


void ShowMatrix(mat A){
  //Prints a matrix
  int ni = size(A)[0];
  int nj = size(A)[1];
  for (int i = 0; i < ni; i++){
    for (int j = 0; j < nj; j++){
        cout << setw(4) << A(i,j) << " ";
    }
    cout << endl;
  }
}


int main(int argc, char* argv[]){



  //Read initial values
  //2 x 2 case:
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
  spin_matrix(1,1) = -1; spin_matrix(1,2) = -1;
  spin_matrix(2,1) = -1; spin_matrix(2,2) = -1;

  //Deploy ghost spins (boundary condidtions)
  for (int i = 1; i < n-1; i++){
    spin_matrix(i,0) = spin_matrix(i, n-2);
    spin_matrix(i,n-1) = spin_matrix(i, 1);
    spin_matrix(0,i) = spin_matrix(n-2,i);
    spin_matrix(n-1,i) = spin_matrix(1, i);
  }

  ShowMatrix(spin_matrix);


  //Initialize energy and magnetization
  int E = 0;
  int M = 0;
  for (int i = 1; i < n-1; i++){
    for (int j = 1; j < n-1; j++){
      E += -spin_matrix(i,j)*(spin_matrix(i,j+1) + spin_matrix(i+1,j));
      M += spin_matrix(i,j);
    }
  }
  cout << E << endl;
  cout << M << endl;




  //Run Monte Carlo computation
  //for (int cycles = 1; cycles <= mcs; cycles++){
    //Metropolis
    //update expectation values



}
