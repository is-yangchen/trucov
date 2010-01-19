#ifndef SECRETGUI_H
#define SECRETGUI_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <assert.h>

struct Function;

struct Source_file 
{
   std::string percentage;
   std::string name;
   std::string path;
   std::vector<Function> functions;
};

struct Function
{
   std::string percentage;
   int lineno;
   std::string name;
   std::string report_path;
   std::string graph_path;
   std::string & source_path; 
};

// Populates sources with data
// Returns pass or failure
bool read_secret_gui(std::vector<Source_file> & sources)
{
   sources.clear();
   std::string data;

   std::ifstream in(".__secret_gui__");
   if (!in.is_open())
   {
      return false;
   }

   getline(in, data);
   while (data != "@@@__END_OF_EVERYTHING__@@@")
   {
      Source_file source;
      source.path = data;
      getline(in, source.percentage);
      getline(in, data);
      while (data != "@@@__END_OF_SOURCE__@@@")
      {
         Function fun;
         fun.name = data;

         getline(in, fun.report_path);
         getline(in, fun.graph_path);
         getline(in, data);
         fun.lineno = atoi( data.c_str() );
         getline(in, fun.percentage);
         getline(in, data);
         source.functions.push_back(fun);
      }
      getline(in, data);
      sources.push_back(source);
   }

   return true;
}

// find the data structure
Function * find_function( 
   const std::string & src_fun, 
   std::vector<Source_file> & sources )
{
   unsigned int found = src_fun.find_first_of(" "); 
   assert( found != std::string::npos );

   std::string src = src_fun.substr(0, found);
   std::string fun = src_fun.substr(found + 1, src_fun.size() - found);  

   // find src
   for (int i = 0; i < sources.size(); i++)
   {
      if (sources[i].name == src)
      {
         for (int j = 0; j < sources[i].functions.size(); j++)
         {
            if (sources[i].functions[j].name == src)
            {
               return &(sources[i].functions[j]);
            }
         }
      } 
   }

   return NULL;
}

#endif

