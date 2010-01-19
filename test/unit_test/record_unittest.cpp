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
///  @file record_unittest.cpp
///
///  @brief
///  Tests the record class. 
///
///  @remarks
///  Tests record.cpp and record.h.
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>

// SOURCE FILE

#define protected public // Needed to test private methods.
#define private public

#include "record.h"

#undef protected // Undo changes made to test private methods.
#undef private

//////////////////////////////////////////////////////////////////////////////
// get_line_nums()
//////////////////////////////////////////////////////////////////////////////

TEST(get_lines_test, no_lines)
{
   // Initialize
   Lines_data l;

   std::vector<Line> line_nums = l.get_lines();

   // Verify no lines assigned yet
   EXPECT_EQ( 0, line_nums.size() );
}

TEST(get_lines_test, has_lines)
{
   // Initialize and add 2 lines
   Lines_data l;
   Line l1;
   Line l2;
   l1.m_line_num = 1;
   l2.m_line_num = 3;
   l.m_lines.push_back( l1 );
   l.m_lines.push_back( l2 );

   std::vector<Line> line_nums = l.get_lines();

   // Verify count
   EXPECT_EQ( 2, line_nums.size() );
}

//////////////////////////////////////////////////////////////////////////////
// Arc( dest, origin, flag)
//////////////////////////////////////////////////////////////////////////////

// Test the Arc class constructor
TEST(arc_constructor_test, default_constructor)
{
   // Initialize
   Arc a( 1, 0, 1 );

   // Test initialization
   EXPECT_EQ(1, a.m_flag);
   EXPECT_EQ(1, a.m_dest_block);
   EXPECT_EQ(0, a.m_origin_block);
   EXPECT_EQ(-1, a.m_count);
}

//////////////////////////////////////////////////////////////////////////////
// is_fake() 
//////////////////////////////////////////////////////////////////////////////

TEST(is_fake_arc_test, not_fake)
{
   // Initialize
   Arc a0( 1, 0, 0 );
   Arc a1( 1, 0, 1 );
   Arc a4( 1, 0, 4 );
   Arc a5( 1, 0, 5 );

   // Test all non-fake arc cases
   EXPECT_FALSE( a0.is_fake() );
   EXPECT_FALSE( a1.is_fake() );
   EXPECT_FALSE( a4.is_fake() );
   EXPECT_FALSE( a5.is_fake() );
}

TEST(is_fake_arc_test, fake)
{
   // Initialize
   Arc a2( 1, 0, 2 );
   Arc a3( 1, 0, 3 );

   // Test all fake arc cases
   EXPECT_TRUE( a2.is_fake() );
   EXPECT_TRUE( a3.is_fake() );
}

//////////////////////////////////////////////////////////////////////////////
// is_taken() 
//////////////////////////////////////////////////////////////////////////////

TEST(is_taken_test, not_taken)
{
   // Initialize
   Arc a0( 1, 0, 0 );
   Arc a1( 1, 0, 0 );
   a1.m_count = 0;

   // False if m_count <= 0
   EXPECT_FALSE( a0.is_taken() );
   EXPECT_FALSE( a1.is_taken() );
}

TEST(is_taken_test, taken)
{
   // Initialize
   Arc a( 1, 0, 0 );
   a.m_count = 1;

   // True if m_count > 0
   EXPECT_TRUE( a.is_taken() );
}

//////////////////////////////////////////////////////////////////////////////
// get_count() 
//////////////////////////////////////////////////////////////////////////////

TEST(get_count_test, getter)
{
   // Initialize
   Arc a0( 1, 0, 0 );
   Arc a1( 1, 0, 1 );
   a1.m_count = 12;

   // Test default case
   EXPECT_EQ( -1, a0.get_count() );
   // Test assignment case
   EXPECT_EQ( 12, a1.get_count() );
}

//////////////////////////////////////////////////////////////////////////////
// get_dest() 
//////////////////////////////////////////////////////////////////////////////

TEST(get_dest_test, getter)
{
   // Initialize
   Arc a( 1, 0, 0 );

   EXPECT_EQ( 1, a.get_dest() );
}

//////////////////////////////////////////////////////////////////////////////
// get_flag() 
//////////////////////////////////////////////////////////////////////////////

TEST(get_flag_test, getter)
{
   // Initialize
   Arc a( 1, 0, 5 );

   EXPECT_EQ( 5, a.get_flag() );
}

