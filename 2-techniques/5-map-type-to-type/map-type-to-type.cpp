template <typename T, typename U>
T *create(const U &u)
{
  return new T(u);
}

struct Widget
{
  Widget() = default;
  Widget(int) {}
};

// error: function template partial specialization is not allowed

// template <class U>
// Widget *create<Widget, U>(const U &u)
// {
//   return new Widget;
// }

// one way to fix it is to use overloading

template <typename T, typename U>
Widget *createOverloaded(const U &u, T /*dummy*/)
{
  return new T(u);
}

// better way is to use tag dispatching

template <typename T>
struct Type2Type
{
  using OriginalType = T;
};

template <typename T, typename U>
T *createTagDispatching(const U &u, Type2Type<T>)
{
  return new T(u);
}

template <typename U>
Widget *createTagDispatching(const U &u, Type2Type<Widget>)
{
  return new Widget(u);
}

int main()
{
  // int x = 42;
  // Widget *pw = create<Widget>(x);
  // delete pw;

  int x = 42;
  Widget *pw = createOverloaded(x, Widget{}); // need to pass a dummy (potentially large) Widget
  delete pw;

  Widget *pw2 = createTagDispatching(x, Type2Type<Widget>{}); // no dummy needed
  delete pw2;

  double *pw3 = createTagDispatching(3.14, Type2Type<double>{}); // works too
  delete pw3;

  return 0;
}
