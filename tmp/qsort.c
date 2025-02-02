#include <stdio.h>
#include <stdlib.h>

int values[] = {88, 56, 100, 2, 25};

int cmpfunc(const void* a, const void* b) {
  //   return (*(int*)a - *(int*)b);
  return (*(int*)b - *(int*)a);
}

// https://www.runoob.com/cprogramming/c-function-qsort.html

int main() {
  int n;

  printf("排序之前的列表：");
  for (n = 0; n < 5; n++) {
    printf("%d ", values[n]);
  }

  qsort(values, 5, sizeof(int), cmpfunc);

  printf("\n排序之后的列表：");
  for (n = 0; n < 5; n++) {
    printf("%d ", values[n]);
  }

  printf("\n");

  return (0);
}