#include <cstdlib>
#include <iostream>

template <typename T>
struct OpNewCreator
{
  static T *Create() { return new T; }
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

template <template <class Created> typename CreationPolicy>
class WidgetManager : public CreationPolicy<Widget>
{
public:
  WidgetManager() : CreationPolicy<Widget>() {}
  WidgetManager(Widget *pw) : CreationPolicy<Widget>(pw) {}
  Widget *Create() { return CreationPolicy<Widget>::Create(); }

  void SwitchPrototype(Widget *pw)
  {
    std::cout << "WidgetManager::SwitchPrototype()\n";
    CreationPolicy<Widget> &policy = *this;
    delete policy.GetPrototype();
    policy.SetPrototype(pw);
  }
};

int main()
{
  WidgetManager<OpNewCreator> wm1;
  Widget *pw1 = wm1.Create();

  // won't compile: no SetPrototype in OpNewCreator
  // Widget w1;
  // wm1.SwitchPrototype(&w1);

  delete pw1;

  std::cout << "-----\n";

  Widget *w = new Widget();
  WidgetManager<PrototypeCreator> wm2(w);
  Widget *pw2 = wm2.Create();

  // compiles fine, PrototypeCreator has SetPrototype
  Widget w2;
  wm2.SwitchPrototype(&w2);
  delete pw2;

  return 0;
}