//////////////////////////////////////////////////////////////////////////////
// Block( block_no )
//////////////////////////////////////////////////////////////////////////////

TEST( block_constructor_test, default_constructor )
{
   // Initialize
   Block b( 4 );

   // Verify initialization
   EXPECT_EQ( 4, b.m_block_no );
   EXPECT_FALSE( b.m_normalized );
   EXPECT_FALSE( b.m_fake );
}

//////////////////////////////////////////////////////////////////////////////
// is_end_block()
//////////////////////////////////////////////////////////////////////////////

TEST( is_end_block_test, not_end_block )
{
   // Initialize
   Block b( 4 );
   Arc a( 1, 0, 0 );
   b.m_arcs.push_back( a );

   EXPECT_FALSE( b.is_end_block() );
}

TEST( is_end_block_test, end_block )
{
   // Initialize
   Block b( 4 );

   EXPECT_TRUE( b.is_end_block() );
}

//////////////////////////////////////////////////////////////////////////////
// is_start_block()
//////////////////////////////////////////////////////////////////////////////

TEST( is_start_block_test, not_start_block )
{
   // Initialize
   Block b( 4 );
   Arc a( 1, 0, 0 );
   b.m_from_arcs.push_back( &a );

   EXPECT_FALSE( b.is_start_block() );
}

TEST( is_start_block_test, start_block )
{
   // Initialize
   Block b( 4 );

   EXPECT_TRUE( b.is_start_block() );
}

//////////////////////////////////////////////////////////////////////////////
// is_branch()
//////////////////////////////////////////////////////////////////////////////

TEST( is_branch_test, not_a_branch )
{
   // Initialize
   Block b0( 0 );
   Block b1( 1 );
   Arc a1( 1, 0, 0 );
   b1.m_arcs.push_back( a1 );

   // Verify case, no outgoing arcs
   EXPECT_FALSE( b0.is_branch() );
   // Verify case, one outgoing arc
   EXPECT_FALSE( b1.is_branch() );
}

TEST( is_branch_test, a_branch )
{
   // Initialize
   Block b( 0 );
   Arc a0( 1, 0, 0 );
   Arc a1( 2, 1, 1 );
   b.m_arcs.push_back( a0 );
   b.m_arcs.push_back( a1 );

   // Verify case more than one outgoing arc
   EXPECT_TRUE( b.is_branch() );
}

//////////////////////////////////////////////////////////////////////////////
// is_fake()
//////////////////////////////////////////////////////////////////////////////

TEST( is_fake_block_test, not_fake )
{
   // Initialize
   Block b( 0 );

   EXPECT_FALSE( b.is_fake() );
}

TEST( is_fake_block_test, fake )
{
   // Initialize
   Block b( 0 );
   b.m_fake = true;

   EXPECT_TRUE( b.is_fake() );
}

//////////////////////////////////////////////////////////////////////////////
// is_inlined()
//////////////////////////////////////////////////////////////////////////////

TEST( is_inlined_block_test, not_inlined )
{
   // Initialize
   Block b( 0 );
   b.m_inlined = false;

   EXPECT_FALSE( b.is_inlined() );
}

TEST( is_inlined_block_test, inlined )
{
   // Initialize
   Block b( 0 );
   b.m_inlined = true;

   EXPECT_TRUE( b.is_inlined() );
}

//////////////////////////////////////////////////////////////////////////////
// is_normalized()
//////////////////////////////////////////////////////////////////////////////

TEST( is_normalized_test, not_normalized )
{
   // Initialize
   Block b( 0 );

   EXPECT_FALSE( b.is_normalized() );
}

TEST( is_normalized_test, normalized )
{
   // Initialize
   Block b( 0 );
   b.m_normalized = true;

   EXPECT_TRUE( b.is_normalized() );
}

//////////////////////////////////////////////////////////////////////////////
// has_full_coverage()
//////////////////////////////////////////////////////////////////////////////

TEST( has_full_coverage_test, normal_no_coverage )
{
   // Initialize

   // Case 1 normal block has only a single outgoing fake arc
   Block b1( 0 );
   Arc a1( 1, 0, 2 );
   b1.m_arcs.push_back( a1 );

   // Case 2 normal block has only a single outgoing normal arc
   Block b2( 0 );
   Arc a2( 1, 0, 1 );
   b2.m_arcs.push_back( a2 );

   // Case 3 normal block has outgoing fake and normal arcs
   Block b3( 0 );
   Arc a3_1( 1, 0, 1 );
   Arc a3_2( 1, 0, 3 );
   b3.m_arcs.push_back( a3_1 );
   b3.m_arcs.push_back( a3_2 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b1.has_full_coverage() );
   EXPECT_FALSE( b2.has_full_coverage() );
   EXPECT_FALSE( b3.has_full_coverage() );
}

