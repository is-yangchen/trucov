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
///  gcno_grammar_unittest.cpp 
///
///  @brief
///  Tests the gcno_grammar class. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

// SOURCE FILE 
#define protected public // Needed to test private methods.
#define private public

#include "gcno_grammar.h"

#undef protected // Undo changes made to test private methods.
#undef private

using std::ifstream;
using std::ofstream;
using std::map;
using std::vector;
using std::string;

TEST(gcno_grammar_constructor, default_case)
{
   ifstream s("../unit_test/resources/fast_protocol_test-control_cadi.gcno");
   map<string, Source_file> r; 
   string path = "../scripts/svn_revision_script.sh";
   vector<string> source_list;
   Parser_builder parser_builder(r, path, source_list); 
   ofstream d;

   Gcno_grammar grammar(parser_builder, false, d);
   
   EXPECT_TRUE(&grammar);
}

