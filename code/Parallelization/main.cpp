#include <mpi.h>
#include <iostream>
#include <armadillo>
#include <random>
#include "functions.h"
#include "time.h"
using namespace std;
using namespace arma;

ofstream ofile;


//J = 1; k = 1
int main(int argc, char* argv[]){
  Functions func;
  string fileout;
  int NSpins, MCcycles;
  double InitialTemp, FinalTemp, TempStep;

  //   MPI initializations
  int numprocs, my_rank;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs); //total number of processors
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); //rank from 0 to numprocs-1


  //Reading from command line
  if (argc <= 6) {
    cout << "Bad Usage: " << argv[0] <<
      " reads: Output filename, number of spins, MC cycles, initial and final temperature and tempurate step" << endl;
    exit(1);
  }

  fileout=argv[1];
  NSpins = atoi(argv[2]);
  MCcycles = atoi(argv[3])/numprocs;
  InitialTemp = atof(argv[4]);
  FinalTemp = atof(argv[5]);
  TempStep = atof(argv[6]);


  // Initialize spin matrix
  mat spin_matrix = zeros<mat>(NSpins + 2, NSpins + 2);
  if (my_rank == 0){
    // Set up configuration and run MC for equilibrium
    func.DefaultConfig(spin_matrix, InitialTemp);
    func.Stabilize(spin_matrix, InitialTemp, 100000);
    // func.LoadConfig(spin_matrix, filename);
  }

  //Send spin_matrix to all nodes
  MPI_Bcast(spin_matrix.begin(), spin_matrix.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);


  double equilibrium_pct = 0.01;
  clock_t start, finish; //Intialize clock for timing
  for (double Temp = InitialTemp; Temp <= FinalTemp; Temp+=TempStep){
    if (my_rank == 0){ start = clock(); } //Start timer

    vec local_ExpectationValues = zeros<mat>(5);
    vec total_ExpectationValues = zeros<mat>(5);
    func.MetropolisSampling(spin_matrix, MCcycles, Temp, local_ExpectationValues, InitialTemp, equilibrium_pct);

    for (int i = 0; i < 5; i++){ //Sum up results
      MPI_Reduce(&local_ExpectationValues(i), &total_ExpectationValues(i), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD); //Make sure all nodes are done
    if (my_rank == 0){ //Let node 0 write averaged result
      //Timing
      finish = clock(); //End timer
      double timeused = (double) (finish - start)/(CLOCKS_PER_SEC );
      // func.WriteTime(NSpins, atoi(argv[3]), Temp, timeused, fileout);

      //Write to file
      func.output(numprocs*MCcycles*(1.0-equilibrium_pct), Temp, InitialTemp, total_ExpectationValues, NSpins, fileout);
      cout << "\r" << "T = "<< Temp << "/" << FinalTemp << " Time used: " << timeused  << flush;
    }
  }
  if (my_rank == 0){
    cout << " "<<endl;
  }


  // func.SaveConfig(spin_matrix, fileout);
  MPI_Finalize (); // End MPI


}
