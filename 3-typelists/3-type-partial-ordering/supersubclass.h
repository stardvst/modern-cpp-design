#pragma once

template <typename T, typename U>
class Conversion
{
  using Small = char;
  class Big
  {
	char dummy[2];
  };

  static Small Test(const U &);
  static Big Test(...);

  static T MakeT();

public:
  enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
  enum { sameType = false };
};

template <typename T>
class Conversion<T, T>
{
public:
  enum { exists = true, sameType = true };
};

#define SUPERSUBCLASS(T, U)                                                    \
  (Conversion<const U *, const T *>::exists &&                                 \
   !Conversion<const T *, const void *>::sameType)

#define SUPERSUBCLASS_STRICT(T, U)                                             \
  (SUPERSUBCLASS(T, U) && !Conversion<const T *, const void *>::sameType)
