//Joseph Steichen
//CSCI 301-01
//3/1/2022

/* This program uses a queue to simulate a line at the post office. */

//Headers
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//Maximum servers
static const int maxServers = 15;

//queue class
class queue {
    private:
        struct node {//queue is made of nodes
            int data = 0;
            node *next;
        };
        node *first;
        node *last;
        int count;
    public:
        queue();//constructor
        void enqueue(int);//adding value to queue
        int dequeue();//retreiving queue value
        bool empty() {return count == 0;}//returning t/f whether queue is empty or not
        int size() {return count;}//return queue size
        void make_empty();//make the queue empty
        void print(); //from first to last
        node* get_node (int, node* const &);//useful function
        void increment(int);//adding an amount to the queue. Used for adding wait time.
};

int main(){
    queue q;//the queue
    int serverNum;//amount of servers
    int transTime;//max transaction time
    int arriveOdds;//odds of customer arriving
    int duration;//how long this goes on for

    //program greeting and information gathering
    cout<<"This program simulates a waiting line at the post office.\n";
    cout<<"Enter the amount of servers (MAX "<<maxServers<<"): ";
    do {cin>>serverNum;} while (serverNum < 0 || serverNum > maxServers);
    cout<<"Enter the odds someone shows up per tick (in %): ";
    cin>>arriveOdds;
    cout<<"Enter max amount of ticks a transaction can take: ";
    cin>>transTime;
    cout<<"Enter how many ticks you'd like to run the simulation for: ";
    cin>>duration;

    //necessary declarations
    int tick = 0;//ticks
    int server[maxServers] = {0};//array for time left in transaction per server
    int saved;//saved dequeued values
    int cServed = 0; //customers served
    int totalWaitTime = 0; //total time everyone waited
    int recordWait = 0;//longest time waited
    int rands = 1;//number to help generate random values

    while (tick <= duration){
        //does a customer enter?
        srand(((unsigned)time(0) + rands++));//rands has to be different each time it's used
        if (rand() % 100 <= arriveOdds)
            q.enqueue(0);

        //progress the people being served
        for (int i=0; i<serverNum; i++){
            if (server[i] != 0){
                server[i] -= 1;
                if (server[i] == 0)
                    cServed++;
            }
        }
        
        //assign people in line to servers
        for (int i=0; i<serverNum; i++){
            if (q.empty() == false){
                if (server[i] == 0){
                    saved = q.dequeue();
                    totalWaitTime += saved;
                    if (saved > recordWait)
                        recordWait = saved;
                    srand(((unsigned)time(0) + (rands++ * 3)));//here we multiply rands by 3 simply for variation
                    server[i] = rand() % (transTime + 1);
                }
            }
        }

        //pass the time and display data
        cout<<tick++<<"\t ";
        for (int i=0; i<serverNum; i++)
            cout<<server[i]<<" ";
        cout<<"\t ";
        q.increment(1);
        q.print();
    }
    //outputting results
    cout<<"Results\n-------\n";
    cout<<"Customers served: "<<cServed<<endl;
    cout<<"Average wait time: "<<totalWaitTime / cServed<<endl;
    cout<<"Longest time waited: "<<recordWait<<endl;
    cout<<"Customers still waiting to be served: "<<q.size()<<endl;
}

queue::node* queue::get_node(int a, node* const &link){//function makes ordered lists way easier
    node *temp;
    temp = new node;
    temp->data = a;
    temp->next = link;
    return temp;
}

queue::queue(){//constructor
    first = NULL;
    last = NULL;
    count = 0;
}

void queue::enqueue(int a){//enqueue function
    if (first == NULL)//adding to an empty queue
        {
            first = last = get_node(a, first);
            count++;
        }
    else {//otherwise it's really easy to queue something
        last->next = get_node(a, NULL);
        last = last->next;
        count++;
    }
}

int queue::dequeue(){//dequeue function
    node *temp = first;
    int x = temp->data;//save data into variable so we can return it after temp is deleted
    first = first->next;
    delete temp;
    count--;
    return x;
}

void queue::make_empty(){//make empty function
    node *temp;
    while (first != NULL){//travel to every node and delete it
        temp = first;
        first = first->next;
        delete temp;
    }
    count = 0;//reset count
}

void queue::print(){//print function
    if (first == NULL)
        cout<<"Queue is empty.\n";
    else {
        node *temp = first;
        while (temp != NULL){
            cout<<temp->data<<" ";
            temp = temp->next;
        }
        cout<<endl;
    }
}

void queue::increment(int x){//increment function
    node *temp = first;
    while (temp != NULL){//travel to every node and add to it
        temp->data += x;
        temp = temp->next;
    }
}