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




