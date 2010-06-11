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
///  @file parser.cpp
///
///  @brief
///  Implements the methods of the Parser Class.
///
///  @remarks
///  Defines all methods of the Parser class.
///////////////////////////////////////////////////////////////////////////////

// HEADER FILE

#include "parser.h"

//  STATIC FIELD INITIALIZATION

// Null value means the object has not been instantiated yet.
Parser * Parser::ptr_instance = NULL;

// Tells the spirit parser if the current file is big endian or little endian.
bool global_little_endian = false;

unsigned int global_parsed_int32 = 0;
uint64_t global_parsed_int64 = 0;
std::string global_parsed_string = "";

//  USING STATMENTS

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::exception;

namespace SP = boost::spirit;

//  METHOD DEFINITIONS

void Parser::parse_all()
{
   Tru_utility* sys_utility = Tru_utility::get_instance();
   vector< pair<string, string> > build_list;
   vector< pair<string, string> >::iterator itr;
   pair<string, string> tmp_pair;

   Config & config = Config::get_instance();
   build_list = config.get_build_files();
   itr = build_list.begin();

   string dump_file;
   int result = 0;

   cout << "Parsing gcno and gcda files ." << flush;

   // Iterates through all pairs of gcno and gcda files and
   // parse each
   for(itr = build_list.begin(); itr != build_list.end(); itr++)
   {
      cout << "." << flush;

      // If dumping parser data
      if (config.get_debug_flag())
      {
         // Generate dump file
         dump_file = "selcov_dump_" +
         sys_utility->genSelcovFileName(itr->first, ".dump");
         dump_file = config.get_out_dir() + dump_file;

         result = parse( itr->first, itr->second, dump_file );
      }
      else // No dump file
      {
         result = parse( itr->first, itr->second );
      }
   }
   cout << endl;

   // For each arc with data from the GCDA data file, assign
   // that arc's traversal count
   assign_arc_counts();

   // For each block, populate a vector with pointers to
   // the arcs entering that block
   assign_entry_arcs();

   // Calculate and assign the traversal count for all arcs
   // not assigned a count from the GCDA data file
   normalize_arcs();

   // Assign the last line of the parent block to any blocks
   // that have no line number assigned
   normalize_lines();

   // For each functions block, determine whether the block
   // is fake
   normalize_fake_blocks();

   // Populate m_blocks_sorted with Blocks sorted by line #
   // for use later by Coverage creator class
   sort_blocks();

} // End of Parser::parse_all(...)

////////////////////////////////////////////////////////////////////////////////
///  @brief
///  Sets up GCNO and GCDA files for parsing, calls parse function, closes files
///
///  INPUTS:
///  @param gcnoFile The string path of the gcno file
///  @param gcdaFile The string path of the gcda file
///
///  @return success(0), or failure(1)
////////////////////////////////////////////////////////////////////////////////
int Parser::parse(
    const string & gcnoFile, 
    const string & gcdaFile )
{
    mGcnoFile.open(gcnoFile.c_str());
    mGcdaFile.open(gcdaFile.c_str());

    if (mGcnoFile.fail())
    {
        cout << "Failed to open files" << endl;
        return 1;
    }

    mIsDump = false;

    int r = parse();

    mGcnoFile.close();
    mGcdaFile.close();

    return r;

} // end of Parser::parse(const string &, const string &)