TEST( has_full_coverage_test, fake_no_coverage )
{
   // Case 0 fake block has only a single outgoing fake arc
   Block b0( 0 );
   b0.m_fake = true;
   Arc a0( 1, 0, 2 );
   b0.m_arcs.push_back( a0 );

   // Case 1 fake block has only a single outgoing normal arc
   Block b1( 0 );
   b1.m_fake = true;
   Arc a1( 1, 0, 1 );
   b1.m_arcs.push_back( a1 );

   // Case 2 fake block has both normal and fake outoing arcs
   Block b2( 0 );
   b2.m_fake = true;
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b0.has_full_coverage() );
   EXPECT_FALSE( b1.has_full_coverage() );
   EXPECT_FALSE( b2.has_full_coverage() );
}

TEST( has_full_coverage_test, normal_partial_coverage )
{
   // Initialize

   // Case 0 normal block has a taken outgoing normal arc
   Block b0( 0 );
   Arc a0_1( 1, 0, 1 );
   Arc a0_2( 1, 0, 3 );
   Arc a0_3( 1, 0, 5 );
   a0_3.m_count = 1;
   b0.m_arcs.push_back( a0_1 );
   b0.m_arcs.push_back( a0_2 );
   b0.m_arcs.push_back( a0_3 );

   // Case 1 normal block has a taken normal and fake outgoing arcs
   Block b1( 0 );
   Arc a1_1( 1, 0, 1 );
   Arc a1_2( 1, 0, 3 );
   Arc a1_3( 1, 0, 5 );
   a1_2.m_count = 1; 
   a1_3.m_count = 1;
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b1.m_arcs.push_back( a1_3 );

   // Case 2 normal block has a taken fake outgoing arc
   Block b2( 0 );
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   Arc a2_3( 1, 0, 5 );
   a2_2.m_count = 1; 
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b0.has_full_coverage() );
   EXPECT_FALSE( b1.has_full_coverage() );
   EXPECT_FALSE( b2.has_full_coverage() );
}

TEST( has_full_coverage_test, fake_partial_coverage )
{
   // Initialize

   // Case 0 fake block has a taken outgoing normal arc
   Block b0( 0 );
   b0.m_fake = true;
   Arc a0_1( 1, 0, 1 );
   Arc a0_2( 1, 0, 3 );
   Arc a0_3( 1, 0, 5 );
   a0_3.m_count = 1;
   b0.m_arcs.push_back( a0_1 );
   b0.m_arcs.push_back( a0_2 );
   b0.m_arcs.push_back( a0_3 );

   // Case 1 fake block has a taken normal and fake outgoing arcs
   Block b1( 0 );
   b1.m_fake = true;
   Arc a1_1( 1, 0, 1 );
   Arc a1_2( 1, 0, 3 );
   Arc a1_3( 1, 0, 5 );
   a1_2.m_count = 1; 
   a1_3.m_count = 1;
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b1.m_arcs.push_back( a1_3 );

   // Case 2 fake block has a taken fake outgoing arc
   Block b2( 0 );
   b2.m_fake = true;
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   Arc a2_3( 1, 0, 5 );
   a2_2.m_count = 1; 
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Case 3 fake block has a taken normal outgoing arcs
   // but fake arc is untaken
   Block b3( 0 );
   b3.m_fake = true;
   Arc a3_1( 1, 0, 1 );
   Arc a3_2( 1, 0, 3 );
   Arc a3_3( 1, 0, 5 );
   a3_1.m_count = 1;
   a3_3.m_count = 1;
   b3.m_arcs.push_back( a3_1 );
   b3.m_arcs.push_back( a3_2 );
   b3.m_arcs.push_back( a3_3 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b0.has_full_coverage() );
   EXPECT_FALSE( b1.has_full_coverage() );
   EXPECT_FALSE( b2.has_full_coverage() );
   EXPECT_FALSE( b3.has_full_coverage() );
}

