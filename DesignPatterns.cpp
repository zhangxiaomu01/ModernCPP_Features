#include<iostream>
#include<vector>
#include<string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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
//The factory method pattern defines an interface for creating an object, 
//but let the subclasses to decide which classes to instantiate. Factory method 
//let the classes defer instantiation to subclasses.
//Factory Method Pattern:
//Different factories create different enemy combinations

//Factory Method Pattern:
//Different factories create different enemy combinations

//Product base class (abstract, in our case, product is enmemy)
class Enemy {
public:
	int m_ID;
	int m_HP;
	int m_Damage;
public:
	Enemy(int id, int HP, int damage) {
		m_ID = id;
		m_HP = HP;
		m_Damage = damage;
	}
	virtual void EnemyAbility() = 0;
};

//Factory base class (in our case, factory is enemy spawner)
class EnemySpawnFactory {
public:
	void showEnemy(std::vector<Enemy*>& V) {
		for (int i = 0; i < V.size(); ++i) {
			std::cout << "Enemy " << V[i]->m_ID << " spawned!" << std::endl;
			std::cout << "HP: " << V[i]->m_HP << std::endl;
			std::cout << "Damage: " << V[i]->m_Damage << std::endl;
			V[i]->EnemyAbility();
		}
	}
	//Different factory will define its specific spawn rule
	virtual std::vector<Enemy*> SpawnEnemy() = 0;
};


//Product concrete implementation
class Goblin : public Enemy {
public:
	Goblin(int ID, int HP, int Damage) : Enemy(ID, HP, Damage){}
	void EnemyAbility() {
		std::cout << "Goblin can sneak!" << std::endl;
	}
};

class Orc : public Enemy {
public:
	Orc(int ID, int HP, int Damage) : Enemy(ID, HP, Damage) {}
	void EnemyAbility() {
		std::cout << "Orcs can have blood rage!" << std::endl;
	}
};

class Giant : public Enemy {
public:
	Giant(int ID, int HP, int Damage) : Enemy(ID, HP, Damage) {}
	void EnemyAbility() {
		std::cout << "Orcs can have devastating power!" << std::endl;
	}
};

//Concrete implementation of Factory, here we have different enemy spawner for different levels
//Actually the concrete factory should be able to create concrete product
class SpawnerLevel1 : public EnemySpawnFactory {
private:
	//A set of enemies
	std::vector<Enemy*> res;
public:
	//Spawn 2 goblins and 1 orc: actually define the spawn rule
	std::vector<Enemy*> SpawnEnemy() {
		Enemy* gob01 = new Goblin(1, 10, 3);
		Enemy* gob02 = new Goblin(2, 8, 5);
		Enemy* orc = new Orc(3, 20, 7);
		res.push_back(gob01);
		res.push_back(gob02);
		res.push_back(orc);
		showEnemy(res);
		return res;
	}
	virtual ~SpawnerLevel1() {
		for (auto it : res)
			delete it;
	}
};

class SpawnerLevel2 : public EnemySpawnFactory {
private:
	//A set of enemies
	std::vector<Enemy*> res;
public:
	//Spawn 1 goblins, 1 orc and 1 giant
	std::vector<Enemy*> SpawnEnemy() {
		Enemy* gob = new Goblin(4, 12, 1);
		Enemy* orc = new Orc(5, 20, 7);
		Enemy* giant = new Giant(6, 35, 15);
		res.push_back(gob);
		res.push_back(orc);
		res.push_back(giant);
		showEnemy(res);
		return res;
	}
	virtual ~SpawnerLevel2() {
		for (auto it : res)
			delete it;
	}
};


int main() {
	SpawnerLevel1 spawnL1;
	spawnL1.SpawnEnemy();
	SpawnerLevel2 spawnL2;
	spawnL2.SpawnEnemy();

	system("pause");
	return 0;
}


//Abstract Factory Pattern
//Abstract Factory pattern provides an interface for creating families of 
//related or dependent objects without specifying their concrete classes. 
//(we can create multiple objects)
//This pattern is similar to Factory Method pattern except it creates families of 
//related or dependent objects
//Please refer to the URMs 
//Omit for now


//Singleton Pattern
//Singleton pattern ensures that a class has only one instance and provides 
//a global access to that single instance.
class Singleton {
private:
	Singleton(int x) { val = x; }

	//Save the only reference here
	static Singleton* instance;
	int val;
public:
	//We use static function to get the only reference to the class
	static Singleton* getInstance(int x) {
		//If instance is not nullptr, we will directly return it
		//Guarantee we have only 1 instance
		if(instance == nullptr)
			instance = new Singleton(x);
		return instance;
	}

