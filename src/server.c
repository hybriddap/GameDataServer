/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h> // For fcntl


#include "server.h"
#include "inputHandler.h"

struct sockaddr_in serv_addr, cli_addr;

char buffer[256];

int sockfd;
int newsockfd;
int canRun=1;


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void writeClient(int newsockfd,char *msg)
{
    //char* msg="I got your message\n";
    int result = write(newsockfd,msg,strlen(msg));
    if (result < 0) error("ERROR writing to socket");
}

void readClient(int newsockfd)
{
    bzero(buffer,256);
    int result = read(newsockfd,buffer,255);
    if (result < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
}

void bindSocket()
{
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
}

void acceptClients()
{
    while(canRun)
    {
        socklen_t clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
        if (newsockfd < 0) {}    //error("ERROR on accept");
        else
        {
            printer("New Client!",0);
            break;
        }
    }   
}

void threadFinishedEvent()
{
    canRun=0;
}

void closeSockets()
{
    close(newsockfd);
    close(sockfd);
}

void setupSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORTNO);
}

void serverFunctionality()
{
    //accept clients
    acceptClients();

    //read from client
    //readClient(newsockfd);
    
    //write to client
    if(canRun)
        writeClient(newsockfd,"Heres the data:\n");

    //close socket
    close(newsockfd);
}

int main(int argc, char *argv[])
{   
    //setup socket
    setupSocket();
    
    //bind and listen
    bindSocket();

    //input thread
    startThread();

    //server functionality
    while (canRun)
    {
        serverFunctionality();
        // printf("Main: %i\n",canRun);
    }
    
    //close sockets
    joinThreads();
    closeSockets();

    printer("Server Closing!",0);
    return 0; 
}