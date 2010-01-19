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
///  prims_unittest.cpp 
///
///  @brief
///  Tests the primitive parsing methods. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

#include <fstream>
#include <vector>
#include <iostream>
#include <map>

#include <boost/ref.hpp>

#include <boost/version.hpp>

#if BOOST_VERSION < NEW_SPIRIT_VERSION
   #define SPIRIT_NAMESPACE boost::spirit
   #include <boost/spirit/core.hpp>
#else
   #define SPIRIT_NAMESPACE boost::spirit::classic
   #include <boost/spirit/include/classic_core.hpp>
#endif

#include "parser.h"
#include "gcno_grammar.h"
#include "gcda_grammar.h"
#include "parser_builder.h"
#include "source_file.h"

// SOURCE FILE 
#define protected public // Needed to test private methods.
#define private public

#include "prims.h"

#undef protected // Undo changes made to test private methods.
#undef private

namespace SP = SPIRIT_NAMESPACE;  
using namespace std;

class PrimsTest : public testing::Test
{
protected:

   char * gcno_first, * gcno_last;
   char * gcda_first, * gcda_last;
   std::vector<char> gcno_buf, gcda_buf;

   char * fail_first, * fail_last;
   string fail_buf;

   Parser_builder * parser_builder;   
   map<string, Source_file> source_files;
   vector<string> found_source_files;

   PrimsTest()
   {
      parser_builder = 0;
   }

   // Called before every test.  
   virtual void SetUp()
   {
      std::ifstream gcno_in("../test/resources/fast_protocol_test-control_cadi.gcno");  
      std::ifstream gcda_in("../test/resources/fast_protocol_test-control_cadi.gcda");  
   
      gcno_in.seekg(0, ios::end);
      unsigned int length = gcno_in.tellg(); 
      gcno_buf.resize( length );
      gcno_in.seekg(0, ios::beg);
      gcno_in.read(&gcno_buf[0], length);
      gcno_first = &gcno_buf[0];
      gcno_last = &gcno_buf[0] + length;

      gcda_in.seekg(0, ios::end);
      length = gcda_in.tellg();
      gcda_buf.resize( length );
      gcda_in.seekg(0, ios::beg);
      gcda_in.read(&gcda_buf[0], length); 
      gcda_first = &gcda_buf[0];
      gcda_last = &gcda_buf[0] + length;

      if (parser_builder)
         delete parser_builder;

      parser_builder = new Parser_builder(source_files, 
         "../scripts/svn_revision_script.sh", found_source_files);
   
      fail_buf = "abcdefaldksjfoaij3294723984yaisdjfdsahflidsajfoidsajfasd";
      fail_first = &fail_buf[0];
      fail_last = &fail_buf[0] + fail_buf.size();
   }

};

class int64_parser_test : public testing::Test
{
protected:

   bool compare_64(uint64_t x, uint64_t y)
   {
      return x == y;
   }
};

//////////////////////////////////////////////////////////////////////////////
// raw_parse and helper method tests. 
//////////////////////////////////////////////////////////////////////////////

TEST_F(PrimsTest, reverse_buffer)
{
   char buf[5] = { 'a', 'b', 'c', 'd', '\0' };
   reverse_buffer( buf ); 
   
   EXPECT_STREQ("dcba", buf);
}

TEST_F(PrimsTest, raw_parse_failure)
{
   ofstream o;
   Gcno_grammar g( *parser_builder, false, o );
   SP::parse_info<> info = raw_parse(fail_first, fail_last, g); 
   
   EXPECT_FALSE(info.full);     
}

TEST_F(PrimsTest, raw_parse_gcno)
{
   ofstream o;
   Gcno_grammar g( *parser_builder, false, o );
   SP::parse_info<> info = raw_parse(gcno_first, gcno_last, g); 
   
   EXPECT_TRUE(info.full);     
}

TEST_F(PrimsTest, raw_parse_gcda)
{
   ofstream o;
   Gcda_grammar g( *parser_builder, false, o );
   SP::parse_info<> info = raw_parse(gcda_first, gcda_last, g); 
   
   EXPECT_TRUE(info.full);     
}

