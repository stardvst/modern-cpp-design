#include <iostream>

template <
    class T,
    template <class> class CheckingPolicy,
    template <class> class ThreadingModel>
class SmartPtr : public CheckingPolicy<T>, public ThreadingModel<T>
{
public:
  SmartPtr(T *ptr = nullptr)
      : CheckingPolicy<T>(), ThreadingModel<T>(), m_ptr(ptr)
  {
  }

  T &operator*()
  {
    typename ThreadingModel<SmartPtr>::Lock guard(*this);
    CheckingPolicy<T>::Check(m_ptr);
    return *m_ptr;
  }

  ~SmartPtr()
  {
    delete m_ptr;
  }

private:
  T *m_ptr;
};

// Checking policies

template <class T>
struct NoChecking
{
  static void Check(T *) {}
};

template <class T>
struct EnforceNotNull
{
  static void Check(T *ptr)
  {
    if (!ptr)
      throw std::runtime_error("null pointer");
  }
};

// Threading models

template <class T>
struct SingleThreadedModel
{
  struct Lock
  {
    Lock(T &) {}
  };
};

int main()
{
  SmartPtr<int, NoChecking, SingleThreadedModel> sp(new int(42));
  std::cout << *sp << "\n";

  try
  {
    SmartPtr<int, EnforceNotNull, SingleThreadedModel> sp2(nullptr);
    std::cout << *sp2 << "\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
