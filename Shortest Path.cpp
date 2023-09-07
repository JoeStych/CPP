//Joseph Steichen
//CSCI 301-01
//4/20/2022

//This program will locate a shortest path between vertices

//headers
#include <iostream>
#include <fstream>
using namespace std;

//constant variables
static const int CAPACITY = 20;
int edges;

//floyd's algorithm
void floyd (int g[][CAPACITY], int a[][CAPACITY], int p[][CAPACITY]){
    int i, j, k;
    //array initializing
    for (i = 0; i<edges; ++i){
        for (j = 0; j<edges; ++j){
            a[i][j] = g[i][j];
        }
    }
    //finding shortest path lengths and paths
    for (k=0; k<edges; ++k){
        for (i=0; i<edges; ++i){
            for (j=0; j<edges; ++j){
                if (a[i][k] + a[k][j] < a[i][j]){
                    a[i][j] = a[i][k] + a[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
}

//the pathfinder
void path (int p[][CAPACITY], int i, int j){
    int k;
    k = p[i][j];
    if (k > -1){
        path(p, i, k);
        cout<<k<<' ';
        path(p, k, j);
    }
}

//main code
int main (){
    //reading the file
    ifstream file;
    file.open("D:\\Desktop\\Projects\\Code\\C++\\DGraph2.txt");
    int graph[CAPACITY][CAPACITY], g[CAPACITY][CAPACITY], p[CAPACITY][CAPACITY];
    file>>edges;
    for (int i=0; i<(edges); i++){
        for (int j=0; j<(edges); j++){
            file>>graph[i][j];
        }
    }

    //p array initializing
    for (int i=0; i<CAPACITY; i++){
        for (int j=0; j<CAPACITY; j++){
            p[i][j] = -1;
        }
    }

    //executing floyd's
    floyd (graph, g, p);

    //displaying the graphs
    cout<<"Original\n";
    for (int i=0; i<(edges); i++){
        for (int j=0; j<(edges); j++){
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"floyd's\n";
    for (int i=0; i<(edges); i++){
        for (int j=0; j<(edges); j++){
            cout<<g[i][j]<<" ";
        }
        cout<<endl;
    }

    //menu setup
    int choice1, choice2;
    while (choice1 > 0){
        cin>>choice1;
        //quitting if user inputs -1
        if (choice1 < 0)
            continue;
        
        cin>>choice2;
        cout<<choice1<<' ';
        path(p, choice1, choice2);
        cout<<choice2;
        cout<<endl;
    }
    cout<<"Goodbye.\n";
}