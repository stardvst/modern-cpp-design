#pragma once

#include "typelist.h"

template <typename TList1, typename T>
struct Append;

template <>
struct Append<NullType, NullType>
{
  using Result = NullType;
};

template <typename T>
struct Append<NullType, T>
{
  using Result = TYPELIST_1(T);
};

template <typename Head, typename Tail>
struct Append<NullType, Typelist<Head, Tail>>
{
  using Result = Typelist<Head, Tail>;
};

template <typename Head, typename Tail, typename T>
struct Append<Typelist<Head, Tail>, T>
{
  using Result = Typelist<Head, typename Append<Tail, T>::Result>;
};
