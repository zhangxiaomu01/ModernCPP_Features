#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace SingletonPatternNameSpace {
    //Singleton Pattern
    //Singleton pattern ensures that a class has only one instance and provides 
    //a global access to that single instance.
    class Singleton {
    private:
        Singleton(int x) { val = x; }

        int val;
    public:
        //We use static function to get the only reference to the class
        static Singleton& getInstance(int x) {
            //Save the only reference here
            static Singleton instance(x);
            return instance;
        }

        void printVal() {
            std::cout << "The singleton class has value: " << val << std::endl;
        }
    };


    void TestDesignPattern() {
        //Directly call the getInstance() function by class
        static Singleton sInstance = Singleton::getInstance(6);
        sInstance.printVal();

        //will not work, still print 6 here, we cannot make another instance
        sInstance = Singleton::getInstance(15);
        sInstance.printVal();

        //The following will have problem,since constructor is private!
        //Singleton* sInstance = new Singleton();

        system("pause");
    }
}