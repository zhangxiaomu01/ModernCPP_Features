//*********************************************************************//
//Section 11: Struct vs. Class
//From language perspective:
//struct's member variables are all public (default)
//class's member variables are all private (default)

//From programmer's perspective:
/*
Typically, struct is used for storing small passive objects that carry public
data and have no or few basic member functions. Good for data container!

Class is used for bigger active objects that carry private data, interfaced through
public member functions.
*/
struct dog{
    std::string name;
    int age;
}

class person{
    std::string name_; //naming convention for class "_"
    int age_;
public:
    //Only get access to private members through public interface
    //However, if a class involves too many getters/setters, we might need to
    //consider other design patterns. Maybe group some of the frequently used
    //data to other classes!
    int getAge() const {return age_;} //getter | accessor
    void setAge(int newAge){age_ = newAge;} //setter | mutator
}



//*********************************************************************//
//Section 12: Resource Managing Class
class person {
private:
	std::string* name_;
	

public:
	/* Preferred! 
	Solution 1: Make the copy constructor and copy assignment operator to be
				   deleted. And when we always push_back pointer of the object
				   instead of the object itself*/
	//person(const person& p) = delete;
	//person& operator=(const person& p) = delete;
    //person* colone() { return new person(*name_); } //if you need a copy
	/* When in use: 
	std::vector<person*> persons;
	persons.push_back(new person("John"));
	persons.back()->printName();
	*/


	/* Solution 2: Define copy constructor and copy assignment operator to do the 
	deep copy!*/
	person(const person& p) {
		name_ = new std::string(*p.name_);
	}

	person& operator=(const person& p) {
		name_ = new std::string(*p.name_);
		return *this;
	}

	person(std::string name) {
		name_ = new std::string(name);
	}
	~person() {
		delete name_;
		name_ = nullptr;
	}
	void printName() {
		std::cout << "The person's name is: " << *name_ << std::endl;
	}

};


void createPerson() {
	std::vector<person> persons;

	//We will have problem with following code!
	/*
	When we call push_back() method, 3 things happen:
	1. create a person object "John"
	2. Copy the person to persons container using shallow copy!
	3. person "John" is deleted

	So when we call the printName function, we will be in trouble!
	*/
	persons.push_back(person("John"));
	persons.back().printName();
}

int main() {
	createPerson();

	system("pause");
	return 0;
}



//*********************************************************************//
//Section 13: Virtual Constructor - Clone() Function
//we can define a virtual clone function to make the copy consistant
//we can define a virtual clone function to make the copy consistant
class dog {
public:
	//return a copy of a dog
	virtual dog* clone() {
		return new dog(*this);
	}
};

class yellowDog : public dog {
public:
	//return a copy of a yellow dog
	//note our clone function can have different return type here
	//co-variant return type!
	virtual yellowDog* clone() { 
		return new yellowDog(*this);
	}
};

void foo(dog* d) { // d is a yellow dog
	dog* c = new dog(*d); // now copy makes a yellow dog become a dog!
	
	//d will call yellow dog's clone function and return a yellow dog
	dog* c2 = d->clone(); // now c2 is actually a yellow dog
	//...

}

int main() {
	yellowDog d1;
	// when we pass d1 to foo function, we are actually passing a yellow dog to 
	//foo() function. However, we are initializing c with dog. Copy is copy, this is
	//bad!
	foo(&d1); 

	system("pause");
	return 0;
}




//*********************************************************************//
//Section 14: User Defined Implicit Type Conversion!
/*
Categories of type conversion:
									Implicit				Explicit
Standard type conversion:			   A						B
User defined type conversion: 		   C						D
														    (Casting)
*/
//Type A: Implicit standard type conversion
char c = 'a';
int k = c;  //Integral promotion!

char* pc = 0;  // int to null pointer!

void foo(int i){}
foo(c);

dog* pd = new yellowDog(); //pointer type conversion!


//Type C: Implicit user defined type conversion!
//Defined inside the class (user defined type)
/*
There are 2 ways to define user defined type conversion:
1) Use constructor that can accept a single parameter.
	- Covert other types of objects into your class
2) Use the type conversion function
	- Convert an object of your class into other types
*/
/*
Principle: make the interface easy to use correctly and hard to use incorrectly.
How hard is enough? Ideally, uncompilable!
General guideline:
1. Avoid defining seemingly unexpected conversion;
2. Avoid defining two-way implicit conversion.
*/
class dog {
private:
	std::string name_;
public:
	dog(std::string name) {
		name_ = name;
	} //No explicit convertor
	/*
	If you only want to define a constructor, and no implicit type conversion,
	always put "explicit" keyword before the constructor to avoid inadvertent
	type conversion!
	//explicit dog(std::string name){}
	*/
	std::string getName() {
		return name_;
	}

	operator std::string() const { return name_; }

};

int main() {
	std::string name = "Jack";
	//this will work because we have an implicit type conversion
	dog d1 = name;
	std::cout << d1.getName() << std::endl;

	//If we will use getName() function many times, we can define a type conversion
	//function to convert an object of your class into other types
	std::string d2 = d1;
	std::cout << std::string(d1) << std::endl;
	
	system("pause");
	return 0;
}


