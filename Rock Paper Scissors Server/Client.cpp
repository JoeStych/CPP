#include "proj3header.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main(){
    //socket variables
    int err;
    int to_sSoc;
    int from_sSoc;
    int sSocLen;

    //initalizing the socket
    to_sSoc = socket(AF_INET, SOCK_STREAM, 0);
    if (to_sSoc == -1){
        printf("Socket creation failure.\n");
        return -1;
    }

    //necessary struct declarations
    struct sockaddr_in cAddr;
    struct addrinfo criteria;
    struct addrinfo *serverResult;

    //initalizing criteria object
    memset(&criteria, 0, sizeof(struct addrinfo));
    criteria.ai_family = AF_INET;
    criteria.ai_socktype = SOCK_STREAM;
    criteria.ai_protocol = 0;
    err = getaddrinfo(NULL, SERVERPORTSTR, &criteria, &serverResult);
    if (err == -1){
        printf("Error doing the thing we don't fully understand.\n");
        return -2;
    }

    //connect to serverC
    printf("Going to attempt a connection...\n");
    err = connect(to_sSoc, serverResult->ai_addr, sizeof(struct sockaddr_in));
    if (err == -1){
        printf("Connection failure.\n");
        return -3;
    }

    printf("Connected Successfully!\n");

    freeaddrinfo(serverResult);

    //send first message to ensure things are working properly
    err = send(to_sSoc, "Greetings!", strlen("Greetings!") + 1, 0);
    if (err != -1)
        printf("Sent message to server.\n");
    else {
        printf("Sending message failed.\n");
        return -4;
    }

    //recieve message right back to ensure proper functioning
    printf("Attempting to recieve a reply...\n");
    char buffer[100];
    err = 0;
    int totalBytes = 0;
    do {
        err = recv(to_sSoc, buffer, 100, 0);
        if (err != -1)
            totalBytes += err;
    }while (err == -1 && errno == 107 && totalBytes < 4);
    if (err == -1){
        printf("Client failed to read from server.\n");
        printf("Errno: %d\n", errno);
        return -4;
    }

    cout<<"Message from server: "<<buffer<<"\n";

    printf("Waiting for ServerG startup...\n");
    //sleep(5);

    //connecting with ServerG
    printf("Sending message to ServerG...\n");

    err = send(to_sSoc, "Hello ServerG!", strlen("Hello ServerG") +1, 0);
    if (err == -1){
        printf("Sending to serverG failed!\n");
    }

    printf("Message sent.\n");

    //getting message back froms serverG
    totalBytes = 0;
    do {
        err = recv(to_sSoc, buffer, 100, 0);
        if (err != -1)
            totalBytes += err;
    }while (err == -1 && errno == 107 && totalBytes < 4);
    if (err == -1){
        printf("Client failed to read from server.\n");
        printf("Errno: %d\n", errno);
        return -4;
    }

    cout<<"Message from server: "<<buffer<<"\n";

    string move;
    stringstream ss;
    string leave;

    //core program loop
    while (true){
        //play msg
        sleep(1);
        do {
            err = recv(to_sSoc, buffer, 100, 0);
            if (err != -1)
                totalBytes += err;
        }while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            printf("Client failed to read from server.\n");
            printf("Errno: %d\n", errno);
            return -4;
        }

        cout<<"Message from serverG: "<<buffer<<"\n";


        //send move to server
        cout<<"Do 'r' for rock, 'p' for paper, 's' for scissors.\n";
        cin>>move;

        ss.clear();
        memset(buffer, 0, sizeof(buffer));
        ss<<move;
        ss>>buffer;

        err = send(to_sSoc, buffer, strlen(buffer) +1, 0);
        if (err == -1){
            printf("Sending to serverG failed!\n");
            return -5;
        }


        //recieve outcome
        memset(buffer, 0, sizeof(buffer));
        totalBytes = 0;
        do {
            err = recv(to_sSoc, buffer, 100, 0);
            if (err != -1)
                totalBytes += err;
        }while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            printf("Client failed to read from server.\n");
            printf("Errno: %d\n", errno);
            return -4;
        }

        cout<<"Message from serverG: "<<buffer<<"\n";

        sleep(1);
        //recieve scoreboard
        totalBytes = 0;
        memset(buffer, 0, sizeof(buffer));
        do {
            err = recv(to_sSoc, buffer, 100, 0);
            if (err != -1)
                totalBytes += err;
        }while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            printf("Client failed to read from server.\n");
            printf("Errno: %d\n", errno);
            return -4;
        }

        cout<<"Message from serverG: "<<buffer<<"\n";

        sleep(1);

        //recieve private msg from server to see if games are over
        memset(buffer, 0, sizeof(buffer));
        totalBytes = 0;
        do {
            err = recv(to_sSoc, buffer, 100, 0);
            if (err != -1)
                totalBytes += err;
        }while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            printf("Client failed to read from server.\n");
            printf("Errno: %d\n", errno);
            return -4;
        }

        ss.clear();
        ss<<buffer;
        ss>>leave;

        if (leave == "exit"){
            break;
        }
    }
}