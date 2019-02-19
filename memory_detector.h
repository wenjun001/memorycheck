#ifndef __MEMORY_DETECTOR_H__
#define __MEMORY_DETECTOR_H__

#include <stddef.h>

#define FILE_NAME_LENGTH        256
#define OUTPUT_FILE             "/tmp/leak_info.txt"
#define malloc(size)            xmalloc (size, __FILE__, __LINE__)
#define calloc(elements, size)  xcalloc (elements, size, __FILE__, __LINE__)
#define free(mem_ref)           xfree(mem_ref)

void *xmalloc (size_t size, const char *file, unsigned int line);
void *xcalloc (unsigned int elements, size_t size, const char *file,
                unsigned int line);
void xfree (void *mem_ref);

void report_mem_leak (void);

#endif /* __MEMORY_DETECTOR_H__ */
