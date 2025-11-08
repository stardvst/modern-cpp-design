#include <cassert>
#include <iostream>

namespace Private
{
class LifetimeTracker
{
public:
	LifetimeTracker(unsigned int longevity) : m_longevity(longevity)
	{
	}

	virtual ~LifetimeTracker() = 0;

	friend inline bool Compare(const LifetimeTracker *lhs, const LifetimeTracker &rhs)
	{
		return lhs->m_longevity < rhs.m_longevity;
	}

private:
	unsigned int m_longevity{};
};

inline LifetimeTracker::~LifetimeTracker() = default;

using LifetimeTrackerArray = LifetimeTracker **;
extern LifetimeTrackerArray pTrackerArray;
extern unsigned int elementCount;

template <typename T>
struct Deleter
{
	static void Delete(T *p)
	{
		delete p;
	}
};

template <typename T, typename Destroyer = Deleter<T>>
class ConcreteLifetimeTracker : public LifetimeTracker
{
public:
	ConcreteLifetimeTracker(T *pInstance, unsigned int longevity, Destroyer deleter)
		: LifetimeTracker(longevity), m_pInstance(pInstance), m_deleter(deleter)
	{
	}

	~ConcreteLifetimeTracker() override
	{
		m_deleter(m_pInstance);
	}

private:
	T *m_pInstance{nullptr};
	Destroyer m_deleter;
};

void atExitFunction();

} // namespace Private

template <typename T, typename Destroyer>
void SetLongevity(T *pInstance, unsigned int longevity, Destroyer deleter = Deleter<T>::Delete)
{
	using namespace Private;

	LifetimeTrackerArray newArray =
	  static_cast<LifetimeTrackerArray>(std::realloc(pTrackerArray, sizeof(*pTrackerArray) * (elementCount + 1)));
	if (!newArray)
		throw std::bad_alloc();

	pTrackerArray = newArray;

	LifetimeTracker *pTracker = new ConcreteLifetimeTracker<T, Destroyer>(pInstance, longevity, deleter);
	LifetimeTrackerArray pos = std::upper_bound(pTrackerArray, pTrackerArray + elementCount, longevity, LifetimeTracker::Compare);
	std::copy_backward(pos, pTrackerArray + elementCount, pTrackerArray + elementCount + 1);

	*pos = pTracker;
	++elementCount;

	std::atexit(atExitFunction);
}

static void atExitFunction()
{
	using namespace Private;
	assert(elementCount > 0 && pTrackerArray != nullptr);
	LifetimeTracker *pTopTracker = pTrackerArray[elementCount - 1];
	pTrackerArray = static_cast<LifetimeTrackerArray>(std::realloc(pTrackerArray, (--elementCount) * sizeof(*pTrackerArray)));
	delete pTopTracker;
}

class Singleton
{
public:
	static Singleton &getInstance()
	{
		if (!instance)
			createInstance();
		return *instance;
	}

	Singleton(const Singleton &) = delete;

protected:
	Singleton() = default;

private:
	static void createInstance()
	{
		instance = new Singleton();
		SetLongevity(instance, 100, [](Singleton *p) { delete p; });
	}

	static Singleton *instance;
};

Singleton *Singleton::instance = nullptr;

int main()
{
	const Singleton &s1 = Singleton::getInstance();
	const Singleton &s2 = Singleton::getInstance();

	std::cout << "s1: " << &s1 << "\n";
	std::cout << "s2: " << &s2 << "\n";
	std::cout << "Are both instances the same? " << (&s1 == &s2 ? "Yes" : "No") << "\n";

	return 0;
}
