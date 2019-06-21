#include<iostream>
#include<vector>
#include<string>
#include<iterator>
using namespace std;

/* 1. Initializer list */
//Implemented by initilaizer list constructor
vector<int> v = {1, 4, 6, 10};
//All the relevant STL containers have been updated to accept the initializer list (e.g. map, list)

//Customized the initializer constructor for our own classes
//For example:
#include<initializer_list>
class OurStudents{
private:
    vector<int> m_studentID;
public:
    OurStudents(const initializer_list<int> & studentLists){
        for(initializer_list<int>::iterator it = studentLists.begin(); it != studentLists.end(); ++it){
            m_student.push_back(*it);
        }
    }
};

OurStudents OurStudents_T1 = {12, 3, 4};
OurStudents OurStudents_T2{12, 3, 4}; //The same as above


/* 2. Uniform Initialization */
//C++ 03 
class dog{ //Aggregate class or struct
public:
    int age;
    string name;
};
dog d1 = {2, "Jack"}; // Aggregate initialization

//C++ 11 extended the scope of curly brace initialization
class dog{
public:
    dog(int age, string name){
        //...
    }
};
dog d2 = {2, "Jack"}; //As long as we have a matching constructor in our class, this is valid

//All three initialization takes exact the same form, however, from compiler's perspective, they are different
/*
The priority of three forms:
1. Initializer list constructor (first priority)
2. Regular constructor takes appropriate parameters
3. Aggregate initializer 
 */

/* 3. auto type */
vector<int> myVec = {3, 4, 4};
vector<int>::iterator it = myVec.begin();
//===the same as above
auto it = myVec.begin();

/* 4. foreach */
for(int i : myVec){ // Work only with classes which have begin() and end() methods
    cout<<i; // read-only 
}

for(int &i : myVec){
    i++; //
}

/* 5. nullptr */
//Replace NULL in C++03
//Compared with NULL, nullptr always represents a pointer.
void foo(int i){cout << "foo int" << endl;}
void foo(char *i){cout << "foo pointer" << endl;}
int main(){
    foo(NULL); //Ambiguity
    foo(nullptr)// always call foo(char *i)
    return 0;
}

/* 6. enum class */
//C++ 11
enum class apple{green, red};
enum class orange(big, small);
apple A = apple::green;
orange O = orange::big;
if(A == O){//Compile error, you should define ==(apple, orange) first
    //...
}

/* 7. static_assert */
//runtime assert
assert(ptr != nullptr);
//Compile time assert
static_assert(sizeof(int) == 4);

/* 8. Delegating Constructor */
//Won't work in C++, work in Java
class dog{
public:
    dog(){ ... }
    dog(int a){
        dog();
        //Other staff here;
    }
}

//C++ 03
class dog{
public:
    void init(){ ... }; //Add one more function here which will be used in both constructors
    dog(){ init(); }
    dog(int a){
        init();
        //Other staff here;
    }
}
/*Downsides:
1. Cubersome code
2. init() maybe invoked by other 
*/

//C++ 11, limitation: first constructor dog() is always called first
class dog{
    int m_age = 9; // C++ 11 initialize member variable directly
public:
    dog(){ ... }
    dog(int a) : dog() {
        //Other staff here;
    }
}












