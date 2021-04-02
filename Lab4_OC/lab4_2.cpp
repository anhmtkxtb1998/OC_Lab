#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if(pid == 0){
     printf("ID of process from child process: %d\n",getpid());
     printf("ID of parent process from child process: %d\n",getppid());
     char filename[] = "prog1";
     char* path = realpath(filename, NULL);
     char* pathvar;
     char newpath[1000];
     int rv;
     pathvar = getenv("PATH");
     strcpy(newpath, pathvar);
     strcat(path,":");
     strcat(newpath, path);
     setenv("PATH",newpath,1);     
     if(argc <  4){
     printf("Used 0 arguments for prog1\n");
     execlp("prog1","prog1",NULL);
     printf("Failed exec with error: %s\n", strerror(errno));
     sleep(1);
     exit(EXIT_FAILURE);
     }
     else if( argc >= 4) {
     printf("Used 3 arguments for prog1\n");
     execlp("prog1","prog1",argv[1], argv[2], argv[3],NULL);
     printf("Failed exec with error: %s\n", strerror(errno));
     sleep(1);
     exit(EXIT_FAILURE);
     }
     free(path);
     free(pathvar);
    }
     else if(pid == -1){
     printf("Failed to create child process with error: %s\n", strerror(errno));
     sleep(1);
     exit(EXIT_FAILURE);
     }
     else
     {
     printf("ID of process lab4_2: %d\n",getpid());
     printf("ID of parent process lab4_2: %d\n",getppid());
     int status;  
        while (waitpid(-1, &status, WNOHANG) == 0){
            printf("Wait...\n");
            usleep(500* 1000);
        }
        if (WIFEXITED(status))
            printf("Process successfully ended with status: %d\n", WEXITSTATUS(status));
        else
            printf("Process ended unsuccessfully!\n");
     sleep(1);
     }
    return 0;
}

