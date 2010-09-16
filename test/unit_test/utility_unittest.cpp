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
///  @file record_unittest.cpp
///
///  @brief
///  Tests the record class. 
///
///  @remarks
///  Tests record.cpp and record.h.
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <gtest/gtest.h>


using namespace std;

// SOURCE FILE

#define protected public // Needed to test private methods.
#define private public
#include "tru_utility.h"

#undef protected // Undo changes made to test private methods.
#undef private

//////////////////////////////////////////////////////////////////////////////
// change_dir()
//////////////////////////////////////////////////////////////////////////////

TEST(Tru_utility, change_dir_get_cwd)
{
   // Initialize
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_dir = tru_utility->get_cwd();

   string rel_dir_path = "../test/resources/";
   string abs_dir_path = tru_utility->get_abs_path( rel_dir_path );

   // Absolute path && exists
   EXPECT_TRUE(tru_utility->change_dir( abs_dir_path ));
   EXPECT_EQ( abs_dir_path , tru_utility->get_cwd() );


   // Absolute path && does not exists
   rel_dir_path = "../test/resources/no_dir";
   abs_dir_path = tru_utility->get_abs_path( rel_dir_path );

   EXPECT_FALSE(tru_utility->change_dir( abs_dir_path ));

   // Relative path
   rel_dir_path = "../test/resources/";
   tru_utility->change_dir( original_dir );
   abs_dir_path = tru_utility->get_abs_path( rel_dir_path );

   EXPECT_TRUE(tru_utility->change_dir( rel_dir_path ));
   EXPECT_EQ( abs_dir_path , tru_utility->get_cwd() );

   // Relative path && does not exists
   tru_utility->change_dir( original_dir );
   rel_dir_path = "../test/resources/no_dir";
   EXPECT_FALSE(tru_utility->change_dir( rel_dir_path ));

   // Relative path using .. and .
   tru_utility->change_dir( "../test/resources/level1_1/level2_1" );
   string tmp_path = tru_utility->get_cwd() + "../../";
   EXPECT_TRUE(tru_utility->change_dir( "../.." ));
   EXPECT_EQ( tru_utility->clean_path(tmp_path) , tru_utility->get_cwd() );

   EXPECT_TRUE(tru_utility->change_dir( "./level1_1" ));

   tru_utility->change_dir( original_dir );
   EXPECT_FALSE(tru_utility->change_dir( "./../bla" ));

   tru_utility->change_dir( original_dir );
}

TEST( Tru_utility, read_dir )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_dir = tru_utility->get_cwd();
   string rel_path = "../test/resources";
   string abs_path = tru_utility->get_abs_path( rel_path );
   vector<string> result = tru_utility->read_dir( abs_path );

   string tmp1 = original_dir + "test/unit_test/resources/.svn";
   string tmp2 = original_dir + "test/unit_test/resources/level1_1";
   string tmp3 = original_dir + "test/unit_test/resources/level1_2";
/*
   EXPECT_EQ(tmp1, result[0]);
   EXPECT_EQ(tmp2, result[1]);
   EXPECT_EQ(tmp3, result[2]);
*/
}

TEST( Tru_utility, is_dir )
{
   string dir_path = "../test/resources";
   
   string file_path = "../test/resources/file.txt";

   Tru_utility* tru_utility = Tru_utility::get_instance();

   dir_path = tru_utility->get_abs_path( dir_path );

   file_path = tru_utility->get_abs_path( file_path );

   EXPECT_TRUE( tru_utility->is_dir( dir_path ) );
   EXPECT_FALSE( tru_utility->is_dir( file_path ) );
}

TEST( Tru_utility, make_dir )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_dir = tru_utility->get_cwd();
   string path = "../test/resources/level1_1/out_dir";

   path = tru_utility->get_abs_path( path );

   EXPECT_TRUE( tru_utility->make_dir( path ) );
   EXPECT_TRUE( tru_utility->change_dir( path ) );

   tru_utility->change_dir( original_dir ); 
   path = "../test/resources/level1_1/bla/file.txt";

   path = tru_utility->get_abs_path( path );

   EXPECT_FALSE( tru_utility->make_dir( path ) );

   tru_utility->change_dir( original_dir );   
}

