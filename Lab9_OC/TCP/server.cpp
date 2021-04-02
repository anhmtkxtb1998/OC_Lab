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
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<vector>
#define PORT 7000

#define SIZE 256
using namespace std;
int listenSocket, serverSocket;
int flag_connect =0, flag_receive = 0, flag_send = 0, flag_request_flow = 0;
pthread_mutex_t muid;
pthread_t id_receive, id_send, id_connect;
vector<string> msg_list;

void * recevie_msg(void * args){
    char rcvbuf[SIZE];
    while(flag_receive != 1){
      memset(rcvbuf,0,SIZE);
     int reccount = recv(serverSocket,rcvbuf, SIZE,0);
     if(reccount == -1){
         perror("recv error");
         sleep(1);
      }
     else if(reccount == 0){
         sleep(1);
     }
     else
     {
         pthread_mutex_lock(&muid);
         printf("\nReceive %s", rcvbuf);
         string tmp = " ";tmp += rcvbuf;
         msg_list.push_back(rcvbuf);
         pthread_mutex_unlock(&muid);
     }
    }
    return NULL;
}

void *send_msg(void * args){
    char sndbuffer[SIZE];
     struct passwd *result;
      uid_t id = geteuid();
     result = getpwuid(id);
   while(flag_send != 1){
        pthread_mutex_lock(&muid);
       if(!msg_list.empty()){
           string request = msg_list.back();
           msg_list.pop_back();
           pthread_mutex_unlock(&muid);
           snprintf(sndbuffer,SIZE,"answer for %s: UID: %d, User: %s, User password: %s",request.c_str(), result->pw_uid,result->pw_name, result->pw_passwd);
           cout << "\nSend "<< sndbuffer;
           int sentcount = send(serverSocket, sndbuffer, strlen(sndbuffer), 0);
           if (sentcount == -1) {
           perror("\nsend error");
           sleep(1);
           }
       }
       else{
           pthread_mutex_unlock(&muid);
           sleep(1);
       }
   }
   return NULL;
}
void * thread_connect(void * args){
    while(flag_connect != 1){
        struct sockaddr_in serverSockAddr;
        socklen_t addrLen = (socklen_t)sizeof(serverSockAddr);
        serverSocket = accept(listenSocket,(struct sockaddr*)&serverSockAddr,&addrLen);
        if(serverSocket == -1){
            perror("\naccept error: ");
            sleep(1);
        }
        else{
            cout <<"\nSucess connect to client!\n";
            pthread_create(&id_receive,NULL,recevie_msg,NULL);
            pthread_create(&id_send,NULL,send_msg,NULL);
            pthread_exit(NULL);
        }
    }
    return NULL;

}
int main()
{
    printf("Start program!\n");
    listenSocket = socket(AF_INET,SOCK_STREAM,0);
    int optval = 1;
    if( listenSocket == 0){
        perror("\nsocket error: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    fcntl(listenSocket,F_SETFL,O_NONBLOCK);
    struct sockaddr_in listenSockAddr;
    listenSockAddr.sin_family = AF_INET;
    listenSockAddr.sin_port = htons(PORT);
    listenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("\nsetsocket: ");
        sleep(3);
        exit(EXIT_FAILURE);
    }
    bind(listenSocket,(struct sockaddr*)&listenSockAddr,sizeof (listenSockAddr));
    listen(listenSocket,SOMAXCONN);
    cout << "\nServer started\n" << endl;
    pthread_mutex_init(&muid,NULL);
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
    pthread_mutex_destroy(&muid);
    shutdown(serverSocket,2);    
    close(serverSocket);
    close(listenSocket);
    printf("\nProgram terminated\n");
    return 0;
}

