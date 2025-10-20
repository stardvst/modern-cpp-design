#pragma once

#include "erase.h"
#include "typelist.h"

template <typename TList>
struct EraseDuplicates;

template <>
struct EraseDuplicates<NullType>
{
  using Result = NullType;
};

template <typename Head, typename Tail>
struct EraseDuplicates<Typelist<Head, Tail>>
{
private:
  using TailWithoutDuplicates = typename EraseDuplicates<Tail>::Result;
  using TailWithoutHead = typename Erase<TailWithoutDuplicates, Head>::Result;

public:
  using Result = Typelist<Head, TailWithoutHead>;
};
