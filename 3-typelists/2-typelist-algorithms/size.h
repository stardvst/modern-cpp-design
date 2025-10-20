#pragma once

#include "typelist.h"

template <typename TList>
struct Size;

template <>
struct Size<NullType>
{
  enum { value = 0 };
};

template <typename T, typename U>
struct Size<Typelist<T, U>>
{
  enum { value = 1 + Size<U>::value };
};
