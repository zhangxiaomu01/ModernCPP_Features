#include<iostream>
#include<string>
#include<vector>
using namespace std;
/* 1. C++ 11 Resource Managing Class */
///////////////////////////////////////////////////////////
//C++ 03:
//Person owns the string through string pointer (m_name)
/* A class owns another class (e.g. string) through its pointer also means that this class must be responsible for creating the object and delting the object. */
class Person{
private:
    string* m_name;
public:
    Person(string name) {m_name = new string(name);}
    ~Person(){delete m_name;}
    void printName(){ cout << *m_name << endl;}
};

int main(){
    vector<Person> persons;
    persons.push_back(Person("Jack"));
    //C++ 03:
    /*
    1. "Jack" is constructed;
    2. A copy of "Jack" is saved in the vector persons; (shallow copy)
    3. "Jack" is destroyed.
    The following printName() will cause an error because the memory of p_name has already been deleted!
     */
    persons.back().printName(); //Error here, dereference dangling pointer is invalid! 
    cout << "Goodbye!" << endl;
    system("pause");
    return 0;
}


//Solution 1: Define copy constructor and copy assignment operator for deep copying! 
class Person {
private:
	string* m_name;
public:
	Person(string name) { m_name = new string(name); }
	Person(const Person& rhs) { m_name = new string(*rhs.m_name); }
	Person& operator=(const Person& rhs) { m_name = new string(*rhs.m_name); };
	~Person() { delete m_name; }
	void printName() { cout << *m_name << endl; }
};

int main() {
	vector<Person> persons;
	persons.push_back(Person("Jack"));
	persons.back().printName();
	cout << "Goodbye!" << endl;
	system("pause");
	return 0;
}

//solution 2: Change the vector<Person> to vector<Person*>. You have to delete the object in the end.
class Person{
private:
    string* m_name;
public:
    Person(string name) {m_name = new string(name);}
    ~Person(){delete m_name;}
    void printName(){ cout << *m_name << endl;}
};

int main(){
    vector<Person*> persons;
    persons.push_back(new Person("Jack"));
    persons.back()->printName(); 
    //Delte memory here...
    cout << "Goodbye!" << endl;
    system("pause");
    return 0;
}



//C++ 11
//Solution: We can use shared pointer to handle the resource allocation and recycle
class Person {
private:
	shared_ptr<string> m_name;
public:
	Person(string name) : m_name(new string(name)) {}
	//~Person() { delete m_name; } //Remember to remove the destructor
	void printName() { cout << *m_name; }
};

int main(){
    vector<Person> persons;
    persons.push_back(Person("Jack"));
    persons.front().printName();
    cout << "Goodbye!" << endl;
    system("pause");
    return 0;
}

//Solution: use unique pointer (which is cheaper)
class Person {
private:
	unique_ptr<string> m_name;
public:
	Person(string name) : m_name(new string(name)) {}
    ~Person(){}
    Person(Person&&) = default;
	void printName() { cout << *m_name; }
};

int main() {
	vector<Person> persons;
	Person p("Jack");
	persons.push_back(move(p));//Note that unique_ptr cannot be copied, so we need to use move() function here!! In order to use std::move(), we cannot define our own destructor, or compile cannot generate the move constructor for us. If we really need the destructor, we can explicitly define a move constructor!
	persons.front().printName();
	cout << "Goodbye!" << endl;
	system("pause");
	return 0;
}


