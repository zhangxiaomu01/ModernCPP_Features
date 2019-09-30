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



