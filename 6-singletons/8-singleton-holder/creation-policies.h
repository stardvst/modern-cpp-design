#pragma once

#include <cstdlib>

template <class T>
struct CreateUsingNew
{
	static T *Create()
	{
		return new T();
	}

	static void Destroy(T *p)
	{
		delete p;
	}
};

template <class T>
struct CreateUsingMalloc
{
	static T *Create()
	{
		void *p = std::malloc(sizeof(T));
		if (!p)
			return nullptr;
		return new (p) T();
	}

	static void Destroy(T *p)
	{
		p->~T();
		std::free(p);
	}
};

template <class T>
struct CreateStatic
{
#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning(push)
	// alignment of a member was sensitive to packing
#pragma warning(disable : 4121)
#endif // _MSC_VER
	// By including these types, the MaxAlign union ensures that
	// the static memory block used for creating and destroying instances of T is
	// aligned for all possible types T.
	union MaxAlign
	{
		// This line creates a character array m_char with a length equal to the size of T.
		// This is used to ensure that the static memory block used for creating and destroying
		// instances of T is aligned for all possible types T.
		char m_char[sizeof(T)];

		short int m_shortInt;
		int m_int;
		long int m_longInt;
		float m_float;
		double m_double;
		long double m_longDouble;
		struct Test;
		int Test::*m_pMember;
		int (Test::*m_pMemberFn)(int);
	};
#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning(pop)
#endif // _MSC_VER

	static T *Create()
	{
		static MaxAlign staticMemory;
		return new (&staticMemory) T;
	}

	static void Destroy(T *p)
	{
		p->~T();
	}
};
