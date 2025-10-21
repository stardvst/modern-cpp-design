#include "select.h"
#include "supersubclass.h"
#include "typelist.h"

#include <cassert>
#include <type_traits>

template <typename TList, typename T>
struct MostDerived;

template <typename T>
struct MostDerived<NullType, T>
{
  using Result = T;
};

template <typename Head, typename Tail, typename T>
struct MostDerived<Typelist<Head, Tail>, T>
{
private:
  using Candidate = typename MostDerived<Tail, T>::Result;

public:
  using Result =
      typename Select<SUPERSUBCLASS(Candidate, Head), Head, Candidate>::Result;
};

template <typename T>
struct DerivedToFront;

template <>
struct DerivedToFront<NullType>
{
  using Result = NullType;
};

template <typename Head, typename Tail>
struct DerivedToFront<Typelist<Head, Tail>>
{
private:
  using TheMostDerived =
      typename MostDerived<Typelist<Head, Tail>, Head>::Result;
  using TailWithoutMostDerived =
      typename Replace<Typelist<Head, Tail>, TheMostDerived, Head>::Result;
  using RearrangedTail =
      typename DerivedToFront<TailWithoutMostDerived>::Result;

public:
  using Result = Typelist<TheMostDerived, RearrangedTail>;
};

struct Base
{
  virtual ~Base() = default;
};

struct Derived1 : public Base
{
};

struct Derived2 : public Derived1
{
};

int main()
{
  using MyTypesBase = TYPELIST_1(Base);
  using MostDerivedTypeBase = MostDerived<MyTypesBase, Base>::Result;
  static_assert(std::is_same_v<MostDerivedTypeBase, Base>);

  using MyTypes = TYPELIST_2(Base, Derived1);
  using MostDerivedType = MostDerived<MyTypes, Base>::Result;
  static_assert(std::is_same_v<MostDerivedType, Derived1>);

  using MyTypesAll = TYPELIST_3(Base, Derived1, Derived2);
  using MostDerivedTypeFromAll = MostDerived<MyTypesAll, Base>::Result;
  static_assert(std::is_same_v<MostDerivedTypeFromAll, Derived2>);
}
