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
//const used with function!
class Dog {
	int age;
	std::string name;
public:
	Dog() { age = 1; name = "Dummy"; }
	//const parameters
	//Only meaningful when we pass by reference (instead of by value!)
	void setAge(const int& a) { age = a; }

	//const return value! caller cannot change name!
	//Only meaningful when we return by reference (instead of by value!)
	const std::string& getName() { return name; }

	//const function!
	//This function will never modify any member variables of this class!!
	void printName() const { std::cout << name << "Const" << std::endl; }

	//The next code won't work, const function can only call const function!
	//void printName() const { std::cout << getName() << std::endl; }

	//we can overload the function by constness! When our object is const, it will
	//call const version of printName(); else it will call the following version!
	void printName() { std::cout << name << "Non-Const" << std::endl; }
};


int main() {
	Dog d;
	const Dog d2;
	int i = 9;
	d.setAge(i);

	const std::string dogName = d.getName();

	//will call non-const version of printName()
	d.printName();
	//will call const version of printName()
	d2.printName();

	std::cout << dogName << std::endl;

	system("pause");
	return 0;
}




//*********************************************************************//
//Section 3: Logic Constness and Bitwise Constness
// The idea of "bitwise constness", which basically means that a const class should 
// have exactly the same representation in memory at all times.
class BigArray {
	std::vector<int> v; //huge array
	//making a variable to be mutable, then we can change it in a const function!
	//preferred!
	mutable int accessCounter;
public:
	//The getItem() should be a const function
	int getItem(int index) const {
		accessCounter++;
		return v[index];
	}
};


class BigArray01 {
	std::vector<int> v; //huge array
	//If we do not want to add mutable here!
	int accessCounter;
	int* v2; //another huge array!
public:
	//The getItem() should be a const function
	int getItem(int index) const {
		//also work. However, in general, we should avoid cast!.
		const_cast<BigArray01*>(this)->accessCounter++;
		return v[index];
	}
	//compile will accept this as const, because the value of pointer v2 does not 
	//change (address)
	void setValue(int index, int val) const {
		*(v2 + index) = val;
	}
};

int main() {
	BigArray b;

	system("pause");
	return 0;
}



//*********************************************************************//
//Section 4: Compiler Generated Functions
/*
Compiler silently writes 4 functions if they are not explicitly declared!
1. Copy constructor
2. Copy assignment operator
3. Destructor
4. Default constructor (only if there is no constructor declared)! 

Note:
1. They are public and inline
2. They are generated only if they are needed!
*/
class dog{};
//equivalent to:
class dog{
public:
	dog(const dog& rhs){...} //member by member initialization
	dog& operator=(const dog& rhs){...}//member by member copying
	dog(){...} //1. call base class's default constructor
			   //2. call data member's default constructor
	
	~dog(){...} //1. call data member's destructor
				//2. call base class's destructor
};

//Another example:
/*
1. Copy constructor				-no
2. Copy assignment operator		-yes
3. Destructor					-no
4. Default constructor			-no
*/
class dog {
public:
/*
If we have const member variable or reference member variable, then we cannot 
do a member by member copy any more. At that case, compiler will never generate
copy assignment operator any more!
e.g. const std::string id = "22";
*/
	std::string m_name;
	/*In c++11, we can use 
	dog() = default;
	to continue using the default constructor.
	*/
	dog(std::string name = "Bob") {
		m_name = name;
		std::cout << "Dog " << m_name << " is born!" << std::endl;
	}
	~dog() {
		std::cout << "Dog " << m_name << " is destroyed!" << std::endl;
	}
};

int main() {
	dog d1("Henry");
	dog d2;
	d2 = d1;
	system("pause");
	return 0;
}

//Example 3:
class collar {
public:
	collar(std::string color) {
		std::cout << "collar is born. \n";
	}
};

class dog {
public:
	collar myCollar;
	//The following won't work because dog's default constructor will construct the
	//reference, however, we cannot initialize the reference. We know reference 
	//must be initialized!
	//std::string& s;	
};

int main() {
	//Will not work because collar does not have a default constructor. When we 
	//create dog d1, we will call dog() default constructor, in that constructor
	//we are supposed to call collar default constructor. However, since we already
	//have a parametered constructor in collar, compiler cannot generate the default
	//constructor for both classes any more!
	dog d1;
	system("pause");
	return 0;
}



