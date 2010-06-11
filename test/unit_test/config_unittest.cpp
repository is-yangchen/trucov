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
#include "boost/filesystem.hpp" 

using namespace std;
namespace PO = boost::program_options;
using namespace boost;
using namespace boost::filesystem;

// SOURCE FILE

#define protected public // Needed to test private methods.
#define private public
#include "config.h"
#include "tru_utility.h"

#undef protected // Undo changes made to test private methods.
#undef private

bool boost_cmd_handler(int ac, char* av[])
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

TEST( Config, check_defaults)
{
    int ac = 1;

    Tru_utility* tru_utility = Tru_utility::get_instance();
    string original_path = tru_utility->get_cwd();
    string rel_proj_dir = "../test/resources/fast_protocol";
    string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
    string argv0 = "./trucov";
    char * av[] = {&argv0[0]};

    bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

    Config & config = Config::get_instance();
   
   EXPECT_FALSE(config.get_flag_value(Config::Debug));      
   EXPECT_FALSE(config.get_flag_value(Config::Per_function));      
   EXPECT_TRUE(config.get_flag_value(Config::Per_source));      
   EXPECT_FALSE(config.get_flag_value(Config::Brief));      
   EXPECT_TRUE(config.get_flag_value(Config::Or));      
   EXPECT_FALSE(config.get_flag_value(Config::And));      
   EXPECT_FALSE(config.get_flag_value(Config::Only_missing));      
   EXPECT_FALSE(config.get_flag_value(Config::Show_fake));      
   EXPECT_TRUE(config.get_flag_value(Config::Hide_fake));      
   EXPECT_FALSE(config.get_flag_value(Config::Mangle));      
   EXPECT_TRUE(config.get_flag_value(Config::Demangle));      
   EXPECT_TRUE(config.get_flag_value(Config::Partial_match));      
   EXPECT_FALSE(config.get_flag_value(Config::Full_match));      
   EXPECT_FALSE(config.get_flag_value(Config::Signature_match));      
   EXPECT_TRUE(config.get_flag_value(Config::Sort_line));      
   EXPECT_FALSE(config.get_flag_value(Config::Sort_name));      
   EXPECT_FALSE(config.get_flag_value(Config::Show_external_inline));      
   EXPECT_TRUE(config.get_flag_value(Config::Hide_external_inline));      
  
   fs::path local( "." );
   string cwd = system_complete(local).directory_string();
   cwd.erase(cwd.end() - 1);
 
   EXPECT_STREQ("svg", config.get_flag_value(Config::Render_format).c_str());
   EXPECT_STREQ("", config.get_flag_value(Config::Revision_script).c_str());
   EXPECT_STREQ(cwd.c_str(), config.get_flag_value(Config::Output).c_str());
   EXPECT_STREQ(cwd.c_str(), config.get_flag_value(Config::Working_directory).c_str());
   EXPECT_STREQ("", config.get_flag_value(Config::Config_file).c_str());

   EXPECT_STREQ("status", config.get_command().c_str());
   EXPECT_EQ(0, config.get_selection().size());
}

