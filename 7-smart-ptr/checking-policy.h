#pragma once

#include <assert.h>
#include <stdexcept>
#include <string>

struct NullPointerException : public std::runtime_error
{
	NullPointerException() : std::runtime_error(std::string())
	{
	}
	const char *what() const throw()
	{
		return "Null Pointer Exception";
	}
};

template <class P>
struct NoCheck
{
	NoCheck() = default;

	template <class P1>
	NoCheck(const NoCheck<P1> &)
	{
	}

	static void OnDefault(const P &)
	{
	}

	static void OnInit(const P &)
	{
	}

	static void OnDereference(P)
	{
	}
};

template <class P>
struct AssertCheck
{
	AssertCheck() = default;

	template <class P1>
	AssertCheck(const AssertCheck<P1> &)
	{
	}

	template <class P1>
	AssertCheck(const NoCheck<P1> &)
	{
	}

	static void OnDefault(const P &)
	{
	}

	static void OnInit(const P &)
	{
	}

	static void OnDereference(P val)
	{
		assert(val);
		(void)val;
	}
};

template <class P>
struct AssertCheckStrict
{
	AssertCheckStrict() = default;

	template <class U>
	AssertCheckStrict(const AssertCheckStrict<U> &)
	{
	}

	template <class U>
	AssertCheckStrict(const AssertCheck<U> &)
	{
	}

	template <class P1>
	AssertCheckStrict(const NoCheck<P1> &)
	{
	}

	static void OnDefault(P val)
	{
		assert(val);
	}

	static void OnInit(P val)
	{
		assert(val);
	}

	static void OnDereference(P val)
	{
		assert(val);
	}
};

template <class P>
struct RejectNull
{
	RejectNull() = default;

	template <class P1>
	RejectNull(const RejectNull<P1> &)
	{
	}

	static void OnInit(P val)
	{
		if (!val)
			throw NullPointerException();
	}

	static void OnDefault(P val)
	{
		OnInit(val);
	}

	static void OnDereference(P val)
	{
		OnInit(val);
	}
};
