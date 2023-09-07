//Joseph Steichen
//CSCI 301-01
//2/4/2022

/* This program creates a list that can be manipulated in various ways by the
user. The list maintains an order from smallest to largest. */

//Headers
#include <iostream>
#include <string>
using namespace std;

//define variable type data as integer
typedef int data;

//list class
class list {
    private:
        struct node{//structure for nodes in the linked list
            data data;
            node *next;
        };
        node *first;
        node* get_node(data, node* const &);//a handy function to create nodes
    public:
        list() {first = NULL;}
        ~list();
        void make_empty();
        void insert(data);
        void remove(data);
        bool empty() {return first == NULL;}
        int length();
        bool value_present(data) const; //Report whether or not a particular value is present in a List.
        data value_k(int);//Return the value of the Kth element of a List.
        void print();
};

void menu(){//menu was contained in a function for simplicity
    cout<<"e -- make the list empty again.\n";
    cout<<"a X -- add value to the list.\n";
    cout<<"d X -- delete the value from the list.\n";
    cout<<"b -- check if list is empty.\n";
    cout<<"l -- displays the length of the list.\n";
    cout<<"c X -- check if value X is present in the list\n";
    cout<<"k X -- checks the Xth number in the list. (i.e. 'k 3' displays the third list value.\n";
    cout<<"s -- display the entire list.\n";
    cout<<"h -- display this menu again.\n";
    cout<<"q -- quits the application.\n";
}

int main(){
    list list;//the list of the program
    char choice;//variable that holds the menu choice of the user
    data x;//variable for x as needed

    //First time loading instructions
    cout<<"This program allows the user to manipulate a list of integers."<<endl;
    cout<<"The list maintains an order from smallest to largest."<<endl;
    cout<<"Use the menu below to change the list as you please."<<endl;
    cout<<"NOTE: 'X' represents an integer value to be entered in the place of 'X'."<<endl;
    menu();

    //Core program loop
    do {
        cin>>choice;//asking for user's choice at loop start

        switch (choice){//menu switch case
            case 'e'://empty the list and let the user know it was successful
                list.make_empty();
                cout<<"List has been emptied."<<endl;//success mission
                break;

            case 'a'://adds the requested value to the list
                cin>>x;
                list.insert(x);

            case 'd'://removes requested value from the list
                cin>>x;
                list.remove(x);

            case 'b': //checking if list is empty
                //calls the function to check if list is empty, then notifies the user acordingly
                if (list.empty() == true)//check if the list empty
                    cout<<"The list is empty.\n";//if it is, notify the user
                else
                    cout<<"The list is not empty.\n";//notify the user if the list isnt empty
                break;

            case 'l'://checking the length of the list
                cout<<"The length of the list is "<<list.length()<<" nodes.\n";//outputting the length
                break;

            case 'c'://checking for the value of x
                cin>>x;
                if (list.value_present(x) == true)//notify the user of the results
                    cout<<"The value "<<x<<" is present in the list.\n";
                else
                    cout<<"The value "<<x<<" is not present in the list.\n";
                break;

            case 'k'://What is in the kth node?
                cin>>x;
                if (x <= 0){//user entered something invalid
                    cout<<"You must enter a value greater than 0.\n";
                    break;
                }
                else if (x > list.length()){//value of k exceeds the length of the list
                    cout<<"Value inputted for X exceeds list length.\n";
                    break;
                }
                else{//otherwise return the kth element
                    cout<<"The "<<x<<"th element is "<<list.value_k(x);
                    break;
                }

            case 's'://printing all list values
                list.print();
                break;
            case 'h'://display the menu again
                menu();
                break;

            case 'q': //quitting the program
                cout<<"Goodbye."<<endl;//goodbye msg
                break;

            default: //case for invalid inputs
                cout<<"Invalid input. Enter h to view menu controls.\n";
        }
    }while (choice != 'q');//end the loop when user inputs q
}

list::node* list::get_node(data a, node* const &link){//function to vastly simplify creating a new node
    node *temp;
    temp = new node;
    temp->data = a;
    temp->next = link;
    return temp;
}

list::~list(){
    node *temp;
    while (first != NULL){
        temp = first;
        first = first->next;
        delete temp;
    }
}

void list::make_empty(){
    node *temp;
    while (first != NULL){
        temp = first;
        first = first->next;
        delete temp;
    }
}

int list::length(){
    node *temp;
    int count = 0;
    temp = first;
    while (temp != NULL){//count the nodes until there are no more
        count++;
        temp = temp->next;
    }
    return count;
}

bool list::value_present(data x) const{//is x present in the list?
    node *temp;
    temp = first;
    while (temp != NULL && temp->data != x){
        temp = temp->next;
    }
    return temp != NULL;
}

data list::value_k(int k){//what is inside the kth node?
    node *temp;
    temp = first;
    for (int i=1; i < k; i++){
        temp = temp->next;
    }
    return temp->data;
}

void list::print(){//print the list
    node *temp;
    temp = first;
    if (temp == NULL)
        cout<<"There is no data to display."<<endl;
    else{
        while (temp != NULL){
            cout<<temp->data<<" ";
            temp = temp->next;
        }
        cout<<endl;
    }
}

void list::insert(data x){//adds data to the list in the proper position
    node *temp;
    if (first == NULL || x < first->data){
        first = get_node(x, first);
    }
    else {
        temp = first;
        while (temp->next != NULL && temp->next->data < x)
            temp = temp->next;
        temp->next = get_node(x, temp->next);
    }
}

void list::remove(data x){//removes data from the list
    node *temp;
    node *prev = first;
    if (prev->data == x){
        first = first->next;
        delete prev;
    }
    else{
        while (prev->next != NULL && prev->next->data < x)
            prev= prev->next;
        temp = prev->next;
        prev->next = temp->next;
        delete temp;
    }
}