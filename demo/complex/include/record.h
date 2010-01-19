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
///  @file record.h
///
///  @brief
///  Stores data from GCNO file parsing
///
///  @remarks
///  Defines class Record
///////////////////////////////////////////////////////////////////////////////

#ifndef RECORD_H
#define RECORD_H

//  SYSTEM INCLUDES
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdint.h>

//  TYPE DEFINITIONS

/// @brief
/// Contains the name of the source file, the function block number
/// and all lines associated with that block
class Lines_data
{
public:

   // PUBLIC METHODS

   /// @brief
   /// Returns the line numbers of a function block
   const std::vector<unsigned> & get_line_nums() const;

   /// @brief
   /// Returns the source file path and the associated line numbers
   const std::string & get_source() const;

   /// @brief
   /// Returns the block number associated with the Lines_data object
   const unsigned get_block() const;

   const std::map< std::string, std::vector<unsigned> > & get_lines() const;

   // FRIEND CLASSES

   // Block class must add data to Lines_data class as GCNO file is being parsed
   friend class Block;
   // Parser class must be able to normalize data within Lines_data class
   friend class Parser;
   // Parser_builder class mst be able to add lines and sources to Lines_data
   friend class Parser_builder;

private:

   // PRIVATE MEMBERS

   // The source file that the line numbers refer to
   std::string m_source;
   // The function block number associated with the line numbers
   unsigned m_block;
   // The function block's line numbers
   std::vector<unsigned> m_lines;
   // The lines associated with each source file
   std::map< std::string, std::vector<unsigned> > m_source_lines;
};

/// @brief
/// Contains the destination block, arc flag, and a count of how
/// many times that arc has been traversed
class Arc
{
public:

   // PUBLIC METHODS

   /// @brief
   /// Arc object constructor
   ///
   /// @param dest  The Arc's destination block number
   /// @param flag  The Arc's flag value
   Arc( const unsigned dest, const unsigned origin, const unsigned flag )
      : m_dest_block( dest ), m_origin_block( origin ), m_flag( flag ),
        m_count( -1 )
   {
   }

   /// @brief
   /// Returns whether or not an Arc is fake
   const bool is_fake() const;

   /// @brief
   /// Returns whether or not an Arc has been traversed
   const bool is_taken() const;

   /// @brief
   /// Returns an Arc's traversal count
   const int64_t get_count() const;

   /// @brief
   /// Returns an Arc's destination block number
   const unsigned get_dest() const;

   /// @brief
   /// Returns an Arc's flag value
   const unsigned get_flag() const;

   // FRIEND CLASSES

   // Parser needs to be a friend class for arc normalization function
   friend class Parser;

private:

   // PRIVATE MEMBERS

   // The Arc's destination block number
   unsigned m_dest_block;
   // The Arc's origin block number
   unsigned m_origin_block;
   // The Arc's flag value
   unsigned m_flag;
   // The Arc's traversal count
   int64_t  m_count;
};

/// @brief
/// Contains the block number, arcs leaving the block, pointers
/// to arcs entering the block, data about the function block's
/// associated line numbers, and a bool indicating if the arc
/// count for that block has been normalized
class Block
{
public:

   // PUBLIC METHODS

   /// @brief
   /// Block object constructor
   ///
   /// @param block_no  The Block's number
   Block( const unsigned block_no ) : m_block_no( block_no ),
      m_normalized( false ), m_fake( false ), m_inline( false )
   {
   }

   /// @brief
   /// Returns whether or not a block has no outgoing arcs
   const bool is_end_block() const;

   /// @brief
   /// Returns whether or not a block has no incoming arcs
   const bool is_start_block() const;

   /// @brief
   /// Returns whether or not a block is a branch
   const bool is_branch() const;

   /// @brief
   /// Returns whether or not a block is a fake block
   const bool is_fake() const;

   /// @brief
   /// Returns whether or not a block has been normalized
   const bool is_normalized() const;

   /// @brief
   /// Returns whether or not a block has had all of its arcs traversed
   const bool has_full_coverage() const;

   /// @brief
   /// Returns whether or not a block has had some of its arcs traversed
   const bool has_partial_coverage() const;

   /// @brief
   /// Returns whether or not a block has associated line numbers
   const bool has_lines() const;

   /// @brief
   /// Returns a function block's number
   const unsigned get_block_no() const;

   /// @brief
   /// Returns a function block's outgoing Arcs
   const std::vector<Arc> & get_arcs() const;

   /// @brief
   /// Returns a function block's associated line number data
   const Lines_data & get_line_data() const;

   /// @brief
   /// Returns the total number of taken arcs in a branch
   const unsigned get_branch_arc_taken() const;

   /// @brief
   /// Returns the total number of arcs in a branch
   const unsigned get_branch_arc_total() const;

   /// @brief
   /// Returns the number of times a block has been executed
   const int64_t get_count() const;

   // FRIEND CLASSES

   // Parser class must access Lines_data and Arc classes for normalization
   friend class Parser;
   // Parser_builder must access Block class for access to Lines_data member
   friend class Parser_builder;

private:

   // PRIVATE MEMBERS

   /// The function block's outgoing Arcs
   std::vector<Arc>  m_arcs;
   /// The function block's incoming Arcs
   std::vector<Arc*> m_from_arcs;
   /// The function block's line number information
   Lines_data m_lines;
   /// The function block's number
   unsigned m_block_no;
   /// The normalization status of a function block
   bool m_normalized;
   /// The fake status of a function block
   bool m_fake;
   /// The inline status of a function block
   bool m_inline;
};

///  @brief
///  Stores gcno and gcda file data
///
///  @remarks
///  Class is instantiated for every instance of a function. Data members
///  are assigned values within selGcnoActors.h and selGcdaActors.h
class Record
{
   public:

   // PUBLIC METHODS

   /// Returns the function signature in HTML friendly style
   const std::string get_HTML_name() const;

   /// Returns the total number of taken arcs in the branches of a function
   const unsigned get_function_arc_taken() const;

   /// Returns the total number of arcs in the branches of a function
   const unsigned get_function_arc_total() const;

   /// Returns the total number of times a function has been executed
   const uint64_t get_execution_count() const;

   /// Returns the coverage percentage of the function
   const double get_coverage_percentage() const;

   // PUBLIC VARIABLES

   /// The record's ident
   unsigned m_ident;
   /// The record's checksum
   unsigned m_checksum;
   /// The record's mangled signature
   std::string m_name;
   /// The record's demangled signature
   std::string m_name_demangled;
   /// The record's source file name
   std::string m_source;
   /// The record's function blocks
   std::vector<Block> m_blocks;
   /// The GCDA parsed arc counts
   std::vector<uint64_t> m_counts; 
   /// A function's blocks, sorted by their line numbers
   std::vector<Block> m_blocks_sorted;

};

#endif // RECORD_H
