#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <limits.h>
#include <omp.h>
#include <time.h>

#include "terarec.h"

#define ZERO ' '
// 95
#define BASE ((unsigned long)('~' - ' ' + 1))
// 49 * 95
#define LENGTH ((unsigned long)('P' - ' ' + 1)) * ((unsigned long)('~' - ' ' + 1))
//* (( unsigned long ) ('~' - ' ' + 1))

void memcheck(void* ptr, int i) {
  if (ptr == NULL) {
    printf("%d: NULL", i);
    fflush(stdout);
  }
}

void terasort(terarec_t* local_data, int local_len, terarec_t** sorted_data, int* sorted_counts, long* sorted_displs) {
  int rank, P;
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  unsigned block_size = (LENGTH / P + 1);  // block_size
  unsigned length_padded = block_size * P;

  printf("rank %d - blocksize: %d, length_padded: %d\n", rank, block_size, length_padded);

  unsigned* send_bufers = calloc(length_padded, sizeof(unsigned));  memcheck(send_bufers, 1);
  unsigned* outgoing_iter = calloc(length_padded, sizeof(unsigned));  memcheck(outgoing_iter, 2);
  unsigned* reci_bufers = malloc(length_padded * sizeof(unsigned));    memcheck(reci_bufers, 3);

  // parallelizable
  // printf("rank %d - ", rank);
  for (int i = 0; i < local_len; i++) {
    unsigned bucket = (local_data[i].key[0] - ZERO) * BASE + (local_data[i].key[1] - ZERO);
    send_bufers[bucket]++;
    // printf("%c ", local_data[i].value[9]);
  }
  // printf("\n");

  MPI_Alltoall(send_bufers, block_size, MPI_UNSIGNED, reci_bufers, block_size, MPI_UNSIGNED, MPI_COMM_WORLD);

  int* sendcounts = calloc(P, sizeof(int));    memcheck(sendcounts, 4);
  int* recvcounts = malloc(P * sizeof(int));   memcheck(recvcounts, 5);
  int* sdispls = malloc(P * sizeof(int));      memcheck(sdispls, 6);
  int* rdispls = malloc(P * sizeof(int));      memcheck(rdispls, 7);

  int c_i, c_o, d_i, d_o;
  d_i = d_o = 0;
  int idx = 0;
  for (int p = 0; p < P; p++) {
    c_i = c_o = 0;
    for (; idx < (p + 1) * block_size; idx++) {
      outgoing_iter[idx] = d_o + c_o;
      c_o += send_bufers[idx];
      c_i += reci_bufers[idx];
    }
    sendcounts[p] = c_o;
    sdispls[p] = d_o;
    recvcounts[p] = c_i;
    rdispls[p] = d_i;
    d_o += c_o;
    d_i += c_i;
  }

  free(send_bufers);

  terarec_t* s_buffer = malloc(d_o * sizeof(terarec_t));
  memcheck(s_buffer, 8);
  terarec_t* r_buffer = malloc(d_i * sizeof(terarec_t));
  memcheck(r_buffer, 9);

  for (int i = 0; i < local_len; i++) {
    unsigned bucket = (local_data[i].key[0] - ZERO) * BASE + (local_data[i].key[1] - ZERO);

    unsigned idx = outgoing_iter[bucket];
    memcpy(s_buffer + idx, local_data + i, sizeof(terarec_t));
    outgoing_iter[bucket]++;
  }

  free(outgoing_iter);

  MPI_Alltoallv(s_buffer, sendcounts, sdispls, mpi_tera_type, r_buffer, recvcounts, rdispls, mpi_tera_type, MPI_COMM_WORLD);

  free(s_buffer);
  free(recvcounts);
  free(sendcounts);
  free(sdispls);

  *sorted_data = malloc(sizeof(terarec_t) * d_i);
  memcheck(sorted_data, 10);

  terarec_t* right = *sorted_data;

  // #pragma omp parallel for
  for (int b = 0; b < block_size; b++) {
    terarec_t* left = right;

    for (int p = 0; p < P; p++) {
      int sub_width = reci_bufers[p * block_size + b];
      memcpy(right, r_buffer + rdispls[p], sub_width * sizeof(terarec_t));
      right += sub_width;
      rdispls[p] += sub_width;
    }

    qsort(left, right - left, sizeof(terarec_t), teraCompare);
  }

  free(r_buffer);
  free(rdispls);
  free(reci_bufers);

  int count = (int)(right - *sorted_data);

  MPI_Allgather(&count, 1, MPI_INT, sorted_counts, 1, MPI_INT, MPI_COMM_WORLD);

  sorted_displs[0] = 0;

  for (int i = 1; i < P; i++){
    sorted_displs[i] = sorted_displs[i - 1] + sorted_counts[i - 1];
  }
}
