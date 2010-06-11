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

// Test dot command
TEST_F(TrucovTest, dot)
{
   string s = execute_trucov("dot");
   string e1 = project_directory + "coverage.dot";

   EXPECT_TRUE(fs::exists(e1));  

   fs::remove_all(e1);
}

// Test output option command line
TEST_F(TrucovTest, dot_cmd_line_outputfile_option_with_slash) 
{
   string e1 = project_directory + "coverage.dot";
   string f1 = project_directory + "outputfile";
   fs::remove_all(e1);
   fs::remove_all(f1);  
 
   // Test 1 option
   string s = execute_trucov("dot -o outputfile/");
   EXPECT_FALSE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(f1)); 
   fs::remove_all(e1);
}

// Test output option command line
TEST_F(TrucovTest, dot_cmd_line_outputfile_option_without_slash) 
{
   string e1 = project_directory + "output.dot";
   string f1 = project_directory + "coverage.dot";
   fs::remove_all(e1);
   fs::remove_all(f1);  
 
   // Test 1 option
   string s = execute_trucov("dot -o output.dot");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(f1)); 
   fs::remove_all(e1);
}

TEST_F(TrucovTest, dot_content_no_flags)
{
   string s = execute_trucov_inline("dot");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_normal.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

TEST_F(TrucovTest, dot_content_ignore_per_function)
{
   string s = execute_trucov_inline("dot --per-function");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_normal.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

TEST_F(TrucovTest, dot_content_ignore_per_source)
{
   string s = execute_trucov_inline("dot --per-source");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_normal.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

TEST_F(TrucovTest, dot_content_ignore_brief)
{
   string s = execute_trucov_inline("dot --brief");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_normal.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

TEST_F(TrucovTest, dot_content_hide_fake)
{
   string s = execute_trucov_inline("dot --hide-fake");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_normal.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

TEST_F(TrucovTest, dot_content_show_fake)
{
   string s = execute_trucov_inline("dot --show-fake");
   string a1 = project_directory_inline + "coverage.dot";
   string e1 = project_directory_inline + "expected/dot_show_fake.dot";

   // Assume
   EXPECT_TRUE( fs::exists(e1) );

   EXPECT_TRUE(fs::exists(a1));  
   EXPECT_TRUE( compare_files( e1, a1) );   

   fs::remove_all(a1);
}