TEST( Tru_utility, get_abs_path )
{
   string path = "../test/resources/";
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();
   string result = original_path + path;
   EXPECT_EQ( tru_utility->clean_path(result), tru_utility->get_abs_path( path ) );

   EXPECT_EQ( tru_utility->clean_path(result), tru_utility->get_abs_path( result ) );
}

TEST( Tru_utility, get_rel_path )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string original_path = tru_utility->get_cwd();

   string path = "../test/resources";
   path = tru_utility->get_abs_path( path );

   string path_two = "../test/resources/level1_2";
   path_two = tru_utility->get_abs_path( path_two );

   EXPECT_EQ( "../test/resources", tru_utility->get_rel_path( path ) );

   path = "../test/resources/level1_1";
   tru_utility->change_dir( path );


   EXPECT_EQ( "../level1_2", tru_utility->get_rel_path( path_two ) );

   tru_utility->change_dir( original_path );   
}

TEST( Tru_utility, get_file_name )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string path = "../test/resources/file.txt";
   path = tru_utility->get_abs_path( path );

   // file name with path   
   EXPECT_EQ("file.txt", tru_utility->get_filename( path ));
   path = "file.txt";

   // only file name
   EXPECT_EQ("file.txt", tru_utility->get_filename( path ));   
}

TEST( Tru_utility, strip_extension )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string path = "../test/resources/file.txt";
   string result = "../test/resources/file";
   path = tru_utility->get_abs_path( path );
   result = tru_utility->get_abs_path( result );   

   // file name with path   
   EXPECT_EQ(result, tru_utility->strip_extension( path ));
   path = "file.txt";

   // only file name
   EXPECT_EQ("file", tru_utility->strip_extension( path ));   
}

TEST( Tru_utility, get_basename )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string path = "../test/resources/file.txt";
   path = tru_utility->get_abs_path( path );
   EXPECT_EQ("file", tru_utility->get_basename( path ));
}

TEST( Tru_utility, genSelcovFileName )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string path = "../test/resources/file.txt";
   string ext = ".trucov";
   path = tru_utility->get_abs_path( path );
   EXPECT_EQ("file.trucov", tru_utility->genSelcovFileName( path, ext ));
}

TEST( Tru_utility, create_file_name )
{
   Tru_utility* tru_utility = Tru_utility::get_instance();
   string path = "../test/resources";
   string ext = ".svg";
   path = tru_utility->get_abs_path( path ) + "/coverage";   
   EXPECT_NE(0, tru_utility->create_file_name(path, ext).length());
   // cout << tru_utility->create_file_name(path, ext) << "\n";
   path = "../test/resources";
   path = tru_utility->get_abs_path( path );
//   cout << tru_utility->get_rel_path( path ) << "\n";
   EXPECT_NE(0, tru_utility->create_file_name(path, ext).length());
   //cout << tru_utility->create_file_name(path, ext) << "\n";   
}


TEST( Tru_utility, get_instance )
{
   // Verify setup.
   EXPECT_NE(NULL, (size_t)Tru_utility::tru_utility_ptr);

   Tru_utility * tru_utility = Tru_utility::get_instance();
   
   EXPECT_NE(NULL, (size_t)Tru_utility::tru_utility_ptr);
}

TEST( Tru_utility, clean_path )
{
   Tru_utility * tu = Tru_utility::get_instance();
   string abs1 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/../././";
   string abs2 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/../././.";
   string abs3 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/.././..";
   string abs4 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/.././../";
   string abs5 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/.././../f";
   string abs6 = "/fish/dog/cat/./././../../dinosaur/../fish/horse/../../cat/meow/see/.././../fish_filename_here";

   EXPECT_STREQ("/fish/cat/meow/", tu->clean_path(abs1).c_str());
   EXPECT_STREQ("/fish/cat/meow/", tu->clean_path(abs2).c_str());
   EXPECT_STREQ("/fish/cat/", tu->clean_path(abs3).c_str());
   EXPECT_STREQ("/fish/cat/", tu->clean_path(abs4).c_str());
   EXPECT_STREQ("/fish/cat/f", tu->clean_path(abs5).c_str());
   EXPECT_STREQ("/fish/cat/fish_filename_here", tu->clean_path(abs6).c_str());


}
