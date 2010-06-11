//
//  Fraction.h
//  Fraction
//
//  Created by William Reinhardt on 3/26/10.
//  Copyright 2010 Washington State University. All rights reserved.
//

#import <Foundation/Foundation.h>

// The Fraction class

@interface Fraction : NSObject {
   int numerator;
   int denominator;
}

@property int numerator, denominator;

-(void)     print;
-(void)     setTo: (int) n over: (int) d;
-(double)   convertToNum;
-(void) add: (Fraction *) f;
-(void) reduce;

@end