TEST( Config, check_single_flags)
{
    int ac = 14;

    Tru_utility* tru_utility = Tru_utility::get_instance();
    string original_path = tru_utility->get_cwd();
    string rel_proj_dir = "../test/resources/F1/";
    string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
    string argv0 = "./trucov";
    string argv1 = "report";
    string argv2 = "--chdir=" + proj_dir;
    string argv3 = "--output=" + proj_dir;
    string argv4 = "--builddir=" + proj_dir;
    string argv5 = "--srcdir=" + proj_dir;
    string argv6 = "--debug";
    string argv7 = "--brief";
    string argv8 = "--selection=test";
    string argv9 = "--selection=test2";
    string argv10 = "--only-missing";
    string argv11 = "--revision-script=../../../scripts/svn_revision_script.sh"; 
    string argv12 = "--signature-match";
    string argv13 = "--render-format=pdf";
    char * av[] = {&argv0[0], &argv1[0], &argv2[0], &argv3[0], &argv4[0], &argv5[0],
                   &argv6[0], &argv7[0], &argv8[0], &argv9[0], &argv10[0], &argv11[0],
                   &argv12[0], &argv13[0] };
    bool res = boost_cmd_handler(ac, av);
    Config & config = Config::get_instance();

    EXPECT_TRUE(res);

   // Set Values
   EXPECT_TRUE(config.get_flag_value(Config::Debug));      
   EXPECT_TRUE(config.get_flag_value(Config::Brief));      
   EXPECT_TRUE(config.get_flag_value(Config::Only_missing));      
   EXPECT_TRUE(config.get_flag_value(Config::Signature_match));      
   EXPECT_STREQ("pdf", config.get_flag_value(Config::Render_format).c_str());
   EXPECT_NE(0, config.get_flag_value(Config::Revision_script).size());
   
   // Defaults
   EXPECT_FALSE(config.get_flag_value(Config::Per_function));      
   EXPECT_TRUE(config.get_flag_value(Config::Per_source));      
   EXPECT_TRUE(config.get_flag_value(Config::Or));      
   EXPECT_FALSE(config.get_flag_value(Config::And));      
   EXPECT_FALSE(config.get_flag_value(Config::Show_fake));      
   EXPECT_TRUE(config.get_flag_value(Config::Hide_fake));      
   EXPECT_FALSE(config.get_flag_value(Config::Mangle));      
   EXPECT_TRUE(config.get_flag_value(Config::Demangle));      
   EXPECT_TRUE(config.get_flag_value(Config::Partial_match));      
   EXPECT_FALSE(config.get_flag_value(Config::Full_match));      
   EXPECT_TRUE(config.get_flag_value(Config::Sort_line));      
   EXPECT_FALSE(config.get_flag_value(Config::Sort_name));      
   EXPECT_FALSE(config.get_flag_value(Config::Show_external_inline));      
   EXPECT_TRUE(config.get_flag_value(Config::Hide_external_inline));      

   string cwd = proj_dir;   
   EXPECT_STREQ(cwd.c_str(), config.get_flag_value(Config::Output).c_str());
   EXPECT_STREQ(cwd.c_str(), config.get_flag_value(Config::Working_directory).c_str());
   EXPECT_STREQ("", config.get_flag_value(Config::Config_file).c_str());
   EXPECT_STREQ("report", config.get_command().c_str());

   EXPECT_EQ(2, config.get_selection().size());
   EXPECT_STREQ("test", config.get_selection()[0].c_str());
   EXPECT_STREQ("test2", config.get_selection()[1].c_str());

   EXPECT_EQ(1, config.get_build_files().size());
   EXPECT_STREQ((cwd + "test.gcno").c_str(), 
      config.get_build_files()[0].first.c_str());
   EXPECT_STREQ((cwd + "test.gcda").c_str(), 
      config.get_build_files()[0].second.c_str());

   EXPECT_EQ(13, config.get_source_files().size());
   EXPECT_STREQ((cwd + "test.cpp").c_str(), 
      config.get_source_files()[0].c_str());
}

