
#include "dog.h"

inline void foof()
{
   if (1)
   {
      int x = 0;
   }
   else
   {
      int x = 1;
   }
}

inline void barb()
{
   foof();
}

void foobar()
{
   barb();
}

int main()
{

   return 0;
}
