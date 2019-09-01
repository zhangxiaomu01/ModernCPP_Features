#include<iostream>
#include<vector>
#include<string>

//01. Strategy Pattern
/*
Using composition rather than inheritance. This pattern defines a family of 
algorithms, and encapsulates these algorithm and make them interchangeable.
This pattern let the algorithm work very independently from the client who use 
it. 
*/
enum DuckColor {
	WHITE,
	RED,
	BLACK
};

const std::string ColorStr[3] = { "white", "red", "black" };

//Strategy pattern
//strategy1: Fly interface
class Fly {
public:
	virtual void fly() = 0;
};

//strategy2: DuckCall interface
class DuckQuack {
public:
	virtual void quack() = 0;
};


//Implement different fly behavior
class LongRangeFly : public Fly {
public:
	void fly() {
		std::cout << "The bird flies with lower frequency to flap its wings and maintain a formation. " << std::endl;
	}
};

class FreeFly : public Fly {
public:
	void fly() {
		std::cout << "The bird flies freely, with higher frequency to flap its wings. " << std::endl;
	}
};

class NoFly : public Fly {
public:
	void fly() {
		std::cout << "The bird cannot fly. " << std::endl;
	}
};

//Implement different call behavior
class NormalQuack : public DuckQuack {
public:
	void quack() {
		std::cout << "This bird calls normally. " << std::endl;
	}
};

class WarningQuack : public DuckQuack {
public:
	void quack() {
		std::cout << "This bird warns other ducks! " << std::endl;
	}
};

//Duck class can inject multiple different strategies. 
//Note these strategies can also be injected to other classes, may be sparrow etc.
class Duck {
private:
	std::string m_name;
	DuckColor m_color;
	//We can inject the strategy class here and use different strategy!
	Fly* m_flyBehavior;
	DuckQuack* m_quackBahavior;
public:
	Duck(std::string name, DuckColor c, Fly* fly = nullptr, DuckQuack* call = nullptr) {
		m_name = name;
		m_color = c;
		m_flyBehavior = fly;
		m_quackBahavior = call;
	}

	void DuckInfo() {
		std::cout << "This " << ColorStr[int(m_color)] << " duck: " << m_name <<"." << std::endl;
		m_flyBehavior->fly();
		m_quackBahavior->quack();
	}
};

int main() {
	Duck WhiteDuck("Jack", DuckColor::WHITE, new FreeFly(), new WarningQuack());
	Duck BlackDuck("Mark", DuckColor::BLACK, new LongRangeFly(), new NormalQuack());
	WhiteDuck.DuckInfo();
	BlackDuck.DuckInfo();
	system("pause");
	return 0;
}

//02. Observer Pattern
/*
We define the observers as the classes who need to observe a frequently changed 
class Observable class. Whenever the observable makes the change, it will push 
the information that it has changed to all the observer classes.

The observer pattern defines an one to many dependency between objects, 
that when one of the objects changes the states, all of its dependencies are 
notified and updated automatically.
*/


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

int main() {
	//Note coffee is the base, is a must. While we can always add different 
    //add-on ingrediants in it
	BeverageClass* bv = new MilkClass(new SugarClass(new CoffeeClass()));
	std::cout << "Beverage costs: " << bv->getCost() << std::endl;
	system("pause");
	return 0;
}

//
//
