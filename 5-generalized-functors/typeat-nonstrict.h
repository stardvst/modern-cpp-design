#pragma once

#include "typelist.h"

template <class TList, unsigned int index, typename DefaultType = NullType>
struct TypeAtNonStrict
{
	typedef DefaultType Result;
};

template <class Head, class Tail, typename DefaultType>
struct TypeAtNonStrict<Typelist<Head, Tail>, 0, DefaultType>
{
	typedef Head Result;
};

template <class Head, class Tail, unsigned int i, typename DefaultType>
struct TypeAtNonStrict<Typelist<Head, Tail>, i, DefaultType>
{
	typedef typename TypeAtNonStrict<Tail, i - 1, DefaultType>::Result Result;
};