////////////////////////////////////////////////////////////////////////////////
///  @brief
///  Sets up GCNO and GCDA files for parsing, calls parse function, closes files
///  Dumps output to dump file
///
///  INPUTS:
///  @param gcnoFile The string path of the gcno file
///  @param gcdaFile The string path of the gcda file
///  @param dumpFile The string path of the dump file
///
///  @return int success(0), or failure(1)
////////////////////////////////////////////////////////////////////////////////
int Parser::parse(
    const string & gcnoFile, 
    const string & gcdaFile,
    const string & dumpFile )
{
    // Open files
    mGcnoFile.open(gcnoFile.c_str());
    mGcdaFile.open(gcdaFile.c_str());
    mDumpFile.open(dumpFile.c_str());
    if ( mGcnoFile.fail() || mDumpFile.fail() )
    {
        cerr << "ERROR: Cannot open gcno file " << gcnoFile << endl;
        return 1;
    }

    // Used with
    mIsDump = true;
    mDumpFile << "**************************************************\n"
              << "GCNO FILE: " << gcnoFile << "\n"
              << "GCDA FILE: " << gcdaFile << "\n";

    int r = parse();

    // Close files
    mGcnoFile.close();
    mGcdaFile.close();
    if (mIsDump)
    {    
        mDumpFile.close();
    }
    return r;

} // end of Parser::parse(const string &, const string &, const string &)

///////////////////////////////////////////////////////////////////////////
/// @brief
/// Parses the current gcno and gcda files pair and merges the coverage
/// data into the data structure.
///
/// @return success(true), failure(false)
///////////////////////////////////////////////////////////////////////////
int Parser::parse()
{
    try
    {
        char buf[4];
 
        // Get gcno magic
        mGcnoFile.read(buf, 4);
        unsigned int gcnoMagic = *( reinterpret_cast<unsigned int *>(buf) );

       // Get gcno file length
        mGcnoFile.seekg(0, ios::end);
        unsigned int gcnoLength = mGcnoFile.tellg();
        mGcnoFile.seekg(0, ios::beg);

        // Read in gcno file
        vector<char> gcno_buf(gcnoLength);
        mGcnoFile.read(&gcno_buf[0], gcnoLength);

        // Check for valid gcno magic
        bool gcno_little_endian = true;
        if (gcnoMagic == 0x6F6E6367)
        {
            // Set little endian
            gcno_little_endian = false;
        }
        else if (gcnoMagic != 0x67636E6F)
        {
            cerr << "Not a valid gcno file" << endl;
            return 1;
        }

        Config & config = Config::get_instance();
        Parser_builder parser_builder( m_source_files,
            config.get_rev_script(),
            config.get_source_files() );
       
        // Parse the gcno file
        Gcno_grammar gcnoGrammar( parser_builder, mIsDump, mDumpFile );
        global_little_endian = gcno_little_endian;
        const char * first = &gcno_buf[0];
        const char * last = &gcno_buf[0] + gcnoLength;
        SP::parse_info<> info = raw_parse(first, last, gcnoGrammar);

        if (!info.full)
        {
            cerr << "Spirit Parser GCNO Failed" << endl;
            return 1;
        }

        if ( !mGcdaFile.fail() )
        {

            // Get gcda magic
            mGcdaFile.read(buf, 4);
            unsigned int gcdaMagic = *( reinterpret_cast<unsigned int *>(buf) );

            // Get gcda file length
            mGcdaFile.seekg(0, ios::end);
            unsigned int gcdaLength = mGcdaFile.tellg();
            mGcdaFile.seekg(0, ios::beg);

            // Read in gcda file
            vector<char> gcda_buf(gcdaLength);
            mGcdaFile.read(&gcda_buf[0], gcdaLength);
     
            // Check for valid gcda magic
            bool gcda_little_endian = true;
            if (gcdaMagic == 0x61646367)
            {
                // Set little endian
                gcda_little_endian = false;
            }
            else if (gcdaMagic != 0x67636461)
            {
                cerr << "Not a valid gcda file" << endl;
            }
            
            // Parse the gcda file
            Gcda_grammar gcdaGrammar( parser_builder, mIsDump, mDumpFile );
            global_little_endian = gcda_little_endian;
            first = &gcda_buf[0];
            last = &gcda_buf[0] + gcdaLength;
            info = raw_parse( first, last, gcdaGrammar );

            if (!info.full)
            {
                cerr << "Spirit Parser GCDA Failed" << endl;
                return 1;
            }
        }
    }
    catch (exception e)
    {
        return 1;
    }

    return 0;

} // end of void Parse()

