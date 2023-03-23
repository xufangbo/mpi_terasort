#include <mpi/mpi.h>
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv) {
  int P, rank;
  int send[10];
  int* recv;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("%d send - ",rank);
  for (int i = 0; i < 10; i++) {
    send[i] = i + rank * 10;
    printf("%02d ", send[i]);
  }
  printf("\n");

  if (rank == 0) {
    recv = (int*)malloc(P * 10 * sizeof(int));
  }
  MPI_Gather(send, 10, MPI_INT, recv, 10, MPI_INT, 0, MPI_COMM_WORLD);

  printf("%d reci - ",rank);
  for (int i = 0; i < P * 10; i++) {
    printf("%02d ", recv[i]);
  }
  printf("\n");

  MPI_Finalize();
  return 0;
}