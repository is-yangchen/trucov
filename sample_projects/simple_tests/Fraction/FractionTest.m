#import "fraction.h"

int main (int argc, const char * argv[]) {
   NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
   
   Fraction *aFraction = [[Fraction alloc] init];
   Fraction *bFraction = [[Fraction alloc] init];
   
   // Set two fractions to 1/4 and 1/2 and add them together
   
   [aFraction setTo: 1 over: 4];
   [bFraction setTo: 1 over: 2];
   
   [aFraction print];
   NSLog (@"+");
   [bFraction print];
   NSLog (@"=");
   
   [aFraction add: bFraction];
   
   // print the result
   
   [aFraction print];
   [aFraction release];
   [bFraction release];

   [pool drain];
   return 0;
}
