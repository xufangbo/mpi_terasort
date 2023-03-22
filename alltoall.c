#include<stdio.h>
#include <mpi/mpi.h>
#include"stdlib.h"

int main(int argc,char **argv)
{
  int proc_nums,rank;
  int block_size=10;//block_size<=10

  int *send_buff;
  int *recv_buff;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&proc_nums);

  send_buff=(int *)malloc(block_size*proc_nums*sizeof(int));
  recv_buff=(int *)malloc(block_size*proc_nums*sizeof(int));

  printf("%d - ",rank);
  for (int i = 0; i < proc_nums; i++) {
    for(int j=0;j<block_size;j++){
      int value = rank * 10 + i;
      send_buff[i * block_size + j] = value;

      printf("%d ",value);
    }
  }
  printf("\n");

  MPI_Alltoall(send_buff,block_size,MPI_INT,recv_buff,block_size,MPI_INT,MPI_COMM_WORLD);

  if(rank==0)
  {
    printf("\n");
    for(int m=0;m<block_size*proc_nums;m++){
      printf("%d,",recv_buff[m]);
    }
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}