TEST( has_full_coverage_test, normal_full_coverage )
{
   // Case 0 normal block has one single taken normal arc
   Block b0( 0 );
   Arc a0( 1, 0, 1 );
   a0.m_count = 1;
   b0.m_arcs.push_back( a0 );

   // Case 1 normal block has one single taken normal arc
   // But an untaken fake arc as well
   Block b1( 0 );
   Arc a1_1( 1, 0, 1 );
   Arc a1_2( 1, 0, 2 );
   a1_1.m_count = 1;
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );

   // Case 2 normal block has all normal arcs taken
   // but an untaken fake arc as well
   Block b2( 0 );
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 2 );
   Arc a2_3( 2, 1, 4 );
   a2_1.m_count = 1;
   a2_3.m_count = 1;
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Case 3 normal block has only a single taken outgoing fake arc
   Block b3( 0 );
   Arc a3( 1, 0, 2 );
   a3.m_count = 1;
   b3.m_arcs.push_back( a3 );

   // Verify that all cases return with true
   EXPECT_TRUE( b0.has_full_coverage() );
   EXPECT_TRUE( b1.has_full_coverage() );
   EXPECT_TRUE( b2.has_full_coverage() );
   EXPECT_TRUE( b3.has_full_coverage() );
}

TEST( has_full_coverage_test, fake_full_coverage )
{
   // Case 0 fake block has one single taken normal arc
   Block b0( 0 );
   b0.m_fake = true;
   Arc a0( 1, 0, 1 );
   a0.m_count = 1;
   b0.m_arcs.push_back( a0 );

   // Case 1 fake block has one single taken fake arc
   Block b1( 0 );
   b1.m_fake = true;
   Arc a1( 1, 0, 2 );
   a1.m_count = 1;
   b1.m_arcs.push_back( a1 );

   // Case 2 normal block has all normal arcs taken
   // and a taken fake arc as well
   Block b2( 0 );
   b2.m_fake = true;
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 2 );
   Arc a2_3( 2, 1, 4 );
   a2_1.m_count = 1;
   a2_2.m_count = 1;
   a2_3.m_count = 1;
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Verify that all cases return with true
   EXPECT_TRUE( b0.has_full_coverage() );
   EXPECT_TRUE( b1.has_full_coverage() );
   EXPECT_TRUE( b2.has_full_coverage() );
}

//////////////////////////////////////////////////////////////////////////////
// has_partial_coverage()
//////////////////////////////////////////////////////////////////////////////

TEST( has_partial_coverage, normal_no_coverage )
{
   // Initialize

   // Case 1 normal block has only a single outgoing fake arc
   Block b1( 0 );
   Arc a1( 1, 0, 2 );
   b1.m_arcs.push_back( a1 );

   // Case 2 normal block has only a single outgoing normal arc
   Block b2( 0 );
   Arc a2( 1, 0, 1 );
   b2.m_arcs.push_back( a2 );

   // Case 3 normal block has outgoing fake and normal arcs
   Block b3( 0 );
   Arc a3_1( 1, 0, 1 );
   Arc a3_2( 1, 0, 3 );
   b3.m_arcs.push_back( a3_1 );
   b3.m_arcs.push_back( a3_2 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b1.has_partial_coverage() );
   EXPECT_FALSE( b2.has_partial_coverage() );
   EXPECT_FALSE( b3.has_partial_coverage() );
}

TEST( has_partial_coverage_test, fake_no_coverage )
{
   // Case 0 fake block has only a single outgoing fake arc
   Block b0( 0 );
   b0.m_fake = true;
   Arc a0( 1, 0, 2 );
   b0.m_arcs.push_back( a0 );

   // Case 1 fake block has only a single outgoing normal arc
   Block b1( 0 );
   b1.m_fake = true;
   Arc a1( 1, 0, 1 );
   b1.m_arcs.push_back( a1 );

   // Case 2 fake block has both normal and fake outoing arcs
   Block b2( 0 );
   b2.m_fake = true;
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );

   // Verify all cases return w/ false
   EXPECT_FALSE( b0.has_partial_coverage() );
   EXPECT_FALSE( b1.has_partial_coverage() );
   EXPECT_FALSE( b2.has_partial_coverage() );
}

