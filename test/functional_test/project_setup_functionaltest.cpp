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
///  @file project_setup_functionaltest.cpp 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

// Test list command.
TEST_F(TrucovTest, find_all_source_files) 
{
   string s =
      execute_trucov_from_cwd("list -c ../test/resources/all_source_types"); 

   EXPECT_STREQ(s.c_str(), 
"\nc()\ncc()\nh()\nhh()\nhpp()\nmain");
}

