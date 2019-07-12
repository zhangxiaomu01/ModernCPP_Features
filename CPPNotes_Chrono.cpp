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

    
    system("pause");
    return 0;
}


