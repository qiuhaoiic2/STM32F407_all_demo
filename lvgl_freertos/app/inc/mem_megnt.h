#ifndef _MEM_MEGNT_H
#define _MEM_MEGNT_H

#include "main.h"
void mem_init(void);
void *mem_malloc(size_t size);
void mem_free(void *ptr);

#endif  /* _MEM_MEGNT_H */
