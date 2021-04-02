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
int clientSocket;
int flag_connect =0, flag_receive = 0, flag_send = 0;
pthread_t id_receive, id_send, id_connect;


void * recevie_msg(void * args){
    char rcvbuf[SIZE];
    while(flag_receive != 1){
        memset(rcvbuf,0, SIZE);
        int reccount = recv(clientSocket,rcvbuf,SIZE,0);
        if (reccount == -1) {
        perror("recv error");
        sleep(1);
        }else if (reccount == 0) {
        sleep(1);
        }else{
        printf("\nReceive %s", rcvbuf);
       }
    }
    return NULL;
}

void *send_msg(void * args){
       char sndbuf[SIZE];
       int count = 1;
   while(flag_send != 1){
        int len = sprintf(sndbuf,"request %d", count);
        int sentcount = send(clientSocket, sndbuf, len, 0);
        if(sentcount == -1){
         perror("\nsend error: ");
        }
        else{
         printf("\nSuccess send %s", sndbuf);
        }
        count++;
        sleep(1);
   }
   return NULL;
}
void * thread_connect(void * args){
    while(flag_connect != 1){
        struct sockaddr_in clientSocketSockAddr;
        clientSocketSockAddr.sin_family = AF_INET;
        clientSocketSockAddr.sin_port = htons(PORT);
        clientSocketSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        int result = connect(clientSocket,(struct sockaddr*)&clientSocketSockAddr,sizeof(clientSocketSockAddr));
        if (result == -1) {
            perror("\nconnect error: ");
            sleep(1);
           }else{
           printf("\nSucess conected to server!\n");
           pthread_create(&id_receive,NULL,recevie_msg,NULL);
           pthread_create(&id_send,NULL,send_msg,NULL);
           pthread_exit(NULL);
       }
    }
    return NULL;

}
void sig_handler(int signo){
    cout << "\nClient is crushed or disconnect with server";
    sleep(1);
}
int main()
{
    printf("Start program!\n");
    signal(SIGPIPE, sig_handler);
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if( clientSocket == 0){
        perror("socket error: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    fcntl(clientSocket,F_SETFL,O_NONBLOCK);
    pthread_create(&id_connect, NULL, thread_connect,NULL);
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag_receive = 1;
    flag_send = 1;
    flag_connect = 1;
    pthread_join(id_receive,NULL);
    pthread_join(id_send,NULL);
    pthread_join(id_connect,NULL);

    shutdown(clientSocket,2);    
    close(clientSocket);
    printf("\nProgram terminated\n");
    return 0;
}
