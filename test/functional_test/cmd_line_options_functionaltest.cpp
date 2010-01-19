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
///  @file cmd_line_options_functionaltest.cpp 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

TEST_F(TrucovTest, cmd_line_chdir_option_with_slash)
{
   string e1 = project_directory + "src1##main.cpp.trucov";
   string e2 = project_directory + "src2##dog.cpp.trucov";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   string s = execute_trucov_from_cwd(
      "report -c ../test/resources/functional_test_project/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
} 

TEST_F(TrucovTest, cmd_line_chdir_option_without_slash)
{
   string e1 = project_directory + "src1##main.cpp.trucov";
   string e2 = project_directory + "src2##dog.cpp.trucov";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   string s = execute_trucov_from_cwd(
      "report -c ../test/resources/functional_test_project");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
} 

TEST_F(TrucovTest, cmd_line_chdir_option_bad)
{
   string e1 = project_directory + "src1##main.cpp.trucov";
   string e2 = project_directory + "src2##dog.cpp.trucov";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   string s = execute_trucov_from_cwd(
      "report -c ../test/resources/this_is_a_fake_dir");
  
   EXPECT_FALSE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   EXPECT_STREQ(
      "ERROR: Directory ../test/resources/this_is_a_fake_dir does not exists.\n", 
      error_output.c_str());
} 

TEST_F(TrucovTest, cmd_line_srcdir_option_with_slash) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e2);
   
   // Test 1 option
   string s = execute_trucov("dot_report -s src1/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   // Test accumlates
   s = execute_trucov("dot_report -s src1/ -s src2/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, cmd_line_srcdir_option_without_slash) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e2);
   
   // Test 1 option
   string s = execute_trucov("dot_report -s src1");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   // Test accumlates
   s = execute_trucov("dot_report -s src1 -s src2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
}

// Should ignore bad src dirs
TEST_F(TrucovTest, cmd_line_srcdir_option_bad) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e2);
   
   // Assume
   EXPECT_FALSE(fs::exists( project_directory + "bad_dir")); 
   
   // Test 1 option
   string s = execute_trucov("dot_report -s src1 -s bad_dir");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);
   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified source directory.") != string::npos);

   // Test accumlates
   s = execute_trucov("dot_report -s src1 -s bad_dir -s src2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified source directory.") != string::npos);
}

// Should ignore bad src dirs
TEST_F(TrucovTest, cmd_line_srcdir_option_file_not_dir) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e2);
   
   // Assume
   EXPECT_FALSE(fs::exists( project_directory + "bad_dir")); 
   ofstream of("bad_dir");
   of.close();
   
   // Test 1 option
   string s = execute_trucov("dot_report -s src1 -s bad_dir");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified source directory.") != string::npos);
   
   // Test accumlates
   s = execute_trucov("dot_report -s src1 -s bad_dir -s src2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);

   fs::remove_all("bad_dir");
   
   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified source directory.") != string::npos);
}

TEST_F(TrucovTest, cmd_line_builddir_option_with_slash) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // Test 1 option
   string s = execute_trucov("dot_report -b build1/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   // Test accumlates
   s = execute_trucov("dot_report -b build1/ -b build2/");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, cmd_line_builddir_option_without_slash) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // Test 1 option
   string s = execute_trucov("dot_report -b build1");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   // Test accumlates
   s = execute_trucov("dot_report -b build1 -b build2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, cmd_line_builddir_option_bad) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // Assume
   EXPECT_FALSE(fs::exists( project_directory + "bad_dir")); 
   
   // Test 1 option
   string s = execute_trucov("dot_report -b build1 -b bad_dir");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified build directory.") != string::npos);
   
   // Test accumlates
   s = execute_trucov("dot_report -b build1 -b bad_dir -b build2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified build directory.") != string::npos);
}

TEST_F(TrucovTest, cmd_line_builddir_option_file_not_dir) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // Assume
   EXPECT_FALSE(fs::exists( project_directory + "bad_dir")); 
   ofstream of("bad_dir");
   of.close();  
 
   // Test 1 option
   string s = execute_trucov("dot_report -b build1 -b bad_dir");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   fs::remove_all(e1);

   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified build directory.") != string::npos);
   
   // Test accumlates
   s = execute_trucov("dot_report -b build1 -b bad_dir -b build2");
   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_TRUE(fs::exists(e2)); 
   fs::remove_all(e1);
   fs::remove_all(e2);

   fs::remove_all("bad_dir");
   
   EXPECT_TRUE( 
      error_output.find(
         "bad_dir is an invalid directory. Continuing without the specified build directory.") != string::npos);
}

TEST_F(TrucovTest, cmd_line_revision_script_option) 
{
   string e1 = project_directory + "src1##main.cpp.trucov";
   string e2 = project_directory + "src2##dog.cpp.trucov";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   string s = execute_trucov("report --revision-script ../../../scripts/svn_revision_script.sh");
   EXPECT_TRUE(fs::exists(e1));  
   
   string data = read_file(e1); 

   EXPECT_STRNE( strstr(data.c_str(), "main.cpp@@"), NULL );  

   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, cmd_line_revision_script_option_bad_file) 
{
   string e1 = project_directory + "src1##main.cpp.trucov";
   string e2 = project_directory + "src2##dog.cpp.trucov";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   string s = execute_trucov("report --revision-script bad");
   EXPECT_TRUE(fs::exists(e1));  
   
   string data = read_file(e1); 

   EXPECT_STREQ( strstr(data.c_str(), "main.cpp@@"), NULL );  
   EXPECT_STRNE( strstr(data.c_str(), "main.cpp"), NULL );  

   fs::remove_all(e1);
   fs::remove_all(e2);
}

TEST_F(TrucovTest, cmd_line_option_version) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // Test accumlates
   string s = execute_trucov("--version");
   EXPECT_FALSE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
}

TEST_F(TrucovTest, cmd_line_option_help) 
{
   string e1 = project_directory + "src1##main.cpp.dot";
   string e2 = project_directory + "src2##dog.cpp.dot";
   fs::remove_all(e1);
   fs::remove_all(e2);
   
   // normal help 
   string s = execute_trucov("--help");
   EXPECT_FALSE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
   
   // config help 
   string s2 = execute_trucov("--help-config");
   EXPECT_FALSE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 

   // they should print different things
   EXPECT_STRNE(s.c_str(), s2.c_str());
}

