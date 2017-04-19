#include<iostream>

class A
{
public:
  int x = 10;
  void gn(const A *a){}
  void fn(const A b){ gn(this); }

  
};

int main()
{
  

}
