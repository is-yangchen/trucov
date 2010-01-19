#include "func.h"

int main( int argc, char** argv )
{
	func(argc);
	if( argc )
	{
		func(argc);
		int i = 2*argc;
		func(argc);
#include "crazy_include.inl"
		func(argc);
	}
	else
	{
		func(argc);
	}
	func(argc);

   return 0;
}
