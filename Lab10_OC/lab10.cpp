#include <iostream>
#include<pthread.h>
#include<string>
#include<pwd.h>
#include <sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sched.h>
#define PORT 8000
int flag = 0;
using namespace std;
struct sockaddr_in listenSockAddr;
int main(int argc, char* argv[])
{   
    printf("Start program\n");
    pid_t	pid	=	fork();
    if (pid == 0) {
    printf("\nChild process start\n");
    int sock = socket(AF_INET,SOCK_DGRAM,0); 
    if(sock < 0){
    perror("\nsock  error");
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    memset(&listenSockAddr, 0 , sizeof(listenSockAddr));
    listenSockAddr.sin_family = AF_INET;
    listenSockAddr.sin_port = htons(PORT);
    listenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if( bind(sock,(struct sockaddr*)&listenSockAddr,sizeof (listenSockAddr)) != 0){
    perror("\nbind  error");
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    printf("\nInfor of socket child process:");
    struct sockaddr_in SockAddr;
    memset(&SockAddr, 0 , sizeof(SockAddr));
    socklen_t len = sizeof(SockAddr);
    if( getsockname(sock,(struct sockaddr*)&SockAddr, &len) != 0 ){
    perror("\ngetsockname error");
    sleep(3);
    }
    cout << "\nsock : " << sock;
    cout << "\nDomain: " << SockAddr.sin_family;
    cout << "\nPort: " << ntohs(SockAddr.sin_port);
    cout << "\nIP Adress: " << inet_ntoa(SockAddr.sin_addr);
    }
     else
     {
    printf("\nParent process start");
    unshare(CLONE_NEWNET);
    int mysock = socket(AF_INET,SOCK_DGRAM,0);
    memset(&listenSockAddr, 0 , sizeof(listenSockAddr));
    listenSockAddr.sin_family = AF_INET;
    listenSockAddr.sin_port = htons(PORT);
    listenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(mysock,(struct sockaddr*)&listenSockAddr,sizeof (listenSockAddr)) != 0){
    perror("\nbind error");
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nInfor of socket parent process:");
    memset(&listenSockAddr, 0 , sizeof(listenSockAddr));
    socklen_t len = sizeof(listenSockAddr);
    if( getsockname(mysock,(struct sockaddr*)&listenSockAddr, &len) != 0 ){
    perror("\ngetsockname error");
    sleep(3);
    }
     cout << "\nmysock : " << mysock;
    cout << "\nDomain: " << listenSockAddr.sin_family;
    cout << "\nPort: " << ntohs(listenSockAddr.sin_port);
    cout << "\nIP Adress: " << inet_ntoa(listenSockAddr.sin_addr);
   
    int status;  
    while (waitpid(-1, &status, WNOHANG) == 0){
            printf("\nWait...\n");
            usleep(500* 1000);
    }
    if (WIFEXITED(status))
        printf("\nProcess successfully ended with status: %d\n", WEXITSTATUS(status));
    else
    printf("\nProcess ended unsuccessfully!\n");
    }
    return 0;
}

