#include<stdio.h>
#include <mpi/mpi.h>
#include"stdlib.h"

// https://blog.csdn.net/weixin_42742438/article/details/83478334

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

  printf("%d send - ",rank);
  for (int i = 0; i < proc_nums; i++) {
    for(int j=0;j<block_size;j++){
      int value = rank * 10 + i;
      send_buff[i * block_size + j] = value;

      printf("%2d ",value);
    }
  }
  printf("\n");

  MPI_Alltoall(send_buff,block_size,MPI_INT,recv_buff,block_size,MPI_INT,MPI_COMM_WORLD);

  printf("%d reci - ",rank);
  for(int m=0;m<block_size*proc_nums;m++){
    printf("%2d ",recv_buff[m]);
  }
  printf("\n");

  MPI_Finalize();
  return 0;
}

// 0 send -  0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  2  2  3  3  3  3  3  3  3  3  3  3 
// 1 send - 10 10 10 10 10 10 10 10 10 10 11 11 11 11 11 11 11 11 11 11 12 12 12 12 12 12 12 12 12 12 13 13 13 13 13 13 13 13 13 13 
// 2 send - 20 20 20 20 20 20 20 20 20 20 21 21 21 21 21 21 21 21 21 21 22 22 22 22 22 22 22 22 22 22 23 23 23 23 23 23 23 23 23 23 
// 3 send - 30 30 30 30 30 30 30 30 30 30 31 31 31 31 31 31 31 31 31 31 32 32 32 32 32 32 32 32 32 32 33 33 33 33 33 33 33 33 33 33 

// 0 reci -  0  0  0  0  0  0  0  0  0  0 10 10 10 10 10 10 10 10 10 10 20 20 20 20 20 20 20 20 20 20 30 30 30 30 30 30 30 30 30 30 
// 1 reci -  1  1  1  1  1  1  1  1  1  1 11 11 11 11 11 11 11 11 11 11 21 21 21 21 21 21 21 21 21 21 31 31 31 31 31 31 31 31 31 31 
// 2 reci -  2  2  2  2  2  2  2  2  2  2 12 12 12 12 12 12 12 12 12 12 22 22 22 22 22 22 22 22 22 22 32 32 32 32 32 32 32 32 32 32
// 3 reci -  3  3  3  3  3  3  3  3  3  3 13 13 13 13 13 13 13 13 13 13 23 23 23 23 23 23 23 23 23 23 33 33 33 33 33 33 33 33 33 33 