//*********************************************************************//
//Section 5: Disallow Functions
/*
In C++ 11, we can also use delete key word to disallow a function to be 
generated!
*/
class openFile{
public:
	//disallow copy constructor! (C++ 11)
	openFile(openFile& rhs) = delete;
	//make openFile(openFile& rhs) private for C++ 03
	//private: openFile(openFile& rhs);
	//Disable default constructor!
	openFile(std::string name){
		std::cout << "File opened: " << name << std::endl;
	}
private:
	//A class with private destructor cannot be stored on heap! Because if it
	//is stored on stack, when it goes out of scope, we cannot get access to
	//destructor to destroy the class! 
	//We have to provide destroy() function to invole destructor though. If 
	//destructor is private!
	~openFile(){
		std::cout << "File destroyed!" << std::endl;
	}
};



//*********************************************************************//
//Section 6: Virtual Destructor
/*
Note: For classes from standard template library, most of them do not have 
virtual keyword for destructors. Be careful if you want to inheritate from
these classes. Better to use shared_ptr for derived class if you have to do
such inheritance!
*/
class Dog {
public:
	//In order to make sure that delete derived class will succesfully reclaim
	//all the memory, we need to have virtual key word for all base classes's
	//destructors
	virtual ~Dog() {
		std::cout << "Dog is destroyed!" << std::endl;
	}
};

class YellowDog : public Dog {
public:
	~YellowDog() {
		std::cout << "Yellow dog is destroyed!" << std::endl;
	}
};

class DogFactory {
public:
	static Dog* generateYellowDog() { return (new YellowDog()); }
	static std::shared_ptr<Dog> generateYellowDog01() {
		return std::shared_ptr<Dog>(new YellowDog());
	}
};

void creatDog() {
	Dog* d1 = DogFactory::generateYellowDog();
	delete d1;
	//If we do not want to use virtual keyword, an alternative way is to use 
	//shared_ptr instead of raw pointer. When shared_ptr goes out of scope,
	//the memory reclaim will also be correct. (Note unique_ptr won't work!)
	//For d2, even dog's destructor is not virtual, it will work as well
	std::shared_ptr<Dog> d2 = DogFactory::generateYellowDog01();
}

int main() {
	creatDog();
	system("pause");
	return 0;
}



//*********************************************************************//
//Section 7: Exceptions in Destructor
/*
Prevent exceptions from leaving destructors.
*/
class dog {
public:
	std::string m_name;
	dog(std::string name) {
		m_name = name;
		std::cout << m_name << " is born!" << std::endl;
	}
	~dog() {
		std::cout << m_name << " is destroyed!" << std::endl;
		/*
		If we put the throw logic in destructor. Then our code will have errors. 
		Because when two objects d1 and d2 go out of scope, then the destructors
		will throw 2 exceptions, and C++ does not accept 2 exceptions at the same 
		time!
		*/
		//throw 20;
	}
	void bark() { 
		//...;
	}
};

int main() {
	try {
		dog d1("Henrry");
		dog d2("Jack");
		//If we have throw here, it should work fine. We will only have one 
		//exception
		throw 20;
		d1.bark();
		d2.bark();
	}
	catch (int e) {
		std::cout << "Exception " << e << " has thrown!" << std::endl;
	}
	system("pause");
	return 0;
}

/* 
How to hanle the situation that we want exception in destructor function?
*/
//Solution 1: Encapsule try ... catch in destructor! Instead of separate them to
//different space!
~dog() {
	try {
		//Encapsule all the exception prone code here
	}
	catch(EXCEPTION e){
		//catch exception
	}
}

//Solution 2: move any exception prone code into another function!
class dog01 {
public:
	std::string m_name;
	dog(std::string name) {
		m_name = name;
		std::cout << m_name << " is born!" << std::endl;
	}
	~dog() {
		std::cout << m_name << " is destroyed!" << std::endl;
	}
	void prepareDestroy() {
		//...
		throw 20;
		//...
	}
	void bark() {
		//...;
	}
};

int main() {
	try {
		dog01 d1("Henrry");
		dog01 d2("Jack");
		d1.bark();
		d2.bark();
		d1.prepareDestroy();
		d2.prepareDestroy();

	}
	catch (int e) {
		std::cout << "Exception " << e << " has thrown!" << std::endl;
	}
	system("pause");
	return 0;
}



//*********************************************************************//
//Section 8: Avoid virtual function in constructor / destructor
class dog {
public:
	dog() {
		std::cout << "A dog is born!" << std::endl;
		/*
		Calling virtual function in constructor or destructor should generally be
		avoided! Since in this case, when we call bark(), we will invoke the base
		class bark() because the derived class has not been created. In destructor,
		we will call base class bark() as well since derived class has been already
		been destroyed! It's not good to call a function from a destroyed class.
		*/
		//bark();
	}
	virtual void bark() {
		std::cout << "The dog is barking" << std::endl;
	}
	void seeCat() {
		bark();
	}
	~dog() {
		std::cout << "The dog is destroyed!" << std::endl;
		//bark();
	}
};

