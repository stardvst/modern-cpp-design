#include <iostream>

struct NullType
{
};

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

template <typename T>
class TypeTraits
{
public:
  template <typename U>
  struct PointerTraits
  {
    enum
    {
      result = false
    };
    using PointeeType = NullType;
  };

  template <typename U>
  struct PointerTraits<U *>
  {
    enum
    {
      result = true
    };
    using PointeeType = U;
  };

  template <typename U>
  struct PointerTraits<U *const>
  {
    enum
    {
      result = true
    };
    using PointeeType = U;
  };

  template <class U>
  struct ReferenceTraits
  {
    enum
    {
      result = false
    };
    typedef U ReferredType;
  };

  template <class U>
  struct ReferenceTraits<U &>
  {
    enum
    {
      result = true
    };
    typedef U ReferredType;
  };

  template <class U>
  struct PToMTraits
  {
    enum
    {
      result = false
    };
  };

  template <class U, class V>
  struct PToMTraits<U V::*>
  {
    enum
    {
      result = true
    };
  };

  template <class U>
  struct UnConst
  {
    typedef U Result;
    enum
    {
      isConst = 0
    };
  };

  template <class U>
  struct UnConst<const U>
  {
    typedef U Result;
    enum
    {
      isConst = 1
    };
  };

  enum
  {
    isPointer = PointerTraits<T>::result
  };

  enum
  {
    isReference = ReferenceTraits<T>::result
  };

  enum
  {
    isMemberPointer = PToMTraits<T>::result
  };

  enum
  {
    isConst = UnConst<T>::isConst
  };

public:
  using PointeeType = typename PointerTraits<T>::PointeeType;
  using ReferredType = typename ReferenceTraits<T>::ReferredType;
  using NonConstType = typename UnConst<T>::Result;
};

class Widget
{
public:
  int a;
  void foo() {}
};

int main()
{
  using namespace std;

  std::cout << boolalpha;

  std::cout << "int: \n";
  std::cout << "isPointer: " << TypeTraits<int>::isPointer << "\n";
  std::cout << "isReference: " << TypeTraits<int>::isReference << "\n";
  std::cout << "isMemberPointer: " << TypeTraits<int>::isMemberPointer << "\n";
  std::cout << "isConst: " << TypeTraits<int>::isConst << "\n";
  std::cout << "\n";

  std::cout << "int *: \n";
  std::cout << "isPointer: " << TypeTraits<int *>::isPointer << "\n";
  std::cout << "isReference: " << TypeTraits<int *>::isReference << "\n";
  std::cout << "isMemberPointer: " << TypeTraits<int *>::isMemberPointer << "\n";
  std::cout << "isConst: " << TypeTraits<int *>::isConst << "\n";
  std::cout << "PointeeType is int: " << is_same<TypeTraits<int *>::PointeeType, int>::value << "\n";
  std::cout << "\n";

  std::cout << "int & : \n";
  std::cout << "isPointer: " << TypeTraits<int &>::isPointer << "\n";
  std::cout << "isReference: " << TypeTraits<int &>::isReference << "\n";
  std::cout << "isMemberPointer: " << TypeTraits<int &>::isMemberPointer << "\n";
  std::cout << "isConst: " << TypeTraits<int &>::isConst << "\n";
  std::cout << "ReferredType is int: " << is_same<TypeTraits<int &>::ReferredType, int>::value << "\n";
  std::cout << "\n";

  std::cout << "int Widget::* : \n";
  std::cout << "isPointer: " << TypeTraits<int Widget::*>::isPointer << "\n";
  std::cout << "isReference: " << TypeTraits<int Widget::*>::isReference << "\n";
  std::cout << "isMemberPointer: " << TypeTraits<int Widget::*>::isMemberPointer << "\n";
  std::cout << "isConst: " + to_string(TypeTraits<int Widget::*>::isConst) + "\n";
  std::cout << "\n";

  std::cout << "const int : \n";
  std::cout << "isPointer: " + to_string(TypeTraits<const int>::isPointer) + "\n";
  std::cout << "isReference: " + to_string(TypeTraits<const int>::isReference) + "\n";
  std::cout << "isMemberPointer: " + to_string(TypeTraits<const int>::isMemberPointer) + "\n";
  std::cout << "isConst: " + to_string(TypeTraits<const int>::isConst) + "\n";
  std::cout << "Result is int: " << is_same<TypeTraits<const int>::ConstTraits<const int>::Result, int>::value << "\n";
  std::cout << "\n";
  std::cout << "const int *: \n";
  std::cout << "isPointer: " + to_string(TypeTraits<const int *>::isPointer) + "\n";
  std::cout << "isReference: " + to_string(TypeTraits<const int *>::isReference) + "\n";
  std::cout << "isMemberPointer: " + to_string(TypeTraits<const int *>::isMemberPointer) + "\n";
  std::cout << "isConst: " + to_string(TypeTraits<const int *>::isConst) + "\n";
  std::cout << "PointeeType is const int: " << is_same<TypeTraits<const int *>::PointeeType, const int>::value << "\n";
  std::cout << "\n";
  std::cout << "const int * const: \n";
  std::cout << "isPointer: " + to_string(TypeTraits<const int *const>::isPointer) + "\n";
  std::cout << "isReference: " + to_string(TypeTraits<const int *const>::isReference) + "\n";
  std::cout << "isMemberPointer: " + to_string(TypeTraits<const int *const>::isMemberPointer) + "\n";
  std::cout << "isConst: " + to_string(TypeTraits<const int *const>::isConst) + "\n";
  std::cout << "PointeeType is const int: " << is_same<TypeTraits<const int *const>::PointeeType, const int>::value << "\n";
  return 0;
}
