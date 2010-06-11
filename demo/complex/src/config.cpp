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
///  @file config.cpp
///
///  @brief
///  Implements the config class methods.
///////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES
#include <iostream>
#include <vector>
//#include <sys/types.h>
//#include <sys/stat.h>

// LOCAL INCLUDES
#include "config.h"

// USING STATEMENTS
using namespace std;

/// Initialize the config instance
Config* Config::config_instance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// @brief
/// Construct config instance and Initializes the config member variables. 
/// 
///////////////////////////////////////////////////////////////////////////////
Config::Config()
{
   workingdir = "";
   srcdir.clear();
   builddir.clear();
   src_list.clear();
   build_list.clear();
   select_list.clear();
   rev_script = "";
   
   //cachefilename = "";
   
   debug = false;
   brief = true;
   is_add = false;
   
   is_and = false;
   is_full = false;
   is_signature = false;
   is_only_missing = false;
   is_show_fake = false;
   is_mangle = false;
   is_full_match = false;

   command = "status";

   default_select.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// @brief
/// Construct config instance and Initializes the config member variables. 
/// 
/// @return a pointer to a config instance
///////////////////////////////////////////////////////////////////////////////
Config* Config::get_instance(po::variables_map vm, int ac, char* av[])
{
    // Config instance has not been created
    if( Config::config_instance == NULL )
    {
       // Create new instance
       config_instance = new Config();

       // initialize member variables
       config_instance->initialize(vm, ac, av);
    }

    // return config instance
    return config_instance;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Returns reference to config object.
///
/// @return Reference to config.
//////////////////////////////////////////////////////////////////////////////
Config & Config::get_instance()
{
    // Config instance has not been created
    if ( Config::config_instance == NULL )
    {
        // Create new instance
        config_instance = new Config();
    }

    // return the address of config instance
    return *config_instance;

} // End of Config::get_instance()

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// initialize the member variables
///
/// @param vm boost program options variable_map
/// @param ac argument count
/// @param av[] argument array
//////////////////////////////////////////////////////////////////////////////
void Config::initialize(po::variables_map var_map, int ac, char* av[])
{
   // ***************************************
   workingdir = "";
   srcdir.clear();
   builddir.clear();
   src_list.clear();
   build_list.clear();
   select_list.clear();
   rev_script = "";
   
   //cachefilename = "";
   
   debug = false;
   brief = true;
   is_add = false;
   
   is_and = false;
   is_full = false;
   is_signature = false;
   is_only_missing = false;
   is_show_fake = false;
   is_mangle = false;
   is_full_match = false;
   // **************************************************
   command = "status";

   default_select.clear();
    // get Tru_utility instance
    Tru_utility* sys_utility = Tru_utility::get_instance();

    // input exists for revision-script
    if ( var_map.count("revision-script") )
    {
        // store revision-script path
        rev_script = var_map["revision-script"].as< string >();

        // get the absolute path and store in rev_script
        rev_script = sys_utility->get_abs_path_file(rev_script);
    }

    // input exists for workingdir
    if ( var_map.count("chdir") )
    {
      // store chdir path
       workingdir = var_map["chdir"].as<string>();

       // get absolute path of workingdir
       workingdir = sys_utility->get_abs_path(workingdir);

       // change directory
       sys_utility->change_dir(workingdir);
    }
    
    // no input for workingdir
    else
    {
        // get current working dir and store
    	workingdir = sys_utility->get_cwd();
    }

    // input exists for outdir
    if ( var_map.count("outdir") )
    {
       // store output dir in outdir
       outdir = var_map["outdir"].as<string>();
       
       // store absolute path 
       fs::path tmp_path = sys_utility->get_abs_path(outdir);
       outdir = tmp_path.string();

       // outdir directory does not exist
       if( !sys_utility->is_dir(tmp_path.string()) )
       {
          // parent dir exists
          if (! sys_utility->make_dir(outdir) )          
          {
             // assign cwd to outdir
             outdir = sys_utility->get_cwd();
          }
       }
    }
    // no input for outdir
    else
    {
        // use cwd as outdir
        outdir = sys_utility->get_cwd();
    }

    // input exists for builddir
    if ( var_map.count("builddir") )
    {
       // store build dir path(s) in the buffer
       vector<string> buildbuf = var_map["builddir"].as< vector<string> >();

       // iterator for the list
       vector<string>::iterator build_itr;

       // assigh beginning of the list to the iterator
       build_itr = buildbuf.begin();

       // go through the list
       while( build_itr != buildbuf.end() )
       {
          // get absolute path for each path
          *build_itr = sys_utility->get_abs_path( *build_itr );

          // the path is a directory path
          if ( sys_utility->is_dir( *build_itr ) )
          {
             // store in builddir list
             builddir.push_back( *build_itr );
          }
          // the path is not a directory
          else
          {
             // print error message
             cerr << "ERROR::Config: " << *build_itr << " does not exist.\n";
          }

          //increment the iterator
          build_itr++;
       }
    }

    // no input for builddir
    else
    {
        // store in the builddir list
        builddir.push_back( sys_utility->get_cwd() );
    }

    // input exists for srcdir
    if ( var_map.count("srcdir") )
    {
       // store direcotry path(s) to the buffer
       vector<string> srcbuf = var_map["srcdir"].as< vector<string> >();

       // iterator for the list
       vector<string>::iterator src_itr;

       // assign beginning of the list to the iterator
       src_itr = srcbuf.begin();

       // go through the list
       while( src_itr != srcbuf.end() )
       {
          // get absolute path 
          *src_itr = sys_utility->get_abs_path( *src_itr );

          // the path is a directory
          if ( sys_utility->is_dir( *src_itr ) )
          {
             // store in srcdir
             srcdir.push_back( *src_itr );
          }

          // the path is not a directory
          else
          {
             // print error message
             cerr << "ERROR::Config: " << *src_itr << " does not exist.\n";
          }

          // increment the iterator
          src_itr++;
       }
    }

    // no input for srcdir
    else
    {
       // store cwd in srcdir
       srcdir.push_back( sys_utility->get_cwd() );
    }
    
    // debug flag is used in the command line
    if ( var_map.count("debug") )
    {
        // assign true to debug
        debug = true;
    }
    
    // brief flag is used in the command line
    if ( var_map.count("brief") )
    {
        // assign fasle to brief
        brief = false;
    }

    // input exists for selection
    if ( var_map.count("selection") )
    {
        // store input in select_list
        select_list = var_map["selection"].as< vector<string> >();
    }
    
    // add flag is used in the command line
    if ( var_map.count("add") )
    {
        // assign true to is_add
        is_add = true;
    }

    // or flag is used in the command line
    if ( var_map.count("or") )
    {
        // assign false to is_and
        is_and = false;
    }

    // and flag is used in the command line
    else if ( var_map.count("and") )
    {
        // assign true to is_and
        is_and = true; 
    }

    // only-missing flag is used in the command line
    if ( var_map.count("only-missing") )
    {
        // assign true to is_only_missing
        is_only_missing = true;
    }

    // show-fake flag is used in the command line
    if ( var_map.count("show-fake") )
    {
        // assign true to is_show_fake
        is_show_fake = true;
    }

    // hide-fake flag is used in the command line
    if ( var_map.count("hide-fake") )
    {
        // assign false to is_show_fake
        is_show_fake = false;
    }

    // mangle flag is used in the command line
    if ( var_map.count("mangle") )
    {
        // assign true to is_mangle
        is_mangle = true;
    }
    
    // demangle flag is used in the command line
    if ( var_map.count("demangle") )
    {
        // assign false to is_mangle
        is_mangle = false;
    }

    // partial-match flag is used in the command line
    if ( var_map.count("partial-match") )
    {
        // assign false to is_full
        is_full= false;
    }
    
    // full-match flag is used in the command line
    if ( var_map.count("full-match") )
    {
        // assign true to is_full
        is_full = true;
    }

    // signature-match flag is used in the command line
    if ( var_map.count("signature-match") )
    {
        // assign true to is_signature
        is_signature = true;
    }

    // input exists for default-command
    if ( var_map.count("default-command") )
    {
        // store input in command
        command = var_map["default-command"].as< string >();
    }

    // input exists for default-selection
    if ( var_map.count("default-selection") )
    {
        // store input in default_selection
        default_select = var_map["default-selection"].as< vector<string> >();
    }

    // input exists for render-format
    if ( var_map.count("render-format") )
    {
        // store input in ren_format
	ren_format = var_map["render-format"].as<string>();	
    }
    // no input for render-format
    else
    {
        // use svg which is default
        ren_format = "svg";
    }

#ifdef DEBUG
   // print out source directories for DEBUG
   cout << "Source Directories = { ";
   for (int i = 0; i < (int)srcdir.size(); i++)
      cout << srcdir[i] << " ";
   cout << "};\n";

   // print out build directories for DEBUG
   cout << "Build Directories = { ";
   for (int i = 0; i < (int)builddir.size(); i++)
      cout << builddir[i] << " ";
   cout << "};\n";

   // print out output directories for DEBUG
   cout << "Output Directory = { " << outdir << " }\n";
#endif

    // get GCNO and GCDA files
    collect_build_files();

    // get source files
    collect_src_files();

    // get selection from 
    collect_selection(ac, av);

    // If command was specified, then use it.
    if (ac > 1 && av[1][0] != '-')
    {
        // store input command
        command = string(av[1]);
    }

#ifdef DEBUG
   // print command for DEBUGGING
   cout << "This is " << command << endl;
#endif


}

/// @brief
/// getter function for output directory path.
///
/// @return absolute path of output dir as a string.
string Config::get_out_dir(void)
{
    return outdir;
}

/// @brief
/// getter function for working directory path.
///
/// @return absolute path of working dir as a string.
string Config::get_work_dir(void)
{
    return workingdir;
}

/// @brief
/// getter function for revision script file path.
///
/// @return absolute path of revision script file path as a string.
const string & Config::get_rev_script()
{
    return rev_script;
}

/// @brief
/// getter function for revision script file format.
///
/// @return revision script file format as a string.
const string & Config::get_render_format()
{
    return ren_format;
}

/// @brief
/// print out the value of config class's member variables.
/// used in testing the config class
///
/// @return void
void Config::print_config(void)
{
    cout << "\ncommand:: " << command;
    cout << "\nworking_dir:: " << workingdir;
    cout << "\n\nout_dir:: " << outdir;
    cout << "\n\ndebug:: " << debug;
    cout << "\n\nbrief:: " << brief;
    cout << "\n\nis_add:: " << is_add;
    cout << "\n\nis_and:: " << is_and;
    cout << "\n\nis_full:: " << is_full;
    cout << "\n\nis_signature:: " << is_signature;
    vector<string>::iterator itr;
    /*
    while (itr != tmp_gcno.end() )
    {
        cout << *itr << "\n";
        itr++;
    }

    itr = tmp_gcda.begin();
    while (itr != tmp_gcda.end() )
    {
        cout << *itr << "\n";
        itr++;
    }*/
    cout << "\n\nPrinting Source List.................\n";
    itr = src_list.begin();
    while (itr != src_list.end() )
    {
        cout << *itr << "\n";
        itr++;
    }
    cout << "\nPrinting Build List..................\n"; 
    vector< pair<string, string> >::iterator b_itr = build_list.begin();
    while ( b_itr != build_list.end() )
    {
        pair<string, string> tmp_pair = *b_itr;
        cout << "_________________________________________________________\n";
        cout << tmp_pair.first << "\n";
        cout << tmp_pair.second << "\n";
        cout << "_________________________________________________________\n\n";
        b_itr++;
    }
    cout << "\nPrinting Selection List...............\n";
    itr = select_list.begin();
    while (itr != select_list.end() )
    {
        cout << *itr << "\n";
        itr++;
    }

    cout << "render-format { " << get_render_format() << " }\n";
}

/// @brief
/// recursively goes through the file system hierarchy and collect GCNO and GCDA files 
///
/// @return void
void Config::collect_build_files()
{
   // get Tru_utility instance
   Tru_utility* sys_utility = Tru_utility::get_instance();

   // create iterator 
   vector<string>::iterator main_itr;

   //temporary container for gcno files
   vector<string> tmp_gcno;

   //temporary container for gcda files
   vector<string> tmp_gcda;

   // builddir is not empty
   while (!(builddir.empty()))
   {
      main_itr = builddir.end();
   
      // seek the last item
      main_itr--;

      // store in path
      string path = *main_itr;

      // pop the last item from the list
      builddir.pop_back();

      // read the dir
      vector<string> dir_contents = sys_utility->read_dir( path );

      // create temp iterator
      vector<string>::iterator tmp_itr = dir_contents.begin();

      // iterator is not pointing to the end
      while ( tmp_itr != dir_contents.end() )
      {
         // file that the iterator is pointing at is a GCNO file
         if ( is_gcno(*tmp_itr) )
         {
             // store in the GCNO list
             tmp_gcno.push_back(*tmp_itr);
         }

         // file that the iterator is pointing at is a GCDA file
         else if ( is_gcda(*tmp_itr) )
         {
             // store in the GCDA list
             tmp_gcda.push_back(*tmp_itr);
         }

         // it is a sub-directory
         else if ( sys_utility->is_dir(*tmp_itr) )
         {
             // store in the builddir list for searching
             builddir.push_back(*tmp_itr);
         }
         // increment the iterator
         tmp_itr++;
      }     
   }// end while

   // match tmp_gcno with tmp_gcda
   // create an iterator for tmp_gcno list 
   vector<string>::iterator g_itr = tmp_gcno.begin();

   // iterator is not pointing at the end
   while ( g_itr != tmp_gcno.end() )
   {
      // copy GCNO file path in gcno_str
      string gcno_str = *g_itr;

      // remove the "gcno" extension from the string
      gcno_str = gcno_str.erase( (gcno_str.size() - 4), 4);
     
      // create an iterator for tmp_gcda list
      vector<string>::iterator d_itr = tmp_gcda.begin();

      // iterator is not pointing at the end
      while ( d_itr != tmp_gcda.end() )
      {
         // copy GCDA file path in gcda_str
         string gcda_str = *d_itr;
         
         // remove the "gcda" extension from the string
         gcda_str = gcda_str.erase( (gcda_str.size() - 4), 4);

         // gcda_str and gcda_string are identical
         if ( gcda_str.compare(gcno_str) == 0 )
         {
             // make a pair of original string and store in the build_list
             build_list.push_back(make_pair(*g_itr, *d_itr));

             // stop searching
             break;
         }
         
         // increment the inner iterator
         d_itr++;
      }

      // no match is found and make a pair with empty string
      build_list.push_back(make_pair(*g_itr, ""));
 
      // increment outter iterator
      g_itr++;
   }

   // This part will be deleted in future releases
   // 
   // DO NOT DELETE THIS COMMENT
   // This is simply removing the pair with empty string from the list
   ////////////////////////////////////////////////////////////////////

   // create a temp_list
   vector< pair<string, string> > tmp_build;

   // empty the list
   tmp_build.empty();

   // declare a variable
   pair<string, string> tmp_pair;

   // get iterator pointed to the beginning of the list
   vector< pair<string, string> >::iterator build_list_itr = build_list.begin();

   // iterator is not at the end
   while (build_list_itr != build_list.end())
   {
      // copy file pair from the list
      tmp_pair = *build_list_itr;

      // second part of the file pair is not an empty string
      if (tmp_pair.second.compare("") != 0)
      {
         // add to the list
         tmp_build.push_back(tmp_pair);
      }

      // increment the iterator
      build_list_itr++;
   }

   // empty the build_list
   build_list.empty();

   // assign tmp list to the build_list
   build_list = tmp_build;
   ////////////////////////////////////////////////////////////////////
}

/// @brief
/// recursively goes through the file system hierarchy and collect source files 
///
/// @return void
void Config::collect_src_files()
{
   // get Tru_utility instance
   Tru_utility* sys_utility = Tru_utility::get_instance();

   // create an iterator for the srcdir list
   vector<string>::iterator main_itr;

   // srcdir list is not empty
   while (!(srcdir.empty()))
   {
      // seek to the end
      main_itr = srcdir.end();

      // seek to the last item
      main_itr--;

      // copy path from the list
      string path = *main_itr;

      // remove the path from the list
      srcdir.pop_back();

      // read the dir
      vector<string> dir_contents = sys_utility->read_dir( path );

      // seek the beginning of the dir_content
      vector<string>::iterator tmp_itr = dir_contents.begin();

      // iterator not at the end
      while ( tmp_itr != dir_contents.end() )
      {
         // header file
         if ( is_header(*tmp_itr) )
         {
             // store to the list
             src_list.push_back(*tmp_itr);
         }

         // C++ or C source file
         else if ( is_cpp(*tmp_itr) || is_c(*tmp_itr) )
         {
             // store to the list
             src_list.push_back(*tmp_itr);
         }
   
         // diretory
         else if ( sys_utility->is_dir(*tmp_itr) )
         {
             // store to the srcdir list
             srcdir.push_back(*tmp_itr);
         }

         // increment the iterator
         tmp_itr++;
      }
   }//end while
}

/// @brief
/// geather file names or function names from the command line and store in the selection list
///
/// @param ac command line argument count
/// @param av[] argument array
///
/// @return void 
///
/// @remark this function is not tested
void Config::collect_selection(int ac, char* av[])
{
   string tmp_str;
   for(int arg_idx = 1; arg_idx < ac; arg_idx++)
   {
      tmp_str = "";
      if (av[arg_idx][0] == '"')
      {
         tmp_str = av[arg_idx];
         tmp_str.erase(0, 1);
         tmp_str.erase(tmp_str.size() - 1, 1);
         select_list.push_back(tmp_str);
         cout << tmp_str << "\n";
      }
   }

   // If command was specified, then remove it from selection.
   // there are more than one argument
   if (ac > 1)
   {
      // the second argument is not an option flag
      if (av[1][0] != '-')
      {
         // it must be a command and remove it from the list
         select_list.erase(select_list.begin());
      } 
   }
   
}


/// @brief
/// getter function for src_list
///
/// @return vector< string >
const vector<string> & Config::get_source_files()
{
    return src_list;
}

/// @brief
/// getter function for build_list
///
/// @return vector< pair <string, string> >
vector< pair<string, string> > Config::get_build_files()
{
   return build_list;
}

/// @brief
/// getter function for selection
///
/// @return vector< string >
const vector<string> & Config::get_selection()
{
   return select_list;
}

/// @brief
/// getter function for debug
///
/// @return bool
bool Config::get_debug_flag()
{
    return debug;
}

/// @brief
/// getter function for brief
///
/// @return bool
bool Config::get_brief_flag()
{
    return brief;
}

/// @brief
/// getter function for and
///
/// @return bool
bool Config::get_and_flag()
{
    return is_and;
}

/// @brief
/// getter function for or
///
/// @return bool
bool Config::get_or_flag()
{
    return !is_and;
}

/// @brief
/// getter function for only_missing
///
/// @return bool
bool Config::get_only_missing_flag()
{
    return is_only_missing;
}

/// @brief
/// getter function for show_fake
///
/// @return bool
bool Config::get_show_fake_flag()
{
    return is_show_fake;
}

/// @brief
/// getter function for mangle
///
/// @return bool
bool Config::get_mangle_flag()
{
    return is_mangle;
}

/// @brief
/// getter function for demangle
///
/// @return bool
bool Config::get_demangle_flag()
{
    return !is_mangle;
}

/// @brief
/// getter function for partial_match
///
/// @return bool
bool Config::get_partial_match_flag()
{
    return !is_full;
}

/// @brief
/// getter function for full_match
///
/// @return bool
bool Config::get_full_match_flag()
{
    return is_full;
}

/// @brief
/// getter function for signature_match
///
/// @return bool
bool Config::get_signature_match_flag()
{
    return is_signature;
}

/// @brief
/// getter function for command 
///
/// @return const string
const string & Config::get_command() const
{
    return command;
}

/// @brief
/// Determine whether input is a GCNO file or not 
///
/// @param file_name file path
///
/// @return bool
const bool Config::is_gcno(string file_name)
{
   // chop up file name and path leave only extension
   file_name.erase(0, file_name.length() - 5);

   // input has GCNO extension
   if (file_name.compare(".gcno") == 0)
   {
      return true;
   }
   return false;
}

/// @brief
/// Determine whether input is a GCDA file or not 
///
/// @param file_name file path
///
/// @return bool
const bool Config::is_gcda(string file_name)
{
   // chop up file name and path leave only extension
   file_name.erase(0, file_name.length() - 5);
   // input is a gcda
   if (file_name.compare(".gcda") == 0)
   {
      return true;
   }
   return false;
}

/// @brief
/// Determine whether input is a C++ source file or not 
///
/// @param file_name file path
///
/// @return bool
const bool Config::is_cpp(string file_name)
{
   // chop up file name and path leave only extension
   file_name.erase(0, file_name.length() - 4);
   // input is a C++ source
   if (file_name.compare(".cpp") == 0)
   {
      return true;
   }
   return false;
}

/// @brief
/// Determine whether input is a C source file or not 
///
/// @param file_name file path
///
/// @return bool
const bool Config::is_c(string file_name)
{
   // chop up file name and path leave only extension
   file_name.erase(0, file_name.length() - 2);

   // input has .C extension
   if (file_name.compare(".c") == 0)
   {
      return true;
   }
   return false;
}

/// @brief
/// Determine whether input is a header file or not 
///
/// @param file_name file path
///
/// @return bool
const bool Config::is_header(string file_name)
{
   // remove file path and file name 
   file_name.erase(0, file_name.length() - 2);

   // file extension is ".h"
   if (file_name.compare(".h") == 0)
   {
      // header file
      return true;
   }
   // not header
   return false;
}

