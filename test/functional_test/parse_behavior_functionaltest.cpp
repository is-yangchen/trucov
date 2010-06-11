///////////////////////////////////////////////////////////////////////////////
//  COPYRIGHT (c) 2009 Schweitzer Engineering Laboratories, Pullman, WA
///////////////////////////////////////////////////////////////////////////////
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
// 
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
///  @file all_functionaltest.cpp 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

// Test all_report 
TEST_F(TrucovTest, adaf)
{
   string s = execute_trucov("all_report");

   string e1 = project_directory + "src1##main.cpp.svg";
   string e2 = project_directory + "src2##dog.cpp.svg";
   string f1 = project_directory + "src1##main.cpp.dot";
   string f2 = project_directory + "src2##dog.cpp.dot";
   string r1 = project_directory + "src1##main.cpp.trucov";
   string r2 = project_directory + "src2##dog.cpp.trucov";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));
   EXPECT_FALSE(fs::exists(f1));  
   EXPECT_FALSE(fs::exists(f2));
   EXPECT_TRUE(fs::exists(r1));
   EXPECT_TRUE(fs::exists(r2));

   fs::remove_all(e1);
   fs::remove_all(e2);
   fs::remove_all(r1);
   fs::remove_all(r2);
}