TEST( Config, check_enable_disable_flags )
{
    int ac = 8;

    Tru_utility* tru_utility = Tru_utility::get_instance();
    string original_path = tru_utility->get_cwd();
    string rel_proj_dir = "../test/resources/F1/";
    string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
    string argv0 = "./trucov";
    string argv1 = "--or";
    string argv2 = "--show-fake"; 
    string argv3 = "--mangle";
    string argv4 = "--partial-match";
    string argv5 = "--sort-line";
    string argv6 = "--show-external-inline";
    string argv7 = "--per-function";

    char * av[] = {&argv0[0], &argv1[0], &argv2[0], &argv3[0], &argv4[0], &argv5[0],
                   &argv6[0], &argv7[0] };
    bool res = boost_cmd_handler(ac, av);
    Config & config = Config::get_instance();

    EXPECT_TRUE(res);

    EXPECT_TRUE( config.get_flag_value( Config::Or ) );
    EXPECT_FALSE( config.get_flag_value( Config::And ) );
    EXPECT_TRUE( config.get_flag_value( Config::Show_fake ) );
    EXPECT_FALSE( config.get_flag_value( Config::Hide_fake ) );
    EXPECT_TRUE( config.get_flag_value( Config::Mangle ) );
    EXPECT_FALSE( config.get_flag_value( Config::Demangle ) );
    EXPECT_TRUE( config.get_flag_value( Config::Partial_match ) );
    EXPECT_FALSE( config.get_flag_value( Config::Full_match ) );
    EXPECT_TRUE( config.get_flag_value( Config::Sort_line ) );
    EXPECT_FALSE( config.get_flag_value( Config::Sort_name ) );
    EXPECT_TRUE( config.get_flag_value( Config::Show_external_inline ) );
    EXPECT_FALSE( config.get_flag_value( Config::Hide_external_inline ) );
    EXPECT_TRUE( config.get_flag_value( Config::Per_function ) );
    EXPECT_FALSE( config.get_flag_value( Config::Per_source ) );

    string oargv0 = "./trucov";
    string oargv1 = "--and";
    string oargv2 = "--hide-fake"; 
    string oargv3 = "--demangle";
    string oargv4 = "--full-match";
    string oargv5 = "--sort-name";
    string oargv6 = "--hide-external-inline";
    string oargv7 = "--per-source";

    char * oav[] = {&oargv0[0], &oargv1[0], &oargv2[0], &oargv3[0], &oargv4[0], &oargv5[0],
                   &oargv6[0], &oargv7[0] };
    bool ores = boost_cmd_handler(ac, oav);

    EXPECT_TRUE(ores);

    EXPECT_FALSE( config.get_flag_value( Config::Or ) );
    EXPECT_TRUE( config.get_flag_value( Config::And ) );
    EXPECT_FALSE( config.get_flag_value( Config::Show_fake ) );
    EXPECT_TRUE( config.get_flag_value( Config::Hide_fake ) );
    EXPECT_FALSE( config.get_flag_value( Config::Mangle ) );
    EXPECT_TRUE( config.get_flag_value( Config::Demangle ) );
    EXPECT_FALSE( config.get_flag_value( Config::Partial_match ) );
    EXPECT_TRUE( config.get_flag_value( Config::Full_match ) );
    EXPECT_FALSE( config.get_flag_value( Config::Sort_line ) );
    EXPECT_TRUE( config.get_flag_value( Config::Sort_name ) );
    EXPECT_FALSE( config.get_flag_value( Config::Show_external_inline ) );
    EXPECT_TRUE( config.get_flag_value( Config::Hide_external_inline ) );
    EXPECT_FALSE( config.get_flag_value( Config::Per_function ) );
    EXPECT_TRUE( config.get_flag_value( Config::Per_source ) );
}

TEST( Config, check_flag_names)
{
    Config & config = Config::get_instance();

    EXPECT_STREQ("or", config.get_flag_name( Config::Or ).c_str());     
    EXPECT_STREQ("and", config.get_flag_name( Config::And ).c_str());     
    EXPECT_STREQ("show-fake", config.get_flag_name( Config::Show_fake ).c_str());     
    EXPECT_STREQ("hide-fake", config.get_flag_name( Config::Hide_fake ).c_str());     
    EXPECT_STREQ("mangle", config.get_flag_name( Config::Mangle ).c_str());     
    EXPECT_STREQ("demangle", config.get_flag_name( Config::Demangle ).c_str());     
    EXPECT_STREQ("partial-match", config.get_flag_name( Config::Partial_match ).c_str()); 
    EXPECT_STREQ("full-match", config.get_flag_name( Config::Full_match ).c_str());     
    EXPECT_STREQ("sort-line", config.get_flag_name( Config::Sort_line ).c_str());
    EXPECT_STREQ("sort-name", config.get_flag_name( Config::Sort_name ).c_str());    
    EXPECT_STREQ("show-external-inline", config.get_flag_name( Config::Show_external_inline ).c_str());
    EXPECT_STREQ("hide-external-inline", config.get_flag_name( Config::Hide_external_inline ).c_str());
    EXPECT_STREQ("per-function", config.get_flag_name( Config::Per_function ).c_str());     
    EXPECT_STREQ("per-source", config.get_flag_name( Config::Per_source ).c_str());     
    EXPECT_STREQ("debug", config.get_flag_name( Config::Debug ).c_str());     
    EXPECT_STREQ("brief", config.get_flag_name( Config::Brief ).c_str());     
    EXPECT_STREQ("only-missing", config.get_flag_name( Config::Only_missing ).c_str());     
    EXPECT_STREQ("signature-match", config.get_flag_name( Config::Signature_match ).c_str());     
    EXPECT_STREQ("render-format", config.get_flag_name( Config::Render_format ).c_str());     
    EXPECT_STREQ("revision-script", config.get_flag_name( Config::Revision_script ).c_str());     
    EXPECT_STREQ("output", config.get_flag_name( Config::Output ).c_str());     
    EXPECT_STREQ("chdir", config.get_flag_name( Config::Working_directory ).c_str());     
    EXPECT_STREQ("config-file", config.get_flag_name( Config::Config_file ).c_str());     
    EXPECT_STREQ("cache-file", config.get_flag_name( Config::Cache_file ).c_str());     
    EXPECT_STREQ("outfile", config.get_flag_name( Config::Outfile ).c_str());     
}

