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


#include<regex>
/* 2. C++ 11 Regular Expression */
/*
Regular Expression: a regular expression is a specific pattern that 
provides concise and flexible means to "match" strings of text, 
such as particular characters, words, or patterns of characters. 
- wikipedia
 */
//Basic grammar
int main(){
    string str;
    while(true){
        cin>>str;
        
        //using regular expression grammar: ECMAScript (default)
        //regex e("^abc.", regex_constants::grep);

        //match string with exactly "abc"
        //regex e("abc"); 

        //match string with "abc", ignore lowercase or uppercase
        //regex e("abc", regex_constants::icase);

        //'.' matches to any character except the newline
        //regex e("abc."); 

        //'?' matches 0 or 1 preceding characters!
        //e.g. "ab", "abc" will be matches
        //regex e("abc?");

        //matches 0 or more preceding characters.
        //regex e("abc*");

        //1 or more preceding characters.
        //regex e("abc+");

        //[...] Any character inside the square brackets, represents 
        //1 character
        //e.g. "abcd", "abc", "abcccddd", "abdcdc" are matches
        //regex e("ab[cd]*"); 

        //[^...] Any character not inside the squre brackets, 
        //represents 1 character
        //regex e("ab[^cd]*");

        //{number} exact match the [number] of preceding characters
        //e.g. "abccc", "abcdd", "abcdc" are matches
        //e.g. "abcdcdcd", "abdddccc", "ab" are not matches
        //regex e("ab[cd]{3}");

        //matches 3 or more preceding characters
        //regex e("ab[cd]{3,}");

        //matches 3, 4 or 5 preceding characters
        //regex e("ab[cd]{3,5}");

        //| - OR
        //e.g. "abc", "def", "deg" are matches
        //"abcd", "defg" are not matches
        //regex e("abc|de[fg]");

        //matches ']'
        //regex e("\]");

        //\1 represents the group 1, the () defines the group
        //e.g. "abcdeeeeabc" is a match
        //regex e("(abc)de+\\1");

        //Note (de+) defines the group, if we have (de+) defined 
        //as "deee", then for group 2, it should always be "deee".
        //regex e("(ab)*c(de+)\\2\\1");

        //matching any e-mail address
        //[[:w:]] word character: digit, number, or underscore!
        //regex e("[[:w:]]+@[[:w:]]+\.com");

        //'^' marks that "abc" should be at the beginning of the string
        //regex e("^abc", regex_constants::icase);

        //'$' marks that "abc." should appear at the end of the string
        regex e("abc.$", regex_constants::icase);

        /*
        [:s:] - a white space character;
        [:w:] - a word character;
        [:d:] - a decimal digit character;
        [:upper:] - an uppercase character;
        [:lower:] - a lowercase character;
        [:alnum:] - an alpha-numerical character;
        [:alpha:] - an alphabetic character;
        [:blank:] - a blank character;
        [:punct:] - a punctuation mark character
         */

        //check whether str can be matched with regular expression e
        bool match = regex_match(str, e);
        //search the string str to see whether there is a substring 
        //of str, that can be matched with e
        bool isFound = regex_search(str, e);

        cout << (match ? "Matched" : "Not matched") << endl << endl;
    }
    return 0;
}

/*
Regular Expression Grammars in C++:
- ECMAScript (default)
- basic
- extended
- awk
- grep
- egrep
 */

// submatch
/*
std::match_result<> - store the detailed matches!
smatch              - detailed match in string!

smatch m;
m[0].str() - The entire match (same with m.str(), m.str(0))
m[1].str() - The substring that matches the first group (same as m.str(1))
m[2].str() - The substring that matches the second group
m.prefix() - Everything before the first matched character
m.suffix() - Everything after the last matched character
 */
int main() {
	string str;
	while (true) {
		cin >> str;
        smatch m; //typedef std::match_results<string>

        //We can define two groups here to extract the user name and 
        //domain name
		regex e("([[:w:]]+)@([[:w:]]+)\.com");

		//search the string for regular expression e and save the 
        //results to m
		bool match = regex_search(str, m, e);

        cout << "match size: " << m.size() << endl;
        for(int i = 0; i < m.size(); ++i){
            //print out the matched results
            cout << "m[" << i <<"]: str() = " << m[i].str() <<endl;
        }
        //prefix is everything before the first matched character
        cout <<"m.prefix().str(): " << m.prefix().str() << endl;
        //suffic is everything after the last matched character
        cout <<"m.suffix().str(): " << m.suffix().str() <<endl;
	}
	return 0;
}

//Iterators
//regex_iterator
//regex_token_iterator

// regex_iterator: pointing to the match results
//If we want to extract all the matches in our string str:
//The following approach won't work! We can only extract the first matching
//result
int main() {
	string str = "zhangxm01@gmail.com; zhan@163.com; zhan_j@yahoo.com";
    smatch m; 
    regex e("([[:w:]]+)@([[:w:]]+)\.com");

    bool found = regex_search(str, m, e);
    for(int i = 0; i < m.size(); ++i){
        //print out the matched results
        cout << "m[" << i <<"]: str() = " << m[i].str() <<endl;
    }
	system("pause");
	return 0;
}

//We can use regular expression iterator here!
int main() {
	string str = "zhangxm01@gmail.com; zhan@163.com; zhan_j@yahoo.com";
    regex e("([[:w:]]+)@([[:w:]]+)\.com");

    //regex iterator
    sregex_iterator pos(str.cbegin(), str.cend(), e);

    //default constructor defines past-the-end iterator
    sregex_iterator end; 
    
    for(; pos != end; ++pos){
        cout << "Matched: " << pos->str(0) << endl;
        cout << "User Name: " << pos->str(1) << endl;
        cout << "Domain: " << pos->str(2) <<endl;
        cout << endl;
    }
	system("pause");
	return 0;
}

//regex_token_iterator: pointing to a sub match
int main() {
	string str = "zhangxm01@gmail.com; zhan@163.com; zhan_j@yahoo.com";
    regex e("([[:w:]]+)@([[:w:]]+)\.com");

    //regex iterator
    sregex_token_iterator pos(str.cbegin(), str.cend(), e);

    //default constructor defines past-the-end iterator
    sregex_token_iterator end; 
    
    for(; pos != end; ++pos){
        //str() cannot have any parameters
        cout << "Matched: " << pos->str() << endl;
        cout << endl;
    }
	system("pause");
	return 0;
}

//regex_replace 
int main() {
	string str = "zhangxm01@gmail.com; zhan@163.com; zhan_j@yahoo.com";
    regex e("([[:w:]]+)@([[:w:]]+)\.com");
    //$1 and $2 represents the group 1 and group 2 in the regex
    cout <<regex_replace(str, e, "$1 is on $2") << endl;;
    //We can add more flags in the regex_replace function
	cout << regex_replace(str, e, "$1 is on $2", regex_constants::format_no_copy|regex_constants::format_first_only);
	cout<< endl;

	system("pause");
	return 0;
}