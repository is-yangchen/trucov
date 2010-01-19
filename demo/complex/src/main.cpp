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
///  @file main.cpp
///
///  @brief
///  Drives the selcov program. It will find all gcno and gcda files in the
///  project directory, pair the correct files together, and output a text
///  coverage and dump file for each source file.
///
///  Requirements Specification:
///     < http://selcov.eecs.wsu.edu/wiki/index.php/SRS >
///
///  Design Description: < Null >
///
///////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES

#include <fstream>
#include <cstdlib>

// PROJECT INCLUDES

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

// LOCAL INCLUDES

#include "command.h"
#include "config.h"
//#include "Utility_Manager.h"

// FORWARD REFERENCES

namespace PO = boost::program_options;

using namespace boost;
using std::string;

// FUNCTION PROTOTYPES

int boost_cmd_handler( int ac, char* av[] );
void print_help();
void cmd_help();
void config_help();
void cmd_config_help();

int boost_cmd_handler(int ac, char* av[])
{
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

        PO::variables_map vm;
        //PO::store(PO::parse_command_line(ac, av, desc), vm);
        PO::store(PO::command_line_parser(ac, av).options(main_options).positional(p).run(), vm);
        PO::notify(vm);

        if (vm.count("help")) 
        {
           print_help();
           // cout << cmd_options << "\n";
           cmd_help();
           // cout << cmd_config << "\n";
           cmd_config_help();
           return 1;
        }

        if (vm.count("help-config"))
        {
	   // cout << config_options << "\n";
           config_help();
           // cout << cmd_config << "\n";
           cmd_config_help();
           return 1;
        }

        if (vm.count("version"))
        {
           cout << "\nTrucov Test Coverage Analysis Tool\n"
                << "Version " << VERSION << "\n\n";
           return 1;
        }

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
                return 1;
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

        Config * config = Config::get_instance(vm, ac, av);
        
        // Initialize command.
        Command & command = Command::get_instance(); 
        command.do_command(config->get_command());
    }
    catch(exception& e) 
    {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) 
    {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}

void print_help()
{
    cout << "\nUsage: trucov [ command ] [ option... ] [ selection... ]\n";
    cout << "\nCommand                         Description\n";
    cout << "-------                         -----------\n";  
            cout << "status          Prints a coverage summary for each function to stdout.          \n\n";

            cout << "list            Print the name of functions to stdout.\n\n";

            cout << "report          Creates multiple text coverage files. Creates or overwrites\n";
            cout << "                a coverage file for each source file to the path\n";
            cout << "                'output_dir/file_path_and_name.trucov'\n\n";

            cout << "dot             Creates or overwrites a single dot file. The dot file will show\n";
            cout << "                the coverage control flow of all functions to the path    \n";
            cout << "                'output_dir/coverage.dot'.\n\n";

            cout << "dot_report      Creates multiple dot files. Creates or overwrites a dot file for\n";
            cout << "                each source file to the path 'file_path_and_name.dot'.\n\n";

            cout << "render          Creates or overwrites a single pdf file. The pdf file will show\n";
            cout << "                the control flow of all functions to the path\n";
            cout << "                'output_dir/coverage.pdf'.       \n\n";

            cout << "render_report   Creates multiple pdf files. Creates or overwrites a pdf file for\n";
            cout << "                each source file to the path 'out_dir/file_path_and_name.pdf'.\n\n";
}

void cmd_help()
{
  cout <<"Command Line Options:\n\n";

  cout <<" -h [ --help ]     produce general help message.\n\n";

  cout <<" --help-config     produce help for options.\n\n";

  cout <<" -c [ --chdir ]    specify the directory where Trucov will run.\n\n";

  cout <<" -o [ --outdir ]   specify the output directory where trucov should use.\n\n";
        
  cout <<" --selection       specify the function(s) in all source files.\n\n";

  cout <<" -v [ --version ]  prints out the working version number of Trucov.\n\n";

  cout <<" -d [ --debug ]    printout debug and testing information.\n\n";

  cout <<" --brief           only prints function summaries in coverage reports.\n\n";

  cout <<" --only-missing    only outputs coverage information on functions with less\n";
  cout <<"                   than 100% coverage.\n\n";

  cout <<" --show-fake       outputs coverage information on fake arcs and fake blocks.\n\n";

  cout <<" --hide-fake       hides coverage information on fake arcs and fake blocks.\n";
  cout <<"                   [ default ]\n\n";
     
  cout <<" --mangle          outputs the mangled names of functions.\n\n";

  cout <<" --demangle        outputs the demangled names of functions.[ default ]\n\n";
        
  cout <<" --partial-match   specify selection will be matched partially.[ default ]\n\n";

  cout <<" --full-match      specify selection will be matched fully.\n\n";

  cout <<" --signature-match specifies selection for functions shall be matched using the\n";
  cout <<"                   full function signature.\n\n";

  cout <<" --config-file     specify the config file.\n\n";
}

void config_help()
{
  cout <<"Config Options:\n\n";

  cout <<" --default-command    the command used when no command is specified.\n\n";

  cout <<" --default-selection  the selection used when no selection is specified.\n\n";
}

void cmd_config_help()
{
  cout <<"Command Line + Config Options:\n\n";

  cout <<" -b [ --builddir ]    specify the root directorie(s) of the GCNO and GCDA files.\n\n";

  cout <<" -s [ --srcdir ]      specify the directorie(s) of the source files.\n\n";

  cout <<" -f [ --cachefile ]   specify the cache file to be used.\n\n";

  cout <<" --and                requires all selection to match.\n\n";

  cout <<" --or                 allows any selection to match.\n\n";

  cout <<" --revision-script    specify the script to be used to get revision information \n";
  cout <<"                      for the source files.\n\n";

  cout <<" --render-format      specifiy the output format of graph files for the render\n";
  cout <<"                      commands.\n\n";
}

// MAIN FUNCTION
int main(int ac, char* av[])
{

#ifdef DEBUG
   cerr << "************ EXECUTING IN DEBUG MODE ************" << endl;
#endif

   boost_cmd_handler(ac, av);

   cout << std::flush;
   cerr << std::flush;
}




