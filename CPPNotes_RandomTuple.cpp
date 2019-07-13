#include<iostream>
#include<string>
#include<random>
#include<vector>
#include<algorithm>
#include<sstream>
#include<chrono>

using namespace std;

/* 1. C++ 11 Random Number Engine */
/*
*Random Engine: 
*Stateful generator that generateds random values within predefined min
*and max. We cannot change the range here. Not truly random -- pseudorandom!
 */
void printRandom(default_random_engine e){
    for(int i = 0; i < 10; i++)
        cout << e() << " ";
    cout << endl;
}

int main(){
    default_random_engine eng;
    //output the min and max values of default random engine. The two values
    //define the range.
    cout << "min value: " << eng.min() <<endl;
    cout << "max value: " << eng.max() <<endl;

    //generate two random numbers
    cout << "random number 1:" << eng() <<endl;
    cout << "random number 2:" << eng() <<endl;

    //Random engine has an internal state which determines which number
    //it should generate. The engine with the same state will always produce
    //the same random number.
	stringstream state;
    state << eng; //save the current state of the engine
    cout << "random number 3:" << eng() <<endl;
    cout << "random number 4:" << eng() <<endl;

    state >> eng; //restore the state we have saved.
    //Now the number 5 and 6 will be exactly the same as number 3 and 4
    cout << "random number 5:" << eng() <<endl;
    cout << "random number 6:" << eng() <<endl;

    //All the default engine will produce the same value;
    default_random_engine e;
    default_random_engine e1;
    //The following code will produce exactly the same result!
    //3499211612 581869302 3890346734 3586334585 545404204 
	//4161255391 3922919429 949333985 2715962298 1323567403
    printRandom(e);
    printRandom(e1);

    unsigned int seed = chrono::steady_clock::now().time_since_epoch().count();
    //with seed, the random engine will be able to generate different 
    //sets of random numbers! It determines the internal state of the engine
    default_random_engine e3(seed);
    printRandom(e3);

    e.seed(); //set engine to initial state
    e.seed(109); //set engine to a state to seed 109

    //Random engine can also be applied to shuffle a vector or deque.
    vector<int> d = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(d.begin(), d.end(), default_random_engine());
    for (int num : d) {
		cout << num << " ";
	}
	cout << endl;

    //C++ stl provides 16 different engine types. 
    //Default engine is a balanced random number generator, reasonable
    //cost and randomization.

    system("pause");
    return 0;
}






