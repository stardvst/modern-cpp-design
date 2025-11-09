#pragma once

#include <cstdlib>
#include <stdexcept>

#ifdef _MSC_VER
#define C_CALLING_CONVENTION_QUALIFIER __cdecl
#else
#define C_CALLING_CONVENTION_QUALIFIER
#endif

using atexit_pfn_t = void(C_CALLING_CONVENTION_QUALIFIER *)();

namespace Private
{
template <class T>
struct Adapter
{
	void operator()(T *)
	{
		return m_pFun();
	}
	atexit_pfn_t m_pFun;
};
} // namespace Private

template <class T>
struct DefaultLifetime
{
	static void ScheduleDestruction(T *, atexit_pfn_t pFun)
	{
		std::atexit(pFun);
	}

	static void OnDestroyedReference()
	{
		throw std::logic_error("Destroyed Reference Detected");
	}
};

template <class T>
class SingletonWithLongevity
{
public:
	static void ScheduleDestruction(T *pObj, atexit_pfn_t pFun)
	{
		Private::Adapter<T> adapter = {pFun};
		SetLongevity(pObj, GetLongevity(pObj), adapter);
	}

	static void OnDestroyedReference()
	{
		throw std::logic_error("Dead Reference Detected");
	}
};

template <class T>
struct NoDestroy
{
	static void ScheduleDestruction(T *, atexit_pfn_t pFun)
	{
	}

	static void OnDestroyedReference()
	{
	}
};