TEST( Config, initialize_command_dot)
{
   int ac = 2;
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = Config::Dot;
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   
   EXPECT_STREQ(config.get_command().c_str(), Config::Dot.c_str());      
}

TEST( Config, initialize_command_graph)
{
   int ac = 2;
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = Config::Graph;
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   
   EXPECT_STREQ(config.get_command().c_str(), Config::Graph.c_str());      
}

TEST(Config, bad_working_dir)
{
   int ac = 2;
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--chdir=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_FALSE(res);
}

TEST(Config, outdir_must_be_file_dot)
{
   int ac = 3;
  
   // assume 
   EXPECT_FALSE( exists("foodir") ); 
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "dot";
   string argv2 = "--output=foodir/";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_FALSE(res);
   EXPECT_FALSE( exists("foodir") ); 
}

TEST(Config, outdir_must_be_file_graph)
{
   int ac = 3;
  
   // assume 
   EXPECT_FALSE( exists("foodir") ); 
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "graph";
   string argv2 = "--output=foodir";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);
   EXPECT_FALSE( exists("foodir") ); 
}

TEST(Config, bad_output_dir)
{
   int ac = 2;
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--output=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);
   EXPECT_FALSE( exists("foobar") ); 
}

TEST(Config, create_output_dir)
{
   int ac = 3;
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "all_report";
   string argv2 = "--output=mynewdirectory";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);
   EXPECT_TRUE( exists("mynewdirectory") ); 
  
   // Remove directory created 
   remove_all("mynewdirectory");
}

TEST(Config, dont_create_output_dir_list)
{
   int ac = 3;
  
   // assume 
   EXPECT_FALSE( exists("foodir") );
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "list";
   string argv2 = "--output=foodir";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);
   EXPECT_FALSE( exists("foodir") );
}

TEST(Config, dont_create_output_dir_status)
{
   int ac = 3;
  
   // assume 
   EXPECT_FALSE( exists("foodir") );
   
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "status";
   string argv2 = "--output=foodir";
   char * av[] = {&argv0[0], &argv1[0], &argv2[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);
   EXPECT_FALSE( exists("foodir") );
}

TEST(Config, bad_rev_script)
{
   int ac = 2; 

   // Assume
   EXPECT_FALSE( exists("foobar") ); 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--revision-script=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_STREQ(config.get_flag_value(Config::Revision_script).c_str(), ""); 
}

TEST(Config, bad_build_dir_nonexistant)
{
   int ac = 2; 

   // Assume
   EXPECT_FALSE( exists("foobar") ); 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--builddir=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_EQ(0, config.get_build_files().size()); 
}

TEST(Config, bad_build_dir_regular_file)
{
   int ac = 2; 

   // Assume
   EXPECT_FALSE( exists("foobar") ); 
   ofstream file("foobar");
   file.close(); 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--builddir=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_EQ(0, config.get_build_files().size()); 

   remove_all("foobar");
}

TEST(Config, bad_src_dir_nonexistant)
{
   int ac = 2; 

   // Assume
   EXPECT_FALSE( exists("foobar") ); 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--srcdir=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_EQ(0, config.get_source_files().size()); 
}

TEST(Config, bad_build_src_regular_file)
{
   int ac = 2; 

   // Assume
   EXPECT_FALSE( exists("foobar") ); 
   ofstream file("foobar");
   file.close(); 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--srcdir=foobar";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_NE(0, config.get_build_files().size()); 

   remove_all("foobar");
}

TEST(Config, bad_render_format)
{
   int ac = 2; 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--render-format=dog";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_STREQ("svg", config.get_flag_value(Config::Render_format).c_str()); 
}

TEST(Config, good_render_format_svg)
{
   int ac = 2; 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--render-format=svg";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_STREQ("svg", config.get_flag_value(Config::Render_format).c_str()); 
}

TEST(Config, good_render_format_pdf)
{
   int ac = 2; 

   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string rel_proj_dir = "../test/resources/fast_protocol";
   string proj_dir = tru_utility->get_abs_path( rel_proj_dir );
   string argv0 = "./trucov";
   string argv1 = "--render-format=pdf";
   char * av[] = {&argv0[0], &argv1[0]};

   bool res = boost_cmd_handler(ac, av);
   EXPECT_TRUE(res);

   Config & config = Config::get_instance();
   EXPECT_STREQ("pdf", config.get_flag_value(Config::Render_format).c_str()); 
}


