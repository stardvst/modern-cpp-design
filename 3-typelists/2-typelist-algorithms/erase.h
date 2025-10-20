#pragma once

#include "typelist.h"

template <typename TList1, typename T>
struct Erase;

template <typename T>
struct Erase<NullType, T>
{
  using Result = NullType;
};

template <typename T, typename Tail>
struct Erase<Typelist<T, Tail>, T>
{
  using Result = Tail;
};

template <typename Head, typename Tail, typename T>
struct Erase<Typelist<Head, Tail>, T>
{
  using Result = Typelist<Head, typename Erase<Tail, T>::Result>;
};
