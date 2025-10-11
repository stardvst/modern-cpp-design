#include <cassert>

template <typename To, typename From>
To safe_reinterpret_cast_1(From from)
{
  assert(sizeof(From) <= sizeof(To));
  return reinterpret_cast<To>(from);
}

#define STATIC_CHECK(expr)        \
  {                               \
    char unnamed[(expr) ? 1 : 0;] \
  }

// template <typename To, typename From>
// To safe_reinterpret_cast_cpp11(From from)
// {
//   static_assert(sizeof(From) <= sizeof(To), "safe_reinterpret_cast: size mismatch");
//   return reinterpret_cast<To>(from);
// }

int main()
{
  int i = 65;
  char c1 = safe_reinterpret_cast_1<char>(i);

  // won't compile: static_assert fails
  long long ll = 123456789012345;
  char c3 = safe_reinterpret_cast_cpp11<char>(ll);

  return 0;
}
