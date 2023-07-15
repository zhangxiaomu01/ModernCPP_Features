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


//Template Method Pattern
//Template Method Pattern defines a skeleton of the algorithm in an operation, 
//deferring some steps to subclasses. Template class let subclasses redefine 
//certain steps of the algorithm without changing the algorithm's structure. 
class Data {
private:
	bool isValid;
public:
	Data(bool v) { isValid = v; }
	bool checkValid() { return isValid; }
};

//Record class is our template!
//In our record class, we define the structure of save() method. The only thing missing is 
//ValidateData() and ValidateDataSave(). We can implement in the concrete class!
class Record {
protected:
	Data* m_data;
	virtual bool ValidateData() = 0;
	virtual bool ValidateDataSave() = 0;
public:
	Record(Data* data) { m_data = data; }
	void Save() {
		std::cout << "Initializing the data base!" << std::endl;
		std::cout << "Now validate data to be saved!" << std::endl;
		if (ValidateData()) {
			std::cout << "Data is OK! Now saving..." << std::endl;
		}
		else {
			std::cout << "Data type not supported. Saving denied!" << std::endl;
			return;
		}
		
		if (ValidateDataSave()) {
			std::cout << "Data successfully saved!" << std::endl;
		}
		else {
			std::cout << "Data save failed! " << std::endl;
		}
	}
	virtual ~Record() { delete m_data; }
	
};

class userRecord : public Record {
public:
	userRecord(Data* d) : Record(d){}
	bool ValidateData() {
		return m_data->checkValid();
	}
	//We simulate the validate process here using rand() - 85% chance success
	bool ValidateDataSave() {
		std::cout << "Now validate data base save!" << std::endl;
		srand(time(NULL));
		int x = rand() % 100;
		return x < 85;
	}
};


int main() {
	Record* aRecord = new userRecord(new Data(false));
	aRecord->Save();
	std::cout << std::endl;
	Record* sRecord = new userRecord(new Data(true));
	sRecord->Save();
	
	system("pause");
	return 0;
}


//Composite Pattern
//The composite pattern composes objects into tree structures to represent 
//part-whole hierarchies. Composite let's clients treat individual objects 
//and compositions of objects uniformly.
//We define the interface for getHtml method
class HTMLLists {
public:
	virtual std::string getHtml() = 0;
};

//The Lists class is a composite class which specifically 
class Lists : public HTMLLists {
private:
	std::vector<HTMLLists*> m_lists;
public:
	Lists( std::vector<HTMLLists*> l) {
		m_lists = l;
	}
	std::string getHtml() {
		unsigned int len = m_lists.size();
		std::string html = "<ul>\n";
		for (int i = 0; i < len; ++i) {
			html += "<li>\n";
			html += m_lists[i]->getHtml();
			html += '\n';
			html += "</li>\n";
		}
		html += "</ul>\n";
		return html;
	}
};

class Leaf : public HTMLLists {
private:
	std::string m_text;
public:
	Leaf(std::string text) {
		m_text = text;
	}
	std::string getHtml() {
		return m_text;
	}
};

int main() {
	std::vector<HTMLLists*> l1 = { new Leaf("TCC"), new Leaf("Sd") };
	std::vector<HTMLLists*> l2 = { new Leaf("printf"), new Lists(l1), new Leaf("It's good!") };
	std::vector<HTMLLists*> OurList = { new Leaf("ACC"), new Lists(l1), new Lists(l2), new Leaf("S1") };

	HTMLLists* lists = new Lists(OurList);

	std::string res = lists->getHtml();
	std::cout << res << std::endl;
	
	system("pause");
	return 0;
}


//Iterator Pattern
//The iterator pattern provides a way to access the element from an aggregate 
//object sequentially without  exposing the object's underlying representation.
//Forward definition for PlayerInventory is needed!
class PlayerInventory;
struct Item {
	std::string itemID;
	Item(std::string ID) {
		itemID = ID;
	}
};

//We will design a simple inventory iterator.
//Iterator Base class
class Iterator {
public:
	virtual bool isDone() = 0;
	virtual int next() = 0;
	virtual Item* currentItem() = 0;
};
//Inventory base class
class Inventory {
public:
	virtual Iterator* getIterator() = 0;
};


//concrete implementation of inventory iterator
//It has a concrete implementation of player inventory
//Since compiler won't see the actual implementation of PlayerInventory now
//We need to give declaration here, and give concrete definition after PlayerInventory 
//class definition!
class InventoryIterator : public Iterator {
private:
	PlayerInventory* m_pInventory;
	int m_currentIndex;
public:
	InventoryIterator(PlayerInventory* pInv);
	
	bool isDone(); 
	int next(); 
	Item* currentItem(); 
};

//Concrete implementation of player inventory.
//It has a concrete player inventory iterator
class PlayerInventory : public Inventory {
public:
	std::vector<Item> m_itemList;
	PlayerInventory(std::vector<Item>& itemList) {
		m_itemList = itemList;
	}
	Iterator* getIterator() {
		return new InventoryIterator(this);
	}
};

InventoryIterator::InventoryIterator(PlayerInventory* pInv){
	m_pInventory = pInv;
	m_currentIndex = 0;
}

bool InventoryIterator::isDone(){
	return m_currentIndex == m_pInventory->m_itemList.size();
}

int InventoryIterator::next() {
	if (m_currentIndex < m_pInventory->m_itemList.size())
		m_currentIndex++;
	return m_currentIndex;
}

Item* InventoryIterator::currentItem(){
	if (isDone()) return nullptr;
	return &m_pInventory->m_itemList[m_currentIndex];
}


int main() {
	std::vector<Item> ourItemList = { Item("Knife"), Item("Sword"), Item("Medical Kit") };
	PlayerInventory pInv(ourItemList);
	Iterator* pInvIt = pInv.getIterator();
	while (!pInvIt->isDone()) {
		std::cout << pInvIt->currentItem()->itemID << std::endl;
		pInvIt->next();
	}

	system("pause");
	return 0;
}


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



