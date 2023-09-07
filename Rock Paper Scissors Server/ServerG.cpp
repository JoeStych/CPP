#include "proj3header.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]){
    //getting the socket descriptor from argument
    stringstream ss;
    int from_cSoc;
    char x[15];
    ss<<argv[0];
    ss>>from_cSoc;
    
    //recieve message from client to ensure everything is working
    printf("\nServerG here, about to try to recieve msg from client...\n");
    printf("Inherited SD: %d from serverC.\n", from_cSoc);
    sleep(2);

    
    char buffer[100];
    int err = 0;
    int totalBytes = 0;
    do {
        err = recv(from_cSoc, buffer, 100, 0);
        if (err != -1)
            totalBytes += err;
    } while (err == -1 && errno == 107 && totalBytes < 4);
    if (err == -1){
        printf("Error recieving.\n");
        printf("Errno: %d\n", errno);
        return -4;
    }

    cout<<"Message from client: "<<buffer<<"\n";

    //deciding how many games to play and let the client know
    srand((unsigned int)time(0));
    int r = rand() % 4;
    int games;
    if (r == 1)
        {games = 3;
        err = send(from_cSoc, "We're going to play 3 games.", strlen("We're going to play 3 games.") +1, 0);}
    else if (r == 2)
        {games = 5;
        err = send(from_cSoc, "We're going to play 5 games.", strlen("We're going to play 3 games.") +1, 0);}
    else if (r == 3)
        {games == 7;
        err = send(from_cSoc, "We're going to play 7 games.", strlen("We're going to play 3 games.") +1, 0);}
    else
        {games = 1;
        err = send(from_cSoc, "We're going to play 1 game.", strlen("We're going to play 1 game.") +1, 0);}
    

    if (err == -1){
        printf("Failed to tell client amount of games to play.\n");
        return -1;
    }

    //important variables for the core program loop
    int rands = 4;      //used to generate random numbers on demand
    int g = 0;          //the number of games we've played
    int myMove;         //serverG's move
    int myWins = 0;     //serverG's wins
    int cWins = 0;      //client's wins
    char cMove;         //client's mvoe
    char betweenmsg[100];//buffer for the socreboard




    printf("Starting the game...\n");



    //core program loop
    while (g <= games){
        //send first message
        err = send(from_cSoc, "What is your move?", strlen("What is your move?") +1, 0);

        if (err == -1){
            printf("Message failed.\n");
            return -1;
        }

        //recieving player move
        do {
        err = recv(from_cSoc, buffer, 100, 0);
        if (err != -1)
            totalBytes += err;
        } while (err == -1 && errno == 107 && totalBytes < 4);
        if (err == -1){
            printf("Error recieving.\n");
            printf("Errno: %d\n", errno);
            return -4;
        }

        sleep(1);

        //generating serverG's move
        srand((unsigned int)time(0) * rands++);
        myMove = rand() % 3;
        ss.clear();
        ss<<buffer;
        ss>>cMove;

        printf("Player move recieved. Finding outcome...\n");

        //determining outcome and sending a message accordingly
        if (cMove == 'r' && myMove == 1){
            printf("Player did rock. I did paper. I win.\n");
            err = send(from_cSoc, "Player did rock. I did paper. I win.", strlen("Player did rock. I did paper. I win.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            myWins++;
        }
        else if (cMove == 'p' && myMove == 2){
            printf("Player did paper and I did scissors. I win.\n");
            err = send(from_cSoc, "Player did paper and I did scissors. I win.", strlen("Player did paper and I did scissors. I win.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            myWins++;
        }
        else if (cMove == 's' && myMove == 0){
            printf("Player did scissors and I did rock. I win.\n");
            err = send(from_cSoc, "Player did scissors and I did rock. I win.", strlen("Player did scissors and I did rock. I win.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            myWins++;
        }
        else if (cMove == 'r' && myMove == 2){
            printf("Player did rock and I did scissors. I lose.\n");
            err = send(from_cSoc, "Player did rock and I did scissors. I lose.", strlen("Player did rock and I did scissors. I lose.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            cWins++;
        }
        else if (cMove == 'p' && myMove == 0){
            printf("Player did paper and I did rock. I lose.\n");
            err = send(from_cSoc, "Player did paper and I did rock. I lose.", strlen("Player did paper and I did rock. I lose.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            cWins++;
        }
        else if (cMove == 's' && myMove == 1){
            printf("Player did scissors and I did paper. I lose.\n");
            err = send(from_cSoc, "Player did scissors and I did paper. I lose.", strlen("Player did scissors and I did paper. I lose.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }

            g++;
            cWins++;
        }
        else if (cMove != 's' && cMove != 'r' && cMove != 'p'){
            printf("Client sent incorrect command!\n");
            err = send(from_cSoc, "Client sent incorrect command!", strlen("Client sent incorrect command!") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }
        }
        else {
            printf("We both did the same action.\n");
            err = send(from_cSoc, "We both did the same action.", strlen("We both did the same action.") +1, 0);

            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }
        }

        printf("Outcome sent to client.\n");
        sleep(2);


        //send scoreboard
        ss.clear();
        memset(buffer, 0, sizeof(buffer));
        memset(betweenmsg, 0, sizeof(betweenmsg));

        printf(betweenmsg, "We have played %d games. You have won %d times and I have won %d times.", g, cWins, myWins);
        sprintf(betweenmsg, "We have played %d games. You have won %d times and I have won %d times.", g, cWins, myWins);


        err = send(from_cSoc, betweenmsg, strlen(betweenmsg) +1, 0);
        if (err == -1){
            printf("Message failed.\n");
            return -1;
        }
        printf("Scoreboard sent to client.\n");



        //private msg client to tell if game is over
        sleep(2);
        memset(buffer, 0, sizeof(buffer));
        if ((games != 1) && (!((games / 2) < myWins) && !((games / 2 < cWins)))){
            printf("We have played %d games. Starting another...\n", g);
            err = send(from_cSoc, "Another", strlen("Another") +1, 0);
            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }
        }
        else {
            err = send(from_cSoc, "exit", strlen("exit") +1, 0);
            if (err == -1){
                printf("Message failed.\n");
                return -1;
            }
        }
        sleep(2);
    }
}