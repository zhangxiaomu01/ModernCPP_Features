#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<list>
#include<queue>
#include<thread>
#include<mutex>
#include<future>

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
//A dead lock example!
class logFile {
private:
	std::ofstream f;
	std::mutex m_mutex;
	//imagine we will have two mutexes to protect different parts of the data!
	std::mutex m_mutex_02;
public:
	logFile() {
		f.open("input.txt");
	}
	~logFile() {
		f.close();
	}
	//The following two shared_print will cause dead lock!
	//The first version of shared_print
	void shared_print(std::string id, int val) {
		std::lock_guard<std::mutex> guard(m_mutex);
		std::lock_guard<std::mutex> guard02(m_mutex_02);
		std::cout << id << " " << val << std::endl;
	}
	
	void shared_print02(std::string id, int val) {
		//we lock the mutex in different order here! 
		//Risk of dead locking!
		std::lock_guard<std::mutex> guard02(m_mutex_02);
		std::lock_guard<std::mutex> guard(m_mutex);
		std::cout << id << " " << val << std::endl;
	}

	//solution 1 to fix above code: make both of the mutex lock in the same order
	/*
	void shared_print(std::string id, int val) {
		std::lock_guard<std::mutex> guard(m_mutex);
		std::lock_guard<std::mutex> guard02(m_mutex_02);
		std::cout << id << " " << val << std::endl;
	}

	void shared_print02(std::string id, int val) {
		std::lock_guard<std::mutex> guard(m_mutex);
		std::lock_guard<std::mutex> guard02(m_mutex_02);
		std::cout << id << " " << val << std::endl;
	}
	*/

	//solution 2: using std::lock function! The reason why it works is the same as
	//solution 1.
	/*
	void shared_print(std::string id, int val) {
		//std::lock defines the algorithm to lock the mutex while preventing 
		//potential deadlock
		std::lock(m_mutex, m_mutex_02);
		//std::adopt_lock tell the guard that mutex has already been locked, guard
		//only needs to handle unlock situation!
		std::lock_guard<std::mutex> guard(m_mutex, std::adopt_lock);
		std::lock_guard<std::mutex> guard02(m_mutex_02, std::adopt_lock);
		std::cout << id << " " << val << std::endl;
	}

	void shared_print02(std::string id, int val) {
		std::lock(m_mutex, m_mutex_02);
		std::lock_guard<std::mutex> guard(m_mutex, std::adopt_lock);
		std::lock_guard<std::mutex> guard02(m_mutex_02, std::adopt_lock);
		std::cout << id << " " << val << std::endl;
	}
	*/
	
};

//call the original shared_print function!
void func_01(logFile& logf) {
	for (int i = 0; i > -100; --i) {
		logf.shared_print("From t1: ", i);
	}
}


/*
Principles to avoid deadlock:
1. Prefer locking single mutex at a time!

2. Avoid locking a single mutex and calling a user provided funtion!
e.g. 
{
	std::lock_guard<std::mutex> guard(mu);
	user_function(); //No!
}
The reason is that user function may lock other mutex or even lock the same mutex
again!

3. Using std::lock() method to lock multiple mutexes if you really want to!

4. When std::lock() is impossible, try to lock the mutexes in the same order for all
threads!

Locking Granularity!
- Fine-grained lock: protects the small amount of data!
- Coarse-grained lock: protects the large amount of data!
*/


int main() {
	logFile logf;
	std::thread t1(func_01, std::ref(logf));

	//call the shared_print02 here
	for (int i = 0; i < 100; ++i) {
		logf.shared_print02("From main thread: ", i);
	}

	if (t1.joinable()) t1.join();

	system("pause");
	return 0;
}



