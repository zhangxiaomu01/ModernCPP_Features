/* 1. C++ 11: R-value Reference */
/*
    1. Moving sementics
    2. Perfect forwarding
 */

/****************************************************************************/
//Warm-up: What are R-value and L-value? Why should we care?
/*
    - Understanding C++ construct, and decypher compiler errors or warnings
    - C++ 11 introduced R-value reference

    Simplified definitions:
    - L-value: An object that occupies some identifiable location in memory
    - R-value: Any object that is not L-value
 */
//L-value examples:
int i = 3; // L-value
int* p = &i; //Can get i's address
i = 5;
class dog;
dog d1; //L-value
//=============================//
//R-value examples:
int k = 2; //2 is R-value
int x = i + 3; // i + 3 is R-value
int *p = &(i+3); //error
i + 2 = 4; //error
2 = i; //error
dog d2;
d2 = dog(); //dog() is user defined R-value
int sum(int x, int y){return x + y;}
int total = sum(3, 6); //sum(3, 6) is R-value
//================================//
//Reference
int i = 5;
int &r = i; //reference
int &q = 6; //Error
//Exception: constant L-value reference can be assigned a R-value
const int &p = 8;
//================================//
//Function reference
int k = 8;
int func(int &x){...}
func(k); //OK
func(23);//Error
int func(const int &x){...} // Now both func(23) and func(k) will work
//================================//
//L-value can be used to create an R-value
int i = 2;
int j = i + 2;
int k = i; //A L-value can be implicitly transferred to an R-value, we cannot do reverse
//R-value can be used to create a L-value
int v[3];
*(v+2) = 4; //Note v+2 is R-value, however, *(v+2) is L-value
//================================//
//Misconception 1: functions or operators always produce R-values
int x = y + 3; // R-value
int y = sum(7, 8); //R-value
int myGoal;
int& func(){
    return myGoal;
}
func() = 89; //func() is L-value
A[8] = 7; //[]operator produces an L-value
//Misconception 2: L-values are always modifiable. 
const int c = 9; //L-value
c = 4; //error
//Misconception 3: R-values are not modifiable.
i + 3 = 6; //Error
sum(6,7) = 0; //Error
//R-value can be modified in user defined type
class dog;
dog().bark(); // bark() might change the dog object state
//========================================//
//Summary
//1. Every C++ expression yields either a L-value or R-value
//2. If the expression has an identifiable memory address, it's L-value. Or it's R-value

/****************************************************************************/
//What is R-value reference?
//Example 1:
//We can use L-value and R-value type to do the function overloading
void printOut(int& i){cout << "L-value reference;"<<endl; }
void printOut(int&& i){cout << "R-value reference;"<<endl;}
//void printOut(int i){cout << "R-value reference;"<<endl;} // Not valid here, compiler cannot tell the ambiguity
int main(){
    int a = 7; //L-value
    int &b = a; //L-value reference
    int &&c = 9; //R-value reference
    printOut(a); //Call printOut(int &i), pass by L-value reference
    printOut(7); //Call printOut(int &&i),pass by R-value reference
}
//Example 2:
class TestC{
private:
    int m_Size;
    double * m_Double;
public:
    TestC(const TestC& C){ //Can accept R-value because of the const here
        m_Size = C.m_Size;
        m_Double = new double[m_Size];
        for(int i = 0; i < m_Size; i++){
            m_Double[i] = C[i];
        }
    }
    TestC(TestC&& C){ //move constructor, we remove const here, because we need to change C
        m_Size = C.m_Size;
        m_Double = C.m_Double; //Move the existing content to current array
        C.m_Double = nullptr; //We need this here, because we need to prvent the destructor clean up the memory
    }
    ~TestC(){
        delete []m_Double;
    }
};

void foo(TestC cC);
TestC createTestC(); // Creates a new TestC object
void foo_byRef(Test &cC);

void main(){
    TestC reusable = createTestC();
    foo_byRef(reusable); //The cheapest operation, no constructor involved
    foo(reusable); //Will call the copy constructor
    //Reusable will not be used any more
    foo(std::move(reusable)); //Move the resuable to foo function with move constructor. The pointer in Reusable is nullptr after the std::move()
    //Do not use the resuable after the move 
    foo(createTestC()); //will call move constructor, since the return value is an R-value
}
//==================================================================//
//Note 1: the most useful of R-value reference is to overload copy construtor
//and copy assignment operator. We can achieve moving sementics by using R-value reference.
X& X::operator=(X const & rhs);
X& X::operator=(X && rhs);

//Note 2: Move sementics have been implemented for all STL containers.
//In C++ 11, your code will be faster without changing anything;
//Passing by value can be used for STL containers
vector<int> foo(){
    //...
    return myVector;
}
void hoo(string s);
bool goo(vector<int>& v);
//==================================================================//
//Summary:
/*
Move constructor/move assignment constructor:
Purpose: conveniently avoid costly and unnecessary deep copy
1. They are particularly powerful where passing by value and passing by reference are both used;
2. Theu give you finer control of which part of your object to be moved.
 */



