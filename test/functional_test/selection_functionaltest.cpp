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
///  @file selection_functionaltest.h 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

TEST_F(TrucovTest, selection_default)
{
   string s =
      execute_trucov("list");
   
   EXPECT_STRNE(strstr(s.c_str(), "main"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "barb()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "bark()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "woof()"), NULL);
}

TEST_F(TrucovTest, selection_partial_match)
{
   string s =
      execute_trucov("list bark fo");
   
   EXPECT_STRNE(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "bark()"), NULL);
   
   EXPECT_STREQ(strstr(s.c_str(), "main"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "woof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "barb()"), NULL);
}

TEST_F(TrucovTest, selection_full_match)
{
   string s =
      execute_trucov("list --full-match bark fo");
   
   EXPECT_STRNE(strstr(s.c_str(), "bark()"), NULL);
   
   EXPECT_STREQ(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "main"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "woof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "barb()"), NULL);
}

TEST_F(TrucovTest, selection_name_match)
{
   string s =
      execute_trucov("list \"bark(void)\"");
   
   EXPECT_STREQ(strstr(s.c_str(), "bark()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "main"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "woof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "barb()"), NULL);
}

TEST_F(TrucovTest, selection_signature_match)
{
   string s =
      execute_trucov("list --signature-match \"bark()\"");
   
   EXPECT_STRNE(strstr(s.c_str(), "bark()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "main"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "woof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "barb()"), NULL);
}

TEST_F(TrucovTest, selection_file_match)
{
   string s =
      execute_trucov("list main");
   
   EXPECT_STRNE(strstr(s.c_str(), "foobar()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "foof()"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "main"), NULL);
   EXPECT_STRNE(strstr(s.c_str(), "barb()"), NULL);
   
   EXPECT_STREQ(strstr(s.c_str(), "woof()"), NULL);
   EXPECT_STREQ(strstr(s.c_str(), "bark()"), NULL);
}
