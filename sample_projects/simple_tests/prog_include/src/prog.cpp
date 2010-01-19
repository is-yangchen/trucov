#include <iostream>
#include "../include/util.h"

using std::cout;
using std::endl;

int main()
{
  int a = 4;
  int b = 5;

  cout << "Max of a and b = " << max(a,b) << endl;
  cout << "Min of a and b = " << min(a,b) << endl;

  return 0;
}