TEST( has_partial_coverage_test, normal_partial_coverage )
{
   // Initialize

   // Case 0 normal block has a taken outgoing normal arc
   Block b0( 0 );
   Arc a0_1( 1, 0, 1 );
   Arc a0_2( 1, 0, 3 );
   Arc a0_3( 1, 0, 5 );
   a0_3.m_count = 1;
   b0.m_arcs.push_back( a0_1 );
   b0.m_arcs.push_back( a0_2 );
   b0.m_arcs.push_back( a0_3 );

   // Case 1 normal block has a taken normal and fake outgoing arcs
   Block b1( 0 );
   Arc a1_1( 1, 0, 1 );
   Arc a1_2( 1, 0, 3 );
   Arc a1_3( 1, 0, 5 );
   a1_2.m_count = 1; 
   a1_3.m_count = 1;
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b1.m_arcs.push_back( a1_3 );

   // Case 2 normal block has a taken fake outgoing arc
   Block b2( 0 );
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   Arc a2_3( 1, 0, 5 );
   a2_2.m_count = 1; 
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Verify all cases
   EXPECT_TRUE( b0.has_partial_coverage() );
   EXPECT_TRUE( b1.has_partial_coverage() );
   EXPECT_FALSE( b2.has_partial_coverage() );
}

TEST( has_partial_coverage_test, fake_partial_coverage )
{
   // Initialize

   // Case 0 fake block has a taken outgoing normal arc
   Block b0( 0 );
   b0.m_fake = true;
   Arc a0_1( 1, 0, 1 );
   Arc a0_2( 1, 0, 3 );
   Arc a0_3( 1, 0, 5 );
   a0_3.m_count = 1;
   b0.m_arcs.push_back( a0_1 );
   b0.m_arcs.push_back( a0_2 );
   b0.m_arcs.push_back( a0_3 );

   // Case 1 fake block has a taken normal and fake outgoing arcs
   Block b1( 0 );
   b1.m_fake = true;
   Arc a1_1( 1, 0, 1 );
   Arc a1_2( 1, 0, 3 );
   Arc a1_3( 1, 0, 5 );
   a1_2.m_count = 1; 
   a1_3.m_count = 1;
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b1.m_arcs.push_back( a1_3 );

   // Case 2 fake block has a taken fake outgoing arc
   Block b2( 0 );
   b2.m_fake = true;
   Arc a2_1( 1, 0, 1 );
   Arc a2_2( 1, 0, 3 );
   Arc a2_3( 1, 0, 5 );
   a2_2.m_count = 1; 
   b2.m_arcs.push_back( a2_1 );
   b2.m_arcs.push_back( a2_2 );
   b2.m_arcs.push_back( a2_3 );

   // Verify all cases return w/ true
   EXPECT_TRUE( b0.has_partial_coverage() );
   EXPECT_TRUE( b1.has_partial_coverage() );
   EXPECT_TRUE( b2.has_partial_coverage() );
}

//////////////////////////////////////////////////////////////////////////////
// get_block_no()
//////////////////////////////////////////////////////////////////////////////

TEST( get_block_no_test, getter )
{
   // Initialize
   Block b( 2 );

   EXPECT_EQ( 2, b.get_block_no() );
}

//////////////////////////////////////////////////////////////////////////////
// get_arcs()
//////////////////////////////////////////////////////////////////////////////

TEST( get_arcs_test, getter )
{
   // Initialize
   Block b( 0 );
   Arc a1( 1, 0, 1 );
   Arc a2( 2, 1, 4 );
   a1.m_count = 1;
   a2.m_count = 2;
   b.m_arcs.push_back( a1 );
   b.m_arcs.push_back( a2 );

   const std::vector<Arc> & arcs = b.get_arcs();

   EXPECT_EQ( 1, arcs[0].m_dest_block );
   EXPECT_EQ( 0, arcs[0].m_origin_block );
   EXPECT_EQ( 1, arcs[0].m_flag );
   EXPECT_EQ( 1, arcs[0].m_count );
   EXPECT_EQ( 2, arcs[1].m_dest_block );
   EXPECT_EQ( 1, arcs[1].m_origin_block );
   EXPECT_EQ( 4, arcs[1].m_flag );
   EXPECT_EQ( 2, arcs[1].m_count );
}

//////////////////////////////////////////////////////////////////////////////
// get_line_data()
//////////////////////////////////////////////////////////////////////////////

TEST( get_line_data_test, getter )
{
   Block b( 1 );
   Line l1;
   Line l2;
   l1.m_line_num = 5;
   l2.m_line_num = 6;
   b.m_lines[ "test" ].m_lines.push_back( l1 );
   b.m_lines[ "test" ].m_lines.push_back( l2 );

   const std::map<std::string, Lines_data> & lines = b.get_lines();

   EXPECT_EQ( 1, lines.size() );
}

//////////////////////////////////////////////////////////////////////////////
// get_branch_arc_taken()
//////////////////////////////////////////////////////////////////////////////

