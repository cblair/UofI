#ifndef _itest_h_included
#define _itest_h_included

#include <iostream>

using namespace std;

class Example
{
 private:
  int m_val;
 public:
  Example()
    : m_val(0)
    {}
  void print();
};

#endif
