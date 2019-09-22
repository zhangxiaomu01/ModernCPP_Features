#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<thread>
#include<mutex>

//section 1
void function_1() {
	std::cout << "I am good today!" << std::endl;
}

void function_2() {
	for (int i = 0; i < 100; ++i) {
		std::cout << "From main thread: " << i << " " << std::endl;
	}
}

int main() {
	//function_1();//no multithreading!
	std::thread t1(function_1); //function_1 is running on thread t1

	//You can only join or detach thread only once.
	//t1.join();//main thread waits for t1 to finish
	//if two threads sharing the same resource (e.g. std::cout), the thread who owns
	//the resource should stay alive as long as other threads is accessing the
	//resource. (e.g. main thread in our case)
	//t1.detach(); //t1 will run freely on its own -- deamon process now

    //handles the situation when other thread has to throw an exception, we can
    //still join the created thread!
    //we can also use a wrapper class for t1
    //wrapper w(t1), then when wrapper destroyed, the destructor handles t1.join()
    try{
        function_2();
    }catch(...){
        t1.join();
        throw 1;
    }

	//will be false for this condition
	if (t1.joinable()) t1.join();

	system("pause");
	return 0;
}


//section 2 : Thread Management
class Fact {
public:
	//By default, parameter to a thread should always be passed by value
	void operator()(std::string msg) {
		std::cout << "from " << std::this_thread::get_id() << msg << std::endl;
	}
};

class Fact_02 {
public:
	//By default, parameter to a thread should always be passed by value
	void operator()(std::string& msg) {
		std::cout << "from t2 " << std::this_thread::get_id() << msg << std::endl;
	}
};

int main() {
	std::string s = "In the name of justice... \n";
	Fact fac;
	//in order pass s to fac, we need to put it here as an additional parameter
	//pass by value.
	std::thread t1(fac, s); 
	//or we can directly initialize functor here
	//note we have to add () to Fact(), or our t1 will be treated as a function with
	//1 function pointer parameter and return type to be a thread object
	//std::thread t1((Fact()));

	//since function in t2 is pass by ref, you must use std::ref() here, or it won't
	//work: Now t1 and t2 share the same memory from s
	std::thread t2((Fact_02()), std::ref(s));

	//We can also move s2 to another thread, which will save the memory and avoid 
	//copy (safe and efficient. Note once moved, can no longer use it in main 
	//thread)
	std::string s2("Love is when I stand by yourside!\n");
	std::thread t3((Fact()), std::move(s2));

	//The following will not compile, because thread can only be moved (no copy)
	//std::thread t4 = t1;
	std::thread t4 = std::move(t1);

	//We can call get_id() method to get the id of a thread
	std::cout << t4.get_id() << std::endl;
	//get id of current thread
	std::cout << std::this_thread::get_id() << std::endl;

	//will be false for this condition
	if (t1.joinable()) t1.join();
	t2.join();
	t3.join();
	if (t4.joinable()) t4.join();

	//Then how many threads should we allocate? Too many threads which exceed the 
	//hardware support will cause too many unnecessarily context switching and 
	//degrade the performance.
	//The following method will provide an indication that how many threads can run
	//concurrently for current hardware
	std::cout << std::thread::hardware_concurrency() << std::endl;


	system("pause");
	return 0;
}


//section 3 : Data Race and Mutex


