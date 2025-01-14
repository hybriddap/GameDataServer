#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "inputHandler.h"
#include "server.h"


pthread_t tid;  //input thread
char error_message[256];

void printer(char* msg,int err)
{
    if(err)
        printf("\033[0;31m%s\033[0m\n",msg);
    else
        printf("\033[0;32m%s\033[0m\n",msg);
}

void inputHandler(int arg)
{
    char str[5];

    while(strcmp(str,"exit"))
    {
        fgets(str, 5, stdin);
        // printf("%s\n",str);
    }

    threadFinishedEvent();
    printer("Exiting...",0);
}

void joinThreads()
{
    pthread_join(tid,NULL);
}

void startThread()
{
    int error = pthread_create(&(tid),NULL,(void *)inputHandler,0);
    if (error != 0)
    {
        snprintf(error_message, sizeof(error_message), "Thread can't be created :[%s]", strerror(error));
        printer(error_message,1);
    }
}