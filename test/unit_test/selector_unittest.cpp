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
///  @file selector_unittest.cpp
///
///  @brief
///  Tests the selector class. 
///
///  @remarks
///  Tests selector.cpp and selector.h.
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

// SOURCE FILE

#define protected public // Needed to test private methods.
#define private public

#include "selector.h"

#undef protected // Undo changes made to test private methods.
#undef private

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// get_instance_test()
//////////////////////////////////////////////////////////////////////////////

// Verify returns instance when unititialized. 
TEST(get_instance_test, default_case)
{
   Selector & selector = Selector::get_instance();
 
   // Pointer should be set.
   EXPECT_NE(NULL, (int)Selector::instance_ptr);
}  

//////////////////////////////////////////////////////////////////////////////
// select() 
//////////////////////////////////////////////////////////////////////////////

// Verify select with empty selection_list
TEST(select_test, empty_list)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selector.select( selection_list ); 

   EXPECT_EQ(0, selector.regex_selections.size());
}

// Verify select with non empty selection list
TEST(select_test, non_empty_list)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo");
   selection_list.push_back(".*dog.*");   
   selector.select( selection_list );

   // Verify count.
   EXPECT_EQ(2, selector.regex_selections.size());
}

TEST (select_test, is_full_match)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo");
   selection_list.push_back("dog");   
   Config & config = Config::get_instance();
   config.m_bool_flag_value[Config::Full_match] = true;
   selector.select( selection_list );

   // Verify count.
   EXPECT_EQ(2, selector.regex_selections.size());
}

//////////////////////////////////////////////////////////////////////////////
// is_selected() 
//////////////////////////////////////////////////////////////////////////////

// Verify is_selected, all selected. 
TEST(is_selected_test, all_selected)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selector.select( selection_list );
  
   EXPECT_TRUE(selector.is_selected("source", "foo"));    
}  

// Verify is_selected, source file    
TEST(is_selected_test, source_selected)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("source");
   selector.select( selection_list );
  
   EXPECT_TRUE(selector.is_selected("source", "foo"));    
   EXPECT_FALSE(selector.is_selected("", ""));
}  

// Verify is_selected, function name is selected.
TEST(is_selected_test, function_name_selected)
{
   // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo");
   selector.select( selection_list );
   selector.is_signature_match = false;
  
   EXPECT_TRUE(selector.is_selected("source", "foo"));    
   EXPECT_FALSE(selector.is_selected("", ""));
}  

// Verify is_selected, function signature is selected
TEST(is_selected_test, function_signature_selected)
{
      // Initialize.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("int foo\\(void\\)");
   Config & config = Config::get_instance();
   config.m_bool_flag_value[Config::Full_match] = false;
   config.m_bool_flag_value[Config::And] = false;
   selector.select( selection_list );
   selector.is_signature_match = true;

   EXPECT_TRUE(selector.check_in_selection("int foo(void)"));
   EXPECT_FALSE(selector.is_selected("", "fooo"));
}

//////////////////////////////////////////////////////////////////////////////
// check_in_selection() 
//////////////////////////////////////////////////////////////////////////////

// Verify returns true when in selection.  
TEST(check_in_selection, is_full_match)
{
   // Setup full match.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo.");
   Config & config = Config::get_instance();
   config.m_bool_flag_value[Config::Full_match] = true;
   config.m_bool_flag_value[Config::And] = false;
   selector.select( selection_list );
 
   EXPECT_TRUE(selector.check_in_selection("foof"));
   EXPECT_FALSE(selector.check_in_selection("foobar"));
   EXPECT_FALSE(selector.check_in_selection("barb"));
}

// Check when not full match
TEST(check_in_selection, is_not_full_match)
{
   // Setup full match.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo");
   Config & config = Config::get_instance();
   config.m_bool_flag_value[Config::Full_match] = false;
   config.m_bool_flag_value[Config::And] = false;
   selector.select( selection_list );
  
   EXPECT_TRUE(selector.check_in_selection("foof"));
   EXPECT_TRUE(selector.check_in_selection("foobar"));
   EXPECT_FALSE(selector.check_in_selection("barb"));
}

TEST(check_in_selection, is_and_match)
{
   // Setup full match.
   Selector & selector = Selector::get_instance();
   vector<string> selection_list;
   selection_list.push_back("foo");
   selection_list.push_back("bar");
   Config & config = Config::get_instance();
   config.m_bool_flag_value[Config::Full_match] = false;
   config.m_bool_flag_value[Config::And] = true; 
   selector.select( selection_list );
 
   EXPECT_TRUE(selector.check_in_selection("foobar"));
   EXPECT_FALSE(selector.check_in_selection("bar"));
   EXPECT_FALSE(selector.check_in_selection("foo"));
}

//////////////////////////////////////////////////////////////////////////////
// parse_function_name() 
//////////////////////////////////////////////////////////////////////////////

// Test returns function name.
TEST(parse_function_name, check_function_name_valid)
{
   // Setup
   Selector & selector = Selector::get_instance();
 
   EXPECT_STREQ(selector.parse_function_name("int something::foo(void)").c_str(), 
      "foo");
   EXPECT_STREQ(selector.parse_function_name("1::2::3::foo(int, int, char)").c_str(),
      "foo");
}