//**************************************************************************
//Section 5: Unique lock and lazy initialization
//Unique lock and lazy initialization
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

	void shared_print(std::string id, int val) {
		//std::lock_guard<std::mutex> guard(m_mutex);

		//std::unique_lock<mutex> is more flexible compared with std::lock_guard
		//You can defer lock, lock and unlock multiple times if using unique_lock
		//It's more expensive compared with lock_guard<mutex>, if there is no need
		//for the flexibility, lock_guard preferred!
		std::unique_lock<std::mutex> uLocker(m_mutex);

		//We can even defer the lock operation for later use
		//std::unique_lock<std::mutex> uLocker02(m_mutex, std::defer_lock);

		//unique_lock can be moved, by moving one unique_lock to another, we are 
		//actually transferring the ownership of the mutex from one lock to another.
		//We cannot even move (no copy) lock_guard! 
		//std::unique_lock<std::mutex> uLocker03 = std::move(uLocker);

		std::cout << id << " " << val << std::endl;
		uLocker.unlock();
		//...Other staff... no need for lock
		uLocker.lock();
		//...Staff need the lock...
	}
};

class logFile01 {
private:
	std::ofstream f;
	std::mutex m_mutex;
	std::mutex m_mutexOpenFile;
	std::once_flag m_flag;

public:
	logFile01() {
		//We do not want to open it right now if our shared_print() method not even
		//called! We can move the function to our shared_print() method!
		//f.open("input.txt");
	}
	~logFile01() {
		if(f.is_open())
			f.close();
	}

	void shared_print(std::string id, int val) {
		//Not thread safe now! We only need the file to be opened once!
		//Lazy initialization. Initialization uppon first use idiom!
		//if(!f.is_open())
		//	f.open("input.txt");

		//Solution 1: make open file thread safe! 
		//This solution is not good because every time when we call shared_print, we
		//need to lock the m_mutexOpenFile mutex. Since we already know that we only
		//need to open the file once! It's too much overhead!
		//{
		//	//We have to put !f.is_open() in this mutex, or it will still not be
		//	//thread safe!
		//	std::unique_lock<std::mutex> uLockerFile(m_mutexOpenFile);
		//	if (!f.is_open())
		//		//Only put locker here will still not be thread safe!
		//		//std::unique_lock<std::mutex> uLockerFile(m_mutexOpenFile);
		//		f.open("input.txt");
		//}

		//Solution 2: using STL once_flag! 
		//& - capture f by reference! 
		//Preferred
		std::call_once(m_flag, [&]() { f.open("input.txt"); });

		std::unique_lock<std::mutex> uLocker(m_mutex);
		f << id << " " << val << std::endl;
	}
};

//call the original shared_print function!
void func_01(logFile& logf) {
	for (int i = 0; i > -100; --i) {
		logf.shared_print("From t1: ", i);
	}
}


int main() {
	logFile logf;
	std::thread t1(func_01, std::ref(logf));

	//call the shared_print02 here
	for (int i = 0; i < 100; ++i) {
		logf.shared_print("From main thread: ", i);
	}

	if (t1.joinable()) t1.join();

	system("pause");
	return 0;
}



//**************************************************************************
//Section 6: Condition variables!
std::mutex g_mu;
std::queue<int> g_queue;
std::condition_variable g_Con;

void producer() {
	int  i = 10;
	while (i >= 0) {
		std::unique_lock<std::mutex> locker(g_mu);
		g_queue.push(i * 10);
		//we need to unlock first before notify other thread(s)
		locker.unlock();
		//notify one thread waiting for the unique_lock
		//we can use notify_all to awake all the threads waiting for the unique_lock
		g_Con.notify_one();
		//sleep for 0.2 seconds!
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		i--;
	}
	
}

void consumer() {
	int var = 12;
	while (var != 0) {
		std::unique_lock<std::mutex> locker(g_mu);
		// When thread consumer goes to sleep, it should release the locker
		// first. 
		//g_Con.wait(locker); //spurious wake!
		//if the thread 2 wakes up and finds that queue is empty, then it will go to
		//sleep again! It will continue when queue is not empty!
		g_Con.wait(locker, []() {return !g_queue.empty(); });
		var = g_queue.front();
		g_queue.pop();
		locker.unlock();
		std::cout << "consumer consumes data from producer: " << var << std::endl;
	}
}


int main() {
	std::thread t1(producer);
	std::thread t2(consumer);

	t1.join();
	t2.join();
	system("pause");
	return 0;
}



//**************************************************************************
//Section 7: Future and Promise!
std::mutex mu;

