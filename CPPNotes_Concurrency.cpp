#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<thread>

void function_1() {
	std::cout << "I am good today!" << std::endl;
}


int main() {
	//function_1();//no multithreading!
	std::thread t1(function_1); //function_1 is running on thread t1

	//You can only join or detach thread only once.
	//t1.join();//main thread waits for t1 to finish
	//if two threads sharing the same resource (e.g. std::cout), the thread who owns
	//the resource should stay alive as long as other threads is accessing the
	//resource. (e.g. main thread in our case)
	t1.detach(); //t1 will run freely on its own -- deamon process now

	//will be false for this condition
	if (t1.joinable()) t1.join();

	system("pause");
	return 0;
}

