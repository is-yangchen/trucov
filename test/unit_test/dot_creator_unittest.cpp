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
///  @file
///  parser_unittest.cpp 
///
///  @brief
///  Tests the Parser class. 
///////////////////////////////////////////////////////////////////////////////

// TEST INCLUDE

#include <gtest/gtest.h>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

// SOURCE FILE 
#define protected public // Needed to test private methods.
#define private public

#include "dot_creator.h"
#include "selector.h"

#undef protected // Undo changes made to test private methods.
#undef private

namespace PO = boost::program_options;
using namespace boost;
using namespace std;

bool boost_cmd_handler4(int ac, char* av[])
{
   PO::variables_map vm;

   Config & config = Config::get_instance();
   
    try {
        PO::options_description cmd_options("Command Line Options");
        cmd_options.add_options()
        ((config.get_flag_name(Config::Working_directory) + ",c").c_str(), PO::value<string>(), 
             "specify the directory where Trucov will run.\n")
        ((config.get_flag_name(Config::Output) + ",o").c_str(), PO::value<string>(), 
            "specify the output directory where trucov should use.\n")
        (Config::Selection.c_str(), PO::value< vector<string> >(), 
            "specify the function(s) in all source files.\n")
        ((config.get_flag_name(Config::Debug) + ",d").c_str(), 
            "printout debug and testing information.\n")
        (config.get_flag_name(Config::Brief).c_str(), 
            "only prints function summaries in coverage reports.\n")
        (config.get_flag_name(Config::Only_missing).c_str(), 
            "only outputs coverage information on functions with less than 100% coverage.\n")
        (config.get_flag_name(Config::Show_fake).c_str(), 
            "outputs coverage information on fake arcs and fake blocks.\n")
        (config.get_flag_name(Config::Hide_fake).c_str(), 
            "hides coverage information on fake arcs and fake blocks.\n")
        (config.get_flag_name(Config::Mangle).c_str(), 
            "outputs the mangled names of functions.\n")
        (config.get_flag_name(Config::Demangle).c_str(), 
            "outputs the demangled names of functions.\n")
        (config.get_flag_name(Config::Partial_match).c_str(), 
            "specify selection will be matched partially.\n")
        (config.get_flag_name(Config::Full_match).c_str(), 
            "specify selection will be matched fully.\n")
        (config.get_flag_name(Config::Signature_match).c_str(), 
            "specifies selection for functions shall be matched using the full function signature.\n")
        (config.get_flag_name(Config::Config_file).c_str(), po::value<string>(), 
            "specify the config file.\n")
        ;

        PO::options_description config_options("Config File Options");
        config_options.add_options()
        ("default-command", PO::value<string>(), 
            "the command used when no command is specified.\n")
        ("default-selection", PO::value< vector<string> >(), 
            "the selection used when no selection is specified.\n")
        ;

        PO::options_description cmd_config("");
        cmd_config.add_options()
       ((Config::Build_directory + ",b").c_str(), PO::value< vector<string> >(),
            "specify the root directorie(s) of the GCNO and GCDA files.\n")
       ((Config::Source_directory + ",s").c_str(), PO::value< vector<string> >(),
            "specify the directorie(s) of the source files.\n")
       ((config.get_flag_name(Config::Cache_file) + ",f").c_str(), PO::value<string>(), 
            "specify the cache file to be used.\n")
        (config.get_flag_name(Config::And).c_str(), "requires all selection to match.\n")
        (config.get_flag_name(Config::Or).c_str(), "allows any selection to match.\n")
        (config.get_flag_name(Config::Revision_script).c_str(), PO::value<string>(),
            "specify the script to be used to get revision information for the source files.\n")
        (config.get_flag_name(Config::Render_format).c_str(), po::value<string>(),
            "specifiy the output format of graph files for the render commands.\n")
       (config.get_flag_name(Config::Per_source).c_str(), "creates output files per source.\n")
       (config.get_flag_name(Config::Per_function).c_str(), "creates output files per function.\n")
       (config.get_flag_name(Config::Sort_line).c_str(), "sorts functions by line number.\n")
       (config.get_flag_name(Config::Sort_name).c_str(), "sorts functions by name.\n")
       (config.get_flag_name(Config::Show_external_inline).c_str(), "shows line coverage information for inlined source code.\n")
       (config.get_flag_name(Config::Hide_external_inline).c_str(), "hidess line coverage information for inlined source code.\n")
        ;

        po::options_description main_options;
        main_options.add(cmd_options).add(config_options).add(cmd_config);
        
        PO::positional_options_description p;
        p.add(Config::Selection.c_str(), -1);

        PO::variables_map vm;
        PO::store(PO::command_line_parser(ac, av).options(main_options).positional(p).run(), vm);
        PO::notify(vm);

        // Get default config path.
        string config_path = getenv( "HOME" );
        config_path.append( "/.trucovrc" );
        ifstream config_stream;
        
        // If config file was specified.
        if (vm.count(config.get_flag_name(Config::Config_file).c_str())) 
        {
            config_stream.open(
                vm[config.get_flag_name(Config::Config_file).c_str()].as<string>().c_str());
            if (!config_stream) {
                cerr << "Config file doesn't exist.\n";
                return false;
            }
        }
        else // Use default config file.
        {
            config_stream.open( config_path.c_str() );;
        }

        // Attempt to 
        if ( config_stream )
        {
           // Read the whole file into a string
           stringstream ss;
           ss << config_stream.rdbuf();
           string file = ss.str();       

           // Split the file content
           char_separator<char> sep(" \n\r");
           tokenizer<char_separator<char> > tok(file, sep);
           vector<string> args;
           copy(tok.begin(), tok.end(), back_inserter(args));
           
           // Parse the file and store the options
           po::store(
              po::command_line_parser(args).options(main_options).positional(p).run(), 
              vm);
        }

        return config.initialize(vm, ac, av);  
   }
   catch (...)
   {
   }
    
   return false;
}

