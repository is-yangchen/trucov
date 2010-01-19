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
///  @file graph_functionaltest.cpp 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "helper_functionaltest.h"

// Test graph 
TEST_F(TrucovTest, graph)
{
   string s = execute_trucov("graph");

   string e1 = project_directory + "coverage.svg";
   string e2 = project_directory + "coverage.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
 
   fs::remove_all(e1);
}

TEST_F(TrucovTest, graph_render_format_svg)
{
   string s = execute_trucov("graph --render-format svg");

   string e1 = project_directory + "coverage.svg";
   string e2 = project_directory + "coverage.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
 
   fs::remove_all(e1);
}

TEST_F(TrucovTest, graph_render_format_pdf)
{
   string s = execute_trucov("graph --render-format pdf");

   string e1 = project_directory + "coverage.pdf";
   string e2 = project_directory + "coverage.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
 
   fs::remove_all(e1);
}

TEST_F(TrucovTest, graph_render_format_bad)
{
   string s = execute_trucov("graph --render-format bad");

   string e1 = project_directory + "coverage.svg";
   string e2 = project_directory + "coverage.dot";

   EXPECT_TRUE(fs::exists(e1));  
   EXPECT_FALSE(fs::exists(e2)); 
 
   fs::remove_all(e1);
}

// Test output option command line
TEST_F(TrucovTest, graph_cmd_line_outputfile_option_with_slash)
{
   string e1 = project_directory + "coverage.svg";
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
TEST_F(TrucovTest, graph_cmd_line_outputfile_option_without_slash)
{
   string e1 = project_directory + "output.svg";
   string f1 = project_directory + "coverage.svg";
   fs::remove_all(e1);
   fs::remove_all(f1);

   // Test 1 option
   string s = execute_trucov("dot -o output.svg");
   EXPECT_TRUE(fs::exists(e1));
   EXPECT_FALSE(fs::exists(f1));
   fs::remove_all(e1);
}

// Test output option command line
TEST_F(TrucovTest, graph_cmd_line_outputfile_option_without_extension)
{
   string e1 = project_directory + "render_file";
   string f1 = project_directory + "coverage.svg";
   fs::remove_all(e1);
   fs::remove_all(f1);

   // Test 1 option
   string s = execute_trucov("dot -o render_file");
   EXPECT_TRUE(fs::exists(e1));
   EXPECT_FALSE(fs::exists(f1));
   fs::remove_all(e1);
}