TEST( get_branch_arc_taken_test, getter )
{
   // Initialize
   Block b0( 0 );
   Arc a0( 1, 0, 0 );
   Arc a1( 2, 0, 1 );
   Arc a2( 4, 0, 4 );
   Arc a3( 5, 0, 3 );
   a0.m_count = 1;
   a1.m_count = 0;
   a2.m_count = 2;
   a3.m_count = 0;
   b0.m_arcs.push_back( a0 );
   b0.m_arcs.push_back( a1 );
   b0.m_arcs.push_back( a2 );
   b0.m_arcs.push_back( a3 );

   Block b1( 1 );

   EXPECT_EQ( 2, b0.get_branch_arc_taken() );
   EXPECT_EQ( 0, b1.get_branch_arc_taken() );
}

//////////////////////////////////////////////////////////////////////////////
// get_branch_arc_total()
//////////////////////////////////////////////////////////////////////////////

TEST( get_branch_arc_total_test, getter )
{
   Block b0( 0 );
   Arc a0( 1, 0, 0 );
   Arc a1( 2, 0, 1 );
   Arc a2( 4, 0, 4 );
   Arc a3( 5, 0, 3 );
   a0.m_count = 1;
   a1.m_count = 0;
   a2.m_count = 2;
   a3.m_count = 0;
   b0.m_arcs.push_back( a0 );
   b0.m_arcs.push_back( a1 );
   b0.m_arcs.push_back( a2 );
   b0.m_arcs.push_back( a3 );

   Block b1( 1 );

   EXPECT_EQ( 3, b0.get_branch_arc_total() );
   EXPECT_EQ( 0, b1.get_branch_arc_total() );
}

//////////////////////////////////////////////////////////////////////////////
// get_count()
//////////////////////////////////////////////////////////////////////////////

TEST( get_block_count_test, getter )
{
   // Case 1: Non start/end block
   Block b( 1 );
   Arc a0( 1, 0, 0 );
   Arc a1( 2, 0, 1 );
   Arc a2( 4, 0, 4 );
   Arc a3( 5, 0, 3 );
   a0.m_count = 1;
   a1.m_count = 0;
   a2.m_count = 2;
   a3.m_count = 2;
   b.m_from_arcs.push_back( &a0 );
   b.m_from_arcs.push_back( &a1 );
   b.m_from_arcs.push_back( &a2 );
   b.m_from_arcs.push_back( &a3 );
   b.m_arcs.push_back( a0 );
   b.m_arcs.push_back( a1 );
   b.m_arcs.push_back( a2 );
   b.m_arcs.push_back( a3 );

   // Case 2: Start block
   Block b2( 2 );
   Arc a4( 3, 0, 1);
   a4.m_count = 3;
   b2.m_arcs.push_back( a4 );

   // Case 3: End block
   Block b3( 3 );
   Arc a5( 5, 0, 5);
   a5.m_count = 5;
   b3.m_from_arcs.push_back( &a5 );

   //EXPECT_EQ( 4, b.m_from_arcs.size() );
   EXPECT_EQ( 5, b.get_count() );
   EXPECT_EQ( 0, b2.get_count() );
   EXPECT_EQ( 0, b3.get_count() );
}

//////////////////////////////////////////////////////////////////////////////
// get_non_inlined()
//////////////////////////////////////////////////////////////////////////////

TEST( get_non_inlined_test, getter )
{
   Block b1( 1 );

   Line l1;
   l1.m_line_num = 12;
   l1.m_inlined = false;
   Line l2;
   l2.m_line_num = 14;
   l2.m_inlined = false;

   b1.m_non_inlined.push_back( l1 );
   b1.m_non_inlined.push_back( l2 );

   const std::vector<Line> & non_inlined = b1.get_non_inlined();

   EXPECT_FALSE( non_inlined.empty() );
   EXPECT_EQ( 2, non_inlined.size() );
   EXPECT_EQ( 12, non_inlined[0].m_line_num );
   EXPECT_EQ( 14, non_inlined[1].m_line_num );
}

//////////////////////////////////////////////////////////////////////////////
// get_HTML_name()
//////////////////////////////////////////////////////////////////////////////

TEST( get_HTML_name_test, getter )
{
   // Initialize
   Record r;
   r.m_name_demangled = "This \"test\" > your test & your test < this \"test\"";

   EXPECT_STREQ( "This &quot;test&quot; &gt; your test &amp; your test &lt; this &quot;test&quot;", r.get_HTML_name().c_str() );
}

