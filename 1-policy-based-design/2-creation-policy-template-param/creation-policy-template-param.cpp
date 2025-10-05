#include <cstdlib>
#include <iostream>

template <typename T>
struct OpNewCreator
{
  static T *Create() { return new T; }
};

template <typename T>
struct MallocCreator
{
  static T *Create()
  {
    void *buf = std::malloc(sizeof(T));
    if (!buf)
      return nullptr;
    return new (buf) T;
  }
};

template <typename T>
struct PrototypeCreator
{
  PrototypeCreator(T *p = nullptr) : m_prototype(p) {}

  T *Create() { return m_prototype ? m_prototype->Clone() : nullptr; }

  T *GetPrototype() { return m_prototype; }
  void SetPrototype(T *p) { m_prototype = p; }

private:
  T *m_prototype;
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

template <template <class Created> typename CreationPolicy>
class WidgetManager : public CreationPolicy<Widget>
{
public:
  WidgetManager() : CreationPolicy<Widget>() {}
  WidgetManager(Widget *pw) : CreationPolicy<Widget>(pw) {}
  Widget *Create() { return CreationPolicy<Widget>::Create(); }

  void doSomething()
  {
    Gadget *g = CreationPolicy<Gadget>::Create();
    g->doSomething();
    delete g;
  }
};

int main()
{
  WidgetManager<OpNewCreator> wm1;
  Widget *pw1 = wm1.Create();
  wm1.doSomething();
  delete pw1;

  std::cout << "-----\n";

  WidgetManager<MallocCreator> wm2;
  Widget *pw2 = wm2.Create();
  pw2->~Widget();
  std::free(pw2);

  std::cout << "-----\n";

  Widget w;
  WidgetManager<PrototypeCreator> wm3(&w);
  Widget *pw3 = wm3.Create();
  delete pw3;

  return 0;
}
