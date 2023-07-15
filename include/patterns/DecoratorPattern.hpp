#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace DecoratorPatternNameSpace {
    //03. Decorator Pattern
    /*
    A decorator pattern attaches an additional responsibility to an object 
    dynamically. Decorators provide a flexible alternative to sub-classing for 
    extending functionality.
    */

    //A simple case: 
    //Decorator Pattern
    //Abstract base class for beverage
    class BeverageClass {
    public:
        //BeverageClass():m_cost(3){}
        virtual int getCost() = 0;
    };
    //Abstract base class for add-on ingrediants
    class AddOnClass : public BeverageClass {
    public:
        virtual int getCost() = 0;
    };


    class CoffeeClass : public BeverageClass {
    private:
        int m_cost;
    public:
        CoffeeClass() :m_cost(4){}
        int getCost() {
            return m_cost;
        }
    };

    class MilkClass : public AddOnClass {
    private:
        int m_cost;
        BeverageClass* beverage;
    public:
        MilkClass(BeverageClass* b) : m_cost(2), beverage(b){}
        int getCost() {
            return beverage->getCost() + m_cost;
        }
    };

    class SugarClass : public AddOnClass {
    private:
        int m_cost;
        BeverageClass* beverage;
    public:
        SugarClass(BeverageClass* b) : m_cost(7), beverage(b) {}
        int getCost() {
            return beverage->getCost() + m_cost;
        }
    };

    void TestDesignPattern() {
        //Note coffee is the base, is a must. While we can always add different 
        //add-on ingrediants in it
        BeverageClass* bv = new MilkClass(new SugarClass(new CoffeeClass()));
        std::cout << "Beverage costs: " << bv->getCost() << std::endl;
        system("pause");
    }
}