//Joseph Steichen
//CSCI 301-01
//3/17/2022

/* This program displays the differences of speeds of sorting algorithms: insertion; merge; quick; heap. */

//headers
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

//max amount of items that can be sorted
static const int capacity = 50000;
//max number for number gen
static const int complexity = 100000;

//counting steps for eaching algorithm
int insertSteps = 0;
int mergeSteps = 0;
int quickSteps = 0;
int heapSteps = 0;

//The bulk of heap sort
void reheap_Down(int a[], int parent, int last){
    int max_child;
    bool done = false;
    while (2*parent + 1 < last && !done){
        heapSteps++;
        max_child = 2*parent + 1;

        if (max_child + 1 < last && a[max_child+1] > a[max_child])
            ++max_child;
        
        if (a[max_child] > a[parent]){
            swap(a[parent], a[max_child]);
            parent = max_child;
        }
        else
            done = true;
    }
}

void merge(int a[], int low, int mid, int high){//merge function for merge sort
    int x[capacity];//the copied array
    int i1, i2, index;//indexes

    for (int i=low; i<=high; i++){//copying array elements from low to high
        x[i] = a[i];
    }

    //setting indexes
    i1 = low;
    i2 = mid + 1;
    index = low;

    //going through both arrays
    while (i1 <= mid && i2 <= high){
        mergeSteps++;
        //inserting the bigger number into original array
        if (x[i1] < x[i2]){
            a[index] = x[i1];
            i1++;
        }
        else {
            a[index] = x[i2];
            i2++;
        }
        index++;
    }

    //taking care of remaining values
    while (i1 <= mid){
        mergeSteps++;
        a[index] = x[i1];
        i1++;
        index++;
    }
    while (i2 <= high){
        mergeSteps++;
        a[index] = x[i2];
        i2++;
        index++;
    }
}

void mergeSort(int a[], int low, int high){//merge sort
    int mid;

    if (low < high){
        mid = (low + high) / 2;
        mergeSort(a, low, mid);
        mergeSort(a, mid+1, high);
        merge(a, low, mid, high);
    }
}

void partition(int a[], int low, int high, int pivot, int &i, int &j){
    int lastS1 = low - 1;
    int firstU = low;
    int firstS3 = high + 1;
    
    while (firstU < firstS3){//the partitioning
        quickSteps++;
        if (a[firstU] < pivot){
            ++lastS1;
            swap(a[firstU], a[lastS1]);
            ++firstU;
        }
        else if (a[firstU] == pivot)
            ++firstU;
        else{
            --firstS3;
            swap(a[firstU], a[firstS3]);
        }
    }
    i = lastS1;
    j = firstS3;
}

void quickSort(int a[], int low, int high){//quicksort code
    int pivot;
    int lastS1, firstS3;
    if (low < high){
        pivot = a[low];
        //partition and recurrsive calls
        partition(a, low, high, pivot, lastS1, firstS3);
        quickSort(a, low, lastS1);
        quickSort(a, firstS3, high);
    }
}

int main(){
    int aSize;//how many integers we are sorting
    char p;//whether or not we're printing the values

    //welcome message and asking for necessary information
    cout<<"This program will generate and sort an array of numbers using 4 different sorting algorithims.\n"
    <<"Enter amount of numbers to generate and sort (MAX "<<capacity<<"): ";
    do {cin>>aSize;} while (aSize < 1 || aSize > capacity);//rejects invalid values

    cout<<"Would you like to print the values (y/n)?: ";
    do {cin>>p;} while (p != 'n' && p != 'y');//rejects invalid entries


    int value[capacity];//the original array of values
    int rands = 1;//helps generate random numbers

    //assigning random numbers to the array
    for (int i=0; i<aSize; i++){
        srand(((unsigned)time(0) + (rands++ * rand())));
        value[i] = rand() % complexity;
    }

    //arrays to be sorted
    int insertion[capacity];
    int merges[capacity];
    int quicks[capacity];
    int heaps[capacity];
    
    //filling them all the same way
    for (int i=0; i<aSize; i++){
        insertion[i] = value[i];
        merges[i] = value[i];
        quicks[i] = value[i];
        heaps[i] = value[i];
    }

    //insertion sort
    int h = 0;
    for (int j=0; j<aSize; j++){
        for (int i=j+1; i<aSize; i++){
            insertSteps++;
            if (insertion[i] < insertion[j]){
                h = insertion[j];
                insertion[j] = insertion[i];
                insertion[i] = h;
            }
        }
    }

    //Merge sort
    mergeSort(merges, 0, aSize-1);

    //quick sort
    quickSort(quicks, 0, aSize-1);

    //heap sort
    for (int i=(aSize-2)/2; i>=0; --i)//turning array into bst
        reheap_Down(heaps, i, aSize-1);
    
    for (int i=aSize-1; i>0; --i){//bst to sorted array convertion
        swap(heaps[0], heaps[i]);
        reheap_Down(heaps, 0, i-1);
    }

    //printing the array
    if (p == 'y'){
        cout<<"Original: ";
        for (int i=0; i<aSize; i++){
            cout<<value[i]<<' ';
        }
        cout<<endl;
        cout<<"Insertion: ";
        for (int i=0; i<aSize; i++){
            cout<<insertion[i]<<' ';
        }
        cout<<endl;
        cout<<"Merge: ";
        for (int i=0; i<aSize; i++){
            cout<<merges[i]<<' ';
        }
        cout<<endl;
        cout<<"Quick: ";
        for (int i=0; i<aSize; i++){
            cout<<quicks[i]<<' ';
        }
        cout<<endl;
        cout<<"Heap: ";
        for (int i=0; i<aSize; i++){
            cout<<heaps[i]<<' ';
        }
        cout<<endl;
    }

    //results
    cout<<"Results\n"
    <<"Insertion Steps: "<<insertSteps<<endl
    <<"Merge Steps: "<<mergeSteps<<endl
    <<"Quick Steps: "<<quickSteps<<endl
    <<"Heap Steps: "<<heapSteps<<endl;
}

