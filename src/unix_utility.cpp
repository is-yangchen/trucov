#include "Unix_Utility.h"

///////////////////////////////////////////////////////////////////////////
/// Constructor
///
/// INPUTS:
///  void
///////////////////////////////////////////////////////////////////////////
Unix_Utility::Unix_Utility()
{
}

///////////////////////////////////////////////////////////////////////////
/// Descructor
///
/// @brief
/// Cleans up after the Unix_Utility object
///////////////////////////////////////////////////////////////////////////
Unix_Utility::~Unix_Utility()
{
}

///////////////////////////////////////////////////////////////////////////
/// public change_dir
///
/// @brief : change working directory
///
/// INPUTS:
/// @param path : string of directory path 
///
/// OUTPUTS:
///  none
////////////////////////////////////////////////////////////////////////////
void Unix_Utility::change_dir(string path)
{
   int check = 0;
   //Change working directory
   check = chdir(path.c_str());
   //Change dir fail
   if (check < 0)
   {
      cout << "Unix_Utility::ERROR::change_dir Error\n";
   }
}

///////////////////////////////////////////////////////////////////////////
/// public open_dir
///
/// @brief : opens a directory stream and returns 
///          a pointer to the directory stream
///
/// INPUTS:
/// @param path : string of directory path
////////////////////////////////////////////////////////////////////////////
void Unix_Utility::open_dir(string path)
{
   //not implemented yet
   //shound return DIR* 
   
   if (fs::is_directory(path))
   {
      cout << "MESSAGE::Unix_utility.cpp:: It is dir.\n";
   }
   else
   {
      cout << "MESSAGE::Unix_utility.cpp:: It is not dir.\n";
   }
}

string Unix_Utility::get_cwd()
{
   fs::path full_path(fs::initial_path<fs::path>() );
   full_path = fs::system_complete( "." );
   string tmp_str =  full_path.file_string();
   tmp_str.erase( tmp_str.size() - 1 );
   return tmp_str;
}

vector<string> Unix_Utility::read_dir(string path)
{
   vector<string> result;
   fs::directory_iterator end_itr;
   for ( fs::directory_iterator dir_itr( path ) ; dir_itr != end_itr; ++dir_itr )
   {
      result.push_back( dir_itr->path().file_string() );
   }
   return result;
}

bool Unix_Utility::is_dir(string path)
{
   return fs::is_directory(path);
}
///////////////////////////////////////////////////////////////////////////
/// public get_rel_path
///
/// @brief : Change absolute path to relative path
///
/// INPUTS:
/// @param path : string of absolute directory path
///
/// @return string of relative directory path 
///          to the current working directory
////////////////////////////////////////////////////////////////////////////
string Unix_Utility::get_rel_path(string path)
{
   char buff[256];
   string tmp_str;

   //Get current working dir
   getcwd(buff, 256);

   //change char* to string
   tmp_str = buff;

   //length of input string must be less than or equal to the cwd
   if(path.length() <= tmp_str.length())
   {
      //print error message
      cout << "Unix_Utility::ERROR::Length Error\n";

      //return empty string
      return "";
   }

   //cwd path must be the preceding part of input string
   if(path.find(tmp_str) == 0)
   {
      //chop the cwd path
      path.erase(0, tmp_str.length() + 1);
   
      //return relative path
      return path;
   }
   else
   {
      //print error message
      cout << "Unix_Utility::ERROR::Path Error\n";
      //return empty string
      return "";
   }
}

///////////////////////////////////////////////////////////////////////////
///  public get_abs_path
///
///  @brief : Change relative path to absolute path
///
///  INPUTS:
///  @param path : string of directory path
///
///  OUTPUTS:
///   none
///
///  @return string of absolute path
////////////////////////////////////////////////////////////////////////////
string Unix_Utility::get_abs_path(string raw_path)
{
   char cwd[1024];
   char tmpCwd[1024];
   string path;
   DIR* dp;
   int chkdir;

   //if input string starts with '/' or '.'
   if ((raw_path[0] == '/') || (raw_path[0] == '.'))
   {
       //copy the string
       path = raw_path;
      
       //open input dir and make sure dir exists
       dp = opendir(path.c_str());

       //dir does not exist
       if (dp == NULL)
       {
           //print error massage
           cout << "Unix_Utility::ERROR:: Directory does not exist\n";
       }
       //dir exists
       else
       {
           //get current working dir and save it in "cwd"
           getcwd(cwd, 1024);
    
           //change working dir to input dir
           chkdir = chdir(path.c_str());
    
           //change dir success
           if (chkdir >= 0)
           {
               //get the absolute path
               getcwd(tmpCwd, 1024);
   
               //save in path
               path = tmpCwd;

               //change back to original working dir
               chdir(cwd);
           }
           //change dir fail
           else
           {
               //print error message
               cout << "Unix_Utility::ERROR:: Unable to convert abs path " << raw_path << endl;
           }
       }
       //close dir
       closedir(dp);
   }
   //
   else
   {
       //get current working dir
       getcwd(cwd, 1024);

       //concatenate cwd with "/"
       strcat(cwd, "/");

       //concatenate cwd with input path
       strcat(cwd, raw_path.c_str());

       //move cwd to path
       path = cwd;
   }

   //return absolute path
   return path;
}
