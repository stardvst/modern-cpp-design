#pragma once

#include "typelist.h"

template <typename TList, class T>
struct indexOf;

template <class T>
struct indexOf<NullType, T>
{
  enum { value = -1 };
};

template <class T, class Tail>
struct indexOf<Typelist<T, Tail>, T>
{
  enum { value = 0 };
};

template <class Head, class Tail, class T>
struct indexOf<Typelist<Head, Tail>, T>
{
private:
  enum { temp = indexOf<Tail, T>::value };

public:
  enum { value = (temp == -1 ? -1 : 1 + temp) };
};
