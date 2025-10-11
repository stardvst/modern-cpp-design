#include <iostream>

template <int v>
struct Int2Type
{
  enum
  {
    value = v
  };
};

struct Widget
{
  virtual Widget *Clone() const
  {
    return new Widget(*this);
  }
};

template <typename T, bool isPolymorphic>
class NiftyContainer
{
public:
  void DoSomething(T *obj)
  {
    if (isPolymorphic)
    {
      // auto newObj = obj->Clone(); // error: no member named 'Clone' in 'NonPoly'
      // std::cout << "Cloned object: " << newObj << "\n";
    }
    else
    {
      auto newObj = new T(*obj);
      std::cout << "Copied object: " << newObj << "\n";
    }
  }
};

template <typename T, bool isPolymorphic>
class NiftyContainerFixed
{
public:
  void DoSomething(T *obj)
  {
    DoSomethingImpl(obj, Int2Type<isPolymorphic>{});
  }

private:
  void DoSomethingImpl(T *obj, Int2Type<true>)
  {
    auto newObj = obj->Clone();
    std::cout << "Cloned object: " << newObj << "\n";
  }

  void DoSomethingImpl(T *obj, Int2Type<false>)
  {
    auto newObj = new T(*obj);
    std::cout << "Copied object: " << newObj << "\n";
  }
};

int main()
{
  Widget w;
  NiftyContainer<Widget, true> polyContainer;
  polyContainer.DoSomething(&w);

  struct NonPoly
  {
    int x;
  };
  NonPoly np{42};
  NiftyContainer<NonPoly, false> nonPolyContainer;
  nonPolyContainer.DoSomething(&np);

  std::cout << "-----\n";

  NiftyContainerFixed<Widget, true> polyContainerFixed;
  polyContainerFixed.DoSomething(&w);

  NiftyContainerFixed<NonPoly, false> nonPolyContainerFixed;
  nonPolyContainerFixed.DoSomething(&np);

  return 0;
}
