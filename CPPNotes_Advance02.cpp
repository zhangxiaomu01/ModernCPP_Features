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



//*********************************************************************//
//Section 16: Static Polymorphism
/*
By default, when we say polymorphism, we refer to dynamic polymorphism. However, 
dynamic polymorphism is sometimes costly for the following reasons:
1. Memory cost for virtual table (not significant);
2. Runtime cost for dynamic binding;
3. The main cost is in cache thrashing, i.e., the large and unpredictable jumps 
in execution causes frequent cache misses. In static poly., the calls are 
either exactly predictable (static) or inlined, which reduces cache misses 
significantly, often leading to an order of magnitude improvement in performance.

*/
/*
We will use the static polymorphism to simulate the dynamic polymorphism 
behavior. Then how can we implement static polymorphism? 
1. is-a relationship between base class and derived class;
2. Base class defines a "generic" algorithm that can be used in derived class;
3. The "generic" algorithm is customized by the derived class.

An example about the static polymorphism:
*/
struct TreeNode {
	int val;
	TreeNode* left;
	TreeNode* right;
};

/*
This is so called curiously recurring template pattern (static polymorphism, 
simulated polymorphism)
This is a small demo of TMP: template metaprogramming. The calculation of 
some part is raised in compiler time (in this example, dynamic binding!)
*/
template <typename T> 
class genericParserClass {
public:
//we can now still call the processNode to process specific node
	void processNode(TreeNode* node) {
		static_cast<T*>(this)->processNode(node);
	}
	void parsePreorder(TreeNode* root) {
		if (!root) return;
		processNode(root);
		parsePreorder(root->left);
		parsePreorder(root->right);
	}
};

class specificParser : public genericParserClass<specificParser> {
public:
	void processNode(TreeNode* node) {
		std::cout << "Customode processer called: " << node->val << std::endl;
	}
};

int main() {
	//Build tree here
	TreeNode* root = nullptr;
	specificParser spParser;
	spParser.parsePreorder(root);

	system("pause");
}

/*
Disadvantages about static polymorphism:
The main draw-back of static polymorphism: you cannot make a run-time decision 
on which "derived class" to use. But, yes, code bloat and increased compilation
times is also a big issue.
*/



//*********************************************************************//
//Section 17: Multiple Inheritance
/*
Only recommended to apply multiple inheritance for Interface segregation 
principle. Split large interface into smaller and more specific one so that the
client only need to know the interfaces that are of interest to them.

We can define derived class to derive from multiple pure abstract class:
An abstract class has one or more pure virtual funtion(s).

A pure abstract class contians only pure virtual functions, no data and no 
concrete functions.

Summary:
Derive only from pure abstract classes when using multiple inheritance 
techniques.
*/



//*********************************************************************//
//Section 18: Duality Of Public Inheritance
//- Inheritance of interface
//- Inheritance of implementation
/*
*Types of inheritance in C++:
*- pure virtul function: inherit interface only;
*- Non-virtual public function: inherit both interface and implementation;
*- Impure virtual public function: inherit interface and defult implementation;
*- protected function: inherit implementation only
*
*As a SDE, it's important to separate the concepts of interface and implementation.
*
*
*Implementation inheritance is generally discouraged.
*Guideline for use implementation inheritance:
*1. Do not use inheritance for code reuse, use composition instead;
*2. Minimize the implementation in base classes. Base classes should be thin.
*3. Minimize the level of hierarchies in implementation inheritance.
*/



//*********************************************************************//
//Section 19: Code Reuse - Inheritance vs Composition
/*
Composition is better than inheritance when we want to reuse code:
1. Less code coupling between reused code and reuser of the code
	a. Child class automatically inherits all parent class's public members.
	b. Child class can assess parent class's protected members.
		-Inheritance breaks encapsulation.

2. Dynamic binding
	a. Inheritance is bounded at compile time.
	b. Composition can be bounded either at compile time or runtime.

3. Flexible code construct
	Dog				ActivityManager
	YellowDog		OutdoorManager
	GreyDog			IndoorManager
	...				...
*/



//*********************************************************************//
//Section 20: Namespace and keyword using
//C++ keyword: using
//1. using directive: to bring all namespace members into current scope.
using namespace std;

//2. using declaration
//	a. briang one specific namespace member to current scope
//  b. bring a member from base class to current class's scope
//Example a:
using std::cout;
cout << "I am good.\n";

//Example b:
using namespace std; // case 1, using directive. Global scope
using std::cout; // case 2.a, global scope.

class B{
public:
	void f() { std::cout << "B::f(): " << std::endl; }
	void f(int a) { std::cout << "B::f(a): " << a << std::endl; }
};

//using B::f; //error: can only be used in class scope

class D : private B{ // D is privately derived from B
public:
	void g(){
		using namespace std; // case 1, local scope
		cout << "D::g() is called.\n" << endl;
	}
	void h(){
		using std::cout; //case 2.a, local scope
		cout << "D::h() is called.\n";
	}

	//error: can not be used in the class scope
	//using namespace std;
	//using std::cout;

	// case 2.b, class scope. Bring f(int a) and f() here
	//This is the only one can be used in the class scope. You cannot use it
	//in local scope or global scope!
	using B::f; 
};

int main() {
	D d;
	//will not work if using B::f is not present in D
	d.f();
	system("pause");
}

//Name hiding:
class B{
public:
	void f(int a) { std::cout << "B::f(a): " << a << std::endl; }
};

class F : public B{
public:
	//with this line of code, the following code will work!
	//using B::f
	void f() { std::cout << "F::f()." << std::endl; }
};

