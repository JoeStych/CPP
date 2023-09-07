#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tokenheader.h"
using namespace std;

int main(){
    //necessary declarations
    int philosID = 0;
    int childPID;
    int status;
    char buffer[10];
    stringstream ss;
    //announce the port we will be using
    cout<<"Starter has begun and will be using PORT: "<<PORT<<endl;

    //start the clients
    for (int i=0; i<5; i++){
        sleep(1);
        cout<<"Starter: Starting Philosopher "<< i << endl;
        memset(buffer, 0, sizeof(buffer));
        ss.clear();
        childPID = fork();
        if (childPID < 0){
            cout<<"Error creating process "<<i<<endl;
            return -1;
        }
        else if (childPID == 0){
            ss<<i;
            ss>>buffer;
            execl("./tokenclient", buffer, NULL);
        }
    }

    while ((childPID = wait(&status)) != -1) {
        cout<<"Child process exited...\n";
    }
}