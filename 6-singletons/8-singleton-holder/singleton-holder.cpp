#include "creation-policies.h"
#include "lifetime-policies.h"
#include "threading-models.h"

#include <cassert>
#include <string>

#ifdef _MSC_VER
#define C_CALLING_CONVENTION_QUALIFIER __cdecl
#else
#define C_CALLING_CONVENTION_QUALIFIER
#endif

template <
	typename Singleton,
	template <class> class CreationPolicy = CreateUsingNew,
	template <class> class LifetimePolicy = DefaultLifetime,
	template <class> class ThreadingModel = SingleThreaded
>
class SingletonHolder
{
public:
	using SingletonType = ThreadingModel<Singleton *>::VolatileType;

	static Singleton &getInstance()
	{
		if (!instance)
			createInstance();
		return *instance;
	}

	SingletonHolder(const SingletonHolder &) = delete;

private:
	SingletonHolder() = default;
	~SingletonHolder() = default;

	static void createInstance();
	static void C_CALLING_CONVENTION_QUALIFIER destroySingleton();

	static SingletonType instance;
	static bool destroyed;
};

template <
	typename S,
	template <class> class C,
	template <class> class L,
	template <class> class T
>
typename SingletonHolder<S, C, L, T>::SingletonType SingletonHolder<S, C, L, T>::instance;

template <
	typename S,
	template <class> class C,
	template <class> class L,
	template <class> class T
>
bool SingletonHolder<S, C, L, T>::destroyed = false;

template <
	typename S,
	template <class> class CreationPolicy,
	template <class> class LifetimePolicy,
	template <class> class ThreadingModel
>
void SingletonHolder<S, CreationPolicy, LifetimePolicy, ThreadingModel>::createInstance()
{
	typename ThreadingModel<S>::Lock guard;

	if (!instance)
	{
		if (destroyed)
		{
			LifetimePolicy<S>::OnDestroyedReference();
			destroyed = false;
		}
		instance = CreationPolicy<S>::Create();
		LifetimePolicy<S>::ScheduleDestruction(instance, &destroySingleton);
	}
}

template <
	typename S,
	template <class> class CreationPolicy,
	template <class> class L,
	template <class> class T
>
void C_CALLING_CONVENTION_QUALIFIER SingletonHolder<S, CreationPolicy, L, T>::destroySingleton()
{
	assert(!destroyed && "Singleton already destroyed");
	CreationPolicy<S>::Destroy(instance);
	instance = nullptr;
	destroyed = true;
}

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
