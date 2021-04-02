#include <iostream>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include <errno.h>
#include<string.h>
using namespace std;

typedef struct {
    int flag;
    char sym;
}targs;

void * proc(void * arg){
     targs * args =(targs*)arg;
     cout << "\nStarted thread " << args->sym <<" with id: "<< pthread_self();
     while(args->flag == 0){
         cout << "\n" << args->sym;
         fflush(stdout);
         sleep(3);
     }
     int * codeReturn = new int;
     if(args-> sym == '1')
     *codeReturn = 2;
     else
     *codeReturn = 3;
     pthread_exit(codeReturn);
}

int main()
{
    printf("\nThe program started working");
    pthread_t id1;
    pthread_t id2;
    targs params_1,params_2;
    params_1.flag =0;params_1.sym ='1';
    params_2.flag =0;params_2.sym ='2';
    int re_value = pthread_create(&id1,NULL,proc,(void *)(&params_1));
    if(re_value != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    return 0;
    }
    re_value  = pthread_create(&id2,NULL,proc,(void*)(&params_2));
    if(re_value != 0){
    cout <<"\nError function pthread_create for 2-th thread: " << strerror(errno);
    sleep(3);
    return 0;
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed");
    params_1.flag =1;
    params_2.flag =1;
    int * codeReturn1;
    int * codeReturn2;
    re_value  = pthread_join(id1,(void**)&codeReturn1);
   if(re_value != 0){
    cout <<"\nError function pthread_join for 1-th thread: "  << strerror(errno);
    sleep(3);
    return 0;
    }
    printf("\nThread 1 ended whith code %d",(*codeReturn1));
    re_value = pthread_join(id2,(void**)&codeReturn2);
    if(re_value != 0){
    cout <<"\nError function pthread_join for 2-th thread: "  << strerror(errno);
    sleep(3);
    return 0;
    }
    printf("\nThread 2 ended whith code %d",(*codeReturn2));
    printf("\nProgram terminated\n");
    delete codeReturn1;
    delete codeReturn2;
    return 0;
}

