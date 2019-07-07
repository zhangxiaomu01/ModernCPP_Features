#include<iostream>
#include<string>
#include<memory>

using namespace std;
/* 1. C++ 11 Weak pointers */
class Dog {
    shared_ptr<Dog> m_pFriend;//cyclic reference
    //weak_ptr<Dog> m_pFriend; //Solution to fix the problem
public:
    string m_name;
    Dog(string name) : m_name(name){cout << "Dog: " << m_name << " is defined!" << endl;}
    void bark() {cout << "Dog " << m_name << " rules!" << endl;}
    ~Dog() {cout << "Dog is destroyed: " << m_name << endl;};
    void makeFriend(shared_ptr<Dog> f){m_pFriend = f;}
};

int main(){
    shared_ptr<Dog> pD(new Dog("Gunner"));
    shared_ptr<Dog> pD1(new Dog("Smile"));
    //In this code, Dog destructor will not be called, when pD goes out of scope, pD1 still has a pointer points to pD; when pD1 goes out of scope, pD still has a pointer points to pD1
    //Will cause memory leak: cyclic reference!
    //using weak pointer to declare m_pFriend. Weak pointer has no ownership of the pointed object.
    pD->makeFriend(pD1);
    pD1->makeFriend(pD);
    return 0;
}
/* Weak pointer */
/*
Having weak pointer acts really as the raw pointer, however, it provides one level of protection: delete operation to weak pointer is forbidden! If the object the weak pointer points to gets deleted, the weak pointer will become empty pointer!
 */
//Weak pointer cannot be used like a normal pointer!
class Dog {
    weak_ptr<Dog> m_pFriend; 
public:
    string m_name;
    Dog(string name) : m_name(name){cout << "Dog: " << m_name << " is defined!" << endl;}
    void bark() {cout << "Dog " << m_name << " rules!" << endl;}
    ~Dog() {cout << "Dog is destroyed: " << m_name << endl;};
    void makeFriend(shared_ptr<Dog> f){m_pFriend = f;}
    void showFriend(){
        //Compile error. m_pFriend cannot direct access the object like the normal pointer.
        cout << "My Friend is: " << m_pFriend->m_name <<endl;

        // The following code fix the problem. lock() function will create a shared_ptr of weak_ptr. It will check whether the weak pointer is still pointing to a valid object, and make sure that when the weak pointer is accessing the object, the object has not been deleted!
        //If the weak pointer is empty, the lock() will throw an exception.
        if(!m_pFriend.expired())
            cout << "My Friend is: " << m_pFriend.lock()->m_name <<endl;
        cout << "He is owned by " << m_pFriend.use_count() << " pointers" << endl;
    }
};
int main(){
    shared_ptr<Dog> pD(new Dog("Gunner"));
    shared_ptr<Dog> pD1(new Dog("Smile"));
    pD->makeFriend(pD1);
    pD1->makeFriend(pD);
    pD->showFriend();
    return 0;
}
/* Other API in weak pointer */
//expired() - check whether the weak pointer is still valid
//use_count() - check how many shared pointers are pointing to the object


/* 2. C++ 11 Unique pointers */











