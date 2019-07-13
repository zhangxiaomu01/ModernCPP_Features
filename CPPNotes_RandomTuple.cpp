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

/* 2. C++ 11 Random Number Distribution */
int main(){
    unsigned int seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    //The range of the generated number is between [e.min(), e.max()]
    cout << e() << endl; 

    //If we want to generate a random number from the range [0, 5]
    cout << e() % 6 << endl;
    // Issues with the above code:
    /* 1. Bad quality of randomness. The default engine can produce the 
    *  random number pretty good between [e.min(), e.max()]. With % 
    *  operation, this randomness does not holds.
    *  2. Can only provide a uniform distribution! 
    */

    //Uniform distribution between [0, 5], inclusive
    uniform_int_distribution<int> distr(0, 5);
    //Now the random engine provides the randomness and distribution 
    //object provides the distribution!
    cout << distr(e) << endl; 

    //Here Range is [0, 5), 5 not included
    uniform_real_distribution<float> distrR(0, 5);
    cout << distrR(e) << endl; 

    //poisson distribution
    poisson_distribution<int> distrP(1.0); // 1.0 is the mean
    cout << distrP(e) << endl; 

    cout << "Normal distribution: " << endl;
    //10.0 is mean, 3.0 is standard deviation
    normal_distribution<double> distrN(10.0, 3.0);
    vector<int> v(20);
    for(int i = 0; i < 800; i++){
        int num = distrN(e); //convert double to int!
        if(num >= 0 && num < 20){
            v[num]++; //v[num] represents the frequency of num
        }
    }
    for(int i = 0; i < 20; i++){
        cout <<"  " << std::string(v[i], '+') << endl;
    }
    cout << endl;

    system("pause");
    return 0;
}





