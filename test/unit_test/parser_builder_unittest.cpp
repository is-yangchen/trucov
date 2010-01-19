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
///  parser_builder_unittest.cpp 
///
///  @brief
///  Tests the Parser_builder class. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>
#include <string>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

// SOURCE FILE 
#define protected public // Needed to test private methods.
#define private public

#include "parser_builder.h"
#include "source_file.h"
#include "record.h"

#undef protected // Undo changes made to test private methods.
#undef private

using namespace std;
using namespace boost;

Parser_builder * pb_parser_builder = NULL;
map<string, Source_file> pb_sources;
vector<string> pb_sources_found; 

class ParserBuilderTest : public testing::Test
{
protected:

   string revision_script;  

   // Called before every test.  
   virtual void SetUp()
   {
      revision_script = "../scripts/svn_revision_script.sh";
   }

   void init_pb(
      Parser_builder *& pb, 
      map<string, Source_file> & sources,
      vector<string> & sources_found,
      string & rev_path,
      vector<string> selection)
   {
      if ( pb != NULL ) {
         delete pb;
      }

      Selector & selector = Selector::get_instance();
      selector.select(selection);

      string path = "../scripts/svn_revision_script.sh";
      pb = new Parser_builder(sources, rev_path, sources_found);
      pb->m_source_files.clear(); 
   }
};

TEST_F(ParserBuilderTest, constructor)
{
   pb_sources_found.push_back("source"); 
   vector<string> selection;
   init_pb(pb_parser_builder, pb_sources, pb_sources_found, revision_script, selection);

   EXPECT_EQ(pb_parser_builder->m_found_source_files.size(), 1);       
   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
}

TEST_F(ParserBuilderTest, store_record_not_within_project)
{
   pb_parser_builder->store_record(1,1,"sourc","foo",1);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   EXPECT_FALSE(pb_parser_builder->m_last_record_selected);
}

TEST_F(ParserBuilderTest, store_blocks_not_within_project)
{
   pb_parser_builder->store_blocks(1, 1, 0);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   EXPECT_FALSE(pb_parser_builder->m_last_record_selected);
}

TEST_F(ParserBuilderTest, store_arcs_not_within_project)
{
   pb_parser_builder->store_arcs(0, 1, 0);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   EXPECT_FALSE(pb_parser_builder->m_last_record_selected);
}

TEST_F(ParserBuilderTest, store_line_number_not_within_project)
{
   pb_parser_builder->store_line_number(0, 42, "linesource");   
   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   EXPECT_FALSE(pb_parser_builder->m_last_record_selected);
}

TEST_F(ParserBuilderTest, store_count_not_found_record)
{
   pb_parser_builder->store_count(1, 1, 1);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   EXPECT_FALSE(pb_parser_builder->m_last_record_selected);
}

TEST_F(ParserBuilderTest, store_record_new_record)
{
   pb_parser_builder->store_record(1,1,"source","foo",1);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 1);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_checksum, 1);
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_name.c_str(), "foo");
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_source.c_str(), "source");
}

TEST_F(ParserBuilderTest, store_blocks_new_blocks)
{
   pb_parser_builder->store_blocks(2, 1, 0);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks.size(), 1);

   pb_parser_builder->store_blocks(2, 1, 1);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks.size(), 2);
}

TEST_F(ParserBuilderTest, store_arcs_new_arcs)
{
   pb_parser_builder->store_arcs(0, 1, 0);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_arcs.size(), 1);
}

TEST_F(ParserBuilderTest, store_line_number_new_line_number)
{
   pb_parser_builder->store_line_number(0, 42, "linesource");   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_lines["linesource"].m_lines.size(), 1);

   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_lines["linesource"].m_lines[0].m_line_num, 42);
}

TEST_F(ParserBuilderTest, store_count_new_count)
{
   pb_parser_builder->store_count(1, 1, 1);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts.size(), 1);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts[0], 1);
   
   pb_parser_builder->store_count(1, 1, 2);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts.size(), 2);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts[1], 2);
}

TEST_F(ParserBuilderTest, store_record_merge)
{
   pb_parser_builder->store_record(1,1,"source","foo",1);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 1);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_checksum, 1);
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_name.c_str(), "foo");
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_source.c_str(), "source");
  
   EXPECT_TRUE(pb_parser_builder->m_merging); 
}

TEST_F(ParserBuilderTest, store_blocks_merge)
{
   pb_parser_builder->store_blocks(2, 1, 0);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks.size(), 2);

   pb_parser_builder->store_blocks(4, 4, 4);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks.size(), 2);
}

TEST_F(ParserBuilderTest, store_arcs_merge)
{
   pb_parser_builder->store_arcs(4, 4, 4);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_arcs.size(), 1);
}

TEST_F(ParserBuilderTest, store_line_number_merge)
{
   pb_parser_builder->store_line_number(0, 33, "linesource");   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_lines["linesource"].m_lines.size(), 1);

   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_blocks[0].m_lines["linesource"].m_lines[0].m_line_num, 42);
}

TEST_F(ParserBuilderTest, store_count_merge_counts)
{
   pb_parser_builder->store_count(1, 1, 1);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts.size(), 2);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts[0], 2);
   
   pb_parser_builder->store_count(1, 1, 2);   
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts.size(), 2);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_counts[1], 4);
}

TEST_F(ParserBuilderTest, store_record_no_rev_script_data)
{
   string rev = "../test/resources/fake_rev_script.sh"; 
   vector<string> selection;
   init_pb(pb_parser_builder, pb_sources, pb_sources_found, rev, selection);

   EXPECT_EQ(pb_parser_builder->m_found_source_files.size(), 1);       
   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 0);
   
   pb_parser_builder->store_record(1,1,"source","foo",1);   

   EXPECT_EQ(pb_parser_builder->m_source_files.size(), 1);
   EXPECT_EQ(pb_parser_builder->m_source_files["source"].m_records[1].m_checksum, 1);
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_name.c_str(), "foo");
   EXPECT_STREQ(pb_parser_builder->m_source_files["source"].m_records[1].m_source.c_str(), "source");
}
