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
///  gcda_actors_unittest.cpp 
///
///  @brief
///  Tests the gcda actors. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

#include <fstream>

// SOURCE FILE 

#define protected public // Needed to test private methods.
#define private public

#include "gcda_actors.h"

#undef protected // Undo changes made to test private methods.
#undef private

using namespace std;

#if BOOST_VERSION < NEW_SPIRIT_VERSION
   #define SPIRIT_NAMESPACE boost::spirit
   #include <boost/spirit/core.hpp>
   #include <boost/spirit/tree/ast.hpp>
   #include <boost/spirit/attribute.hpp>
   #include <boost/spirit/phoenix.hpp>
   #include <boost/spirit/utility.hpp>
   #include <boost/spirit/dynamic.hpp>
#else
   #define SPIRIT_NAMESPACE boost::spirit::classic
   #include <boost/spirit/include/classic_core.hpp>
   #include <boost/spirit/include/classic_ast.hpp>
   #include <boost/spirit/include/classic_attribute.hpp>
   #include <boost/spirit/include/phoenix1.hpp>
   #include <boost/spirit/include/classic_utility.hpp>
   #include <boost/spirit/include/classic_dynamic.hpp>
#endif

class Gcda_actors : public testing::Test
{
protected:

   Parser_builder * m_parser_builder;
   Parsing_data_gcda * m_parsing_data; 

   // Called before every test.  
   virtual void SetUp()
   {
      map<string, Source_file> r;
      string path = "../scripts/svn_revision_script.sh";
      vector<string> source_list;
      m_parser_builder = new Parser_builder(r, path, source_list);
  
      ofstream o;
      m_parsing_data = new Parsing_data_gcda(*m_parser_builder, false, o);         
   }

   // Quick way to create a parser_data_gcda.
   Parsing_data_gcda create_parser_data(void)
   {
      map<string, Source_file> r;
      string path = "../scripts/svn_revision_script.sh";
      vector<string> source_list;
      Parser_builder parser_builder(r, path, source_list);
      ofstream o;

      Parsing_data_gcda data(parser_builder, false, o);    
      return data; 
   }                  
};

//////////////////////////////////////////////////////////////////////////////
// Test Parsing_data_gnco      
//////////////////////////////////////////////////////////////////////////////

TEST(parsing_data_gcda, constructor)
{
   map<string, Source_file> r;
   string path = "../scripts/svn_revision_script.sh";
   vector<string> source_list;
   Parser_builder parser_builder(r, path, source_list);
   ofstream o;

   Parsing_data_gcda data(parser_builder, false, o);    
 
   EXPECT_FALSE(data.is_dump);
   EXPECT_EQ( &data.parser_builder, &parser_builder );
   EXPECT_EQ( &data.dump_file, &o );
}

//////////////////////////////////////////////////////////////////////////////
// Test Gcda Grammar Actions 
//////////////////////////////////////////////////////////////////////////////

TEST_F(Gcda_actors, gcda_action_gcdafile)
{
   gcda_action_gcdafile g;
   Parsing_data_gcda data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_gcdafile)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_gcdafile>
      actor = do_gcdafile(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcda_actors, gcda_action_AnnounceFunction)
{
   gcda_action_AnnounceFunction g;
   Parsing_data_gcda data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_AnnounceFunction)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_AnnounceFunction>
      actor = do_AnnounceFunction(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcda_actors, gcda_action_aLength)
{
   gcda_action_aLength g;
   Parsing_data_gcda data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_aLength)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_aLength>
      actor = do_aLength(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcda_actors, gcda_action_aCount)
{
   gcda_action_aCount g;
   Parsing_data_gcda data = create_parser_data();
   m_parsing_data->rIdent = 1;
   m_parsing_data->rChecksum = 1;
   m_parsing_data->aCount = 1;
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_aCount)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_aCount>
      actor = do_aCount(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcda_actors, gcda_action_ObjectSummary)
{
   gcda_action_ObjectSummary g;
   Parsing_data_gcda data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_ObjectSummary)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_ObjectSummary>
      actor = do_ObjectSummary(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcda_actors, gcda_action_ProgramSummary)
{
   gcda_action_ProgramSummary g;
   Parsing_data_gcda data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcda_actors, do_ProgramSummary)
{
   Parsing_data_gcda data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcda, gcda_action_ProgramSummary>
      actor = do_ProgramSummary(data); 
   EXPECT_TRUE(&actor);
}


