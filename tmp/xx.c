#include <memory.h>
#include <stdio.h>

int main() {
  int a = 7;
  char* ptr = (char*)&a;
  if (ptr[0] == 7) {
    printf("little endian\n");
  } else {
    printf("big endian\n");
  }
  return 0;
}