#include    <malloc.h>
#include <stdio.h>
#include    "memory_detecter.h"


int main()
{
   char * ptr1 = (char *)malloc(10);
   // char *m0 = (char *)malloc(sizeof(LS_MSG));
  //  int * ptr2 = (int *)calloc(10, sizeof(int));
  //  int * ptr2 = (int *)calloc(10, sizeof(int));
   // float * ptr3 = (float *) calloc(15, sizeof(float));
   free(ptr1);
    atexit(report_mem_leak);
    return 0;
}