/*
A very good example for implicit type conversion is for numeric type of class:
for example, a rational class.
*/

class Rational {
public:
	int num, den;
	//Rational is a constructor, it is also an implicit type converter!
	Rational(int nominator = 0, int denominator = 1) : 
		num(nominator), den(denominator) {

	}
	friend const Rational operator* (const Rational& r1, const Rational& r2); 
};

const Rational operator*(const Rational & r1, const Rational & r2)
{
	return Rational(r1.num * r2.num, r1.den * r2.den);
}

int main() {
	Rational r1 = 3;
	Rational r2 = 6;
	Rational r3 = r1 * 7;
	Rational r4 = 9 * r2;
	Rational r5 = r1 * r2;
	system("pause");
	return 0;
}




//*********************************************************************//
//Section 15: All casting considered!
/*
Type conversion:
	1. Implicit type conversion
	2. Explicit type conversion - casting
*/

//1. static_cast
int i = 9;
//convert one object type to another
float f = static_cast<float>(i);
//type conversion needs to be defined!
dog d1 = static_cast<dog>(string("Jack"));
//convert pointers from one type to a related type! (down / up cast)
dog* pd = staic_cast<dog*>(new yellowDog());


//2. dynamic_cast
//equivalent to static_cast here!
dog* d1 = new yellowDog();

/*
1. dynamic_cast can only work on pointers, not on objects. It converts the 
pointers from one type to another type (down cast!);
2. run-time check. If succeed, y1 == d1; if fails, y1 == nullptr;
3. It requires the two types to be polymorphic (have virtual function(s)).
*/
yellowDog* y1 = dynamic_cast<yellowDog*>(d1);

//3. const_cast
/*
1. Only works on pointer or reference;
2. Only works on the same type;
3. Cast away the constness of the object being pointed to.
*/
const char* str = "Hello World!";
char* modifiedStr = const_cast<char*>(str);


//4. reinterpret_cast
long p = 5110980;
//reinterpret the bits of the object pointed to!
dog* dd = reinterpret_cast<dog*>(p);
//The ultimate cast that can cast one pointer to **any type** of pointer.


/*C - style cast. Inheritate from C*/
short a = 2000;
int i = (int)a; // c-like cast notation
int j = int(a); //functional notation
//A mixture of static_cast, const_cast and reinterpret_cast. (No dynamic cast)

/*
Generally, C++ style casts are preferred over c style cast:
1. Easier to identify in the code;
2. Less usage error. C++ style provides:
	a) Narrowly specified purpose of each cast;
	b) Runtime type check capability.
*/


/* Example with dynamic cast: */
class dog {
private:
	std::string m_name;
public:
	dog(std::string name_){
		m_name = name_;
	}
	//Remember in order to use the dynamic_cast, we need to at least have one 
	//virtual function!
	virtual ~dog() {
		std::cout << "Dog is gone!" << std::endl;
	}

	//cast could be a handy hack tool
	void showDog() const{ //*this become const (by adding const here)
		std::cout << "Dog " << m_name << " is here!" << std::endl;
		//Not allowed, because *this is const!
		//m_name = "Test";
		//If we really to change m_name:
		const_cast<dog*>(this)->m_name = "Henry";
	}
};

class yellowDog : public dog {
	int age = 3;
public:
	void bark() { std::cout << "Whoop!" << std::endl; }
	void bark02() { std::cout << "Whoop!" << age << std::endl; }
};
/* A: 8, B: 16, C: 24, D: 16 */
int main() {
	dog* d1 = new dog();

	yellowDog* yd1 = dynamic_cast<yellowDog*>(d1);

	//If we use the static_cast, d1 will always be converted to yd1. When we 
	//call the bark02() function, we may access the memory does not belong to
	//yd1 (age in this case), and result in undefined behavior!
	//yellowDog* yd1 = static_cast<yellowDog*>(d1);

	yellowDog* yd2 = dynamic_cast<yellowDog*>(d1);

	//Compiler will treat bark() as a static function, so it will work here.
	//(bark() does not involve any member data)
	//We need to check wheather yd1 is nullptr before using it!
	yd1->bark();
	//will crash when we call bark02. Because compiler can no longer treat 
	//bark02() as static function!
	//yd2->bark02();

	//yd1 will be 0, since the dynamic cast fails because d1 is a dog class ptr
	std::cout << "yd1: " << yd1 << std::endl;
	std::cout << "d1: " << d1 << std::endl;
	
	system("pause");
}


//Summary:
/*
===============================C++ style casting ==============================
						Generate	Generate	risky	data type			
						code? 		data
Object casting:
	static cast		  	  yes		  yes		  2	 	Any type (with type conversion)

Pointer casting:
	static_cast			  no		   no		  4     Related types
	dynamic_cast		  yes 		   no         3     Related types (down cast)
	const_cast			  no		   no		  1     Same types
	reinterpret_cast	  no		   no		  5     Any types!

*/






