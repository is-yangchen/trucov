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
///  @file dot_creator.cpp
///
///  @brief
///  Implements the methods of the Dot_creator class
///////////////////////////////////////////////////////////////////////////////

// HEADER FILE

#include "dot_creator.h"

// USING STATEMENTS

using std::map;
using std::string;
using std::vector;

///////////////////////////////////////////////////////////////////////////////
/// @brief
/// Initializes the dot attributes.
///////////////////////////////////////////////////////////////////////////////
Dot_creator::Dot_creator()
{
   // Set block type shapes
   m_normal_block_shape = "box";
   m_end_block_shape = "ellipse";
   m_fake_block_shape = "ellipse";
   m_branch_block_shape = "diamond";
   m_function_block_shape = "note";

   // Set block type fillcolors
   m_default_block_fillcolor = "grey100";
   m_start_end_block_fillcolor = "grey80";
   m_taken_block_fillcolor = "palegreen";
   m_untaken_block_fillcolor = "rosybrown1";
   m_partial_block_fillcolor = "#FFFFB0";

   // Set blocktype styles
   m_normal_block_style = "filled, rounded";
   m_fake_block_style = "dashed";

   // Set arc type styles
   m_fake_style = "dashed";
   m_normal_style = "solid";

   // Set arc and block type line colors
   m_normal_color = "black";
   m_taken_color = "darkgreen";
   m_untaken_color = "red3";
   m_partial_color = "goldenrod1";
   m_unknown_color = "purple";

   // Set standard pen widths
   m_normal_width = 1.0;
   m_bold_block_width = 3.5;
   m_bold_line_width = 2.5;

} // End of Dot_creator default constructor

///////////////////////////////////////////////////////////////////////////////
///  @brief
///  Generates a single DOT file containing the control flow graph data of
///  all source files' records
///
///  @param parser  Parser class instantiation
///
///  @return void
///////////////////////////////////////////////////////////////////////////////
void Dot_creator::generate_file( Parser & parser )
{
   // Get instances of Utility and Config classes
   Tru_utility * ptr_utility = Tru_utility::get_instance();
   Config & config = Config::get_instance();

   // Get the desired output directory
   const string & output_dir = config.get_out_dir();
   // Create name of DOT file
   const string full_path = output_dir + "coverage.dot";

   // Open coverage file for output
   outfile.open( full_path.c_str() );

   // If file was successfully opened
   if ( outfile.is_open() )
   {
      // Begin directional graph description
      outfile << "digraph coverage{\n";

      // For each source file
      for ( map<string, Source_file>::const_iterator source_iter = 
               parser.get_source_files().begin();
            source_iter != parser.get_source_files().end();
            ++source_iter )
      {
         // Create a subgraph for each source file
         const string & source_path = source_iter->second.m_source_path;
         outfile << "   subgraph \""
                 << ptr_utility->create_file_name( source_path, "" )
                 << "\"{\n";

         const map<Source_file::Source_key,Record> & records
            = source_iter->second.m_records;

         // For each record (function)
         for( map<Source_file::Source_key, Record>::const_iterator it = records.begin();
         it != records.end(); 
         ++it )
         {
            const Record & rec = it->second;

            // Create a subgraph for each function
            outfile << "      subgraph function" << rec.m_checksum
                    << "{\n";
            // Generate the functions arc and block DOT syntax
            generate_arcs( rec );
            generate_blocks( rec );
            outfile << "      }\n";
         }

         outfile << "   }\n";
      }
      // End directional graph description
      outfile << "}\n";

      // Close the DOT file
      outfile.close();
     
      // Optionally render the coverage graph
      string output_file = ptr_utility->get_rel_path(config.get_out_dir()) + "coverage";
      create_render_file( output_dir, output_file, full_path );
   }   
}