	void printVal() {
		std::cout << "The singleton class has value: " << val << std::endl;
	}

	~Singleton() { delete instance; }
};

// Initialize static member of class (It's a must)
Singleton* Singleton::instance = nullptr;


int main() {
	//Directly call the getInstance() function by class
	static Singleton* sInstance = Singleton::getInstance(6);
	sInstance->printVal();

	//will not work, still print 6 here, we cannot make another instance
	sInstance = Singleton::getInstance(15);
	sInstance->printVal();

	//The following will have problem,since constructor is private!
	//Singleton* sInstance = new Singleton();

	system("pause");
	return 0;
}


//Command Pattern
//Command Pattern encapsulates a request as an object thereby letting you 
//parameterize other objects with different request queue or log request and 
//support undoable operations. 

//Define a concrete object which will react to the command (Receiver)
//For example, table light
class TableLight {
public:
	void turnOn() {
		std::cout << "The light is now on!" << std::endl;
	}

	void turnOff() {
		std::cout << "The light is now off!" << std::endl;
	}
};

//Command interface: we can execute or revoke the command
class ICommand {
public:
	virtual void execute() = 0;
	virtual void unexecute() = 0;
};
//Implement of command, can have different concrete implementation
class tableLightCommand : public ICommand {
private:
	TableLight* m_tableLight;
public:
	tableLightCommand(TableLight* L) {
		m_tableLight = L;
	}
	virtual ~tableLightCommand() {
		delete m_tableLight;
	}
	
	void execute() {
		m_tableLight->turnOn();
	}

	void unexecute() {
		m_tableLight->turnOff();
	}
};

//Invoker is like a controller, which will trigger the command to execute
class Invoker {
private:
	//we can have multiple ICommand here, represents to different input
	ICommand* m_OnOff;
	//ICommand* m_LightDim;
	//ICommand* m_Mode;
public:
	Invoker(ICommand* C) {
		m_OnOff = C;
	}
	virtual ~Invoker() {
		delete m_OnOff;
	}
	void clickOn() {
		m_OnOff->execute();
	}
	void clickOff() {
		m_OnOff->unexecute();
	}

};

int main() {
	Invoker* testInvoker = new Invoker(new tableLightCommand(new TableLight()));
	testInvoker->clickOn();
	testInvoker->clickOff();

	system("pause");
	return 0;
}


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

int main() {
	std::string* S1 = new std::string("123");
	std::string* S2 = new std::string("-23");
	ITarget* iAdaptor = new Adaptor(new Adaptee(), S1, S2, '-');
	std::cout << "The sum of the two is: " << iAdaptor->Sum() << std::endl;
	system("pause");
	return 0;
}

//Facade Pattern
//Façade Pattern provides a unified interface to a set of interfaces in a 
//subsystem. Façade defines a high level interface that makes the subsystem 
//easier to use.
//Omit the code here, check URM graph.


//Proxy Pattern
//Proxy Pattern  provides surrogate or placeholder for another object to control access to it. 
//Some intentions for using proxy: remote proxy, virtual proxy or protection proxy.
enum AccountType {
	Administrator,
	NormalClient
};

//Interface for base class, we will have a delete operation in this class.
class IRequestDelete {
public:
	virtual void DeleteFile(std::string* myFile) = 0;
};

//Concrete implementation of Request Delete class
class CRequestDelete : public IRequestDelete {
public:
	//Do the actual deletion here
	void DeleteFile(std::string* myFile) {
		std::cout << *myFile << " has been deleted!" << std::endl;
		delete myFile;
	}
};

//Proxy of request delete class. Client should always interact with proxy instead of concrete
//implementation of request delete class.
class proxyRequestDelete : public IRequestDelete {
private:
	AccountType m_Acc;
	IRequestDelete* m_cRD;
public:
	proxyRequestDelete(AccountType Acc, IRequestDelete* iRD) {
		m_Acc = Acc;
		m_cRD = iRD;
	}
	~proxyRequestDelete() {
		delete m_cRD;
	}
	void DeleteFile(std::string* myFile) {
		if (m_Acc == AccountType::NormalClient)
			std::cout << "You do not have permission to do delete operation!" << std::endl;
		else
			m_cRD->DeleteFile(myFile);
	}
};

int main() {
	std::string* testString = new std::string("SecretFile.text");
	IRequestDelete* iRD = new proxyRequestDelete(AccountType::NormalClient, new CRequestDelete());
	iRD->DeleteFile(testString);
	IRequestDelete* iRD1 = new proxyRequestDelete(AccountType::Administrator, new CRequestDelete());
	iRD1->DeleteFile(testString);

	system("pause");
	return 0;
}