class yellowDog : public dog {
public:
	yellowDog() {
		std::cout << "A yellow dog is born!" << std::endl;
	}
	//when bark() is defined as virtual! It implicitly becomes virtual function
	//It's good to add virtual keyword here
	virtual void bark() {
		std::cout << "The yellow dog is barking!" << std::endl;
	}
};

void createDog() {
	yellowDog d;
	d.seeCat();
}

int main() {
	createDog();
	system("pause");
	return 0;
}




//*********************************************************************//
//Section 9: Handling self-assignment
class collar {};
class dog {
public:
	collar* m_collar;
	/* will have problem in two situations:
	1. Self-assignment. We will delete our dog first before assignment!
	2. constructing new collar throws an exception! 
	then m_collar will be nullptr since we already delete it! */
	/*
	dog& operator=(const dog* rhs) {
		delete m_collar;
		m_collar = new collar(*rhs->m_collar);
		return *this;
	}
	*/


	/* solution 1: Do a check before deleting! */
	/*
	dog& operator=(const dog* rhs) {
		//self-assignment safe now
		if (rhs == this)
			return this;

		collar* pOriginalCollar = m_collar;
		//exception safe now!
		m_collar = new collar(*rhs->m_collar);
		delete pOriginalCollar;
		return *this;
	}
	*/

	/* solution 2: Delegation! */
	dog& operator=(const dog* rhs) {
		//Member by member copying or call collar's assignment operator!
		*m_collar = *rhs->m_collar;
		return *this;
	}

};



//*********************************************************************//
//Section 10: Resource acquisition is initialization
/*
Use Objects to manage resources: hardware device, memory, network handle etc.
*/
Mutex_t mu = MUTEX_INITIALIALIZER;
void functionA(){
	Mutex_lock(&mu);
	//do some staff
	Mutex_unlock(&mu); //no guarantee to always excecute
}

/**
 * Solution:
 */
class Lock{
private:
	Mutex_t* m_mu;
public:
	explicit Lock(Mutex_t *pm){
		Mutex_lock(pm);
		m_pm = pm;
	}
	~Lock(){
		Mutex_unlock(pm);
	}
};

void functionA(){
	Lock myLock(& mu);
	// ....
}//The mutex mu will always be released when myLock goes out of scope!

/**
 * Conclusion:
 * The only code that can guarantee to execute after an exception is thrown is
 * the destructors of objects residing in the stack.
 * 
 * Resource management therefore has to be tied to the lifespan of suitable
 * objects in order to gain automatic deallocation and reclamation.
 */


//extra Example 1:
/*
Another good example of RAII: tri: shared_ptr
*/
int functionA(){
	std::shared_ptr<dog> pd(new dog());
	//... Other staff 
}
//when pd goes out of scope, the resources will be reclaimed!

//extra Example 2:
class dog;
class trick;
void trainDog(dog& d, trick& t){}
trick getTrick();
int main(){
	train(shared_ptr<dog>(new dog()), getTrick());
}
//What happened in train()'s parameter passing
/*
1. new dog()
2. getTrick()
3. construct: shared_ptr<dog>
The order of these operations are determined by compiler!
It will be possible if we first execute new dog(), then getTrick().
When executing getTrick(), an exception is thrown and shared_ptr can never
be able to constructed, and new dog() will cause memory leak!

Conclusion: do not combine storing objects in shared pointer with other 
statement.
*/ 


//extra Example 3:
/*
What happend when resource management objects get copied?
*/
/* Solution 1: disallow copying. Disallow compiler generated functions. */

/* Solution 2: reference count the underlying resource by using shared_ptr.
Hard to use! */
template<class other, class D> shared_ptr(other* ptr, D* deleter);
//By default the deleter is "delete"
std::shared_ptr<dog>(new dog());
//So we can utilize the feature of the deleter function:
class Lock{
private:
	std::shared_ptr<Mutex_t> m_mu;
public:
	explicit Lock(Mutex_t *pm): m_mu(pm, Mutex_unlock){
		Mutex_lock(pm);
		//Now when all the objects of Lock class goes out of scope, then m_mu
		//will call deleter function Mutex_unlock() to unlock the mutex!
	}
};

