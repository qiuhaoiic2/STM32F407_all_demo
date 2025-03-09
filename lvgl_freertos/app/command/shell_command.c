#include "shell_command.h"
#include "stdlib.h"//atoi 函数将字符串转化为整数

Shell shell;
char shell_buffer[SHELL_BUFFER_SIZE];



signed short shell_write(char *buf, unsigned short len)
{
    unsigned short i = 0;
    while (i < len)
    {
        uart1_send_data((uint8_t)buf[i++]);
    }
    return i;
}

void shell_init(void)
{
    shell.write = shell_write;
    shellInit(&shell, shell_buffer, sizeof(shell_buffer));

}
void cmd_helloword(int argc, char *argv[])
{   
    unsigned short i = 0;
    shellPrint(&shell, "cmd_helloword have %d parameters\n",argc);
    shellWriteString(&shell,"hello world\n");
    for (i = 0;i < argc;i++)
    {   
        shellPrint(&shell,"argv[%d] = %s\n",i,argv[i]);
    }
}

void cmd_add(int arge,char *argv[])
{
    if (arge != 3)
    {
        shellWriteString(&shell,"Usage add err parameter\n");
        return ; 
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = a + b;
    shellPrint(&shell,"%d + %d = %d\n",a,b,c);
}

void cmd_mem_megnet(int arge,char *argv[])
{
    size_t size = 0;
    size = atoi(argv[1]);
    uint8_t *p = NULL;
    p = malloc(size);
    if (p != NULL)
    {
        shellPrint(&shell,"mam_malloc %d bytes memory at %p\n",size,p);

    }
    
    free(p);

}

