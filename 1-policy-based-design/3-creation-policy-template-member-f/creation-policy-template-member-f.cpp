#include <cstdlib>
#include <iostream>

struct OpNewCreator
{
  template <typename T>
  static T *Create() { return new T; }
};

struct MallocCreator
{
  template <typename T>
  static T *Create()
  {
    void *buf = std::malloc(sizeof(T));
    if (!buf)
      return nullptr;
    return new (buf) T;
  }
};

class Widget
{
public:
  Widget() { std::cout << "Widget::Widget()\n"; }
  Widget(const Widget &) { std::cout << "Widget::Widget(const Widget&)\n"; }
  virtual ~Widget() { std::cout << "Widget::~Widget()\n"; }

  virtual Widget *Clone() const { return new Widget(*this); }
};

class Gadget : public Widget
{
public:
  void doSomething() { std::cout << "Gadget::doSomething()\n"; }
};

template <typename Created>
class WidgetManager
{
public:
  template <typename CreationPolicy>
  Widget *Create() { return CreationPolicy::template Create<Created>(); }
};

int main()
{
  WidgetManager<Widget> wm1;
  Widget *pw1 = wm1.Create<OpNewCreator>();
  delete pw1;

  std::cout << "-----\n";

  WidgetManager<Widget> wm2;
  Widget *pw2 = wm2.Create<MallocCreator>();
  pw2->~Widget();
  std::free(pw2);

  return 0;
}
