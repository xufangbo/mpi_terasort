#include <mpi/mpi.h>
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv) {
  int P, rank;
  int send[10];
  

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  

  printf("%d send - ", rank);
  for (int i = 0; i < 10; i++) {
    send[i] = i + rank * 10;
    printf("%02d ", send[i]);
  }
  printf("\n");

  // for (int i = 0; i < P; i++) {
  //   recv = (int*)malloc(P * 10 * sizeof(int));
  // }

  int *recv = (int *)calloc(10 * P,sizeof(int));

  MPI_Allgather(send, 10, MPI_INT, recv, 10, MPI_INT, MPI_COMM_WORLD);

  printf("%d reci - ", rank);
  for (int i = 0; i < P * 10; i++) {
    printf("%02d ", recv[i]);
  }
  printf("\n");

  MPI_Finalize();
  return 0;
}

// $ mpirun -np 2 ./allgather
// 0 send - 00 01 02 03 04 05 06 07 08 09 
// 1 send - 10 11 12 13 14 15 16 17 18 19 
// 0 reci - 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 
// 1 reci - 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19