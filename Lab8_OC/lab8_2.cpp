#include <iostream>
#include<pthread.h>
#include<string>
#include<pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h> 
#include <signal.h>
#include <mqueue.h>
using namespace std;
#define  SIZE 50
typedef struct{
 int flag;
 mqd_t mqid;
}tagrs; 
char queue[] = {"/myqueue"};
mqd_t mqid;
int flag = 0;

void * receive_(void * arg){
     cout <<"\nStart receive message with thread id: " << pthread_self() << endl;  
     char buffer[SIZE]; 
     struct timespec ts;
     while(flag != 1){
         if(clock_gettime(CLOCK_REALTIME,&ts) == -1){
       cout <<"\nClock gettime failed with error: " << strerror(errno);
       sleep(1);
       exit(EXIT_FAILURE);
       }
        ts.tv_sec +=1;
         memset(buffer,0,SIZE);
         ssize_t result = mq_timedreceive(mqid,buffer,SIZE,0,&ts);
         if(result == -1)
         {
         perror("mq_receive: ");
         sleep(1);
         continue;
         }
         else if(result > 0){
         cout <<"\nmq_receive: " << buffer;
         sleep(1);
         }
       
     }

     return NULL;
}

int main()
{
    printf("\nThe program started working");
    pthread_t id1;
    mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = SIZE * sizeof(char);
    attr.mq_curmsgs = 0;
    mqid = mq_open(queue,O_CREAT | O_RDONLY, 0644, &attr);
    if( mqid == -1){
    perror("mq_open: ");
    sleep(1);
    }
    if(pthread_create(&id1,NULL,receive_,NULL) != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag =1;
    if(pthread_join(id1, NULL) != 0){
    cout <<"\nError function pthread_join for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    mq_close(mqid);
    mq_unlink(queue);
    printf("\nProgram terminated\n");
    return 0;
}


