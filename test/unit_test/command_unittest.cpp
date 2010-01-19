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
///  @file config_unittest.cpp
///
///  @brief
///  Tests the config class. 
///
///  @remarks
///  Tests config.cpp and config.h.
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

using namespace std;
namespace PO = boost::program_options;
using namespace boost;
using std::string;

#define protected public // Needed to test private methods.
#define private public
#include "command.h"


#undef protected // Undo changes made to test private methods.
#undef private

//////////////////////////////////////////////////////////////////////////////
// get_instance()
//////////////////////////////////////////////////////////////////////////////
PO::variables_map boost_cmd(int ac, char* av[])
{
    PO::variables_map vm;
    try {

        PO::options_description cmd_options("Command Line Options");
        cmd_options.add_options()
        ("help,h", "produce general help message.\n")

        ("help-config", "produce help for options.\n")

        ("chdir,c", PO::value<string>(), 
         "specify the directory where Trucov will run.\n")

        ("outdir,o", PO::value<string>(), 
         "specify the output directory where trucov should use.\n")
        
        ("selection", PO::value< vector<string> >(), 
        "specify the function(s) in all source files.\n")

        ("version,v", "prints out the working version number of Trucov.\n")

	("debug,d", "printout debug and testing information.\n")

	("brief", "only prints function summaries in coverage reports.\n")

        ("only-missing", "only outputs coverage information on functions with less than 100% coverage.\n")

        ("show-fake", "outputs coverage information on fake arcs and fake blocks.\n")

        ("hide-fake", "hides coverage information on fake arcs and fake blocks.\n")
       
        ("mangle", "outputs the mangled names of functions.\n")

        ("demangle", "outputs the demangled names of functions.\n")
        
        ("partial-match", "specify selection will be matched partially.\n")

        ("full-match", "specify selection will be matched fully.\n")

        ("signature-match", "specifies selection for functions shall be matched using the full function signature.\n")

        ("config-file", po::value<string>(), 
         "specify the config file.\n")

        ;

        PO::options_description config_options("Config File Options");
        config_options.add_options()
        ("default-command", PO::value<string>(), "the command used when no command is specified.\n")

        ("default-selection", PO::value< vector<string> >(), "the selection used when no selection is specified.\n")

        ;

        PO::options_description cmd_config("");
        cmd_config.add_options()
	("builddir,b", PO::value< vector<string> >(), 
         "specify the root directorie(s) of the GCNO and GCDA files.\n")

	("srcdir,s", PO::value< vector<string> >(), 
         "specify the directorie(s) of the source files.\n")

	("cachefile,f", PO::value<string>(), 
         "specify the cache file to be used.\n")

        ("and", "requires all selection to match.\n")

        ("or", "allows any selection to match.\n")

        ("revision-script", PO::value<string>(), 
         "specify the script to be used to get revision information for the source files.\n")

        ("render-format", po::value<string>(), "specifiy the output format of graph files for the render commands.\n")

        ;

        po::options_description main_options;
        main_options.add(cmd_options).add(config_options).add(cmd_config);
        
        PO::positional_options_description p;
        p.add("selection", -1);


        //PO::store(PO::parse_command_line(ac, av, desc), vm);
        PO::store(PO::command_line_parser(ac, av).options(main_options).positional(p).run(), vm);
        PO::notify(vm);


        // Get default config path.
        string config_path = getenv( "HOME" );
        config_path.append( "/.trucovrc" );
        ifstream config_stream;
        
        // If config file was specified.
        if (vm.count("config-file")) 
        {
            config_stream.open(vm["config-file"].as<string>().c_str());
            if (!config_stream) {
                cerr << "Config file doesn't exist.\n";
                //return 1;
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
/*
        Config * config = Config::get_instance(vm, ac, av);
        
        // Initialize command.
        Command & command = Command::get_instance(); 
        command.do_command(config->get_command());    */
    }
    catch(std::exception & e) 
    {
        cerr << "error: " << e.what() << "\n";
        //return 1;
    }
    catch(...) 
    {
        cerr << "Exception of unknown type!\n";
    }

    return vm;
}


void initialize_config()
{
    int ac = 4;

    Tru_utility* tru_utility = Tru_utility::get_instance();
    string original_path = tru_utility->get_cwd();
    string rel_proj_dir = "../test/resources/BST/";
    string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
    string argv0 = "./trucov";
    string argv1 = "--chdir=" + proj_dir;
    string argv2 = "insert";
    string argv3 = "Bst";
    char * av[] = {&argv0[0], &argv1[0], &argv2[0], &argv3[0]};
    //char * av[] = {&argv0[0], &argv1[0]};

    PO::variables_map vm = boost_cmd(ac, av);

    Config & config = Config::get_instance();
    config.initialize(vm, ac, av);
}

TEST( Command, get_instance )
{
   // Verify setup.
   EXPECT_EQ(NULL, (int)Command::instance_ptr);

   Command & command = Command::get_instance();
   
   EXPECT_NE(NULL, (int)Command::instance_ptr);
}

TEST( Command, do_status )
{
    initialize_config();
    Command & command = Command::get_instance();   
    EXPECT_EQ(0, command.do_status() );
}

TEST( Command, do_list )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_list() );
}

TEST( Command, do_report )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_report() );
}

TEST( Command, do_dot )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_dot() );
}

TEST( Command, do_dot_report )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_dot_report() );
}

TEST( Command, do_render )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_render() );
}

TEST( Command, do_render_report )
{
   Command & command = Command::get_instance();   
   EXPECT_EQ(0, command.do_render_report() );
}

TEST( Command, do_all_report )
{
/*   Command & command = Command::get_instance();
   EXPECT_EQ(0, command.do_all_report() );
*/
}

TEST( Command, do_command )
{
   string cmd = "list";
   Command & command = Command::get_instance();
//   EXPECT_TRUE( command.do_command( cmd ) );

   cmd = "nocommand";
   EXPECT_FALSE( command.do_command( cmd) ); 
}
