#include <iostream>
#include <string.h>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include "tokenheader.h"
#include <pthread.h>
#include <ctime>
#include <cstdlib>
using namespace std;

//shared variables between threads
static const int DINERS = 5;
bool wantEat = false;
bool eatAllowed = false;
bool done = false;
bool alldone = false;

//struct to pass params to thread function
struct threadParams {
    int p1;
    int p2;
    int p3;
};

void tokenring(int nextSoc, int prevSoc, int philosID){
    //token thread
    cout<<"Phil "<<philosID<<": "<<"Thread Started.\n";
    stringstream ss;
    char buffer[50];
    string converter;
    char eaters[5];
    int totalBytes;
    bool token = false;
    int err;
    bool success;

    //intializing eaters
    for (int i=0; i<5; i++){
        eaters[i] = 'n';
    }

    //giving the token to philosopher 0
    if (philosID == 0){
        token = true;
        cout<<"Phil "<<philosID<<": "<<"Starting with the token...\n";
    }

    //core token loop
    while (!alldone){
        memset(buffer, 0, sizeof(buffer));
        ss.str("");
        converter = "";
        if (token == true){//I have a token
            if (wantEat && !(eaters[(philosID + 4) % 5] == 'y' || eaters[(philosID + 1) % 5] == 'y')){//do we want to eat and can we eat?
                eatAllowed = true;
                eaters[philosID] = 'y';
            }
            else if (!wantEat && eaters[philosID] == 'y'){//do we want to keep eating?
                if (!done)
                    eaters[philosID] = 'n';
                else
                    eaters[philosID] = 'd';
            }

            //update the token
            for (int i=0; i<5; i++){
                ss<<eaters[i]<<" ";
            }

            //converting string stream to c string
            converter = ss.str();
            memcpy(buffer, converter.c_str(), converter.size() + 1);

            //sending the token to next in line
            err = send(nextSoc, buffer, strlen(buffer) +1, 0);
            if (err == -1){
                return;
            }
            token = false;
            continue;
        }
        else {//I do not have a token
            totalBytes = 0;
            do {//wait for my turn
                err = recv(prevSoc, buffer, 50, 0);
                if (err != -1)
                    totalBytes += err;
            } while (err == -1 && errno == 107 && totalBytes < 12);

            //reading variables from the token
            converter.assign(buffer);
            ss<<converter;
            token = true;

            //reading the token
            for (int i=0; i<5; i++){
                ss>>eaters[i];
            }

            alldone = true;
            for (int i=0; i<5; i++){
                if (eaters[i] != 'd'){
                    alldone = false;
                    break;
                }
            }
        }
    } 
}

//function that passes params to the actual function
void* threadFunction(void* arg) {
    threadParams* args = (threadParams*)arg;

    // Call the original thread function with the correct arguments
    tokenring(args->p1, args->p2, args->p3);

    pthread_exit(NULL);
}

