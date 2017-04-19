#include <iostream>

class A{

  virtual void fn() = 0;
};

class B : public A{
public:
  void fn(int i){};
  void fn(char f){};

};

int main()
{
  B b;

  b.fn(1);
  b.fn('a');
}
