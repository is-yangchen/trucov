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
///  @file tru_utility.h 
///
///  @brief
///  Defines the utility functions for Trucov.
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

#ifndef TRU_UTILITY_H
#define TRU_UTILITY_H

// SYSTEM INCLUDES

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>

// FOWARD REFERECNES

using std::cout;
using std::string;
using std::vector;
namespace fs = boost::filesystem;

///////////////////////////////////////////////////////////////////////////////
///  @class Tru_Utility
///
///  @brief
///  Utility functions for trucov
///////////////////////////////////////////////////////////////////////////////
class Tru_utility
{
   public:
      /// @brief
      /// Cleans up after the Tru_Utility object
      ~Tru_utility();

      /// @brief
      /// Change current working directory. Returns true when success, false otherwise.
      ///
      /// @param absolute or relative directory path.
      ///
      /// @return boolean
      bool change_dir( string path ); 

      /// @brief
      /// getter for m_curr_dir
      ///
      /// @return
      /// currently working directory path
      string get_cwd();

      /// @brief
      /// read contents of directory
      ///
      /// @param path absolute directory path
      ///
      /// @return
      /// a list of strings containing directory contents
      vector<string> read_dir( string path );

      /// @brief
      /// determine input path is directory or not
      ///
      /// @param path directory path
      ///
      /// @return
      /// boolean
      bool is_dir(string path); 

      /// @brief
      /// Create a directory
      ///
      /// @param path absolute directory path
      ///
      /// @return
      /// boolean
      bool make_dir(string path);

      /// @brief
      /// find the relative path to the current working directory
      ///
      /// @param path absolute directory path
      ///
      /// @return
      /// relative path as a string
      string get_rel_path(string path);

      /// @brief
      /// find the absolute directory path of the input
      ///
      /// @param raw_path directory path
      ///
      /// @return
      /// an absolute path as a string
      string get_abs_path(string raw_path);

      /// @brief
      /// find the absolute file path of the input
      ///
      /// @param raw_path file path
      ///
      /// @return
      /// an absolute path as a string
      string get_abs_path_file(string raw_path);

      /// @brief
      /// get the pointer of tru_utility instance
      ///
      /// @return
      /// a pointer to the tru_utility instance
      ///
      /// @remark it doesn't gurantee that the directory or file exists
      static Tru_utility* get_instance();

      /// @brief
      /// 
      ///
      /// @param const  vestor<string> &srcFiles : dir list of source file 
      /// @param const  string &srcGCNO : path from GCNO
      ///
      /// @return 
      /// const boolean
      const bool is_within_project( const vector<string> &srcFiles,
                                    const string &srcGCNO ) const;

      /// @brief
      /// 
      /// @param const  vestor<string> &srcFiles : dir list of source file 
      /// @param const  string &srcGCNO : path from GCNO
      /// @param string & source_path
      ///
      /// @return 
      /// const boolean
      const bool is_within_project( const vector<string> &srcFiles,  
                                    const string & srcGCNO,
                                    string & source_path ) const;

      /// @brief
      /// 
      /// @param const  string & pathname : path from GCNO
      ///
      /// @return 
      /// const string
      const string get_filename( const string & pathname ) const;

      /// @brief
      /// 
      /// @param const  string & filename
      ///
      /// @return 
      /// const string
      const string strip_extension( const string & filename ) const;

      /// @brief
      /// 
      /// @param const  string & pathname
      ///
      /// @return 
      /// const string
      const string get_basename( const string & pathname ) const;

      /// @brief
      /// 
      /// @param string str
      /// @param const  string & type
      ///
      /// @return 
      /// string
      string genSelcovFileName(string str, const string & type);

      /// @brief
      /// 
      /// @param string source_name
      /// @param const string & extension
      ///
      /// @return 
      /// string
      string create_file_name( string source_name, const string & extension );

      /// @brief 
      /// 
      /// @param string command_name
      /// @param string command_argument
      ///
      /// @return 
      /// string
      string execute_pipe(string command_name, string command_argument);

   private:

      /// @brief Tru_utility constructor
      Tru_utility();

      // a pointer for Tru_utility instance
      static Tru_utility* tru_utility_ptr;

      // current working directory
      fs::path m_curr_dir;

      // deliminator 
      char m_del;
};
#endif
