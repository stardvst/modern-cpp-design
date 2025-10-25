#pragma once

#include "typelist.h"

template <class TList, template <class> class Branch>
class GenerateHierarchy;

template <class Head, class Tail, template <class> class Branch>
class GenerateHierarchy<Typelist<Head, Tail>, Branch>
	: public GenerateHierarchy<Head, Branch>
	, public GenerateHierarchy<Tail, Branch>
{
  public:
	using TList = Typelist<Head, Tail>;
	using LeftBase = GenerateHierarchy<Head, Branch>;
	using RightBase = GenerateHierarchy<Tail, Branch>;

	template <typename T>
	struct Rebind
	{
		using Result = Branch<T>;
	};
};

template <class AtomicType, template <class> class Branch>
class GenerateHierarchy : public Branch<AtomicType>
{
  public:
	using LeftBase = Branch<AtomicType>;
};
