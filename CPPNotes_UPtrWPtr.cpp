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

        // The following code fix the problem. lock() function will create 
        //a shared_ptr of weak_ptr. It will check whether the weak pointer 
        //is still pointing to a valid object, and make sure that when the
        // weak pointer is accessing the object, the object has not been 
        //deleted!
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
//Unique pointer: an exclusive ownership, light weight smart pointer.
//Only one unique pointer can point to an object at the same time.
class Dog{
private:
    unique_ptr<Dog> someDog; //using unique pointer here to prevent memory leak due to constructor errors/exceptions
public:
    string m_name;
    Dog(){ cout << "Nameless dog created!" << endl;  m_name = "Nameless";}
    void bark(){cout << "Dog " << m_name << " rules!" << endl;}
    Dog(string name) {m_name = name; cout << "Dog is created: " << m_name << endl; }
    ~Dog(){ cout << "Dog is destroyed! " << m_name << endl;}
};

void test(){
    Dog* pD = new Dog("Gunner");
    pD->bark();
    //If we return earlier, pD will cause memory leak!
    delete pD;
}
//Using unique pointer here 
void test01(){
    unique_ptr<Dog> pD (new Dog("Gunner"));
    pD->bark();
    //If we return earlier, pD will not cause memory leak!

    Dog* p = pD.release(); // release() function will return the raw pointer. It will also change the ownership of unique pointer and set it to nullptr.
    //pD.reset(new Dog(smokey)); //reset pD to other object, if unique pointer originally owns an object, that object will be deleted!
    //pD.reset(); //same as pD = nullptr
    if(!pD) {
        cout << "pD is empty now." <<endl;
    } 
}
void test02(){
    //unique pointer can access the same object at different time
    unique_ptr<Dog> pD (new Dog("Gunner"));
    unique_ptr<Dog> pD1 (new Dog("Smile"));
    pD1->bark();
    pD1 = move(pD);
    pD1->bark();
}

void foo(unique_ptr<Dog> p){
    p->bark();
}

unique_ptr<Dog> getDog(){
    unique_ptr<Dog> p(new Dog("Jack"));
    //return p will use the move semantics! so p will no longer has the ownership of Jack
    return p;
}

void test03(){
    unique_ptr<Dog> pD (new Dog("DND"));
    foo(move(pD));//Cannot directly pass to foo(), since pD is unique pointer. pD will be destroyed in foo().
    unique_ptr<Dog> pD2 = getDog();//pD2 owns the jack now, so it's not nullptr!
    if(!pD2) {cout << "pD2 is nullptr. " << endl;}
}

int main(){
    test();
    test01();
    test02();
    test03();
    //using customized deleter
    shared_ptr<Dog> pSD(new Dog[3], [](Dog *p){delete []p;});
    //for unique pointer, we do not need customized deleter. Need to indicate it in template parameter: unique_ptr<Dog[]>.
    unique_ptr<Dog[]> dogs(new Dog[3]);

    system("pause");
    return 0;
}

