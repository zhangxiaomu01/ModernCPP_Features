#include<iostream>
#include<vector>
#include<string>

//Section 1: const
int main() {
	//const
	//a compile time constraint that an object cannot be modified.
	const int k = 9; //cannot modify k anymore

	int i = 10;
	const int* p = &i; //data is const, pointer is not!
	//*p = 12; //invalid!
	p++; //valid!

	int* const p2 = &i; //data is not const, pointer is const!
	*p2 = 12; //valid!
	//p2++; //Invalid

	const int* const p3 = &i;//both data and pointer are const

	//if const is on the left of the *, then data is const
	//if const is on the right of the *, then pointer is const
	int const* p4 = &i; //data is const
	//the same as const int* p = &i;

	//const k1
	const int k1 = 12;
	//cast the const k1 to int
	const_cast<int &> (k1) = 7;

	int k2 = 15;
	static_cast<const int&> (k2);//Now k2 become const!!

	//cast should be generally avoided during programming. Not safe!

	//Why const?
	/*
	a) Guard against inadvertently write to the variable;
	b) Self documenting!
	c) Enable compiler to do more optimization, make code tighter!
	d) Const means that the data can be put in ROM.
	*/

	system("pause");
	return 0;
}



//*********************************************************************//
//Section 2: const and functions