/*
Calculate factorial in child thread works perfectly here. However, imagine if we 
want to get the result from child, we need to define a shared variable from main
thread and pass it as a reference to factorial! In order to prevent data racing,
we also need to define mutex and condition variable to handle synchronization! 
Which is over complicated for this simple demand! We will discuss asych() method 
to handle the situation!
*/
void factorial(int N) {
	int res = 1;
	for (int i = N; i > 1; --i) {
		res *= i;
	}
	std::cout << "The res from child thread: " << res << std::endl;
}

int factorial_01(int N) {
	int res = 1;
	for (int i = N; i > 1; --i) {
		res *= i;
	}
	std::cout << "The res from child thread: " << res << std::endl;
	return res;
}

//We need to pass the future as a reference in order to get the promise 
//from main thread in the future!
int factorial_02(std::future<int>& f) {
	int res = 1;
	int N = f.get();//exception: std::future_errc::broken_promise
	for (int i = N; i > 1; --i) {
		res *= i;
	}
	std::cout << "The res from child thread: " << res << std::endl;
	return res;
}

int factorial_03(std::shared_future<int> f) {
	std::lock_guard<std::mutex> locker(mu);
	int res = 1;
	int N = f.get();//exception: std::future_errc::broken_promise
	for (int i = N; i > 1; --i) {
		res *= i;
	}
	std::cout << "The res from sf thread: " << res << std::endl;
	return res;
}

int main() {
	//works perfectly
	//std::thread t1(factorial, 4);
	//t1.join();

	//If we want to get result from child thread!
	//future class wrapper the result that we can get in the future!
	//This may or may not create a child thread to do the calculation. We can use 
	//the flag to control whether we want a new thread!
	std::future<int> fu = std::async(factorial_01, 4);

	//when std::launch::deferred flag is set, we actually deferred execution of
	//factorial_01 until we can fu.get() method! We will not create new threads and
	//factorial will be calculated in the main thread!
	//std::future<int> fu = std::async(std::launch::deferred,factorial_01, 4);

	//when std::launch::async flag is set, we create a new thread and calculate the
	//result
	//std::future<int> fu = std::async(std::launch::async, factorial_01, 4);

	//When combined together, it depends on the implementation to decide whether to
	//create a new thread! This is the default behavior of std::async()
	//std::future<int> fu = std::async(std::launch::async | std::launch::deferred, factorial_01, 4);


	//get the result from factorial function (need a return value for factorial
	//function)
	//For each future, we can only call the get() method once
	int x = fu.get();
	std::cout << "Print result from main thread: " << x << std::endl;

	//************************************************************************//
	//We can also use future to pass the value from parent thread to child thread!
	//a promise is made that in some future, we will provide the a value to child
	//thread! 
	std::promise<int> p;
	std::future<int> fu_01 = p.get_future();
	std::future<int> fu_02 = std::async(std::launch::async, factorial_02, std::ref(fu_01));

	//do something else
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	//We pass our promise with a value! If we forget to keep our promise, we will 
	//have an exception of std::future_errc::broken_promise
	p.set_value(4);
	//We can set an expection if we know we cannot pass the value to the promise
	//p.set_exception(std::make_exception_ptr(std::runtime_error("Broken promise!")));


	int res_01 = fu_02.get();
	std::cout << "Print result from future 02: " << res_01 << std::endl;

	//Just like thread, neighther promise or future can be copied, only can be moved!
	std::promise<int> p2 = std::move(p);

	
	//************************************************************************//
	//shared_future! If we want to launch the factorial function many times by many
	//threads! If we stick to future, then we have to created multiple future 
	//objects. Instead, we can pass the shared_future (can be copied to) to multiple
	//threads!
	std::promise<int> pk;
	std::future<int> cfu = pk.get_future();
	std::shared_future<int> sf = cfu.share();

	//Note we pass a copy of sf to all the factorial_03
	std::future<int> f_01 = std::async(std::launch::async, factorial_03, sf);
	std::future<int> f_02 = std::async(std::launch::async, factorial_03, sf);
	std::future<int> f_03 = std::async(std::launch::async, factorial_03, sf);

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	//now we have synchronization issue, with std::cout. We fix that using mutex!
	pk.set_value(5);


	system("pause");
	return 0;
}