int main(int argc, char **argv){
    //connection variables
    int philosID = atoi(argv[0]);
    int myPort = PORT + philosID;
    bool connected = false;
    char buffer[50];
    int otherPort;

    cout<<"Phil "<<philosID<<": Client Started.\n";


    //attempting connection first
    int err;
    int toNextSoc;

    //initalizing the socket
    toNextSoc = socket(AF_INET, SOCK_STREAM, 0);
    if (toNextSoc == -1){
        cout<<"Phil "<<philosID<<": "<<"Socket creation failure.\n";
        return -1;
    }

    //necessary struct declarations for connecting to next philospher
    struct addrinfo criteria;
    struct addrinfo *serverResult;

    //initalizing criteria object
    memset(&criteria, 0, sizeof(struct addrinfo));
    criteria.ai_family = AF_INET;
    criteria.ai_socktype = SOCK_STREAM;
    criteria.ai_protocol = 0;
    otherPort = PORT + ((philosID + 1) % DINERS);
    sprintf(buffer, "%d", otherPort);
    err = getaddrinfo(NULL, buffer, &criteria, &serverResult);
    if (err == -1){
        cout<<"Phil "<<philosID<<": "<<"Error locating the port at an address.\n";
        return -2;
    }

    //connect to next philospher
    cout<<"Phil "<<philosID<<": "<<"Going to attempt a connection...\n";
    err = connect(toNextSoc, serverResult->ai_addr, sizeof(struct sockaddr_in));
    if (!(err == -1)){
        cout<<"Phil "<<philosID<<": "<<"Connected to the next philospher.\n";
        connected = true;
    }
    else {
        cout<<"Phil "<<philosID<<": "<<"Connection has failed. Will try to connect again after setting up connection for previous.\n";
    }

    cout<<"Phil "<<philosID<<": "<<"Now setting up server for previous philospher to connect to.\n";

    //establish personal socket for connection from previous philospher
    //socket variables
    int fromPrevSocSetup;
    int fromPrevSoc;
    socklen_t cSocLen;

    //forming the socket
    fromPrevSocSetup = socket(AF_INET, SOCK_STREAM, 0);

    if (fromPrevSocSetup == -1){
        cout<<"Phil "<<philosID<<": "<<"Socket creation failure.\n";
        return 0;
    }

    //necessary structs and initialization for recieving a connection
    struct sockaddr_in cAddr;
    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(myPort);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    //binding socket to server
    do {
        err = bind(fromPrevSocSetup, (struct sockaddr*) &sAddr, sizeof(cAddr));
        if (err == -1){
            cout<<"Phil "<<philosID<<": "<<"Failed to bind socket to server.\n";
            sleep(1);
        }
    }while (err == -1);

    //setting up listen queue
    cout<<"Phil "<<philosID<<": "<<"Listening for client...\n";
    err = listen (fromPrevSocSetup, 5);
    if (err == -1){
        cout<<"Phil "<<philosID<<": "<<"Listening failed\n";
        return -2;
    }

    //accept a connection
    cout<<"Phil "<<philosID<<": "<<"Waiting to accept connection...\n";
    fromPrevSoc = accept(fromPrevSocSetup, (struct sockaddr*) &cAddr, &cSocLen);
    if (fromPrevSoc == -1){
        cout<<"Phil "<<philosID<<": "<<"Acceptance failure.\n";
        cout<<"Phil "<<philosID<<": "<<"Errno:" << errno;
        return -3;
    }

    cout<<"Phil "<<philosID<<": "<<"Connection accepted.\n";

    //if we haven't connected to the next philospher, we are doing that here
    if (connected == false){
        do {
            cout<<"Phil "<<philosID<<": "<<"Now going to connect to next philospher.\n";
            err = connect(toNextSoc, serverResult->ai_addr, sizeof(struct sockaddr_in));
            if (err == -1){
                cout<<"Phil "<<philosID<<": "<<"Connection failed. Trying again...\n";
                sleep(1);
            }
            else {
                cout<<"Phil "<<philosID<<": "<<"Successful connection has been made.\n";
                connected = true;
            }
        }while (err == -1);
    }

    cout<<"Phil "<<philosID<<": "<<"has now been connected to previous and next philosphers in the ring.\n";

    //letting every philospher have a chance to finish connecting
    sleep(2);

    //send next philosopher in line my philosID
    memset(buffer, 0, sizeof(buffer));
    cout<<"Phil "<<philosID<<": "<<"Sending ID to next philospher in line...\n";
    sprintf(buffer, "%d", philosID);

    err = send(toNextSoc, buffer, strlen(buffer) +1, 0);
        if (err == -1){
            cout<<"Phil "<<philosID<<": "<<"Message failed.\n";
            return -1;
    }

    //recieve message from previous to ensure everything is working correctly
    memset(buffer, 0, sizeof(buffer));
    int totalBytes = 0;
    do {
        err = recv(fromPrevSoc, buffer, 50, 0);
        if (err != -1)
            totalBytes += err;
    } while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            cout<<"Phil "<<philosID<<": "<<"Error recieving.\n";
            cout<<"Phil "<<philosID<<": "<<"Errno: "<< errno << endl;
            return -4;
    }

    cout<<"Phil "<<philosID<<": "<<"Recieved message from Philospher "<<buffer<<".\n";

    //we'll need these to convert strings
    stringstream ss;
    string converter;


    int count = 0;
    wantEat = true;
    pthread_t t;
    threadParams params;
    params.p1 = toNextSoc;
    params.p2 = fromPrevSoc;
    params.p3 = philosID;
    pthread_create(&t, NULL, threadFunction, (void *)&params);
    int randomTime;
    int rands = 4;//variable used for random number generation

    //begin the exchange
    while (count < 7){
        //new random times every loop iteration
        srand(time(NULL) + (rands++ * 3));
        randomTime = (rand() % 5) + 3;
        if (wantEat == true){//do I want to eat?
            while (wantEat){
                if (eatAllowed){//am I allowed to eat?
                //eating
                    cout<<"Phil "<<philosID<<": "<<"I am eating now...\n";
                    sleep(randomTime);
                    cout<<"Phil "<<philosID<<": "<<"Finished eating. Thinking now...\n";
                    count += 1;
                    //check to see if im done eating
                    if (count >= 7){
                        done = true;
                        cout<<"Phil "<<philosID<<": "<<"I have finished doing everything. Waiting for others to finish...\n";
                    }
                    wantEat = false;
                    eatAllowed = false;
                    break;
                }
                else {
                    //wait a second and check if its my turn to eat again
                    sleep(1);
                }
            }
        }
        else {
            sleep(randomTime);
            cout<<"Phil "<<philosID<<": "<<"I am done thinking. I want to eat now.\n";
            wantEat = true;
        }
    }

    pthread_join(t, NULL);

    cout<<"Phil "<<philosID<<": "<<"Everyone has finished. Shutting down now...\n";
}