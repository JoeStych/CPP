//Joseph Steichen
//CSCI 301-01
//4/5/2022

//This program takes a graph file and spits back a minimum spanning tree

//headers
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//max number of vertices
static const int CAPACITY = 50;

struct edge {
    int v1, v2, w;//vertex 1, vertex 2, weight
};

void reheap_Down(edge a[], int parent, int last){//modified version of reheap_down to sort edge structs
    int max_child;
    bool done = false;
    while (2*parent + 1 < last && !done){
        max_child = 2*parent + 1;
        if (max_child + 1 < last && a[max_child+1].w > a[max_child].w)
            ++max_child;
        
        if (a[max_child].w > a[parent].w){
            swap(a[parent], a[max_child]);
            parent = max_child;
        }
        else
            done = true;
    }
}

void heapSort(edge a[], int s){//modified version of heapsort to sort edge structs
    for (int i=s/2 - 1; i>=0; i--)//turning array into bst
        reheap_Down(a, i, s);
    
    for (int i=s-1; i>=0; i--){//bst to sorted array convertion
        swap(a[0], a[i]);
        reheap_Down(a, 0, i);
    }
}

class partition {
    private:
        int p[CAPACITY];
    public:
        partition();
        int uf_find(int);
        void uf_union(int, int);
};

int main(){
    partition p;//declaring main partition
    ifstream file;//in file

    string filename;
    cout<<"Enter path to file: ";
    cin>>filename;
    file.open(filename);

    int graphSize;//number of vertices in the graph
    edge e[(CAPACITY * (CAPACITY-1)) / 2];//declaring max amount of edges
    int x, y, z;//vertex 1, vertex 2, weight
    int i = 0;//list position
    file>>graphSize;//number of vertices
    
    while (!file.eof()){//reading the file of edges into the program
        file>>x>>y>>z;
        e[i].v1 = x;
        e[i].v2 = y;
        e[i].w = z;
        i++;
    }

    //sorting edges by weight
    heapSort(e, i);
    
    //Kruskal's Algorithm
    int count = 0;
    int totalCost = 0;
    int k = 0;
    while (count < graphSize-1){
        x = e[k].v1;
        y = e[k].v2;
        z = e[k].w;
        if (p.uf_find(x) != p.uf_find(y)){
            totalCost += z;
            p.uf_union(x, y);
            count++;
            cout<<"Edge = (v"<<x<<", v"<<y<<"); weight = "<<z<<endl;
        }
        k++;
    }
    cout<<"Total Weight: "<<totalCost<<endl;
}

partition::partition(){//partition constructor, sets all values to -1
    for (int i=0; i<CAPACITY; i++)
        p[i] = -1;
}

void partition::uf_union(int x, int y){//forms a union between vertices
    int root = uf_find(x);
    int newChild = uf_find(y);
    p[newChild] = root;
}

int partition::uf_find(int a){//returns the root of a vertex
    int temp = a;
    while (p[temp] >= 0)
        temp = p[temp];
    return temp;
}