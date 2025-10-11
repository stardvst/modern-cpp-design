#include <iostream>
#include <vector>

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
  enum
  {
    exists = sizeof(Test(MakeT())) == sizeof(Small)
  };
  enum
  {
    sameType = false
  };
};

template <typename T>
class Conversion<T, T>
{
public:
  enum
  {
    exists = true,
    sameType = true
  };
};

#define SUPERSUBCLASS(T, U) \
  (Conversion<const U *, const T *>::exists && !Conversion<const T *, const void *>::sameType)

#define SUPERSUBCLASS_STRICT(T, U) \
  (SUPERSUBCLASS(T, U) && !Conversion<const T *, const void *>::sameType)

int main()
{
  std::cout << "Conversion<int, int>::exists = " << Conversion<int, int>::exists << "\n";
  std::cout << "Conversion<int, int>::exists = " << Conversion<int, int>::sameType << "\n";

  std::cout << "Conversion<int, double>::exists = " << Conversion<int, double>::exists << "\n";
  std::cout << "Conversion<int, double>::exists = " << Conversion<int, double>::sameType << "\n";

  std::cout << "Conversion<char, char *>::exists = " << Conversion<char, char *>::exists << "\n";
  std::cout << "Conversion<char, char *>::exists = " << Conversion<char, char *>::sameType << "\n";

  std::cout << "Conversion<std::size_t, std::vector<int>>::exists = " << Conversion<std::size_t, std::vector<int>>::exists << "\n";
  std::cout << "Conversion<std::size_t, std::vector<int>>::exists = " << Conversion<std::size_t, std::vector<int>>::sameType << "\n";

  return 0;
}
