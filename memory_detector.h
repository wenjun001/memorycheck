#ifndef __MEMORY_DETECTOR_H__
#define __MEMORY_DETECTOR_H__

#include <stddef.h>

#define FILE_NAME_LENGTH        256
#define OUTPUT_FILE             "/tmp/leak_info.txt"
#define malloc(size)            xmalloc (size, __FILE__, __LINE__)
#define calloc(elements, size)  xcalloc (elements, size, __FILE__, __LINE__)
#define free(mem_ref)           xfree(mem_ref)

typedef struct {
    void *address;
    size_t size;
    char file_name[FILE_NAME_LENGTH];
    unsigned int line;
} MEM_INFO;

typedef struct _MEM_LEAK {
    MEM_INFO mem_info;
    struct _MEM_LEAK *next;
} MEM_LEAK;

void add(MEM_INFO alloc_info);
void erase(unsigned pos);
void clear(void);

void *xmalloc (size_t size, const char *file, unsigned int line);
void *xcalloc (unsigned int elements, size_t size, const char *file,
                unsigned int line);
void xfree (void *mem_ref);

void add_mem_info (void *mem_ref, size_t size,  const char *file,
                    unsigned int line);
void remove_mem_info (void *mem_ref);
void report_mem_leak (void);

#endif /* __MEMORY_DETECTOR_H__ */
