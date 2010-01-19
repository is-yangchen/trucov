#include "cat.h"

// Cats can meow. 
void meow( int loud )
{
   if ( loud )
   {
      printf( "MEOW\n" );
   }
   else
   {
      printf( "meow\n" );
   }
}

// Cats can purr.
void purr( int loud )
{
   if ( loud )
   {
      printf( "PURR\n" );
   }
   else
   {
      printf( "purr\n" );
   }   
}

