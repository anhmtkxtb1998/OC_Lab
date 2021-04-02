#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char* argv[])
{

    printf("ID of process lab_4_1: %d\n", getpid());
    printf("Id of parent process lab_4_1: %d\n", getppid());
     if(argc >= 4 && argc <= 6){
        for (int i = 1; i < argc; i++)
        {
            printf("%s\n", argv[i]);
            sleep(1);
        }
    }       
    exit(3);
}

