#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace AdapterPatternNameSpace {
    //Adaptor Pattern
    //Adaptor Pattern  converts the interface of a class into another interface 
    //that the client expects. Adaptors let classes work together that couldn't 
    //otherwise possible because of the incompatible interfaces.
    //Define Adaptee, imagine this as a third party library. We need to convert the interface
    //of Adaptee to satisfy our client's need!
    class Adaptee {
    public:
        int Sum(int a, int b) {
            return a + b;
        }
    };

    //Define the common interface for Adaptor class
    class ITarget {
    public:
        virtual int Sum() = 0;
    };

    //Actual implementation of Adaptor, we convert string to integer and call the sum
    class Adaptor : public ITarget {
    private:
        Adaptee* m_Adaptee;
        std::string* m_s1;
        std::string* m_s2;
        //suppose op will have only '+ / -'
        char m_op;
    public:
        Adaptor(Adaptee* A, std::string* s1, std::string* s2, char op) {
            m_Adaptee = A;
            m_s1 = s1;
            m_s2 = s2;
            m_op = op;
        }
        virtual ~Adaptor() {
            delete m_Adaptee;
            delete m_s1, m_s2;
        }
        int Sum() {
            int a = std::stoi(*m_s1);
            int b = std::stoi(*m_s2);
            if (m_op == '-') b = -b;
            if (m_op != '+' && m_op != '-') return INT_MIN;
            return m_Adaptee->Sum(a, b);
        }
    };

    void TestDesignPattern() {
        std::string* S1 = new std::string("123");
        std::string* S2 = new std::string("-23");
        ITarget* iAdaptor = new Adaptor(new Adaptee(), S1, S2, '-');
        std::cout << "The sum of the two is: " << iAdaptor->Sum() << std::endl;
        system("pause");
    }
}