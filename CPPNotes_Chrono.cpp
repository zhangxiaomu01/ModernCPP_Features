#include<iostream>
#include<string>
#include<vector>
#include<chrono> // a precision neutral library to deal with time and date

using namespace std;

/*
Clock: 
1. std::chorono::system_clock: current time according to the system (not
   steady)
2. std::chorono::steady_clock: goes at a uniform rate
3. std::chorono::high_resolution_clock: provides smallest possible tick 
   period

Duration:
    std::chrono::duration<>: represents time duration.	
    duration<int, ratio<1, 1>>: //number of seconds stored in a int
    duration<double, ratio<60,1>> //number of minutes stored in a 
    double

    some predefined time units:
    nanoseconds, microseconds, milliseconds, seconds, minutes, 
    hours...

    system_clock::duration -- duration<T, system clock::period>

    std::chrono::time_point<>: represents a point of time
    00:00 January 1st, 1970 (corordinated Universal Time - UTC) - 
    epoch of a clock

    time_point<system_clock, millisecond>: according to system_clock, 
    the elapsed time since epoch in milliseconds.

    system_clock::time_point -- time_point<system_clock, system_clock::duration>
    steady_clock::time_point -- time_point<steady_clock, steady_clock::duration>


*/

int main(){
    std::ratio<1, 10> r; //1/10
    cout << r.num << "/" << r.den <<endl; 
    //ALl clock frequency is represented by a ratio, we can print the 
    //ratio using the similar approach.
    cout << chrono::system_clock::period::num <<"/" <<chrono::steady_clock::period::den <<endl;
    
    //duration:
    chrono::microseconds mi(2700);//2700 microseconds!
    mi.count(); //get the value 2700 here
    //2700000 nanoseconds. The coverssion works fine here for we are converting
    //low resolution to high resolution
    chrono::nanoseconds na = mi; 
    na.count(); //get the value 2700000 here
    //We need to do the cast here if we want to convert high resolution
    //to low resolution. 2 milliseconds, the value is truncated.
    chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(mi);
    
    mi = mill + mi; //4700

    //Time point//
    //current time of system clock
    chrono::system_clock::time_point tp = chrono::system_clock::now();
    cout << tp.time_since_epoch().count() << endl;

    tp = tp + chrono::seconds(2);
    cout << tp.time_since_epoch().count() << endl;

    //Measure the time span
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    cout <<"I am bored!!!" <<endl;
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::steady_clock::duration d = end - start;
    if(d == chrono::steady_clock::duration::zero()){
        cout << "No time elapsed!" <<endl;
    }
    cout << chrono::duration_cast<chrono::microseconds>(d).count() <<endl;
    
    system("pause");
    return 0;
}


