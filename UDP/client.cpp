#include<iostream>
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
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 7000
#define SIZE 256
using namespace std;
struct sockaddr_in saddr;
socklen_t saddrlen;
int mysock;
int flag_receive = 0, flag_send = 0;
pthread_t id_receive, id_send;


void * receive_msg(void * args){
    char rcvbuf[SIZE];
    while(flag_receive != 1){
        memset(rcvbuf,0, SIZE);
        int reccount = recvfrom(mysock,rcvbuf,SIZE,0,(struct sockaddr*)&saddr,&saddrlen);
        if (reccount == -1) {
        perror("\nrecvfrom error");
        sleep(1);
        }
        else{
        printf("\nReceive %s\n", rcvbuf);
       }
    }
    return NULL;
}

void *send_msg(void * args){
       char sndbuf[SIZE];
       int count = 1;
   while(flag_send != 1){
        int len = sprintf(sndbuf,"request %d", count);
        int sentcount = sendto(mysock, sndbuf, len, 0,(struct sockaddr*)&saddr,saddrlen);
        if(sentcount == -1){
         perror("\nsend error");
        }
        else{
         printf("\nSuccess send %s\n", sndbuf);
        }
        count++;
        sleep(1);
   }
   return NULL;
}


int main()
{
    printf("Start program!\n");
    mysock = socket(AF_INET,SOCK_DGRAM,0);
    fcntl(mysock,F_SETFL,O_NONBLOCK);
    int optval  = 1;
    setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in  bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(7000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(mysock,(struct sockaddr*)&bindaddr,sizeof(bindaddr));
    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddrlen = sizeof(saddr);
    
    pthread_create(&id_send, NULL, send_msg,NULL);
    pthread_create(&id_receive, NULL, receive_msg,NULL);
    
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag_receive = 1;
    flag_send = 1;

    pthread_join(id_receive,NULL);
    pthread_join(id_send,NULL);
    close(mysock);
    printf("\nProgram terminated\n");
    return 0;
}
