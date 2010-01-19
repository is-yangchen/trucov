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
///  @file parser.h
///
///  @brief
///  Prototype GCNO/GCDA Parser
///
///  @remarks
///  Defines class Parser
///
///  Requirements Specification: < Null >
///
///  Design Description: < Null >
///
///////////////////////////////////////////////////////////////////////////////
#ifndef PARSER_H
#define PARSER_H

// SYSTEM INCLUDES

#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <vector>
#include <map>

// PROJECT INCLUDES

#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/parse_tree.hpp>

// LOCAL INCLUDES

#include "parser_builder.h"
#include "gcno_grammar.h"
#include "gcda_grammar.h"
#include "record.h"
#include "config.h"

// DEFINTIIONS

///////////////////////////////////////////////////////////////////////////////
///  @class Parser
///
///  @brief
///  Parser gcno and gcda files
///
///  @remarks
///  Class takes a name of a string when constructed, and then parses that
///  file when the parse method is called
///////////////////////////////////////////////////////////////////////////////
class Parser
{
public:

   // PUBLIC METHODS

	/// Destructor
	///
   /// @brief
   /// Clean up the instance memory
   ~Parser()
   {
      if (ptr_instance != NULL)
      {
         delete ptr_instance;
      }
   }

   /// @brief
   /// Returns the single instance of this singleton.
   ///
   /// @return Returns a reference to the singleton Parser object
   static Parser & get_instance()
   {
      // Create instance is non existant
      if ( Parser::ptr_instance == NULL )
      {
         ptr_instance = new Parser();
      }

      return *ptr_instance;
   } // end of get_instance()

   ///////////////////////////////////////////////////////////////////////////
   /// Constructor with input file
   ///
   /// @param a an integer argument
   ///
   /// @remarks
   /// Constructs the Parser object and opens the input file given
   ///////////////////////////////////////////////////////////////////////////
   Parser(const char * gcnoFile, const char * gcdaFile);

   ///////////////////////////////////////////////////////////////////////////
   ///
   ///  @return success(0), failure(-1)
   ///
   ///  @remarks
   ///  Parses the entire input file
   ///  Pre  : infile points to a valid input file stream
   ///  Post : Parse Tree is constructed and all data on the program and test
   ///         coverage has been gathered/calculated.
   ///  Throw: IOException
   ////////////////////////////////////////////////////////////////////////////
   int parse();

   int parse(
       const string & gcnoFile, 
       const string & gcdaFile,
       const string & dumpFile );

   int parse(
       const string & gcnoFile, 
       const string & gcdaFile );

   void parse_all(); 

   const map<string, Source_file> & get_source_files();

   const map<unsigned int, Record> & get_records();

   // PUBLIC VARAIBLES

	static const int TagLength = 4;

private:

   // PRIVATE METHODS

   /// @brief
   /// Constructs the Parser object, and initializes private data fields.
   ///
   /// @remarks
   /// Private default constructor required for singleton.
   explicit Parser()
   {
      // void
   }

   /// @brief
   /// Constructs this object to be exactly the same as the source.
   ///
   /// @remarks
   /// Private copy constructor required for singleton.
   explicit Parser( const Parser & source )
   {
      // void
   }

   /// @brief
   /// Assigns this objects state to be the same as the source object.
   ///
   /// @remarks
   /// Private copy constructor required for singleton.
   Parser & operator=( const Parser & source )
   {
      return *this;
   }

   /// Populate each Block's m_from_arcs with incoming Arc pointers
   void assign_entry_arcs();
   /// Assign each counted Arc a value
   void assign_arc_counts();
   /// Calculate the traversal count of each uncounted Arc
   void normalize_arcs();
   /// Populate the Lines data for all blocks without line information
   void normalize_lines();
   /// Calculate whether each block is fake
   void normalize_fake_blocks();
   /// Populate m_blocks_sorted with Blocks sorted by line #
   void sort_blocks();

   // PRIVATE VARAIBLES

   ifstream    mGcnoFile;                 // Gcno file stream
   ifstream    mGcdaFile;                 // Gcda file stream
   ofstream    mDumpFile;
   bool        mIsDump;
   string      mMagic;                    // Magic
   string      mVersion;                  // Version of the input file
   string      mStamp;                    // Stamp of the file

   bool        mIsGCNO;
   bool        mIsLittleEndian;

   // Pointer to the singleton instance of Dot_creator.
   static Parser * ptr_instance;

   map<std::string, Source_file> m_source_files;

}; // end of class Parse

/// Compares the last line numbers of two blocks to determine order
const bool compare_line_nums( const Block & lhs, const Block & rhs );

#endif
