#include <iostream>

class Interface
{
public:
  virtual ~Interface() = default;
  virtual void Fun() = 0;
};

template <typename T, typename P>
Interface *makeAdapter(const T &obj, const P &arg)
{
  class LocalAdapter : public Interface
  {
  public:
    LocalAdapter(const T &o, const P &a) : m_obj(o), m_arg(a) {}
    void Fun() override { m_obj.Fun(m_arg); }

  private:
    T m_obj;
    P m_arg;
  };

  return new LocalAdapter(obj, arg);
}

int main()
{
  class Adaptee
  {
  public:
    void Fun(int x)
    {
      std::cout << "Adaptee::Fun(" << x << ")\n";
    }
  };

  Adaptee a;
  Interface *p = makeAdapter(a, 42);
  p->Fun();
  delete p;

  return 0;
}
