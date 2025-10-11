template <bool>
struct CompileTimeError;

template <>
struct CompileTimeError<true>
{
};

#define STATIC_CHECK(expr) \
  (CompileTimeError<(expr) != 0>())

template <bool>
struct CompileTimeChecker
{
  CompileTimeChecker(...) {}
};

template <>
struct CompileTimeChecker<false>
{
  CompileTimeChecker(...);
};

#define STATIC_CHECK_MSG(expr, msg)                               \
  {                                                               \
    class ERROR_##msg                                             \
    {                                                             \
    };                                                            \
    (void)sizeof(CompileTimeChecker<(expr) != 0>(ERROR_##msg{})); \
  }

template <typename To, typename From>
To safe_reinterpret_cast(From from)
{
  STATIC_CHECK_MSG(sizeof(From) <= sizeof(To), Destination_Type_Too_Narrow);
  return reinterpret_cast<To>(from);
}

int main()
{
  // int i = 65;
  // char c1 = safe_reinterpret_cast<char>(i);

  // won't compile: static assertion fails
  long long ll = 123456789012345;
  char c3 = safe_reinterpret_cast<char>(ll);

  return 0;
}
