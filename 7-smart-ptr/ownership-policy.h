#pragma once

template <int>
struct CompileTimeError;
template <>
struct CompileTimeError<true>
{
};

#define STATIC_CHECK(expr, msg)                                                                                                            \
	{                                                                                                                                      \
		CompileTimeError<((expr) != 0)> ERROR_##msg;                                                                                       \
		(void)ERROR_##msg;                                                                                                                 \
	}

template <class P>
class RefCounted
{
public:
	RefCounted() = default;

	RefCounted(const RefCounted &rhs) : m_count(rhs.m_count)
	{
	}

	template <class P1>
	RefCounted(const RefCounted<P1> &rhs) : m_count(reinterpret_cast<const RefCounted &>(rhs).m_count)
	{
	}

protected:
	P Clone(const P &val)
	{
		++(*m_count);
		return val;
	}

	bool Release(const P &)
	{
		if (--(*m_count) == 0)
		{
			delete m_count;
			return true; // Delete the object
		}
		return false; // Don't delete, still references exist
	}

	enum { destructiveCopy = false };

private:
	unsigned int *m_count{new unsigned int(1)};
};

template <class P>
struct DeepCopy
{
	DeepCopy() = default;

	template <class P1>
	DeepCopy(const DeepCopy<P1> &)
	{
	}

	static P Clone(const P &val)
	{
		return val->Clone();
	}

	static bool Release(const P &val)
	{
		return true;
	}

	enum { destructiveCopy = false };
};

template <class P>
class DestructiveCopy
{
public:
	DestructiveCopy() = default;

	template <class P1>
	DestructiveCopy(const DestructiveCopy<P1> &)
	{
	}

	template <class P1>
	static P Clone(P1 &val)
	{
		P result(val);
		val = P1();
		return result;
	}

	static bool Release(const P &)
	{
		return true;
	}

	enum { destructiveCopy = true };
};

template <class P>
class NoCopy
{
public:
	NoCopy() = default;

	template <class P1>
	NoCopy(const NoCopy<P1> &)
	{
	}

	static P Clone(const P &)
	{
		// Make it depended on template parameter
		static const bool DependedFalse = sizeof(P *) == 0;
		STATIC_CHECK(DependedFalse, This_Policy_Disallows_Value_Copying);
	}

	static bool Release(const P &)
	{
		return true;
	}

	static void Swap(NoCopy &)
	{
	}

	enum { destructiveCopy = false };
};