///////////////////////////////////////////////////////////////////////////////
///  @brief
///  Generates all of the DOT files containing the control flow graph data of
///  the records within each source file
///
///  @param parser  Parser class instantiation
///
///  @return void
///////////////////////////////////////////////////////////////////////////////
void Dot_creator::generate_files( Parser & parser )
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
         ++source_iter )
   {
      // Create name of DOT file
      const string & source_path = source_iter->second.m_source_path;
      const string file_name = ptr_utility->create_file_name( source_path, ".dot" );
      const string full_path = output_dir + file_name;

      // Open coverage file for output
      outfile.open( full_path.c_str() );

      // If file was successfully opened
      if ( outfile.is_open() )
      {
         // Begin direcitonal graph discriptoin
         outfile << "digraph coverage{\n";

         const map<Source_file::Source_key,Record> & records = source_iter->second.m_records;

         for( map<Source_file::Source_key, Record>::const_iterator it = records.begin(); 
         it != records.end(); 
         ++it )
         {
            const Record & rec = it->second;
            // Create a subgraph for each function
            outfile << "   subgraph function" << rec.m_checksum
                    << "{\n";
            // Generate the functions arc and block DOT syntax
            generate_arcs( rec );
            generate_blocks( rec );
            outfile << "   }\n";
         }
         // End directional graph description
         outfile << "}\n";

         // Close the DOT file
         outfile.close();
      }
      
      // Optionally render each coverage graph
      create_render_file( output_dir, source_path, full_path );
   }
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Creates a render file if a render type is set.
///
/// @param output_dir The destination output directory path.
/// @param source_path The destination file name.
/// @param full_path The full path to the destination file
////////////////////////////////////////////////////////////////////////////// 
void Dot_creator::create_render_file( 
   const string & output_dir,
   const string & source_path,
   const string & full_path )
{
   // Get output render format.
   string render_str = "";
   switch ( m_render_type )
   {
      case Pdf: 
         render_str = "pdf";
         break;
      case Svg:
         render_str = "svg";
         break;
      default:
         break;
   }

   // Only render if valid format.
   if ( render_str.size() > 0 )
   {
      string extension = ".";
      Tru_utility * ptr_utility = Tru_utility::get_instance();
      
      const string render_path = 
         output_dir + ptr_utility->create_file_name( 
            source_path, "." + render_str);

      const string render_pipe = "dot -T" + render_str + " " + full_path 
         + " -o " + render_path;

      // Give user feedback on rendering status
      cout << "Rendering " << ptr_utility->get_rel_path(render_path) << "\n";

      FILE * render_ptr =
         popen( render_pipe.c_str(), "r" );

      pclose( render_ptr );
   }

} // End of Dot_creator::create_render_file(...)

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Sets the render type of the next run.
///
/// @param render_type Type of render of next run.  
//////////////////////////////////////////////////////////////////////////////
void Dot_creator::set_render_type( Render_type render_type )
{
   m_render_type = render_type; 

} // End of Dot_creator::set_render_type(...)

