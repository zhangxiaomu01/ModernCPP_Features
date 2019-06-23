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


/* 9. Override for virtual functions */
//To avoid inadvertently create new functions in derived class
class dog{
    virtual void A(int a);
    virtual void B() const;
};

class YellowDog : dog{
    /* Explicitly tell the compiler that A and B should override the function from parent class. 
    In this case, since we cannot find any valid function to override. Compiler will issue errors.
     */
    virtual void A(float) override; //error
    virtual void B() override;//error
}


/* 10. final key word for virtual function and class */
class dog final { //No class can be derived from dog any more
    //...
};

class dog {
    virtual void bark() final; //No derived class can override bark() any more
};

/* 11. Compiler generated default constructor */
/*
Typically, compiler will not generate default constructor if we provide one. However, in C++ 11 we can force the
compiler to generate default constructor.
 */
class dog{
public:
    dog(int age){
        //...
    }
};
dog D1; //error: compiler cannot find constructor with no parameters

class dog{
public:
    dog(int age){
        //...
    }
    dog() = default; //Force the compiler to generate a default constructor
};
dog D1; // OK

/* 12. delete key word */
class dog{
    dog(int age){
        //...
    }
};
dog D1(3); //OKay, with constructor we provide
dog D2(4.0); //Okay, compiler will convert double to int and call the constructor we provide
D1 = D2; //Okay, compiler will generate an assign constructor override for us
/*
If we want to restrict such situations, let's say users can only give integer as the age, we can restrict compiler by delete key word
 */
class dog{
    dog(int age){
        //...
    }
    dog(double age) = delete;
    dog& operator=(const dog& d) = delete;
};
dog D1(3); //OKay, with constructor we provide
dog D2(4.0); //Compile error
D1 = D2;//Compile error


/* 13. constexpr */
int A[6]; // It's OKay
int Func(){return 3;}
int A[Func() + 3];//Error: Compile does not know the return value at compile time
constexpr int Func(){return 3;}//Force the computation to happen at compile time, the return value is treated as constant value
int A[Func()+ 3];//Okay, an array with length to be 6

//Write a faster program
constexpr int cubed(int x){ return x*x*x; } //Compile time calculation
int k = cubed(26);


/* 14. New string literals */
//C++ 03
char *a = "Hello World";

//C++ 11
char *a = u8"Hello World"; //UTF 8 string
char16_t *b = u"Hello World"; //UTF 16 string
char32_t *c = U"Hello World"; //UTF 32 string
char* d = R"(Hello World)"; //Raw string


/* 15. lambda function */
cout << [](int x, int y){return x + y;}(3, 4) << endl; //print 7
auto f = [](int x, int y){return x + y;};
cout << f(3, 4) << endl; //print 7












