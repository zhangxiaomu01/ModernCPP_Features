#include<iostream>
#include<fstream>
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


//**************************************************************************
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


//**************************************************************************
//Section 3 : Data Race and Mutex
std::mutex mu;

//This will work perfectly for this case, however, if the code between lock() and 
//unlock() throws an exception, then we will be unable to unlock the mutex!
//We can use lock_guard to handle the lock and unlock operation!
/*
void shared_print(std::string msg, int num) {
	mu.lock();
	std::cout << msg << num << std::endl;
	mu.unlock();
}
*/

void shared_print(std::string msg, int num) {
	//whenever guard goes out of scope, mu will be unlocked!
	std::lock_guard<std::mutex> guard(mu);
	//mu.lock();
	//problem# 2: Now we still have a problem with this code, since cout is a 
	//global method, other thread could still be possible to directly use it!
	std::cout << msg << num << std::endl;
	//mu.unlock();
}

//Let's handle problem #2
//We encapsulate the mutex and stream data to class and never expose ofstream to
//out!
class logFile {
private:
	std::ofstream f;
	std::mutex m_mutex;
public:
	logFile() {
		f.open("input.txt");
	}
	~logFile() {
		f.close();
	}
	//Now all the threads must call shared_print function to output our val
	void shared_print(std::string id, int val) {
		std::lock_guard<std::mutex> guard(m_mutex);
		f << id << " " << val << std::endl;
	}
	
	//Never return common method out of the class
	std::ofstream& getStream() {
		return f;
	}
	//Never pass ofstream to user provided function!
	void processf(void fun(std::ostream& f)) {
		fun(f);
	}

};


void func_01(logFile& logf) {
	for (int i = 0; i > -100; --i) {
		logf.shared_print("From t1: ", i);
	}
}

//problem# 3, even if we encapsulate the mutex and data in a class, never expose 
//data outside the class like the above example. It does not guarantee the thread
//safe! 
//If we design a stack like before, using the mutex to protect the top() and pop()
//method.
class stack {
private:
	int* m_array;
	int m_size;
	std::mutex m_mutex;
public:
	//get the top element of the stack
	int& top();
	//pop the top element
	void pop();
	//...
};

void process(int& v) {
	v = 0;
}

//suppose that function_02 will be used in 2 threads. Then consider the case
//stack: [3,2,9,11]. In the first thread, we will get 11, then we go to second
//thread, and get 11 as well, then in thread 1, we pop 11, then in thread 2, we pop
//9. Then we process v in thread 1, then in thread 2. You can see we lost 9 
//permanantly!
void function_02(stack& st) {
	int v = st.top();
	st.pop();
	process(v);
}

//Problem#3: we need to handle problem 3 now! The problem is because we separate 
//top() and pop() method separately, which caused the above problem! We can delete
//top() method, and let pop() method both get the top element and delete the top
//element. (It is not exceptional safe, but now it is thread safe!)

/*
Avoid data racing:
1. Use the mutex to synchronize the data access
2. Never leak a handle of file to outside!
3. Design interface properly!
*/

int main() {
	logFile logf;
	std::thread t1(func_01, std::ref(logf));

	for (int i = 0; i < 100; ++i) {
		logf.shared_print("From main thread: ", i);
	}

	if (t1.joinable()) t1.join();

	system("pause");
	return 0;
}


//**************************************************************************
//Section 4: Dead lock