//////////////////////////////////////////////////////////////////////////////
// INT32 Primitive Tests  
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Int32 on your x86 computer:
// 
//    | lowlow | low | high | highhigh |
//    0                                32
//////////////////////////////////////////////////////////////////////////////
// Int32 in the input stream:
//
// Little Endian = | lowlow byte | low byte | high byte | highhigh byte |   
//                 0                                                     32
//
// Big Endian = | highigh byte | high byte | low byte | lowlow byte |
//              0                                                   32
//////////////////////////////////////////////////////////////////////////////

TEST_F(PrimsTest, int32lit_ctor)
{
   int32lit<unsigned int> p(1);

   EXPECT_EQ(1, p.int32);
}

TEST_F(PrimsTest, int32lit_test)
{
   int32lit<unsigned int> p(1);
  
   EXPECT_TRUE(p.test(1));
   
   EXPECT_FALSE(p.test(0));    
   EXPECT_FALSE(p.test(2));    
}
 
TEST_F(PrimsTest, anyint32_test)
{
   anyint32<unsigned int> p;

   EXPECT_TRUE(p.test(0));
   EXPECT_TRUE(p.test(1));
   EXPECT_TRUE(p.test(2));
}

TEST_F(PrimsTest, nonzero32_test)
{
   nonzero32<unsigned int> p;
  
   EXPECT_TRUE(p.test(1));
   EXPECT_TRUE(p.test(2));    
   
   EXPECT_FALSE(p.test(0));    
}
 
TEST_F(PrimsTest, TOKEN32)
{
   int32lit<int> r = TOKEN32(5);
        
   EXPECT_TRUE(r.test(5));
    
   EXPECT_FALSE(r.test(4));    
   EXPECT_FALSE(r.test(6));    
}

TEST(int32_parser, big_endian_pass)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = &b1[4];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(1, global_parsed_int32); 
   EXPECT_TRUE(f == l && hit);        // good parse
} 

TEST(int32_parser, little_endian_pass)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = true;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 1, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[4];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(1, global_parsed_int32); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(int32_parser, test_check_fails)
{
   int32lit<unsigned int> p(2); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = &b1[4];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(1, global_parsed_int32); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}
 
TEST(int32_parser, parse_failure_length0)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = f;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(0, global_parsed_int32); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}
 
TEST(int32_parser, parse_failure_length1)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = f + 1;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(0, global_parsed_int32); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}
 
TEST(int32_parser, parse_failure_length2)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = f + 2;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(0, global_parsed_int32); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}
 
