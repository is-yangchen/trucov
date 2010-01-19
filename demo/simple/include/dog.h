#ifndef DOG_H
#define DOG_H

#include <iostream>

class Dog
{
   public:
   
   // A dog can woof.
   void woof()
   {
      std::cout << "woof" << std::endl;
   }

   // A dog can bark.
   void bark()
   {
      std::cout << "bark" << std::endl;
   }
};

#endif

