#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
typedef struct{
    int flag;
} targs;
sem_t* semaphore_read;
sem_t * semaphore_write;
int fd;
int * addr;
void* proc(void* arg) {
    targs * args = (targs *) arg;
    printf("\nStart thread\n");
    while ( args->flag != 1){
        int num;  
        sem_wait(semaphore_write);
        memcpy(&num, addr, sizeof(num));
        printf("\nRead: %d",num);
        fflush(stdout);
        sem_post(semaphore_read);
        }
    return NULL;
}

int main(){
    printf("Start program\n");
    pthread_t id1;
    targs param1;
    param1.flag = 0;
     char shm_name[] = {"/shmtest"};
    char sem_name_read[] = {"/sem_read"};
    char sem_name_write[] = {"/sem_write"};
    int fd = shm_open(shm_name, O_CREAT| O_RDWR, 0644);
    if( fd == -1){
     perror("\nshm_opne: ");
     sleep(3);
     exit(EXIT_FAILURE);
    }
    if(ftruncate(fd,sizeof(int)) == -1){
    perror("\nftruncate: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    addr = (int*)mmap(NULL,sizeof(int), PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
    if( addr == MAP_FAILED){
    perror("\nnmap: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    semaphore_read = sem_open(sem_name_read, O_CREAT, 0644, 0);
    if (semaphore_read == NULL){
        perror("\nsem_open for read: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    semaphore_write = sem_open(sem_name_write, O_CREAT, 0644, 0);
    if (semaphore_read == NULL){
        perror("\nsem_open for write: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    pthread_create(&id1, NULL, proc,(void *) &param1);
    printf("\nPress any key");
    getchar();
    param1.flag = 1;
    printf("Key pressed\n");
    if(pthread_join(id1, NULL) !=0){
    perror("\npthread_join: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    
   if( sem_close(semaphore_read) !=0){
    perror("\nsem_close for read: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if( sem_unlink(sem_name_read) != 0){
    perror("\nsem_unlink for read: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if( sem_close(semaphore_write) !=0){
    perror("\nsem_close for write: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if(sem_unlink(sem_name_write) !=0){
    perror("\nsem_unlink for write: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if( munmap(0, sizeof(int)) != 0){
    perror("\nmunmap for write: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if( close(fd) != 0){
    perror("\nclose: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    if( shm_unlink(shm_name) != 0){
    perror("\nclose: ");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    free(addr);
    printf("\nProgram terminated\n");
    return 0;
    }
