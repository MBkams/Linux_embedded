#include <stdio.h>
#include <stdint.h>

struct asserv {
char version[10];
float taille_roue;
double entraxe;
unsigned char ratio;
unsigned short p;
unsigned int i;
int d;
};

int main(void)
{
    printf("%d\n", sizeof(struct asserv));
    return 0;
}

