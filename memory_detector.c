#include "memory_detector.h"
#undef malloc
#undef calloc
#undef free

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

static MEM_LEAK *ptr_start = NULL;
static MEM_LEAK *ptr_next =  NULL;

static void add(MEM_INFO alloc_info)
{
    MEM_LEAK *mem_leak_info = NULL;
    mem_leak_info = (MEM_LEAK *) calloc(1, sizeof(MEM_LEAK));
    mem_leak_info->mem_info.address = alloc_info.address;
    mem_leak_info->mem_info.size = alloc_info.size;
    mem_leak_info->mem_info.line = alloc_info.line;
    mem_leak_info->next = NULL;
    strcpy(mem_leak_info->mem_info.file_name, alloc_info.file_name);

    if (ptr_start == NULL) {
        ptr_start = mem_leak_info;
        ptr_next = ptr_start;
    } else {
        ptr_next->next = mem_leak_info;
        ptr_next = ptr_next->next;
    }
}


static void erase(unsigned pos)
{
    unsigned index = 0;
    MEM_LEAK *alloc_info, *temp;

    if(pos == 0) {
        MEM_LEAK *temp = ptr_start;
        ptr_start = ptr_start->next;
        free(temp);
    } else {
        for(index = 0, alloc_info = ptr_start; index < pos;
            alloc_info = alloc_info->next, ++index) {
            if(pos == index + 1) {
                temp = alloc_info->next;
                alloc_info->next =  temp->next;
                free(temp);
                break;
            }
        }
    }
}


static void clear()
{
    MEM_LEAK *temp = ptr_start;
    MEM_LEAK *alloc_info = ptr_start;

    while(alloc_info != NULL) {
        alloc_info = alloc_info->next;
        free(temp);
        temp = alloc_info;
    }
}

static void add_mem_info (void *mem_ref, size_t size,  const char *file,
                            unsigned int line)
{
    MEM_INFO mem_alloc_info;

    memset(&mem_alloc_info, 0, sizeof(mem_alloc_info));
    mem_alloc_info.address = mem_ref;
    mem_alloc_info.size = size;
    mem_alloc_info.line = line;
    strncpy(mem_alloc_info.file_name, file, FILE_NAME_LENGTH);

    add(mem_alloc_info);
}

static void remove_mem_info (void *mem_ref)
{
    unsigned short index;
    MEM_LEAK *leak_info = ptr_start;

    for(index = 0; leak_info != NULL; ++index, leak_info = leak_info->next) {
        if (leak_info->mem_info.address == mem_ref) {
            erase(index);
            break;
        }
    }
}

void *xmalloc (size_t size, const char *file, unsigned int line)
{
    void *ptr = malloc(size);
    if (ptr != NULL) {
        add_mem_info(ptr, size, file, line);
    }
    return ptr;
}

void *xcalloc (unsigned int elements, size_t size, const char *file,
                unsigned int line)
{
    unsigned total_size;
    void *ptr = calloc(elements , size);
    if(ptr != NULL) {
        total_size = elements * size;
        add_mem_info (ptr, total_size, file, line);
    }
    return ptr;
}

void xfree (void *mem_ref)
{
    remove_mem_info(mem_ref);
    free(mem_ref);
}

void duplicate_print(FILE *fp, char *fmt,  ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
}

void report_mem_leak (void)
{
    unsigned short index;
    MEM_LEAK *leak_info;

    FILE *fp_write = fopen (OUTPUT_FILE, "wt");
    char info[1024];

    if(fp_write != NULL) {
        sprintf(info, "%s\n", "Memory Leak Summary");
        duplicate_print(fp_write, info);

        sprintf(info, "%s\n", "-----------------------------------");
        duplicate_print(fp_write, info);

        for(leak_info = ptr_start; leak_info != NULL;
            leak_info = leak_info->next) {
            sprintf(info, "address : %p\n", leak_info->mem_info.address);
            duplicate_print(fp_write, info);

            sprintf(info, "size    : %lu bytes\n", leak_info->mem_info.size);
            duplicate_print(fp_write, info);

            sprintf(info, "file    : %s\n", leak_info->mem_info.file_name);
            duplicate_print(fp_write, info);

            sprintf(info, "line    : %d\n", leak_info->mem_info.line);
            duplicate_print(fp_write, info);

            sprintf(info, "%s\n", "-----------------------------------");
            duplicate_print(fp_write, info);
        }
    }

    clear();
}
