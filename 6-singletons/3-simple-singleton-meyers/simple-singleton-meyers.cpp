#include <iostream>

class Singleton
{
public:
	static Singleton &getInstance()
	{
		static Singleton instance;
		return instance;
	}

	Singleton(const Singleton &) = delete;

private:
	Singleton() = default;
	~Singleton() = default;
};

int main()
{
	const Singleton &s1 = Singleton::getInstance();
	const Singleton &s2 = Singleton::getInstance();

	std::cout << "s1: " << &s1 << "\n";
	std::cout << "s2: " << &s2 << "\n";
	std::cout << "Are both instances the same? " << (&s1 == &s2 ? "Yes" : "No") << "\n";

	return 0;
}
