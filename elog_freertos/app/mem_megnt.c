#include "mem_megnt.h"


uint8_t mem_pool[64*1024] = {0};
tlsf_t tlsf = NULL;
void mem_init(void)
{
    tlsf = tlsf_create_with_pool(mem_pool,sizeof(mem_pool));
    if (tlsf == NULL)//…Í«Îƒ⁄¥Ê ß∞‹
    {
        while (1)
        {
            ;
        }
    }

}
void *mem_malloc(size_t size)
{
    return tlsf_malloc(tlsf,size);
}

void mem_free(void *ptr)
{
    tlsf_free(tlsf,ptr);
}
void *malloc(size_t size)
{
    return mem_malloc(size);
}
void free(void *ptr)
{   
    mem_free(ptr);
}
