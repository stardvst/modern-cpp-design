#include <cstdlib>
#include <iostream>

class Singleton
{
public:
	static Singleton &getInstance()
	{
		if (!instance)
		{
			if (destroyed)
				onDestroyedReference();
			else
				createInstance();
		}
		return *instance;
	}

	Singleton(const Singleton &) = delete;

protected:
	Singleton() = default;

	virtual ~Singleton()
	{
		instance = nullptr;
		destroyed = true;
	}

private:
	static void createInstance()
	{
		static Singleton localInstance;
		instance = &localInstance;
	}

	static void onDestroyedReference()
	{
		createInstance();

		new (instance) Singleton(); // UB???
		atexit(killPhoenixSingleton);
		destroyed = false;
	}

	static void killPhoenixSingleton()
	{
		if (instance)
			instance->~Singleton();
	}

	static Singleton *instance;
	static bool destroyed;
};

Singleton *Singleton::instance = nullptr;
bool Singleton::destroyed = false;

class Logger : public Singleton
{
public:
	void log(const std::string &message)
	{
		std::cout << "Log: " << message << "\n";
	}
};

class Displayer : public Singleton
{
public:
	Displayer()
	{
		static_cast<Logger &>(Singleton::getInstance()).log("Displayer construction failed");
	}
};

class KeyboardHandler : public Singleton
{
public:
	~KeyboardHandler()
	{
		try
		{
			static_cast<Logger &>(Singleton::getInstance()).log("KeyboardHandler destruction failed"); // not printed
		}
		catch (const std::exception &e)
		{
			std::cerr << "Failed to log during KeyboardHandler destruction: " << e.what() << '\n'; // printed
		}
	}
};

int main()
{
	KeyboardHandler kb;
	Displayer dp;

	return 0;
}
