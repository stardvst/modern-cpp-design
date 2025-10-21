#pragma once

#include "typelist.h"

template <typename TList1, typename T, typename U>
struct Replace;

template <typename T, typename U>
struct Replace<NullType, T, U>
{
  using Result = NullType;
};

template <typename T, typename Tail, typename U>
struct Replace<Typelist<T, Tail>, T, U>
{
  using Result = Typelist<U, Tail>;

  // replace all
  // using Result = Typelist<U, typename Replace<Tail, T, U>::Result>;
};

template <typename Head, typename Tail, typename T, typename U>
struct Replace<Typelist<Head, Tail>, T, U>
{
  using Result = Typelist<Head, typename Replace<Tail, T, U>::Result>;
};