const map<string, Source_file> & Parser::get_source_files()
{
    return m_source_files;
}

//////////////////////////////////////////////////////////////////////////////
///  @brief
///  Assigns each block's fromArc vector pointers to any entry arcs
///
///  @remarks
///  Pre conditions: map of records is initialized, arc list is populated.
//////////////////////////////////////////////////////////////////////////////
void Parser::assign_entry_arcs()
{
   // For all source files
   for ( map<string, Source_file>::iterator source_iter = 
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   { 
      map<Source_file::Source_key, Record> & records = 
      source_iter->second.m_records;
 
      // For each record (function)
      for( map<Source_file::Source_key, Record>::iterator i = records.begin(); 
           i != records.end(); 
           ++i )
      {
         unsigned source = 0;

         // For each block within function
         for( unsigned j = 0; j < i->second.m_blocks.size(); ++j )
         {
            // For each arc
            for ( unsigned k = 0; k < i->second.m_blocks[j].m_arcs.size(); ++k )
            {
               // Assign destination block a pointer to the arc entering it
               unsigned dest = i->second.m_blocks[j].m_arcs[k].m_dest_block;
               i->second.m_blocks[dest].m_from_arcs.push_back( &(i->second.m_blocks[j].m_arcs[k]) );
            }
            ++source;
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Assigns each counted arc the count value read from the gcda file
///
///  @return void
///
///  @remarks
///  Pre  : Map of records is initialized, gcda file has been parsed
///  Post : none
///////////////////////////////////////////////////////////////////////////
void Parser::assign_arc_counts()
{
   // For all source files
   for ( map<string, Source_file>::iterator source_iter =
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   {
      map<Source_file::Source_key, Record> & records = 
         source_iter->second.m_records;

      // For each record (function)
      for ( map<Source_file::Source_key, Record>::iterator i = 
               records.begin(); 
            i != records.end(); 
            ++i )
      {
         unsigned pos = 0;

         // For each block
         for ( unsigned j = 0; j < i->second.m_blocks.size(); ++j )
         {
            // For each arc
            for ( unsigned k = 0; k < i->second.m_blocks[j].m_arcs.size(); ++k )
            {
               // Test if lowest bit is not set and if so then assign arc count
               if ( ! ( i->second.m_blocks[j].m_arcs[k].m_flag & 1 ) )
               {
                  i->second.m_blocks[j].m_arcs[k].m_count =
                     static_cast<int64_t>( i->second.m_counts.at( pos ) );
                  pos++;
               }
               else
               {
                  // Arcs without GCDA data are assigned -1 before they are normalized
                  i->second.m_blocks[j].m_arcs[k].m_count = -1;
               }
            }
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Normalizes the arc counts between counted and non-counted arcs
///
///  @return void
///
///  @remarks
///  Pre  : Map of records is initialized, gcda file has been parsed
///  Post : All arcs have an assigned traversal count
///////////////////////////////////////////////////////////////////////////
void Parser::normalize_arcs()
{
   bool normalized;

   // For all source files
   for ( map<string, Source_file>::iterator source_iter =
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   {
      map<Source_file::Source_key, Record> & records = 
         source_iter->second.m_records;

      // For each record (function)
      for ( map<Source_file::Source_key, Record>::iterator i = records.begin(); 
            i != records.end(); 
            ++i )
      {
         do {
            // Initialize normalized to false
            normalized = false;

            // For each function block starting with the 2nd to last block
            for ( int j = (int) i->second.m_blocks.size() - 2; j >= 1; --j )
            {
               // If block hasn't been normalized yet and block isn't start block
               if ( i->second.m_blocks[j].m_normalized == false )
               {
                  // Set arc count, arc location, and totals to 0
                  unsigned count = 0;
                  unsigned loc   = 0;
                  unsigned arcCountTotal     = 0;
                  unsigned fromArcCountTotal = 0;

                  // For each arc in block
                  for ( unsigned k = 0; k < i->second.m_blocks[j].m_arcs.size(); ++k )
                  {
                     // If arc hasn't been normalized
                     if ( i->second.m_blocks[j].m_arcs[k].m_count == -1 )
                     {
                        // Increment count and store arc number
                        ++count;
                        loc = k;
                     }
                     else
                     {
                        // Add arc's count to arc count total
                        arcCountTotal += i->second.m_blocks[j].m_arcs[k].m_count;
                     }
                  }

                  // For each arc entering the block
                  for ( unsigned k = 0; k < i->second.m_blocks[j].m_from_arcs.size(); ++k )
                  {
                     // If arc hasn't been normalized
                     if ( i->second.m_blocks[j].m_from_arcs[k]->m_count == -1 )
                     {
                        // Increment count and store arc number
                        ++count;
                        loc = k;
                     }
                     else
                     {
                        // Add entering arc's count to entering arc count total
                        fromArcCountTotal += i->second.m_blocks[j].m_from_arcs[k]->m_count;
                     }
                  }

                  // If block has no non-normalized arcs, block is normalized
                  if ( count == 0 )
                  {
                      i->second.m_blocks[j].m_normalized = true;
                      normalized = true;
                  }
                  // If block only has one non-normalized arc, calculate its count
                  else if ( count == 1 )
                  {
                     unsigned diff = 0;

                     // Take the difference from entering and exiting arcs' counts
                     if ( arcCountTotal >= fromArcCountTotal )
                     {
                        diff = arcCountTotal - fromArcCountTotal;
                     }
                     else
                     {
                        diff = fromArcCountTotal - arcCountTotal;
                     }

                     // If number of exiting arcs is >= arc location
                     if ( i->second.m_blocks[j].m_arcs.size() >= loc + 1)
                     {
                        // Assign count to non-normalized arc
                        if ( i->second.m_blocks[j].m_arcs[loc].m_count == -1 )
                        {
                           i->second.m_blocks[j].m_arcs[loc].m_count = diff;
                        }
                        else
                        {
                           i->second.m_blocks[j].m_from_arcs[loc]->m_count = diff;
                        }
                     }
                     // Assign count to entering arc
                     else
                     {
                        i->second.m_blocks[j].m_from_arcs[loc]->m_count = diff;
                     }

                     // Mark block as normalized
                     i->second.m_blocks[j].m_normalized = true;
                     normalized = true;
                  }
               }
            }
         // Loop until no normalization has occurred
         } while ( normalized == true );
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Inserts the last line # from parent for all blocks without a line #
///
///  @return void
///
///  @remarks
///  Pre  : Map of records has been initialized and populated
///  Post : All blocks have at least one assigned line #
///////////////////////////////////////////////////////////////////////////
void Parser::normalize_lines()
{
   // For all source files
   for ( map<string, Source_file>::iterator source_iter =
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   {
      map<Source_file::Source_key, Record> & records = 
         source_iter->second.m_records;

      // For each Record (function)
      for ( map<Source_file::Source_key, Record>::iterator i = records.begin();
            i != records.end(); ++i )
      {
         // For each block within the function
         for ( unsigned j = 0; j < i->second.m_blocks.size(); ++j )
         {
            // If the block has no associated lines and the block
            // is not a start or end block
            if ( i->second.m_blocks[j].m_lines.m_lines.size() == 0
                 && j != 0 && j != i->second.m_blocks.size() - 1 )
            {
               // Assign information and lines from parent block to block
               i->second.m_blocks[j].m_lines.m_source = i->second.m_blocks[j-1].m_lines.m_source;
               i->second.m_blocks[j].m_lines.m_block = j;
               i->second.m_blocks[j].m_lines.m_lines.push_back( i->second.m_blocks[j-1].m_lines.m_lines.back() );
            }
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Examines each record's function blocks to determine if they're fake
///
///  @return void
///
///  @pre No function blocks are marked as fake
///  @post Qualifying function blocks are flagged as fake
///////////////////////////////////////////////////////////////////////////
void Parser::normalize_fake_blocks()
{
   // For all source files
   for ( map<string, Source_file>::iterator source_iter =
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   {
      map<Source_file::Source_key, Record> & records = 
         source_iter->second.m_records;

      // For each Record (function)
      for ( map<Source_file::Source_key, Record>::iterator i = records.begin();
            i != records.end(); ++i )
      {
         bool done;

         // Loop until no blocks have had their fake status changed
         do
         {
            done = true;

            // For each block within the function
            for ( unsigned j = 0; j < i->second.m_blocks.size(); ++j )
            {
               Block & block_ref = i->second.m_blocks[j];
               // Only look at blocks not marked as fake and ignore the
               // function entry and exit blocks
               if ( ! block_ref.is_fake() && ! block_ref.is_start_block() )
                    // && ! block_ref.is_end_block() )
               {
                  // First case: Check if all incoming arcs to a block are fake
                  bool found_real = false;
                  
                  for ( unsigned k = 0; k < block_ref.m_from_arcs.size(); ++k )
                  {
                     // If a non-fake entry arc is found, set found flag and
                     // exit loop
                     if ( ! block_ref.m_from_arcs[k]->is_fake() )
                     {
                        found_real = true;
                        break;
                     }
                  }

                  // Mark block as fake if no real entry arc(s) found
                  if ( ! found_real )
                  {
                     block_ref.m_fake = true;
                     done = false;
                     break;
                  }

                  // Second case: Check if all immediate "parent" blocks are
                  // fake
                  found_real = false;

                  for ( unsigned k = 0; k < block_ref.m_from_arcs.size(); ++k )
                  {
                     // If a non-fake parent block is found, set found flag
                     // and exit loop
                     if ( ! i->second.m_blocks[ block_ref.m_from_arcs[k]->m_origin_block ].is_fake() )
                     {
                        found_real = true;
                        break;
                     }
                  }

                  // Mark block as fake if no real parent blocks found
                  if ( ! found_real )
                  {
                     block_ref.m_fake = true;
                     done = false;
                     break;
                  }
               }
            }
         } while ( ! done );
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///
///  @brief
///  Populate m_blocks_sorted with Blocks sorted by line #
///
///  @return void
///
///  @pre m_blocks_sorted is empty
///  @post m_blocks_sorted contains a vector of Blocks sorted by line #
///////////////////////////////////////////////////////////////////////////
void Parser::sort_blocks()
{
   // For all source files
   for ( map<string, Source_file>::iterator source_iter =
            m_source_files.begin();
         source_iter != m_source_files.end();
         source_iter++ )
   {
      map<Source_file::Source_key, Record> & records = 
         source_iter->second.m_records;

      // For each Record (function)
      for ( map<Source_file::Source_key, Record>::iterator i = records.begin();
            i != records.end(); ++i )
      {
         i->second.m_blocks_sorted = i->second.m_blocks;
         std::sort( i->second.m_blocks_sorted.begin() + 1, 
                    i->second.m_blocks_sorted.end() - 1, compare_line_nums);
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///
///  @brief
///  Returns true if first Block's last line # is < right Block's
///
///  @return bool
///////////////////////////////////////////////////////////////////////////
const bool compare_line_nums( const Block & lhs, const Block & rhs )
{
   // Get block's line information
   const Lines_data & line_data_lhs = lhs.get_line_data();
   const Lines_data & line_data_rhs = rhs.get_line_data();
   const vector<unsigned> & line_nums_lhs = line_data_lhs.get_line_nums();
   const vector<unsigned> & line_nums_rhs = line_data_rhs.get_line_nums();

   // Get last line # of function block
   unsigned line_lhs = line_nums_lhs[ line_nums_lhs.size() - 1 ];
   unsigned line_rhs = line_nums_rhs[ line_nums_rhs.size() - 1 ];

   // Compare line numbers
   return line_lhs < line_rhs;
}
