#pragma once

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
namespace StrategyPatternNameSpace {
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

	void testDesignPattern() {
		Duck WhiteDuck("Jack", DuckColor::WHITE, new FreeFly(), new WarningQuack());
		Duck BlackDuck("Mark", DuckColor::BLACK, new LongRangeFly(), new NormalQuack());
		WhiteDuck.DuckInfo();
		BlackDuck.DuckInfo();
		system("pause");
	}
}