TEST( generate_files, default_single )
{
   int ac = 3;

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string proj_dir = "../test/resources/fast_protocol/";
   string argv0 = "./trucov";
   string argv1 = "dot";
   string argv2 = "--chdir=" + proj_dir;
   char * av[] = {&argv0[0], &argv1[0], &argv2[0] };
   bool res = boost_cmd_handler4(ac, av);
   Config & config = Config::get_instance();

   Selector & selector = Selector::get_instance();
   selector.select( config.get_selection() );
   Parser & ref_parser = Parser::get_instance();

   bool result = ref_parser.parse_all();

   EXPECT_TRUE( result );
   
   Dot_creator dc;
   dc.generate_file( ref_parser );
}

TEST( generate_files, default_multiple )
{
   int ac = 3;

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string proj_dir = "../test/resources/fast_protocol/";
   string argv0 = "./trucov";
   string argv1 = "dot";
   string argv2 = "--chdir=" + proj_dir;
   char * av[] = {&argv0[0], &argv1[0], &argv2[0] };
   bool res = boost_cmd_handler4(ac, av);
   Config & config = Config::get_instance();

   Selector & selector = Selector::get_instance();
   selector.select( config.get_selection() );
   Parser & ref_parser = Parser::get_instance();

   bool result = ref_parser.parse_all();

   EXPECT_TRUE( result );
   
   Dot_creator dc;
   dc.generate_files( ref_parser );
}

TEST( generate_file, show_all_single )
{
   int ac = 9;

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string proj_dir = "../test/resources/fast_protocol/";
   string argv0 = "./trucov";
   string argv1 = "graph";
   string argv2 = "--chdir=" + proj_dir;
   string argv3 = "--show-fake";
   string argv4 = "--show-external-inline";
   string argv5 = "-d";
   string argv6 = "plan";
   string argv7 = "--render-format=pdf";
   string argv8 = "-o blah.dot";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0], &argv3[0], &argv4[0],
                  &argv5[0], &argv6[0], &argv7[0], &argv8[0] };
   bool res = boost_cmd_handler4(ac, av);
   Config & config = Config::get_instance();

   Selector & selector = Selector::get_instance();
   selector.select( config.get_selection() );
   Parser & ref_parser = Parser::get_instance();

   bool result = ref_parser.parse_all();

   EXPECT_TRUE( result );
   
   Dot_creator dc;
   dc.generate_file( ref_parser );
}

TEST( generate_files, dot_per_function )
{
   int ac = 4;

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string proj_dir = "../test/resources/fast_protocol/";
   string argv0 = "./trucov";
   string argv1 = "dot";
   string argv2 = "--chdir=" + proj_dir;
   string argv3 = "--per-function";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0], &argv3[0] };
   bool res = boost_cmd_handler4(ac, av);
   Config & config = Config::get_instance();

   Selector & selector = Selector::get_instance();
   selector.select( config.get_selection() );
   Parser & ref_parser = Parser::get_instance();

   bool result = ref_parser.parse_all();

   EXPECT_TRUE( result );
   
   Dot_creator dc;
   dc.generate_files( ref_parser );
}