//Bridge Pattern
//Bridge pattern: The intent of the bridge pattern is to decouple an abstraction
//from its implementation so that the two can vary independently.
//The actual content class
class ArtistInfo {
private:
	std::string m_name;
	int m_age;
	std::string m_Intro;

public:
	ArtistInfo(std::string name, int age, std::string intro) {
		m_name = name;
		m_age = age;
		m_Intro = intro;
	}
	std::string getName() {
		return m_name;
	}
	std::string Summary() {
		return m_Intro;
	}
	int getAge() {
		return m_age;
	}

};

class BookInfo {
private:
	std::string m_name;
	int m_publishYear;
	std::string m_Author;
public:
	BookInfo(std::string name, int year, std::string author) {
		m_name = name;
		m_publishYear = year;
		m_Author = author;
	}
	std::string getName() {
		return m_name;
		
	}
	std::string AuthorInfo() {
		return m_Author;
	}
	int getYear() {
		return m_publishYear;
	}
};

//Define the resource interface. The actual implementation can vary based on the same 
//interface
class IResources {
public:
	virtual void Title() = 0;
	virtual void chronoInfo() = 0;
	virtual void Intro() = 0;
};

//Define the display interface to display information of Resources
class IDisplay {
protected:
	IResources* m_resource;
public:
	IDisplay(IResources* res) {
		m_resource = res;
	}
	virtual ~IDisplay() {
		delete m_resource;
	}
	virtual void ShowInfo() = 0;
};

//Concrete implementation, note we can change ShowInfo content here!
class LongForm : public IDisplay {
public:
	LongForm(IResources* res) : IDisplay(res){}
	void ShowInfo() {
		std::cout << "This is the long form view!" << std::endl;
		std::cout << "+++++++++++++++++++++++++++" << std::endl;
		std::cout << "Title: ";
		m_resource->Title();
		std::cout << "The images is below: " << std::endl;
		std::cout << "===========================" << std::endl;
		m_resource->Intro();
		m_resource->chronoInfo();
		std::cout << "Close the Page!" << std::endl;
		std::cout << std::endl;
	}
};

class ShortForm : public IDisplay {
public:
	ShortForm(IResources* res) : IDisplay(res) {}
	void ShowInfo() {
		std::cout << "This is the short form view!" << std::endl;
		std::cout << "............................" << std::endl;
		std::cout << "Title: ";
		m_resource->Title();
		std::cout << "The images is below: " << std::endl;
		std::cout << "*****------*****" << std::endl;
		m_resource->chronoInfo();
		m_resource->Intro();
		std::cout << std::endl;
	}
};

class ArtistResource : public IResources {
private:
	ArtistInfo* m_Artist;
public:
	ArtistResource(ArtistInfo* a) {
		m_Artist = a;
	}
	void Title() {
		std::cout << m_Artist->getName() << std::endl; 
	}
	void Intro() {
		std::cout << m_Artist->Summary() << std::endl;
	}
	void chronoInfo() {
		std::cout << "This artist is " << m_Artist->getAge() << " years old." << std::endl;
	}
};

class BookResource : public IResources {
private:
	BookInfo* m_Book;
public:
	BookResource(BookInfo* b) {
		m_Book = b;
	}
	void Title() {
		std::cout << m_Book->getName() << std::endl;
	}
	void Intro() {
		std::cout << "This book is written by ";
		std::cout << m_Book->AuthorInfo() << std::endl;
	}
	void chronoInfo() {
		std::cout << "This book is published in year " << m_Book-> getYear() << std::endl;
	}
};

/*
Note we now have artist resource (A) and book resource (B), long term view (L) and short 
term view (S). We can easily combine them together. For example, (A-S), (A-L), (B-L), (B-S).
This is what this meant to solve. The combination problem within different classes.
*/
int main() {
	ArtistInfo aArtist("Machael Jackson", 39, "He is a gigant in music which affect thousands of people!");
	BookInfo aBook("Justice", 1987, "Eric Meier");
	IDisplay* longFormInst = new LongForm(new ArtistResource(&aArtist));
	IDisplay* shortFormInst = new ShortForm(new BookResource(&aBook));
	IDisplay* longFormInst_02 = new LongForm(new BookResource(&aBook));
	longFormInst->ShowInfo();
	longFormInst_02->ShowInfo();
	shortFormInst->ShowInfo();
	
	system("pause");
	return 0;
}


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





