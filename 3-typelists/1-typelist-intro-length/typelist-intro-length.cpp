#include <typeinfo>

template <typename T, typename U>
struct Typelist
{
  using Head = T;
  using Tail = U;
};

struct NullType
{
};

// flattened typelist macros
#define TYPELIST_1(T1) Typelist<T1, NullType>
#define TYPELIST_2(T1, T2) Typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) Typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) Typelist<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
/// ...

template <typename TList>
struct Length
{
};

template <>
struct Length<NullType>
{
  enum
  {
    value = 0
  };
};

template <typename T, typename U>
struct Length<Typelist<T, U>>
{
  enum
  {
    value = 1 + Length<U>::value
  };
};

int main()
{
  using CharList =
      Typelist<char, Typelist<signed char, Typelist<unsigned char, NullType>>>;

  using IntList = TYPELIST_3(int, unsigned int, signed int);

  static_assert(Length<CharList>::value == 3);
  static_assert(Length<IntList>::value == 3);

  std::type_info *intRTTI[Length<IntList>::value];
  std::type_info *charRTTI[Length<CharList>::value];
}
