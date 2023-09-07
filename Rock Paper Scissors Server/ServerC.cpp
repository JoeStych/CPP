#include "proj3header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const char *mmsg = "Greetings!";

void sigintHandler(int sig){
    //do nothing
}

//serverC

int main() {
    //socket variables
    int err;
    int to_cSoc;
    int from_cSoc;
    socklen_t cSocLen;

    //handles any unrelated signals when sending and recieving messages
    struct sigaction sa;
    sa.sa_handler = (void(*) (int))sigintHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    err = sigaction(SIGINT, &sa, NULL);
    if (err == -1){
        printf("Sigaction thingy was broken.\n");
        return -5;
    }

    //forming the socket
    to_cSoc = socket(AF_INET, SOCK_STREAM, 0);

    if (to_cSoc == -1){
        printf("Socket creation failure.\n");
        return 0;
    }

    //necessary structs and initialization
    struct sockaddr_in cAddr;
    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(PORT);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    //binding socket to server
    err = bind(to_cSoc, (struct sockaddr*) &sAddr, sizeof(cAddr));
    if (err == -1){
        printf("Failed to bind socket to server.\n");
        return -1;
    }

    //setting up listen queue
    printf("Listening for client...\n");
    err = listen (to_cSoc, 5);
    if (err == -1){
        printf("Listening failed\n");
        return -2;
    }

    //accept a connection
    printf("Waiting to accept connection...\n");
    from_cSoc = accept(to_cSoc, (struct sockaddr*) &cAddr, &cSocLen);
    if (from_cSoc == -1){
        printf("Acceptance failure.\n");
        printf("Errno: %d\n", errno);
        return -3;
    }

    printf("Accepted.\nWaiting for message...\n");


    //recieve message from client to ensure everything is working
    char buffer[100];
    err = 0;
    int totalBytes = 0;
    do {
        err = recv(from_cSoc, buffer, 100, 0);
        if (err != -1)
            totalBytes += err;
    } while (err == -1 && errno == 107 && totalBytes < 4);
    if (err == -1){
        printf("Error recieving. Damn so close!\n");
        printf("Errno: %d\n", errno);
        return -4;
    }

    cout<<"Message from client: "<<buffer<<"\n";


    //send a message to the client to ensure everything is working
    printf("Sending server's own message to client...\n");
    printf("SD: %d\n", from_cSoc);

    err = send(from_cSoc, "Greetings right back!", strlen("Greetings right back!") + 1, 0);
    if (err == -1){
        printf("Failed to send message. Darn.\n");
        return -12;
    }


    //start up a child process
    printf("Starting up ServerG.\n");
    pid_t child_id = fork();

    if (child_id == -1){//fork failure
        printf("Failed forking! Dang all this way and it failed...\n");
        return -15;
    }
    else if (child_id == 0){//child process executing serverG
        printf("ServerC Child: Sending SD %d to child...\n", from_cSoc);
        stringstream ss;
        char x[15];
        ss<<from_cSoc;
        ss>>x;
        execl("./ServerG", x, NULL);
    }
    else {//parent process waiting for child to finish then closing
        close(from_cSoc);
        waitpid(-1, NULL, 0);
        printf("Shutting down ServerC.\n");
    }
}