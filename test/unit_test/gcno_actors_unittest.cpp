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
///  gcno_actors_unittest.cpp 
///
///  @brief
///  Tests the gcno actors. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

#include <fstream>

// SOURCE FILE 

#define protected public // Needed to test private methods.
#define private public

#include "gcno_actors.h"
#include "parser_builder.h"

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

Parser_builder * m_parser_builder;
Parsing_data_gcno * m_parsing_data; 
map<string, Source_file> r;

class Gcno_actors : public testing::Test
{
protected:

   void Initialize()
   {
      Selector & selector = Selector::get_instance();
      vector<string> selection;
      selector.select(selection);

      string path = "../scripts/svn_revision_script.sh";
      vector<string> source_list;
      source_list.push_back("source");
      m_parser_builder = new Parser_builder(r, path, source_list);
  
      ofstream o;
      m_parsing_data = new Parsing_data_gcno(*m_parser_builder, false, o); 
   }

   // Quick way to create a parser_data_gcno.
   Parsing_data_gcno create_parser_data(void)
   {
      map<string, Source_file> r;
      string path = "../scripts/svn_revision_script.sh";
      vector<string> source_list;
      Parser_builder parser_builder(r, path, source_list);
      ofstream o;

      Parsing_data_gcno data(parser_builder, false, o);    
      return data; 
   }                  
};

//////////////////////////////////////////////////////////////////////////////
// Test Parsing_data_gnco      
//////////////////////////////////////////////////////////////////////////////

TEST(parsing_data_gcno, constructor)
{
   map<string, Source_file> r;
   string path = "../scripts/svn_revision_script.sh";
   vector<string> source_list;
   Parser_builder parser_builder(r, path, source_list);
   ofstream o;

   Parsing_data_gcno data(parser_builder, false, o);    
 
   EXPECT_FALSE(data.is_dump);
   EXPECT_EQ( &data.parser_builder, &parser_builder );
   EXPECT_EQ( &data.dump_file, &o );
}

//////////////////////////////////////////////////////////////////////////////
// Test Gcno Grammar Actions 
//////////////////////////////////////////////////////////////////////////////

TEST_F(Gcno_actors, gcno_action_gcnofile)
{
   Initialize();
   
   gcno_action_gcnofile g;
   Parsing_data_gcno data = create_parser_data();
   char * c = NULL;

   g.act(data, c, c); 
}

TEST_F(Gcno_actors, do_gcnofile)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_gcnofile>
      actor = do_gcnofile(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_AnnounceFunction)
{
   gcno_action_AnnounceFunction g;
   char * c = NULL;
   m_parsing_data->rIdent = 1;
   m_parsing_data->rChecksum = 1;
   m_parsing_data->rSource = "source";
   m_parsing_data->rName = "name";
      

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_AnnounceFunction)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_AnnounceFunction>
      actor = do_AnnounceFunction(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_BasicBlocks)
{
   gcno_action_BasicBlocks g;
   char * c = NULL;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_BasicBlocks)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_BasicBlocks>
      actor = do_BasicBlocks(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_bFlags)
{
   gcno_action_bFlags g;
   char * c = NULL;
   m_parsing_data->blocks = 1;
   m_parsing_data->bIteration = 0;
   m_parsing_data->bFlags = 1;
   
   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_bFlags)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_bFlags>
      actor = do_bFlags(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_Arcs)
{
   gcno_action_Arcs g;
   char * c = NULL;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_Arcs)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_Arcs>
      actor = do_Arcs(data); 
   EXPECT_TRUE(&actor);
}


TEST_F(Gcno_actors, gcno_action_Arc)
{
   m_parser_builder->m_merging = true; 
   gcno_action_Arc g;
   char * c = NULL;
   m_parsing_data->aDestBlock = 1;
   m_parsing_data->aFlags = 1;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_Arc)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_Arc>
      actor = do_Arc(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_Lines)
{
   gcno_action_Lines g;
   char * c = NULL;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_Lines)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_Lines>
      actor = do_Lines(data); 
   EXPECT_TRUE(&actor);
}

TEST_F(Gcno_actors, gcno_action_lName)
{
   gcno_action_lName g;
   m_parsing_data->lName = "source";
   char * c = NULL;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_lName)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_lName>
      actor = do_lName(data); 
   EXPECT_TRUE(&actor);
}


TEST_F(Gcno_actors, gcno_action_lLineno)
{
   gcno_action_lLineno g;
   m_parsing_data->lLineno = 1;
   char * c = NULL;

   g.act(*m_parsing_data, c, c);
}

TEST_F(Gcno_actors, do_lLineno)
{
   Parsing_data_gcno data = create_parser_data();
   SPIRIT_NAMESPACE::ref_value_actor<Parsing_data_gcno, gcno_action_lLineno>
      actor = do_lLineno(data); 
   EXPECT_TRUE(&actor);
}



