#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LED_NUMBER 10

char file_name[100];

int main(int argc, char *argv[])
{
    FILE* inputFILE[LED_NUMBER];

    for (int i = 0 ; i < LED_NUMBER ; i++)
    {
        sprintf(file_name, "/sys/class/leds/fpga_led%d/brightness", i);
        inputFILE[i] = fopen(file_name,"r+");
        printf("File open %d\n",i);
    }

    while(1)
    {
            for (int i = 0 ; i < LED_NUMBER ; i++){
                if (inputFILE[i] != NULL)
                {
                    fputc('0', inputFILE[i]);
                    fflush(inputFILE[i]);

                    printf("Led %d off\n",i);
                    usleep(500000);
                }
                else
                {
                    printf("Failed to open file for LED %d\n", i);
                }
            }

            for (int i = 0 ; i < LED_NUMBER ; i++){
                if (inputFILE[i] != NULL)
                {
                    fputc('1', inputFILE[i]);
                    fflush(inputFILE[i]);
                    printf("Led %d on\n",i);
                    usleep(50000);

                }
                else
                {
                    printf("Failed to open file for LED %d\n", i);
                }
            }
    }

    return 0;
}