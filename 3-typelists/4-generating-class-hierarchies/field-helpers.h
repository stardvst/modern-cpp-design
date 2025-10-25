#pragma once

template <class T, class H>
typename H::template Rebind<T>::Result &Field(H &value)
{
	// relies on implicit derived-to-base conversion
	return value;
}

template <typename T>
struct Type2Type
{
	using OriginalType = T;
};

template <int v>
struct Int2Type
{
	enum { value = v };
};

// Forward declaration with index first
template <int i, class H>
struct FieldHelper;

// base case: index 0 -> LeftBase
template <class H>
struct FieldHelper<0, H>
{
	using Result = typename H::LeftBase;
	static Result &Get(H &obj)
	{
		return static_cast<Result &>(obj);
	}
};

// recursive case: descend into RightBase and decrement index
template <int i, class H>
struct FieldHelper
{
	using Result = typename FieldHelper<i - 1, typename H::RightBase>::Result;
	static Result &Get(H &obj)
	{
		return FieldHelper<i - 1, typename H::RightBase>::Get(
		  static_cast<typename H::RightBase &>(obj));
	}
};

// index-based Field accessor
template <int i, class H>
typename FieldHelper<i, H>::Result &Field(H &obj)
{
	return FieldHelper<i, H>::Get(obj);
}
