#pragma once

#include "typelist.h"

template <typename TList, unsigned int index>
struct TypeAt;

template <typename Head, typename Tail>
struct TypeAt<Typelist<Head, Tail>, 0>
{
  using Result = Head;
};

template <typename Head, typename Tail, unsigned int index>
struct TypeAt<Typelist<Head, Tail>, index>
{
  using Result = typename TypeAt<Tail, index - 1>::Result;
};
