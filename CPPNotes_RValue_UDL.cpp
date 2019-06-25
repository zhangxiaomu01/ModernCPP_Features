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






