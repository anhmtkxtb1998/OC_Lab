#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

using namespace std;
typedef struct {
    int flag;
    char sym;
}targs;
pthread_mutex_t mutexID; 
void* proc(void * arg) {
    targs * args =(targs*)arg;
    cout << "\nStarted thread " << args->sym << " with id: " << pthread_self();
    while (args->flag != 1){
        int re_value = pthread_mutex_lock(&mutexID);
        if(re_value != 0){
         cout <<"\nError function pthread_mutex_lock:" << strerror(re_value);
         sleep(1);
        exit(EXIT_FAILURE);        
        }
        for( int i = 0;i < 5;i++)
        {
        cout << "\n" << args->sym;
        sleep(1);
        }
        re_value = pthread_mutex_unlock(&mutexID);
        if(re_value !=0){
        cout <<"\nError function pthread_mutex_lock:" << strerror(re_value);
        sleep(1);
        exit(EXIT_FAILURE);  
        }      
        sleep(1);
        }
    void* returned_value = NULL;
    return returned_value; 
   }
int main(){
    pthread_t id1;
    pthread_t id2;
    targs params_1,params_2;
    params_1.flag =0;params_1.sym ='1';
    params_2.flag =0;params_2.sym ='2';

    int re_value = pthread_mutex_init(&mutexID,NULL);
    if(re_value !=0){
    cout <<"\nError function pthread_mutex_init: " << strerror(re_value);
    exit(EXIT_FAILURE);
    }
    if(pthread_create(&id1,NULL,proc,(void *)(&params_1)) != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if(pthread_create(&id2,NULL,proc,(void*)(&params_2)) != 0){
    cout <<"\nError function pthread_create for 2-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed");
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
    re_value = pthread_mutex_destroy(&mutexID);
    if(re_value !=0)
    {
    cout <<"\nError function pthread_mutex_destroy: " << strerror(re_value);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nProgram terminated\n");
    return 0;
}
