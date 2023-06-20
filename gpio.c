#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h> 


int main(int argc, char *argv[])
{
    uint32_t * p;
    int fd = open("/dev/mem", O_RDWR);
    p = (uint32_t*)mmap(NULL, 4, PROT_WRITE|PROT_READ, MAP_SHARED,fd, 0xFF203000);

while(1)
{
    for (int i = 1; i < 10; i++)
    {
        *p &= ~(1<<i);
        printf("Led %d off\n",i);

        usleep(500000);

    }

    for (int i = 1; i < 10; i++)
    {
        *p |= (1<<i);
        printf("Led %d on\n",i);

        usleep(50000);

    }
}


    return 0;
}
