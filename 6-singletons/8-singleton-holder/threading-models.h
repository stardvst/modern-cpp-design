#pragma once

template <class T>
class SingleThreaded
{
public:
	struct Lock
	{
		Lock() = default;
		explicit Lock(const SingleThreaded &)
		{
		}
	};

	using VolatileType = T;
	using IntType = int;

	static IntType AtomicAdd(volatile IntType &lval, IntType val)
	{
		return lval += val;
	}

	static IntType AtomicSubtract(volatile IntType &lval, IntType val)
	{
		return lval -= val;
	}

	static IntType AtomicMultiply(volatile IntType &lval, IntType val)
	{
		return lval *= val;
	}

	static IntType AtomicDivide(volatile IntType &lval, IntType val)
	{
		return lval /= val;
	}

	static IntType AtomicIncrement(volatile IntType &lval)
	{
		return lval += 1;
	}

	static IntType AtomicDecrement(volatile IntType &lval)
	{
		return lval -= 1;
	}

	static void AtomicAssign(volatile IntType &lval, IntType val)
	{
		lval = val;
	}

	static void AtomicAssign(IntType &lval, volatile IntType &val)
	{
		lval = val;
	}
};
