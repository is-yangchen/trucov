#ifndef HELPER_H
#define HELPER_H

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
///  @file all_functionaltest.cpp 
///
///  @brief
///  Invokes all functional tests. 
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

namespace fs = boost::filesystem;
using namespace std;

// FIXTURE 

class TrucovTest : public testing::Test 
{
protected:  

   string project_directory; 
   string project_directory_inline; 
   string error_output;

   // Called before every test.  
   virtual void SetUp() 
   {
      project_directory = "../test/resources/functional_test_project/";
      project_directory_inline = "../test/resources/functional_test_project_inline/";
   }

   // checks to see if two files are the same
   bool compare_files(string file, string file2)
   {
      string s1 = read_file(file);
      string s2 = read_file(file2);

      return ( s1 == s2 );
   }

   // Returns a file's content as a string.
   string read_file(string file)
   {
      string res; 
      ifstream infile(file.c_str());
      if (!infile.fail())
      {
         infile.seekg(0, ios::end);
         unsigned int file_length = infile.tellg();
         infile.seekg(0, ios::beg);

         vector<char> buf(file_length);
         infile.read(&buf[0], file_length);

         for (int i = 0; i < buf.size(); i++)
            res.push_back(buf[i]);
      }
      infile.close(); 
      return res;
   }

   // Executes trucov program.
   string execute_trucov(string argument)
   {
      char input[512];
      string rec_name = "";
      FILE *fp = 
         popen( ("./trucov " + argument + " -c " + project_directory + " 2> fun_error_dump").c_str(), "r" );

      if (fp != NULL)
      {  
         while ( fgets( input, sizeof( input ), fp ) != NULL )
         {
            rec_name.append(input);
         }

         if (rec_name[rec_name.size() - 1] == '\n')
            rec_name.erase(rec_name.size() - 1, 1);

         pclose(fp);
      }

      error_output = read_file("fun_error_dump");
      fs::remove_all("fun_error_dump");

      return rec_name;
   }

   string execute_trucov_inline(string argument)
   {
      char input[512];
      string rec_name = "";
      FILE *fp = 
         popen( ("./trucov " + argument + " -c " + project_directory_inline + " 2> fun_error_dump").c_str(), "r" );

      if (fp != NULL)
      {  
         while ( fgets( input, sizeof( input ), fp ) != NULL )
         {
            rec_name.append(input);
         }

         if (rec_name[rec_name.size() - 1] == '\n')
            rec_name.erase(rec_name.size() - 1, 1);

         pclose(fp);
      }

      error_output = read_file("fun_error_dump");
      fs::remove_all("fun_error_dump");

      return rec_name;
   }
   
   // Executes trucov program.
   string execute_trucov_from_cwd(string argument)
   {
      char input[512];
      string rec_name = "";
      FILE *fp = 
         popen( ("./trucov " + argument + " 2> fun_error_dump").c_str() , "r" );

      if (fp != NULL)
      {  
         while ( fgets( input, sizeof( input ), fp ) != NULL )
         {
            rec_name.append(input);
         }

         if (rec_name[rec_name.size() - 1] == '\n')
            rec_name.erase(rec_name.size() - 1, 1);

         pclose(fp);
      }

      error_output = read_file("fun_error_dump");
      fs::remove_all("fun_error_dump");
      
      return rec_name;
   }
};

#endif
