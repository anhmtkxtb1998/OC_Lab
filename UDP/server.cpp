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
#include <arpa/inet.h>

#define SIZE 256
using namespace std;
struct sockaddr_in saddr;
socklen_t saddrlen;
int mysock;
int flag_receive = 0, flag_send = 0;
pthread_mutex_t muid;
pthread_t id_receive, id_send;
vector<string> msg_list;

void * receive_msg(void * args){
    char rcvbuf[SIZE];
    while(flag_receive != 1){
      memset(rcvbuf,0,SIZE);
     int recvcount = recvfrom(mysock,rcvbuf,sizeof(rcvbuf),0,(struct sockaddr*)&saddr,&saddrlen);
     if(recvcount == -1){
         perror("\nrecvfrom error");
         sleep(1);
      }
     else
     {   
         printf("\nReceive %s", rcvbuf);
         pthread_mutex_lock(&muid);
         msg_list.push_back(string(rcvbuf));
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
           
           int len = sprintf(sndbuffer,"answer for %s: UID: %d, User: %s, User password: %s",request.c_str(), result->pw_uid,result->pw_name, result->pw_passwd);
           cout << "\nSend "<< sndbuffer;
           int sentcount = sendto(mysock, sndbuffer,len, 0,(struct sockaddr*)&saddr,saddrlen);
           if (sentcount == -1) {
           perror("\nsend error");
           }
           else{
            printf("\nSend Ok ");
            }
    }
       else{
           pthread_mutex_unlock(&muid);
           //sleep(1);
       }
   }
   return NULL;
}

int main()
{
    printf("Start program!\n");
    mysock = socket(AF_INET,SOCK_DGRAM,0);
    fcntl(mysock,F_SETFL,O_NONBLOCK);
    int optval = 1;
    setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(8000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(mysock,(struct sockaddr*)&bindaddr,sizeof(bindaddr));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddrlen = sizeof(saddr);
    
    cout << "\nServer started\n" << endl;
    pthread_mutex_init(&muid,NULL);
    pthread_create(&id_receive, NULL, receive_msg,NULL);
    pthread_create(&id_send, NULL, send_msg,NULL);
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag_receive = 1;
    flag_send = 1;
    pthread_join(id_receive,NULL);
    pthread_join(id_send,NULL);
    pthread_mutex_destroy(&muid);
    close(mysock);
    printf("\nProgram terminated\n");
    return 0;
}

