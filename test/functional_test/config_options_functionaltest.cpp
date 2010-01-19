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
///  @file config_options_functionaltest.cpp 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

// Test empty config file
TEST_F(TrucovTest, config_empty)
{
   string s =
      execute_trucov("list --config-file " + project_directory + "config/empty_config");

   EXPECT_STREQ(
"\nfoof()\nmain\nbarb()\nfoobar()\nbark()\nwoof()",
      s.c_str());
}

// Test 1 option config file
TEST_F(TrucovTest, config_1_directory)
{
   string s =
      execute_trucov("list --config-file " + project_directory + "config/dir_1_config");

   EXPECT_STREQ(
"\nfoof()\nmain\nbarb()\nfoobar()",
      s.c_str());
}

// Test options accumulate config file
TEST_F(TrucovTest, config_2_directory)
{
   string s =
      execute_trucov("list --config-file " + project_directory + "config/dir_1_2_config");

   EXPECT_STREQ(
"\nfoof()\nmain\nbarb()\nfoobar()\nbark()\nwoof()",
      s.c_str());
}



