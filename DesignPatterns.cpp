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
//Observer Pattern:
//Abstract class for observers (different display monitors in our case)
class Observer {
public:
	virtual void update() = 0;
	virtual void showInfo() = 0;
};

//Class for observee interface:
//We can add observer reference here
//We can update any observers if we made the change
class Observee {
protected:
	virtual void notifyUpdate() = 0;
public:
	virtual void addObserver(Observer* ob) = 0;
	//We can also add removeObserver(Observer* ob) method here
};

//Concrete implementation below
//Concrete observer class, wheather station!
class WheatherInfo : public Observee {
private:
	//temperature
	float m_temp;
	//moisture
	float m_mois;
	//A set of observers
	std::vector<Observer*> m_observerList;
	//Update Observers
	void notifyUpdate() {
		for (int i = 0; i < m_observerList.size(); ++i) {
			m_observerList[i]->update();
		}
	}
public:
	WheatherInfo(float temperature, float mois) {
		m_temp = temperature;
		m_mois = mois;
		notifyUpdate();
	}

	void addObserver(Observer* ob) {
		m_observerList.push_back(ob);
		ob->update();
	}

	void setTemp(float temperature) {
		m_temp = temperature;
		notifyUpdate();
	}
	void setMoisture(float moisture) {
		m_mois = moisture;
		notifyUpdate();
	}
	float getTemp() {
		return m_temp;
	}
	float getMoisture() {
		return m_mois;
	}
};

//I chose the display mode as the observers. This is not a good example, since the two 
//classes are pretty the same, we can inheritate them from a display class
//We may have totally different logic pattern in these two observers
//Observer 1: Phone display
class PhoneDisplay : public Observer {
private:
	//Note here we should define WheatherInfo, the concrete example
	WheatherInfo* m_wheatherInfo;
	void showInfo() {
		std::cout << "====== This is the phone display =======" << std::endl;
		std::cout << "Current temperature is: " << m_wheatherInfo->getTemp() << "." << std::endl;
		std::cout << "Current moinstrue is: " << m_wheatherInfo->getMoisture() << "." << std::endl;
	}
public:
	PhoneDisplay(WheatherInfo* wIF) {
		m_wheatherInfo = wIF;
		m_wheatherInfo->addObserver(this);
	}
	void update() {
		//Just a simple example for update the info, we could also save temperature etc.
		showInfo();
	}

};

//Observer 2: LCD Display
class LCDDisplay : public Observer {
private:
	WheatherInfo* m_wheatherInfo;

	void showInfo() {
		std::cout << "====== This is the LCD display =======" << std::endl;
		std::cout << "Temperature: " << m_wheatherInfo->getTemp() << "." << std::endl;
		std::cout << "Moisture: " << m_wheatherInfo->getMoisture() << "." << std::endl;
	}
public:
	LCDDisplay(WheatherInfo* wIF) {
		m_wheatherInfo = wIF;
		m_wheatherInfo->addObserver(this);
	}
	void update() {
		showInfo();
	}
};

int main() {
	WheatherInfo currentWL(75.0f, 67.0f);
	PhoneDisplay* myPhone = new PhoneDisplay(&currentWL);
	LCDDisplay* myLCD = new LCDDisplay(&currentWL);

	currentWL.setTemp(34.0f);
	currentWL.setMoisture(78.0f);

	system("pause");
	return 0;
}


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

//4. Factory Method Pattern
//Factory Method

