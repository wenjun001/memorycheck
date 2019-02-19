#include <stdio.h>
#include <stdlib.h>
#include "memory_detector.h"

int main()
{
    char * ptr1 = (char *)malloc(10);

    /* Register exit callback before anything else */
    atexit(report_mem_leak);

//    free(ptr1);
    return 0;
}
