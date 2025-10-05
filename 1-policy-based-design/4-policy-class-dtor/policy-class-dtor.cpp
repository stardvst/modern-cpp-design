#include <cstdlib>
#include <iostream>

template <typename T>
struct OpNewCreator
{
  static T *Create() { return new T; }
};

template <typename T>
struct OpNewCreatorFixed
{
  static T *Create() { return new T; }

protected:
  // make destructor protected to prevent deletion through base pointer
  // now only derived classes can destroy the policy object
  ~OpNewCreatorFixed() { std::cout << "OpNewCreatorFixed::~OpNewCreatorFixed()\n"; }
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
};

int main()
{
  WidgetManager<OpNewCreator> wm1;
  OpNewCreator<Widget> *creator = &wm1;

  // compiles fine but has undefined behavior: no policy destructor call!
  // delete creator;

  WidgetManager<OpNewCreatorFixed> wm2;
}
