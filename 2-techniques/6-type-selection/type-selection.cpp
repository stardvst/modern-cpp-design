template <typename T, bool isPolymorphic>
struct NiftyContainerValueTraits
{
  using ValueType = T *;
};

template <typename T>
struct NiftyContainerValueTraits<T, false>
{
  using ValueType = T;
};

// this way of doing doesn't scale well (for each type selection we need to add a new traits)
template <typename T, bool isPolymorphic>
class NiftyContainer
{
public:
  using Traits = NiftyContainerValueTraits<T, isPolymorphic>;
  using ValueType = typename Traits::ValueType;
};

// better way
template <bool flag, typename T, typename U>
struct Select
{
  using Result = T;
};

template <typename T, typename U>
struct Select<false, T, U>
{
  using Result = U;
};

template <typename T, bool isPolymorphic>
class NiftyContainerFixed
{
public:
  using ValueType = typename Select<isPolymorphic, T *, T>::Result;
};

int main()
{
  NiftyContainer<int, false> nc1;
  NiftyContainer<int, true> nc2;

  NiftyContainerFixed<int, false> ncf1;
  NiftyContainerFixed<int, true> ncf2;

  return 0;
}
