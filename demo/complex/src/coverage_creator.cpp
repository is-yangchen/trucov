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
///  @file coverage_creator.cpp
///
///  @brief
///  Implements the methods of the Coverage_creator class
///////////////////////////////////////////////////////////////////////////////

// HEADER FILE

#include "coverage_creator.h"

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Generates all coverage files for a given set of records within
///  the parser class
///
///  @param config  Config class instantiation
///  @param parser  Parser class instantiation
///
///  @return void
///////////////////////////////////////////////////////////////////////////
void Coverage_creator::generate_files( Parser & parser )
{
   // Get instances of Utility and Config classes
   Tru_utility * ptr_utility = Tru_utility::get_instance();
   Config & config = Config::get_instance();

   // Get the desired output directory
   string output_dir = config.get_out_dir();
   
   // For each source file
   for ( map<string, Source_file>::const_iterator source_iter = 
            parser.get_source_files().begin();
         source_iter != parser.get_source_files().end();
         source_iter++ )
   {
      // Create shortcut to source file name and records
      const string & source_path = source_iter->second.m_source_path;
      const map<Source_file::Source_key,Record> & records = 
         source_iter->second.m_records;
      // Create name of coverage file
      const string file_name = ptr_utility->create_file_name( source_path, ".trucov" );
      const string full_path = output_dir + file_name;

      // Stores the lines of the source file(s)
      vector<string> contents;

      // Open the source file for read
      infile.open( source_path.c_str() );
      // If source file was opened
      if ( infile.is_open() )
      {
         contents.clear();
         string temp_string;

         // Read in each line of source file and push back contents into vector
         while ( getline( infile, temp_string ) )
         {
            contents.push_back( temp_string );
         }
         // Close the source file
         infile.close();
      }
      // If source file could not be opened for read, display error to user
      else
      {
         cout << "Could not open input file: " << source_path << endl;
      }

      // Open the coverage output file for write
      outfile.open( full_path.c_str() );
      // If the output file was opened
      if ( outfile.is_open() )
      {
         // Output the coverage file's header information
         outfile << source_path << source_iter->second.m_revision_number << "\n";

         // For each record (function)
         for( map<Source_file::Source_key, Record>::const_iterator i = records.begin(); 
              i != records.end(); 
              ++i )
         {
            string source = ptr_utility->get_rel_path( source_path );
            // Output function summary
            do_func_summary( i->second, contents, source );
         }
      }
      // Close the output file
      outfile.close();
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Outputs function summary information
///
///  @param rec       The current record (function)
///  @param contents  The contents of the input source file
///  @param source    The relative path and name of the input source file
///
///  @return void
///////////////////////////////////////////////////////////////////////////
void Coverage_creator::do_func_summary( const Record & rec, const vector<string> & contents,
                                        const string & source )
{
   double den = rec.get_function_arc_total();

   // If function has no branch arcs
   if ( den == 0 )
   {
      // Output either 0% or 100%
      double percentage = rec.get_coverage_percentage();
      outfile << setw(3) << fixed << setprecision(0) << percentage * 100 << "% "
              << rec.m_name_demangled << " no branches\n";
   }
   else
   {
      // Output function coverage information
      double num = rec.get_function_arc_taken();
      double percentage = rec.get_coverage_percentage();
      outfile << setw(3) << fixed << setprecision(0) << percentage * 100 << "% "
              << rec.m_name_demangled << " (" << num << "/" << den << ") branches\n";
      
      // If function coverage is not 100%
      if ( num != den )
      {
         // For each function block
         for ( unsigned i = 0; i < rec.m_blocks_sorted.size(); ++i )
         {
            // If block is a branch
            if ( rec.m_blocks_sorted[i].is_branch() )
            {
               // Output branch summary information
               do_branch_summary( rec, rec.m_blocks_sorted[i], contents, source );
            }
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////
///  @brief
///  Outputs branch summary information
///
///  @param rec       The current record (function)
///  @param block     The current function block
///  @param contents  The contents of the input source file
///  @param source    The relative path and name of the input source file
///
///  @return void
///
///  @pre contents contains lines of a source file
///////////////////////////////////////////////////////////////////////////
void Coverage_creator::do_branch_summary( const Record & rec, const Block & block,
                                          const vector<string> & contents,
                                          const string & source )
{
   // Get total # of branches and # of branches taken
   int branch_taken = block.get_branch_arc_taken();
   int branch_total = block.get_branch_arc_total();

   // Only print branch information if coverage is not 100%
   if ( branch_taken != branch_total )
   {
      // Get block's line information
      const Lines_data & line_data = block.get_line_data();
      const vector<unsigned> & line_nos = line_data.get_line_nums();
      // Get last line # of function block
      unsigned line_no = line_nos[ line_nos.size() - 1 ];

      // Output branch coverage information
      outfile << "\t" << source << ":"   << line_no << ": "
              << branch_taken   << "/"   << branch_total << " branches: ";
      if ( line_no - 1 < contents.size() )
      {
         // Output branch's corresponding source file line
         string temp_string = contents[ line_no - 1 ];
         boost::trim( temp_string );
         outfile << temp_string << "\n";
      }

      // Get block's arcs
      const vector<Arc> & arcs = block.get_arcs();

      // For each arc
      for ( unsigned i = 0; i < arcs.size(); ++i )
      {
         // If not a fake arc
         if ( ! arcs[i].is_fake() )
         {
            // If arc not taken
            if ( arcs[i].get_count() <= 0 )
            {
               // Output first line of block
               const Lines_data & dest_line_data
                  = rec.m_blocks[ arcs[i].get_dest() ].get_line_data();
               const vector<unsigned> & dest_line_nos
                  = dest_line_data.get_line_nums();

               // Output branch destination data
               outfile << "\t" << source   << ":"
                       << dest_line_nos[0] << ":" << " destination: ";

               if ( dest_line_nos[0] - 1 < contents.size() )
               {
                  // Output destination's corresponding source file line
                  string temp_string = contents[ dest_line_nos[0] - 1 ];
                  boost::trim( temp_string );
                  outfile << temp_string << "\n";
               }
            }
         }
      }
   }
}
