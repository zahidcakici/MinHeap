#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
///Zahid Esad Cakici 150190006
using namespace std;

class Vehicle{
public:
    int vehicle_ID;
    string location;
    double distance;
    int speed;
    double keyValue;
    Vehicle(int vehicle_ID, string location, double distance, int speed){
        this->vehicle_ID = vehicle_ID;
        this->location = location;
        this->distance = distance;
        this->speed = speed;
        keyValue = distance / speed;
    }
    Vehicle(const Vehicle &v){
        this->vehicle_ID = v.vehicle_ID;
        this->location = v.location;
        this->distance = v.distance;
        this->speed = v.speed;
        this->keyValue = v.keyValue;
    }
};

class Request{
public:
    string location;
    double distance;
    int lucky_number;
    Request(string location, double distance, int lucky_number){
        this->location = location;
        this->distance = distance;
        this->lucky_number = lucky_number;
    }
};

void Heapify(vector<Vehicle>& Array,int i){
    int left = 2*(i+1) - 1;
    int right = 2*(i + 1);
    int smallest = i;
    if(left < Array.size() && Array[left].keyValue < Array[smallest].keyValue){
        smallest = left;
    }
    if(right < Array.size() && Array[right].keyValue < Array[smallest].keyValue){
        smallest = right;
    }
    if(smallest != i){
        iter_swap(Array.begin()+i,Array.begin()+smallest);
        Heapify(Array,smallest);
    }
}

void buildMinHeap(vector<Vehicle>& Array){
    int heap_size = Array.size();
    for(int i = floor(Array.size()/2); i >= 0; i--){
        Heapify(Array, i);
    }
}

Vehicle Extract(vector<Vehicle>& Array){
    Vehicle root = Array[0];
    Array[0] = Array[Array.size() - 1]; //
    Array.pop_back();
    Heapify(Array,0);
    return root;
}

void Decrease(vector<Vehicle>& Array, int i){
    Array[i].keyValue = -1; //we make key value -1 to make it smallest
    while(i > 0 && Array[floor((i - 1)/2)].keyValue > Array[i].keyValue){
        iter_swap(Array.begin() + i, Array.begin()+ floor((i-1)/2));
        i = floor((i - 1) / 2); 
    }

}


void Insert(vector<Vehicle>& Array, Vehicle  toInsert){
    Array.push_back(toInsert);
    int i = Array.size() - 1;
    while(i > 0 && Array[floor((i - 1)/2)].keyValue > Array[i].keyValue){
        iter_swap(Array.begin() + i, Array.begin()+ floor((i-1)/2));
        i = floor((i - 1) / 2);
    }
}

int main(int argc,char* argv[]) {

    int N = stoi(argv[1]);
    vector<Vehicle> vehicles;
    vector<Request> requests;

    string headers;
    string myLine, word,mySecondLine;
    fstream file;

    file.open("vehicles.txt");
    if(file.is_open()) {
        getline(file,headers);

        while (getline(file,myLine)){
            vector<string> vehicleLine;
            istringstream ss(myLine);

            while (getline(ss,word,'\t'))
            {
                vehicleLine.push_back(word);
            }

            Vehicle v(stoi(vehicleLine[0]), vehicleLine[1], stod(vehicleLine[2]),stoi(vehicleLine[3]));
            vehicles.push_back(v);
        }

    }
    buildMinHeap(vehicles);
    file.close();

    string requestsLine, reqHeaders, line2;
    file.open("requests.txt");
    if(file.is_open()) {
        getline(file,reqHeaders);

        while (getline(file,line2)){
            vector<string> requestsLine;
            istringstream ss(line2);

            while (getline(ss,word,'\t'))
            {
                requestsLine.push_back(word);
            }
            Request r(requestsLine[0], stod(requestsLine[1]), stoi(requestsLine[2]));
            requests.push_back(r);
        }

    }

    ofstream fout("call_history.txt");
    clock_t timer;
    timer = clock();
    cout << requests[43877].location << endl;
    for(int i = 0; i < N; i++){
        if(requests[i].lucky_number == 0){
            Vehicle v = Extract(vehicles);
            fout << v.vehicle_ID << endl;
            v.location = requests[i].location;
            v.distance = requests[i].distance;
            v.keyValue = v.distance/v.speed;
            Insert(vehicles, v);
        }
        else{
            Decrease(vehicles, requests[i].lucky_number);
            Vehicle v = Extract(vehicles);
            fout << v.vehicle_ID << endl;
            v.location = requests[i].location;
            v.distance = requests[i].distance;
            v.keyValue = v.distance/v.speed;
            Insert(vehicles, v);
        }
    }
    timer = clock() - timer;
    cout << "Sorted in " <<  ((float)timer)/CLOCKS_PER_SEC*1000 << " milliseconds" << endl;
    fout.close();
    return 0;
}