#include<stdio.h>
#include <mpi/mpi.h>
#include"stdlib.h"
#include<math.h>
#include <unistd.h>

// https://blog.csdn.net/weixin_42742438/article/details/83478334

int main(int argc,char **argv)
{
  int proc_nums,rank;//此处使用数据的十位来表示数据来自哪个进程，使用个位表示来自哪一块，所以最大进程数目为10。
  int *send_buff;//各个进程发送缓冲区
  int *recv_buff;//各个进程的接收缓冲区
  int *sendcounts;//向各个进程发送数据数目数组
  int *recvcounts;//从各个进程接收数据数目数组
  int *sdispls;//发送偏移数组
  int *rdispls;//接收偏移数组
  int malloc_size;//各进程发送缓冲区，在本例中，各进程发送缓冲区大小相同。

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&proc_nums);


  recv_buff=(int *)malloc((rank+1)*proc_nums*sizeof(int));//各进程的接收缓冲区大小不同。
  sendcounts=(int *)malloc(proc_nums*sizeof(int));
  recvcounts=(int *)malloc(proc_nums*sizeof(int));
  rdispls=(int *)malloc(proc_nums*sizeof(int));
  sdispls=(int *)malloc(proc_nums*sizeof(int));
  int location=0;
  for(int i=0;i<proc_nums;i++){//定义数目数组与偏移数组并得到发送缓冲区大小。
    sendcounts[i]=i+1;
    recvcounts[i]=rank+1;
    rdispls[i]=i*(rank+1);
    location+=i;
    sdispls[i]=location;

     printf("%d send - %2d -> %2d : %d : %d\n",rank,rdispls[i],sdispls[i],sendcounts[i], recvcounts[i]);
  }
  malloc_size=location+proc_nums;
  send_buff=(int *)malloc(malloc_size*sizeof(int));
//发送缓冲区内容
  int start=0;
  int hu;
  printf("%d send - ",rank);
  for(int j=0;j<proc_nums;j++){    
    for(int n=0;n<j+1;n++){
      send_buff[start+n]=rank*10+j;
      hu=start+n;
      printf("%02d ",rank*10+j);
    }
    start=hu+1;
  }
  printf("\n");

  MPI_Alltoallv(send_buff,sendcounts,sdispls,MPI_INT,recv_buff,recvcounts,rdispls,MPI_INT,MPI_COMM_WORLD);
//测试
  for(int k=0;k<proc_nums;k++){
    sleep(1);
    if(rank==k){
      printf("------------------------------------------------------------\n");
      printf("processor %d 's recv_buff is:\n",k);
      for(int m=0;m<(rank+1)*proc_nums;m++){printf("%d,",recv_buff[m]);}
      printf("\n");
    }
  }
  MPI_Finalize();
  return 0;
}

// $ mpirun -np 2 ./alltoallv
// 1 send -  0 ->  0 : 1 : 2
// 1 send -  2 ->  1 : 2 : 2
// 1 send - 10 11 11
// 0 send -  0 ->  0 : 1 : 1
// 0 send -  1 ->  1 : 2 : 1
// 0 send - 00 01 01
// ------------------------------------------------------------
// processor 0 's recv_buff is:
// 0,10,
// ------------------------------------------------------------
// processor 1 's recv_buff is:
// 1,1,11,11,