int main() {
	F fclass;
	//will not compile because f() will shade f(int a)
	fclass.f(4);
	system("pause");
}



//*********************************************************************//
//Section 21: Koenig Lookup - Argument Dependent Lookup (ADL)
//Example 1:
namespace A{
	struct X { };
	void g(X x){std::cout << "calling g(X x). \n";} 
}

//If we have the global function here, the following code will not work because
//compiler does not know which function to call
//void g(X x){std::cout << "calling g(X x). \n";} 

int main() {
	A:: X x1;
	A:: g(x1);

	//also works because of koenig lookup or argument dependent lookup (ADL)
	//Compiler will search the argument's namespace to find g(X x)
	g(x1);
	system("pause");
}

//Example 2:
class C {
public:
	struct Y{};
	static void h(Y y) { std::cout << "C::h(Y y) called.\n"; }
};

int main() {
	C::Y y1;
	C::h(y1);

	//will not compiler, koenig lookup only works for namespace
	//h(y1);
	system("pause");
}

//Name hiding: namespace example
namespace A{
	struct X {};
	void f(X x1){std::cout << "A::f(X x1).\n";}
	void g(int a){std::cout << "A::g(int a). \n";}

	namespace C{
		void f(){std::cout << "A::C::f().\n";}
		void g(){std::cout << "A::C::g(). \n";}
		void j(){
			//we have to include the A::g here or the g(8) will not compile
			//because of name hiding!
			using A::g;
			g(8);

			//f(x1) will compile because of koenig lookup
			X x1;
			f(x1);
		}
	}
}

int main() {
	A::C::j();
	system("pause");
}

/*
Name loop-up sequence:
----------------------------------------------------------------
With namespaces:
current scope => next enclosed scope => ... => global scope

To override the sequence:
1. Qualifier or using declaration
2. Koenig lookup

----------------------------------------------------------------
With classes:
current class scope => parent class scope => ... => global scope

To override the sequence:
1. Qualifier or using declaration
*/

/*
Why koenig lookup? 
Engineer principle:
	a) Functions that operate on class C and in the same namespace with C are 
	part of C's interface;
	b) Functions that are part of C's interface should be in the same namespace as
	C.
Then by having koenig lookup, we can simplify the code when we want to call the
function to operate on C.
*/


//*********************************************************************//
//Section 22: Demystifying operator new / delete 
//What happened when the following code is executed?

/*
step 1: operator new is called to allocate the memory.
step 2: dog's constructor is called to create dog.
step 3: if step 2 throws an exception, call operator delete to free the memory
		allocated in step 1. (optional)
*/
dog* pd = new dog();

/*
step 1: dog's destructor is called.
step 2: operator delete is called to free the memory.
*/
delete pd;

/*
The following code is a simplified version of standard operator new implementation.
Note: new handler is a function invoked when operator new failed to allocate 
memory. 
set_new_handler() installs a new hanlder and returns current new handler (old handler).
*/
void* operator new(std::size_t size) throw(std::bad_alloc){
	while(true){
		//Allocate memory
		void* pMem = malloc(size);
		//Return the memory if successful
		if(pMem)
			return pMem;
		
		//Get new handler
		std::new_handler Handler = std::set_new_handler(0);
		std::set_new_handler(Handler);

		//Invoke new handler to make more memory available. 
		//So next iteration we will have more memory to allocate
		if(Handler)
			(*Handler)();
		else
			throw std::bad_alloc();
	}
}


/*
Member operator new
*/
class dog{
	//...
public:
	static void* operator new(std::size_t size) throw(std::bad_alloc){
		customizeNewForDog(size);
		//In order to fix the error below, we need to do a check here
		/*
		if(size == sizeof(dog))
			customizeNewForDog(size);
		else
			::operator new(size);
		*/
	}
	static void operator delete(void* pMem) throw(){
		std::cout << "Deleting a dog. \n" << std::endl;
		customizeDeleteForDog();
		free(pMem);
	}

	//One solution to fix our operator delete is to include a virtual destructor
	vitrual ~dog(){}

	//...
}

class yellowDog : public dog{
	int age;
public:
	//Or we overload operator new in yellowDog as well
	/*
	static void* operator new(std::size_t size) throw(std::bad_alloc){
		customizeNewForyellowDog(size);
	}
	*/
	static void operator delete(void* pMem) throw(){
		std::cout << "Deleting a yellow dog. \n" << std::endl;
		customizeDeleteForYellowDog();
		free(pMem);
	}
}

int main(){
	//Error: we will call dog()'s operator new in this case
	yellowDog* pd = new yellowDog();
	dog* pd1 = new yellowDog();
	//when we delete dog, we will invoke dog's customized deleter. However, 
	//we need to delete yellowDog. We cannot add virtual keyword for delete
	//or new function because staic function and virtual function work for 
	//different scopes.
	delete pd1;
}

/*
** Why do we want to customize new/delete:
1. Usage error detection:
	- Memory leak detection / garbage collection;
	- Array index overrun / underrun;
2. Improve efficiency:
	- Clustering related objects to reduce page fault;
	- Fixed size allocation (good for application with so many samll objects);
	- Align similar size objects to same place to reduce fragmentation
3. Perform additional tasks:
	- Fill the deallocated memory with '0's =>security
	- Collect usage statistics.


Writing a good memory manager is HARD!
Before writing your own version of new/delete, consider:
1. Tweak your compiler towards your needs;
2. Search for memory management library, e.g. Pool library from Boost
*/