///////////////////////////////////////////////////////////////////////////////
///  @brief
///  Generates a record's arc information in the DOT file
///
///  @param rec The current record (function) information
///
///  @return void
///////////////////////////////////////////////////////////////////////////////
void Dot_creator::generate_arcs( const Record & rec )
{
   // Get instantiation of Config class
   Config & config = Config::get_instance();

   // Create invisible arc that ties function header block to rest of graph
   if ( rec.m_blocks.size() > 0 )
   {
      outfile << "      B_Header_" << rec.m_checksum << " -> "
              << " B_" << rec.m_checksum << "_" << rec.m_ident << "_0"
              << " [ color=\"black\", style=\"invis\" ];\n";
   }

   // For each functon block
   for ( unsigned i = 0; i < rec.m_blocks.size(); ++i )
   {
      // Only output blocks's outgoing arcs if either block isn't fake or if
      // show-fake flag is set
      if ( ! rec.m_blocks[i].is_fake() || config.get_show_fake_flag() )
      {
         // Get block's arcs
         const vector<Arc> & arcs = rec.m_blocks[i].get_arcs();

         // For each arc in the block
         for ( unsigned j = 0; j < arcs.size(); ++j )
         {
            // Only output arc information if arc isn't fake or if
            // show-fake flag is set
            if ( ! arcs[j].is_fake() || config.get_show_fake_flag() )
            {
               // Begin arc descriptoin
               outfile << "      B_"  << rec.m_checksum << "_" << rec.m_ident
                       << "_" << i << " -> B_" << rec.m_checksum << "_"
                       << rec.m_ident << "_" << arcs[j].get_dest()
                       << " [ label = \""
                       << arcs[j].get_count()
                       << "\" , color=\"";

               // If arc is fake and taken
               if( arcs[j].is_fake() && arcs[j].is_taken() )
               {
                  outfile << m_taken_color << "\" , style=\"" 
                          << m_fake_style << "\" ]; \n";
               }
               // If arc is fake but not taken
               else if( arcs[j].is_fake() )
               {
                  outfile << m_untaken_color << "\" , style=\""
                          << m_fake_style << "\", penwidth=" << m_bold_line_width 
                          << " ]; \n";
               }
               // If arc is normal and taken
               else if( arcs[j].is_taken() )
               {
                  outfile << m_taken_color << "\" , style=\""
                          << m_normal_style << "\" ]; \n";
               }
               // For all others
               else
               {
                  outfile << m_untaken_color << "\" , style=\""
                          << m_normal_style << "\", penwidth=" << m_bold_line_width
                          << " ]; \n";
               }
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///  @brief
///  Generates a record's block information in the DOT file
///
///  @param rec The current record (function) information
///
///  @return void
///////////////////////////////////////////////////////////////////////////////
void Dot_creator::generate_blocks( const Record & rec )
{
   // Get instantiation of Config class
   Config & config = Config::get_instance();
   double percentage = rec.get_coverage_percentage();

   // Create function header block
   outfile << "      B_Header_" << rec.m_checksum
           << " [ label=< " << rec.m_source << " <br /> "
           << rec.get_HTML_name() << " <br /> "
           << "Exec Count: " << rec.get_execution_count() << " <br /> "
           << "Coverage: " << fixed << setprecision(0) << percentage * 100 << "% "
           << " > style=\"filled\" fillcolor=\"" << m_default_block_fillcolor 
           << "\" color=\"" << m_normal_color
           << "\" penwidth=" << m_normal_width 
           << " shape=\"" << m_function_block_shape << "\" ];\n";

   // For each block in a function
   for ( unsigned i = 0; i < rec.m_blocks.size(); ++i )
   {
      const Block & block = rec.m_blocks[i];

      // Only output block information if block is not fake or if
      // show-fake flag is set
      if ( ! block.is_fake() || config.get_show_fake_flag() )
      {
         // Begin block description
         outfile << "      B_" << rec.m_checksum << "_" << rec.m_ident << "_" << i;
         outfile << " [ label=< ";

         // If starting block, mark as the entry point
         if( block.is_start_block() )
         {
            outfile << "_entry_";        
         }
         // If exit block, mark as exit point
         else if ( block.is_end_block() )
         {
            outfile << "_exit_"; 
         }

         // Output the line #s and execution count for function blocks,
         // except first and last block
         if ( ! block.is_start_block() && ! block.is_end_block() )
         {
            // Get block's line information
            const Lines_data & line_data = block.get_line_data();
            const vector<unsigned> & line_nums = line_data.get_line_nums();

            // If block has line numbers assigned
            if ( line_nums.size() > 0 )
            {
               // If block only has one associated line number
               if ( line_nums.size() == 1 )
               {
                  outfile << " Line: " << line_nums[0];
               }
               // If block has multiple associated line numbers
               else
               {
                  outfile << " Lines: " << line_nums[0] << "..."
                          << line_nums[ line_nums.size() - 1 ];
               }
            }

            // Output block's execution count
            outfile << "<br />Count: " << block.get_count();
         }

         // Output debug info if flag is set
         if ( config.get_debug_flag() )
         {
            // Output block #
            outfile << "<br />Block#: " << i;
         }

         // Set block's style
         if ( ! block.is_fake() )
         {
            outfile << " > style=\"" << m_normal_block_style << "\" ";
         }
         else
         {
            outfile << " > style=\"" << m_fake_block_style << "\" ";
         }

         outfile << " fillcolor=\"";

         // Depending on block type, fill with appropriate color
         if ( block.is_start_block() || block.is_end_block() )
         {
            outfile << m_start_end_block_fillcolor;
         }         
         else if ( block.has_full_coverage() )
         {
            outfile << m_taken_block_fillcolor;
         }
         else if ( block.has_partial_coverage() )
         {
            outfile << m_partial_block_fillcolor;
         }
         else
         {
            outfile << m_untaken_block_fillcolor;
         }

         outfile << "\" color=\"";

         // Determine block's outline color and pen width by the block's
         // coverage
         if ( block.is_start_block() || block.is_end_block() )
         {
            outfile << m_normal_color << "\"";

            if ( block.is_end_block() )
            {
               outfile << " penwidth=" << m_bold_block_width; 
            }
         }
         else if ( block.has_full_coverage() )
         {
            outfile << m_taken_color << "\"";
         }
         else if ( block.has_partial_coverage() )
         {
            outfile << m_partial_color << "\" penwidth=" << m_bold_block_width;
         }
         else
         {
            outfile << m_untaken_color << "\" penwidth=" << m_bold_block_width;
         }

         outfile << " shape=\"";

         // Draw start block
         if ( block.is_start_block() )
         {
            outfile << m_normal_block_shape; 
         } // Draw last block 
         else if ( block.is_end_block() )
         {
            outfile << m_end_block_shape;
         } // Draw fake block
         else if ( block.is_fake() )
         {
            outfile << m_fake_block_shape;
         } // Draw a branch
         else if ( block.is_branch() )
         {
            outfile << m_branch_block_shape; 
         } 
         else // All other blocks
         {
            outfile << m_normal_block_shape; 
         }

         // End block description
         outfile << "\" ];\n";
      }
   }
}