//////////////////////////////////////////////////////////////////////////////
// get_function_arc_taken()
//////////////////////////////////////////////////////////////////////////////

TEST( get_function_arc_taken_test, getter )
{
   // Initialize
   Record r;

   Block b0( 0 );
   Block b1( 1 );
   Block b2( 2 );
   Block b3( 3 );
   Block b4( 4 );
   Block b5( 5 );
   Block b6( 6 );

   Arc a0_0( 1, 0, 1 );
   Arc a1_0( 2, 1, 1 );
   Arc a1_1( 3, 1, 4 );
   Arc a1_2( 4, 1, 2 );
   Arc a2_0( 6, 2, 4 );
   Arc a3_0( 6, 3, 4 );
   Arc a4_0( 6, 4, 1 );
   Arc a4_1( 5, 4, 4 );
   Arc a5_0( 6, 5, 4 );

   a0_0.m_count = 2;
   a1_0.m_count = 1;
   a1_1.m_count = 1;
   a1_2.m_count = 0;
   a2_0.m_count = 1;
   a3_0.m_count = 1;
   a4_0.m_count = 0;
   a4_1.m_count = 0;
   a5_0.m_count = 0;

   b0.m_arcs.push_back( a0_0 );
   b1.m_arcs.push_back( a1_0 );
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b2.m_arcs.push_back( a2_0 );
   b3.m_arcs.push_back( a3_0 );
   b4.m_arcs.push_back( a4_0 );
   b4.m_arcs.push_back( a4_1 );
   b5.m_arcs.push_back( a5_0 );
   b4.m_fake = true;

   r.m_blocks.push_back( b0 );
   r.m_blocks.push_back( b1 );
   r.m_blocks.push_back( b2 );
   r.m_blocks.push_back( b3 );
   r.m_blocks.push_back( b4 );
   r.m_blocks.push_back( b5 );
   r.m_blocks.push_back( b6 );

   EXPECT_EQ( 2, r.get_function_arc_taken() );
}

//////////////////////////////////////////////////////////////////////////////
// get_function_arc_total()
//////////////////////////////////////////////////////////////////////////////

TEST( get_function_arc_total_test, getter )
{
   // Initialize
   Record r;

   Block b0( 0 );
   Block b1( 1 );
   Block b2( 2 );
   Block b3( 3 );
   Block b4( 4 );
   Block b5( 5 );
   Block b6( 6 );

   Arc a0_0( 1, 0, 1 );
   Arc a1_0( 2, 1, 1 );
   Arc a1_1( 3, 1, 4 );
   Arc a1_2( 4, 1, 2 );
   Arc a2_0( 6, 2, 4 );
   Arc a3_0( 6, 3, 4 );
   Arc a4_0( 6, 4, 1 );
   Arc a4_1( 5, 4, 4 );
   Arc a5_0( 6, 5, 4 );

   a0_0.m_count = 2;
   a1_0.m_count = 1;
   a1_1.m_count = 1;
   a1_2.m_count = 0;
   a2_0.m_count = 1;
   a3_0.m_count = 1;
   a4_0.m_count = 0;
   a4_1.m_count = 0;
   a5_0.m_count = 0;

   b0.m_arcs.push_back( a0_0 );
   b1.m_arcs.push_back( a1_0 );
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_2 );
   b2.m_arcs.push_back( a2_0 );
   b3.m_arcs.push_back( a3_0 );
   b4.m_arcs.push_back( a4_0 );
   b4.m_arcs.push_back( a4_1 );
   b5.m_arcs.push_back( a5_0 );
   b4.m_fake = true;

   r.m_blocks.push_back( b0 );
   r.m_blocks.push_back( b1 );
   r.m_blocks.push_back( b2 );
   r.m_blocks.push_back( b3 );
   r.m_blocks.push_back( b4 );
   r.m_blocks.push_back( b5 );
   r.m_blocks.push_back( b6 );

   EXPECT_EQ( 2, r.get_function_arc_total() );
}

//////////////////////////////////////////////////////////////////////////////
// get_execution_count()
//////////////////////////////////////////////////////////////////////////////

