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

