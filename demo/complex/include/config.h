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
///  @file config.h 
///
///  @brief
///  Defines the Configuration for Trucov.
/// 
///
///  @remarks
///  
///  
///
///  Requirements Specification:
///     < http://code.google.com/p/trucov/wiki/SRS > 
///
///  Design Description: 
///     < http://code.google.com/p/trucov/wiki/TrucovDesignDocumentation >
///////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_H
#define CONFIG_H

// SYSTEM INLUEDES

#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// LOCAL INCLUDES

#include "tru_utility.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
///  @class Config
///
///  @brief
///  Specify the configuration for trucov.
///
///  @remarks
///  Class takes a variable map, argument count and argument array when
///  constructed.
///////////////////////////////////////////////////////////////////////////////
class Config
{
  public:

      /// @brief
      /// Initialize the config object and returns the single instance of this 
      /// singleton.
      ///
      /// @remarks
      /// Returns a pointer to the singleton Config object.
      static Config* get_instance(po::variables_map vm, int ac, char* av[]);

      /// @brief
      /// Returns the single instance of this singleton.
      ///
      /// @remarks
      /// Returns a reference to the singleton Config object.
      static Config & get_instance();

      /// @brief
      /// initialize the member variables
      ///
      /// @param vm boost program options variable_map
      /// @param ac argument count
      /// @param av[] argument array
      void initialize(po::variables_map vm, int ac, char* av[]);

      /// @brief
      /// Returns a string of the absolute path of the output directory.
      ///
      /// @remarks
      /// Returns the path of current working dir if outdir is not specified.
      string get_out_dir();

      /// @brief
      /// Returns a string of the absolute path to the working directory.
      ///
      /// @remarks
      /// Returns the path of current working dir if working directory
      /// is not specified.
      string get_work_dir();

      /// @brief
      /// Returns true if "--debug" flage is used. Returns false otherwise.
      ///
      /// @remarks
      /// default = false
      bool get_debug_flag();

      /// @brief
      /// Returns false if "--brief" flag is used. Returns true otherwise.
      ///
      /// @remarks
      /// default = true
      bool get_brief_flag();

      /// @brief
      /// Returns true if "--and" flag is used. Returns false otherwise.
      ///
      /// @remarks
      /// default = false
      bool get_and_flag();

      /// @brief
      /// Returns true if "--or" flag is used. Returns false otherwise.
      ///
      /// @remarks
      /// default = false
      bool get_or_flag();

      /// @brief
      /// Returns true if "--only-missing" flag is used. Returns false otherwise.
      ///
      /// @remarks
      /// default = false
      bool get_only_missing_flag();

      /// @brief
      /// Returns true if "--show-fake" flag is used. Return false otherwise.
      ///
      /// @remarks
      /// default = false
      bool get_show_fake_flag();

      /// @brief
      /// Returns true if "--mangle" flag is used. Return false otherwise.
      ///
      /// @remarks
      /// default is_mangle = false
      bool get_mangle_flag();

      /// @brief
      /// Returns true if "--demangle" flag is used. Return false otherwise.
      ///
      /// @remarks
      /// default is_mangle = false
      bool get_demangle_flag();

      /// @brief
      /// Returns true if "--partial-match" flag is used. Return false otherwise.
      bool get_partial_match_flag();

      /// @brief
      /// Returns true if "--full-match" flag is used. Return false otherwise.
      bool get_full_match_flag();

      /// @brief
      /// Returns true if "--signature-match" flag is used. Return false otherwise.
      bool get_signature_match_flag();

      /// @brief
      /// Specify the output file type of the coverage flow graph.
      const string & get_render_format();

      /// @brief
      /// Prints out detail configuration to the stdout.
      ///
      /// @remarks
      /// This function is for testing.
      void print_config();

      /// @brief
      /// Returns a list of ".c" or ".cpp" files from the src_dir.
      const vector<string> & get_source_files();

      /// @brief
      /// Returns a list of ".gcno" and ".gcda" files from the build_dir.
      vector< pair<string, string> > get_build_files();

      /// @brief
      /// Returns a list of selection.
      const vector<string> & get_selection();

      /// @brief
      /// Returns an absolute path of the revision script.
      const string & get_rev_script();

      /// @brief
      /// Returns command that is used in the command line.
      const string & get_command() const;

  private:

// PRIVATE MEMBERS

      // The command specified.
      string command;

      // Pointer to the config instance
      static Config* config_instance;

      // Working directory
      string workingdir;

      // Output directory
      string outdir;

      // Revision script
      string rev_script;

      // List of source directorie(s)
      vector<string> srcdir;

      // List of build directorie(s)
      vector<string> builddir;

      // List of source file(s)
      vector<string> src_list;

      // List of GCNO and GCDA fiels
      vector< pair<string, string> > build_list;

      // List of selection.
      vector<string> select_list;

      // Determines if debug is selected or not
      bool debug;

      // Determines if brief is selected or not
      bool brief;

      // Determines if add is selected or not
      bool is_add;

      // Determines if and is selected or not
      bool is_and;

      // Determines if full is selected or not
      bool is_full;

      // Determines if signature-match is selected or not
      bool is_signature;

      // Determines if only-missing is selected or not
      bool is_only_missing;

      // Determines if show-fake is selected or not
      bool is_show_fake;

      // Determines if mangle is selected or demangle
      bool is_mangle;

      // Determines if full match is selected or partial match
      bool is_full_match;

      // Specifies default command
      string default_cmd;

      // Specifies default selection
      vector<string> default_select;

      // Specifies render format
      string ren_format;

      /// @brief 
      /// config constructor.
      Config();

      /// @brief
      /// collects GCDA and GCNO files from the build dir
      void collect_build_files();

      /// @brief
      /// collects source files from the src_dir
      void collect_src_files();

      /// @brief
      /// collects selection files name or function name from the command line
      ///
      /// @param ac argument count
      /// @param av[] argument array
      ///
      /// @remark this function is not in use
      void collect_selection(int ac, char* av[]);

      /// @brief
      /// Determines the input path is a GCNO file or not
      ///
      /// @param string file path.
      const bool is_gcno(string);

      /// @brief
      /// Determines the input path is a GCDA file or not
      ///
      /// @param string file path.
      const bool is_gcda(string);

      /// @brief
      /// Determines the input path is a cpp file or not
      ///
      /// @param string file path.
      const bool is_cpp(string);

      /// @brief
      /// Determines the input path is a c file or not
      ///
      /// @param string file path.
      const bool is_c(string);

      /// @brief
      /// Determines the input path is a HEADER file or not
      ///
      /// @param string file path.
      const bool is_header(string);
};

#endif
