#pragma once

#include "typelist.h"

template <typename TList1, typename T>
struct EraseAll;

template <typename T>
struct EraseAll<NullType, T>
{
  using Result = NullType;
};

template <typename T, typename Tail>
struct EraseAll<Typelist<T, Tail>, T>
{
  // the difference from Erase is here: we continue erasing in the Tail
  using Result = typename EraseAll<Tail, T>::Result;
};

template <typename Head, typename Tail, typename T>
struct EraseAll<Typelist<Head, Tail>, T>
{
  using Result = Typelist<Head, typename EraseAll<Tail, T>::Result>;
};
