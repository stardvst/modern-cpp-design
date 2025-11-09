#include <iostream>
#include <mutex>
#include <thread>

class Singleton
{
public:
	static Singleton &getInstance()
	{
		// double checked locking pattern
		if (!instance)
		{
			std::lock_guard<std::mutex> lock(mutex);
			if (!instance)
				instance = new Singleton();
		}
		return *instance;
	}

	Singleton(const Singleton &) = delete;

private:
	Singleton() = default;
	~Singleton() = default;

	static Singleton *instance;
	static std::mutex mutex;
};

Singleton *Singleton::instance = nullptr;
std::mutex Singleton::mutex;

int main()
{
	std::thread t1([] {
		const Singleton &s1 = Singleton::getInstance();
		std::cout << "s1: " << &s1 << "\n";
	});

	std::thread t2([] {
		const Singleton &s2 = Singleton::getInstance();
		std::cout << "s2: " << &s2 << "\n";
	});

	t1.join();
	t2.join();

	std::cout << "Are both instances the same? " << (&Singleton::getInstance() == &Singleton::getInstance() ? "Yes" : "No") << "\n";

	return 0;
}