TEST(int32_parser, parse_failure_length3)
{
   int32lit<unsigned int> p(1); 
   global_little_endian = false;
   global_parsed_int32 = 0; 
 
   char b1[4] = { 0, 0, 0, 1 };    
   char * f = &b1[0];
   char * l = f + 3;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_EQ(0, global_parsed_int32); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

/////////////////////////////////////////////////////////////////////////////// 
// String Primitive tests. 
///////////////////////////////////////////////////////////////////////////////

TEST(string_parser, big_endian_pass)
{
   string_parser<string> p;
   global_little_endian = false;
   global_parsed_string = "";
 
   char b1[8] = { 0, 0, 0, 1, 'a', 'b', 'c', '\0' };    
   char * f = &b1[0];
   char * l = &b1[8];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("abc", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, little_endian_pass)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 1, 0, 0, 0, 'a', 'b', 'c', '\0' };    
   char * f = &b1[0];
   char * l = &b1[8];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("abc", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, consume_padding_length_1)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 1, 0, 0, 0, 'a', 'b', 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[8];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("ab", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}


TEST(string_parser, consume_padding_length_2)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 1, 0, 0, 0, 'a', 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[8];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("a", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, consume_padding_length_3)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[12] = { 2, 0, 0, 0, 'a', 'b', 'c', 'd', 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[12];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("abcd", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, consume_padding_length_4)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[12] = { 2, 0, 0, 0, 'a', 'b', 'c', 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[12];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("abc", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, consume_padding_length_5)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[12] = { 2, 0, 0, 0, 'a', 'b', 0, 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[12];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("ab", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, large_string)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[24] = {  5,   0,   0,   0, 
                  'a', 'b', 'c', 'd', 
                  'e', 'f', 'g', 'h', 
                  'i', 'j', 'k',  'l',
                   0,   0,   0,   0,
                   0,   0,   0,   0,};    
   char * f = &b1[0];
   char * l = f + 24;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("abcdefghijkl", global_parsed_string.c_str()); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST(string_parser, failure_string_size_value_is_0)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = &b1[8];

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_size_is_0_bytes)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = f;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_size_is_1_bytes)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = f + 1;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_size_is_3_bytes)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b1[0];
   char * l = f + 3;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_is_4_bytes_short)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[12] = { 2, 0, 0, 0, 'a', 'b', 'c', 'd' };    
   char * f = &b1[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_is_3_bytes_short)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 1, 0, 0, 0, 'a', 'b', 'c', 'd' };    
   char * f = &b1[0];
   char * l = f + 4;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST(string_parser, failure_string_is_1_byte_short)
{
   string_parser<string> p;
   global_little_endian = true;
   global_parsed_string = "";
 
   char b1[8] = { 1, 0, 0, 0, 'a', 'b', 'c', 'd' };    
   char * f = &b1[0];
   char * l = f + 7;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_STREQ("", global_parsed_string.c_str()); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

////////////////////////////////////////////////////////////////////////////// 
// INT64 Primitive tests. 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Int64 on your x86 computer:
//
//  | high_int | low_int |
//  0                    64   
//////////////////////////////////////////////////////////////////////////////
//
// Int64 in the input stream:    (big and little endian)
//
//  | low_int | high_int |
//  0                    64  
//////////////////////////////////////////////////////////////////////////////

TEST_F(int64_parser_test, pass_little_endian_number_is_1 )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(1, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_little_endian_number_1_below_max)
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   uint64_t r = -2; 
   EXPECT_TRUE( compare_64(r, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_little_endian_small_number )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(0x44332211, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_little_endian_large_number )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   uint64_t c = 0xfffffffe;
   c |= ((uint64_t) 0xffffffff) << 32; 
   uint64_t c2 = -2;

   // Assume on little endian machine
   EXPECT_TRUE( compare_64(c, c2) );

   uint64_t r = 0x44332211;
   r |= ((uint64_t) 0x88776655) << 32; 
   EXPECT_TRUE( compare_64(r, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_big_endian_number_is_1 )
{
   int64_parser<uint64_t> p;
   global_little_endian = false;
   global_parsed_int64 = 0;
 
   char b[8] = { 0, 0, 0, 1, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(1, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_big_endian_number_1_below_max)
{
   int64_parser<uint64_t> p;
   global_little_endian = false;
   global_parsed_int64 = 0;
 
   char b[8] = { 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   uint64_t r = -2; 
   EXPECT_TRUE( compare_64(r, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_big_endian_small_number )
{
   int64_parser<uint64_t> p;
   global_little_endian = false;
   global_parsed_int64 = 0;
 
   char b[8] = { 0x44, 0x33, 0x22, 0x11, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(0x44332211, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, pass_big_endian_large_number )
{
   int64_parser<uint64_t> p;
   global_little_endian = false;
   global_parsed_int64 = 0;
 
   char b[8] = { 0x44, 0x33, 0x22, 0x11, 0x88, 0x77, 0x66, 0x55 };    
   char * f = &b[0];
   char * l = f + 8;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   uint64_t c = 0xfffffffe;
   c |= ((uint64_t) 0xffffffff) << 32; 
   uint64_t c2 = -2;

   // Assume on little endian machine
   EXPECT_TRUE( compare_64(c, c2) );

   uint64_t r = 0x44332211;
   r |= ((uint64_t) 0x88776655) << 32; 
   EXPECT_TRUE( compare_64(r, global_parsed_int64) ); 
   EXPECT_TRUE(f == l && hit);        // good parse
}

TEST_F(int64_parser_test, failure_parse_length_0 )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 0;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(0, global_parsed_int64) ); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST_F(int64_parser_test, failure_parse_length_1 )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 1;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(0, global_parsed_int64) ); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

TEST_F(int64_parser_test, failure_parse_length_7 )
{
   int64_parser<uint64_t> p;
   global_little_endian = true;
   global_parsed_int64 = 0;
 
   char b[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };    
   char * f = &b[0];
   char * l = f + 7;

   SP::scanner<char *, SP::scanner_policies<> > scan(f, l);
   SP::match<SP::nil_t> hit = p.parse(scan);

   EXPECT_TRUE( compare_64(0, global_parsed_int64) ); 
   EXPECT_FALSE(f == l && hit);        // bad parse
}

