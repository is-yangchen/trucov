#include "dog.h"
extern "C"
{
   #include "cat.h"
}

int main()
{
   int a = 1;

   Dog dog;
   dog.woof();

   meow( 0 );

   switch( a )
   {
      case 0:
         a++;
         break;
      case 1:
         a--;
      case 2:
         a++;
         break;
      default:
         break;
    }

   return 0;
}

