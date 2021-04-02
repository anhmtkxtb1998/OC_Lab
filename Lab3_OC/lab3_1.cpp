#include <iostream>
#include<pthread.h>
#include<string>
#include<pwd.h>
#include <sys/types.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h> 
using namespace std;
#define  SIZE 200
typedef struct {
    int flag;
}targs;
int filedes[2];
void * write_pipe(void * arg){
     cout <<"\nStart write with thread id: " << pthread_self() << endl;
     targs * args =(targs*)arg;
     uid_t id = geteuid();
         struct passwd *result;
         result = getpwuid(id);
         if(result == NULL)
         {
             cout <<"Error getpwuid: " << strerror(errno);
             sleep(1);
             exit(EXIT_FAILURE);
         }
     char buffer[SIZE];
     while(args->flag != 1){
         int size = snprintf(buffer,SIZE,"UID: %d, User: %s, User password: %s, Home directory: %s\n",result->pw_uid,result->pw_name, result->pw_passwd, result->pw_dir);
         ssize_t re_value = write(filedes[1],buffer,strlen(buffer));
         if(re_value == -1){
             cout <<"\nFailed  write with error: " << strerror(errno);
             sleep(1);
             continue;
         }
         sleep(1);
     }
     cout <<"End write with thread id: " << pthread_self() << endl;
     void * return_value = NULL;
     return return_value;
}
void * read_pipe(void * arg){
     cout <<"\nStart read with thread id: " << pthread_self() << endl;
     targs * args =(targs*)arg;
     char buffer[SIZE] = {'\0'};
     while(args->flag != 1){
        memset(buffer,0,SIZE);
        ssize_t re_value = read(filedes[0],buffer,SIZE);
         if(re_value == -1)
         {
         cout <<"Failed read with error: " << strerror(errno);
         sleep(1);
         }
         else
             cout <<"\nRead: " << buffer << endl;
             
     }
     cout <<"End read with thread id: " << pthread_self() << endl;
     void * return_value = NULL;
     return return_value;
}
int main()
{
    printf("\nThe program started working");
    pthread_t id1;
    pthread_t id2;
    targs params_1,params_2;
    params_1.flag =0;
    params_2.flag =0;

    if(pipe(filedes) == -1){
    cout <<"\nError function pipe with error: " << strerror(errno);
    sleep(1);
    exit(EXIT_FAILURE);
    }
    if(pthread_create(&id1,NULL,read_pipe,(void *)(&params_1)) != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if(pthread_create(&id2,NULL,write_pipe,(void*)(&params_2)) != 0){
    cout <<"\nError function pthread_create for 2-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    params_1.flag =1;
    params_2.flag =1;
    if(pthread_join(id1, NULL) != 0){
    cout <<"\nError function pthread_join for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    if(pthread_join(id2, NULL) != 0){
    cout <<"\nError function pthread_join for 2-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    close(filedes[1]);
    close(filedes[0]);
    printf("\nProgram terminated\n");
    return 0;
}


