#include "singleton-holder.h"

#include <cassert>
#include <string>

int main()
{
	using IntSingleton = SingletonHolder<int>;
	IntSingleton::getInstance() = 42;
	assert(IntSingleton::getInstance() == 42);

	using AnotherIntSingleton = SingletonHolder<int, CreateUsingMalloc>;
	AnotherIntSingleton::getInstance() = 84;
	assert(AnotherIntSingleton::getInstance() == 84);

	using StaticIntSingleton = SingletonHolder<int, CreateStatic>;
	StaticIntSingleton::getInstance() = 168;
	assert(StaticIntSingleton::getInstance() == 168);

	std::string hello = "Hello, Singleton!";
	using NoDestroyStringSingleton = SingletonHolder<std::string, CreateUsingNew, NoDestroy>;
	NoDestroyStringSingleton::getInstance() = hello;
	assert(NoDestroyStringSingleton::getInstance() == hello);
}
