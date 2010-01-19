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
///  @file
///  revision_script_manager_unittest.cpp
///
///  @brief
///  Tests the revision_script_manager class. 
///
///  @remarks
///  Tests revision_script_manager.h and revision_script_manager.cpp. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

// SOURCE FILE

#define protected public // Needed to test private methods.
#define private public

#include "revision_script_manager.h"

#undef protected // Undo changes made to test private methods.
#undef private

//////////////////////////////////////////////////////////////////////////////
// Default Constructor
//////////////////////////////////////////////////////////////////////////////

TEST(revision_script_manager_constructor, empty_string)
{
   // Setup
   Revision_script_manager rm("");

   // Test rev script path.
   EXPECT_STREQ(rm.m_revision_script_path.c_str(), "");

   // Test invalid.
   EXPECT_FALSE(rm.m_revision_valid);
} 

TEST(revision_script_manager_constructor, non_empty_string)
{
   // Setup
   Revision_script_manager rm("horse");

   // Test rev script path
   EXPECT_STREQ(rm.m_revision_script_path.c_str(), "horse");
   
   // Test valid is set.
   EXPECT_TRUE(rm.m_revision_valid); 
}

//////////////////////////////////////////////////////////////////////////////
// is_valid() 
//////////////////////////////////////////////////////////////////////////////

TEST(is_valid, normal_case)
{
   // Setup
   Revision_script_manager rm("horse");

   // Test valid method
   EXPECT_TRUE(rm.is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// get_revision_number(...) 
//////////////////////////////////////////////////////////////////////////////

TEST(get_revision_number, invalid_revision_script)
{
   // Setup
   Revision_script_manager rm("");

   // Test 
   EXPECT_STREQ(rm.get_revision_number("foo").c_str(), "");    
}

TEST(get_revision_number, invalid_file_path)
{
   // Setup
   Revision_script_manager rm("../scripts/svn_revision_script.sh");
   rm.m_revision_valid = false;

   EXPECT_STREQ("", rm.get_revision_number("horse").c_str()); 
}

TEST(get_revision_number, valid_file_path)
{
   // Setup
   Revision_script_manager rm("../scripts/svn_revision_script.sh"); 

   EXPECT_EQ(5, rm.get_revision_number("../scripts/svn_revision_script.sh").size());
}

