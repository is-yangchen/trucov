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

// Test dot_report 
TEST_F(TrucovTest, dot_report)
{
   string s = execute_trucov("dot_report");

   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));

   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, dot_report_per_source)
{
   string s = execute_trucov("dot_report --per-source");

   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));

   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, dot_report_per_function_mangle)
{
   string s = execute_trucov("dot_report --per-function --mangle");

   string ext = ".dot";
   string e1 = project_directory + "src1##main.cpp##main" + ext;
   string e2 = project_directory + "src1##main.cpp##_Z4barbv" + ext;
   string e3 = project_directory + "src1##main.cpp##_Z4foofv" + ext;
   string e4 = project_directory + "src1##main.cpp##_Z6foobarv" + ext;
   string e5 = project_directory + "src2##dog.cpp##_Z4barkv" + ext;
   string e6 = project_directory + "src2##dog.cpp##_Z4woofv" + ext;
   string f1 = project_directory + "src1##main.cpp" + ext;
   string f2 = project_directory + "src2##dog.cpp" + ext;

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));
   EXPECT_TRUE(fs::exists(e3));
   EXPECT_TRUE(fs::exists(e4));
   EXPECT_TRUE(fs::exists(e5));
   EXPECT_TRUE(fs::exists(e6));
   EXPECT_FALSE(fs::exists(f1));
   EXPECT_FALSE(fs::exists(f2));

   fs::remove_all(e1);
   fs::remove_all(e2);
   fs::remove_all(e3);
   fs::remove_all(e4);
   fs::remove_all(e5);
   fs::remove_all(e6);
}

TEST_F(TrucovTest, dot_report_per_function_demangle)
{
   string s = execute_trucov("dot_report --per-function --demangle");

   string ext = ".dot";
   string e1 = project_directory + "src1##main.cpp##main" + ext;
   string e2 = project_directory + "src1##main.cpp##barb()" + ext;
   string e3 = project_directory + "src1##main.cpp##foof()" + ext;
   string e4 = project_directory + "src1##main.cpp##foobar()" + ext;
   string e5 = project_directory + "src2##dog.cpp##bark()" + ext;
   string e6 = project_directory + "src2##dog.cpp##woof()" + ext;
   string f1 = project_directory + "src1##main.cpp" + ext;
   string f2 = project_directory + "src2##dog.cpp" + ext;

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));
   EXPECT_TRUE(fs::exists(e3));
   EXPECT_TRUE(fs::exists(e4));
   EXPECT_TRUE(fs::exists(e5));
   EXPECT_TRUE(fs::exists(e6));
   EXPECT_FALSE(fs::exists(f1));
   EXPECT_FALSE(fs::exists(f2));

   fs::remove_all(e1);
   fs::remove_all(e2);
   fs::remove_all(e3);
   fs::remove_all(e4);
   fs::remove_all(e5);
   fs::remove_all(e6);
}

TEST_F(TrucovTest, dot_report_relative_updir_filenames)
{
   string s = execute_trucov_from_cwd(
"dot_report -s ../test/resources/functional_test_project/src1/ -s ../test/resources/functional_test_project/src2/ -b ../test/resources/functional_test_project/build1/ -b ../test/resources/functional_test_project/build2/");

   string e1 = "_..##test##resources##functional_test_project##src1##main.cpp.dot";
   string e2 = "_..##test##resources##functional_test_project##src2##dog.cpp.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));

   fs::remove_all(e1);
   fs::remove_all(e2);
}

// Test output option command line
TEST_F(TrucovTest, dot_report_cmd_line_outputdir_option_with_slash) 
{
   string e1 = project_directory + "output/src1##main.cpp.dot";
   string e2 = project_directory + "output/src2##dog.cpp.dot";
   string f1 = project_directory + "src1##main.cpp.dot";
   string f2 = project_directory + "src2##dog.cpp.dot";
   fs::remove(e1);
   fs::remove(e2);  
   fs::remove(f1);  
   fs::remove(f2);  
 
   // Test 1 option
   string s = execute_trucov("dot_report -o output/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));  
   EXPECT_FALSE(fs::exists(f1)); 
   EXPECT_FALSE(fs::exists(f2)); 
   fs::remove(e1);
   fs::remove(e2);
}

TEST_F(TrucovTest, dot_report_cmd_line_outputdir_option_without_slash) 
{
   string e1 = project_directory + "output/src1##main.cpp.dot";
   string e2 = project_directory + "output/src2##dog.cpp.dot";
   string f1 = project_directory + "src1##main.cpp.dot";
   string f2 = project_directory + "src2##dog.cpp.dot";
   fs::remove(e1);
   fs::remove(e2);  
   fs::remove(f1);  
   fs::remove(f2);  
 
   // Test 1 option
   string s = execute_trucov("dot_report -o output");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2));  
   EXPECT_FALSE(fs::exists(f1)); 
   EXPECT_FALSE(fs::exists(f2)); 
   fs::remove(e1);
   fs::remove(e2);
}

