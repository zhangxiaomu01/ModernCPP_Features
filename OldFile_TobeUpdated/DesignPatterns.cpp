#include<iostream>
#include<vector>
#include<string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


//Facade Pattern
//Façade Pattern provides a unified interface to a set of interfaces in a 
//subsystem. Façade defines a high level interface that makes the subsystem 
//easier to use.
//Omit the code here, check URM graph.






//State Pattern
//The state pattern allows an object to alter its behavior when it internal 
//state changes. The object will appear to change its class.
//State pattern is interesting and very powerful. It can eliminate the complexity
//of different combinations. Double check later!
//Forward definition for gate!
class Gate;
class GateState {
public:
	std::string m_status;
	virtual void enter() = 0;
	virtual void payOK() = 0;
	virtual void payFailed() = 0;
};

class openState : public GateState {
private:
	Gate* m_gate;
public:
	openState(Gate* g);
	void enter();
	void payOK();
	void payFailed();
};

class closeState : public GateState {
private:
	Gate* m_gate;
public:
	closeState(Gate* g);
	void enter();
	void payOK();
	void payFailed();
};

class Gate {
private:
	GateState* m_gState;
public:
	Gate() { m_gState = new closeState(this); }
	void enter() {
		m_gState->enter();
	}
	void payOK() {
		m_gState->payOK();
	}
	void payFailed() {
		m_gState->payFailed();
	}
	void changeState(GateState* newState) {
		m_gState = newState;
	};
	void printStatus() {
		std::cout << "The gate is now: " << m_gState->m_status << std::endl;
		std::cout << std::endl;
	}
};

openState::openState(Gate * g)
{
	m_gate = g;
	m_status = "Open";
}

void openState::enter() {
	m_gate->changeState(new closeState(m_gate));
	std::cout << "Someone has entered the gate, gate closed!" << std::endl;
}

void openState::payOK()
{
	std::cout << "Your payment is OK, the gate is open!" << std::endl;
}

void openState::payFailed()
{
	std::cout << "Your payment is not applicable! " << std::endl;
}

closeState::closeState(Gate * g)
{
	m_gate = g; 
	m_status = "Closed";
}

void closeState::enter()
{
	std::cout << "You cannot enter, gate closed!" << std::endl;
}

void closeState::payOK()
{
	m_gate->changeState(new openState(m_gate));
	std::cout << "Your payment is OK, the gate is open!" << std::endl;
}

void closeState::payFailed()
{
	std::cout << "Your payment is not applicable! Gate remains closed! " << std::endl;
}

int main() {
	Gate metroGate;
	metroGate.printStatus();
	metroGate.enter();
	metroGate.printStatus();
	metroGate.payFailed();
	metroGate.printStatus();
	metroGate.payOK();
	metroGate.printStatus();
	metroGate.enter();
	metroGate.printStatus();
	system("pause");
	return 0;
}



