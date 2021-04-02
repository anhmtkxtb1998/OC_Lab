#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include<stdlib.h>

typedef struct{
    char sym;
    int flag;
} targs;
sem_t* semaphoreID;
FILE* shared_file;

void* thread_func(void* arg) {
     targs * args = (targs *) arg;
    printf("\nStart thread\n");
    while ( args->flag != 1){
        if(sem_wait(semaphoreID) != 0){
        perror("\nsem_wait: ");
        sleep(3);
        exit(EXIT_FAILURE); 
        }
        for (int i = 0; i < 5; i++){
            printf("%c", args->sym);
            fprintf(shared_file, "%c", args->sym);
            fflush(stdout);
            fflush(shared_file);
            sleep(1);
        }
        if( sem_post(semaphoreID) !=0){
        perror("\nsem_post: ");
        sleep(1);
        }
        sleep(1);
    }
    return NULL;
}

int main(){
    printf("Start program\n");
    pthread_t id_thread;
    targs param1;
    param1.flag = 0; param1.sym = '2';
    char semaphoreName[] = {"/semaphore"};

    semaphoreID = sem_open(semaphoreName, O_CREAT, 1777, 1);
    if (semaphoreID == NULL){
        perror("\nsem_open: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    shared_file = fopen("data.txt", "a+");
    if(shared_file == NULL){
    perror("\nfopen: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
   
    if(pthread_create(&id_thread , NULL , thread_func ,(void*)&param1) != 0){
    perror("\npthread_create: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nPress any key");
    getchar();
    param1.flag = 1;
    printf("Key pressed\n");
    if(pthread_join(id_thread, NULL) !=0){
    perror("\npthread_join: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    fclose(shared_file);
    if(sem_close(semaphoreID) != 0){
    perror("\nsem_close: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    return 0;
    }