TEST( get_execution_count, getter )
{
   // Initialize
   Record r;
   Block b( 0 );
   Arc a0( 1, 0, 1 );
   Arc a1( 1, 0, 3 );
   Arc a2( 1, 0, 5 );
   a0.m_count = 2;
   a1.m_count = 3;
   a2.m_count = 0;
   b.m_arcs.push_back( a0 );
   b.m_arcs.push_back( a1 );
   b.m_arcs.push_back( a2 );
   r.m_blocks.push_back( b );

   EXPECT_EQ( 5, r.get_execution_count() );
}

//////////////////////////////////////////////////////////////////////////////
// get_coverage_percentage()
//////////////////////////////////////////////////////////////////////////////

TEST( get_coverage_percentage_test, getter )
{
   // Initialize
   Record r;
   Block b0( 0 );
   Block b1( 1 );
   Block b2( 2 );
   Block b3( 3 );
   Arc a0( 1, 0, 1 );
   Arc a1_0( 2, 1, 4 );
   Arc a1_1( 3, 1, 5 );
   Arc a1_2( 3, 1, 3 );
   Arc a2( 3, 2, 1 );
   a0.m_count = 1;
   a1_0.m_count = 1;
   a1_1.m_count = 0;
   a1_2.m_count = 0;
   a2.m_count = 0;
   b0.m_arcs.push_back( a0 );
   b1.m_arcs.push_back( a1_0 );
   b1.m_arcs.push_back( a1_1 );
   b1.m_arcs.push_back( a1_1 );
   b2.m_arcs.push_back( a2 );
   r.m_blocks.push_back( b0 );
   r.m_blocks.push_back( b1 );
   r.m_blocks.push_back( b2 );
   r.m_blocks.push_back( b3 );

   EXPECT_DOUBLE_EQ( 0.333, 
      (double)(int)(r.get_coverage_percentage() * 1000) / 1000 );
}

TEST( get_coverage_percentage_test, getter_no_branch_no_coverage )
{
   // Initialize
   Record r;
   Block b0( 0 );
   Block b1( 1 );
   Block b2( 2 );

   Arc a0( 1, 0, 1 );
   Arc a1( 2, 1, 4 );

   a0.m_count = 0;
   a1.m_count = 0;
   b0.m_arcs.push_back( a0 );
   b1.m_arcs.push_back( a1 );
   r.m_blocks.push_back( b0 );
   r.m_blocks.push_back( b1 );
   r.m_blocks.push_back( b2 );

   EXPECT_DOUBLE_EQ( 0.000, 
      (double)(int)(r.get_coverage_percentage() * 1000) / 1000 );
}

TEST( get_coverage_percentage_test, getter_no_branch_full_coverage )
{
   // Initialize
   Record r;
   Block b0( 0 );
   Block b1( 1 );
   Block b2( 2 );

   Arc a0( 1, 0, 1 );
   Arc a1( 2, 1, 4 );

   a0.m_count = 1;
   a1.m_count = 1;
   b0.m_arcs.push_back( a0 );
   b1.m_arcs.push_back( a1 );
   r.m_blocks.push_back( b0 );
   r.m_blocks.push_back( b1 );
   r.m_blocks.push_back( b2 );

   EXPECT_DOUBLE_EQ( 1.000, 
      (double)(int)(r.get_coverage_percentage() * 1000) / 1000 );
}

//////////////////////////////////////////////////////////////////////////////
// record_line_lessthan()
//////////////////////////////////////////////////////////////////////////////

TEST( record_line_lessthan, greater_than )
{
   Record r1;
   Record r2;

   r1.m_line_num = 56;
   r2.m_line_num = 42;

   EXPECT_FALSE( record_line_lessthan( &r1, &r2 ) );
}

TEST( record_line_lessthan, less_than )
{
   Record r1;
   Record r2;

   r1.m_line_num = 42;
   r2.m_line_num = 56;

   EXPECT_TRUE( record_line_lessthan( &r1, &r2 ) );
}

//////////////////////////////////////////////////////////////////////////////
// record_name_lessthan()
//////////////////////////////////////////////////////////////////////////////

TEST( record_name_lessthan, greater_than )
{
   Record r1;
   Record r2;

   r1.m_name_demangled = "Tyranosaurus";
   r2.m_name_demangled = "Brachiosaurus";

   EXPECT_FALSE( record_name_lessthan( &r1, &r2 ) );
}

TEST( record_name_lessthan, less_than )
{
   Record r1;
   Record r2;

   r1.m_name_demangled = "Brachiosaurus";
   r2.m_name_demangled = "Tyranosaurus";

   EXPECT_TRUE( record_name_lessthan( &r1, &r2 ) );
}
