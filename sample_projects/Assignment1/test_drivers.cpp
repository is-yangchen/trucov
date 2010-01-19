// William Reinhardt
// Cpt S 422 Fall '09
// Assignment 1

// LOCAL INCLUDES

#include "test_drivers.h"

// USING STATEMENTS

using std::cout;
using std::cin;

void test_menu( void )
{
   unsigned selection;

   do {
      cout << "BINARY SEARCH TREE TEST MENU:\n";
      cout << "1.) INSERT()\n";
      cout << "2.) REMOVE()\n";
      cout << "3.) SEARCH()\n";
      cout << "4.) MAXIMUM()\n";
      cout << "5.) MINIMUM()\n";
      cout << "6.) IS_EMPTY()\n";
      cout << "7.) PRINT_PREORDER_TRAVERSAL()\n";
      cout << "8.) PRINT_INORDER_TRAVERSAL()\n";
      cout << "9.) PRINT_POSTORDER_TRAVERSAL()\n";
      cout << "10.) RUN ALL TESTS\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN OPERATION TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            insert();
            break;
         case 2:
            remove();
            break;
         case 3:
            search();
            break;
         case 4:
            maximum();
            break;
         case 5:
            minimum();
            break;
         case 6:
            is_empty();
            break;
         case 7:
            print_preorder_traversal();
            break;
         case 8:
            print_inorder_traversal();
            break;
         case 9:
            print_postorder_traversal();
            break;
         case 10:
            run_all_tests();
            break;
         default:
            break;
      }

   } while ( selection != 0 );
}

void insert( void )
{
   unsigned selection;

   do {
      cout << "\nINSERT() TEST CASES:\n";
      cout << "1.) CASE 1: NEW STUDENT INTO EMPTY TREE\n";
      cout << "2.) CASE 2: NEW STUDENT HAS ID < ANOTHER STUDENT IN TREE\n";
      cout << "3.) CASE 3: NEW STUDENT HAS ID > ANOTHER STUDENT IN TREE\n";
      cout << "4.) CASE 4: NEW STUDENT HAS ID = ANOTHER STUDENT IN TREE\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN INSERT CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            insert_case_1();
            break;
         case 2:
            insert_case_2();
            break;
         case 3:
            insert_case_3();
            break;
         case 4:
            insert_case_4();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void insert_case_1( void )
{
   cout << "TESTING INSERT() CASE 1\n";
   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void insert_case_2( void )
{
   cout << "TESTING INSERT() CASE 2\n";
   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   Student s2( "Jacob Mills", 1, "Junior" );

   BST.insert( s1 );
   BST.insert( s2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void insert_case_3( void )
{
   cout << "TESTING INSERT() CASE 3\n";
   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   Student s2( "Jacob Mills", 1, "Junior" );
   Student s3( "Matthew Miller", 3, "Senior" );

   BST.insert( s1 );
   BST.insert( s2 );
   BST.insert( s3 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void insert_case_4( void )
{
   cout << "TESTING INSERT() CASE 4\n";
   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );

   BST.insert( s1 );
   BST.insert( s1 ); 
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove( void )
{
   unsigned selection;

   do {
      cout << "\nREMOVE() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE DOESN'T CONTAIN MATCHING NODE ID WHERE ID > EXISTING ID\n";
      cout << "3.) CASE 3: TREE DOESN'T CONTAIN MATCHING NODE ID WHERE ID < EXISTING ID\n";
      cout << "4.) CASE 4: TREE CONTAINS MATCHING NODE ID AND NODE IS A LEAF NODE\n";
      cout << "5.) CASE 5: TREE CONTAINS MATCHING NODE ID AND NODE HAS A LEFT LEAF\n";
      cout << "6.) CASE 6: TREE CONTAINS MATCHING NODE ID AND NODE HAS A RIGHT LEAF\n";
      cout << "7.) CASE 7: TREE CONTAINS MATCHING NODE ID AND NODE HAS LEFT AND RIGHT LEAVES\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN REMOVE CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            remove_case_1();
            break;
         case 2:
            remove_case_2();
            break;
         case 3:
            remove_case_3();
            break;
         case 4:
            remove_case_4();
            break;
         case 5:
            remove_case_5();
            break;
         case 6:
            remove_case_6();
            break;
         case 7:
            remove_case_7();
         default:
            break;
      }
   } while ( selection != 0 );
}

void remove_case_1( void )
{
   cout << "TESTING REMOVE() CASE 1\n";
   Binary_search_tree BST;

   BST.remove( 1 );
}

void remove_case_2( void )
{
   cout << "TESTING REMOVE() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 2\n";
   BST.remove( 2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove_case_3( void )
{
   cout << "TESTING REMOVE() CASE 3\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 1\n";
   BST.remove( 1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove_case_4( void )
{
   cout << "TESTING REMOVE() CASE 4\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 1\n";
   BST.remove( 1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove_case_5( void )
{
   cout << "TESTING REMOVE() CASE 5\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   Student s2( "Jacob Mills", 1, "Junior" );
   BST.insert( s1 );
   BST.insert( s2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 2\n";
   BST.remove( 2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove_case_6( void )
{
   cout << "TESTING REMOVE() CASE 6\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   Student s2( "Jacob Mills", 2, "Junior" );
   BST.insert( s1 );
   BST.insert( s2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 1\n";
   BST.remove( 1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void remove_case_7( void )
{
   cout << "TESTING REMOVE() CASE 7\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   Student s2( "Jacob Mills", 1, "Junior" );
   Student s3( "Matthew Miller", 3, "Senior" );
   Student s4( "Dale Cox", 4, "Senior" );
   BST.insert( s1 );
   BST.insert( s2 );
   BST.insert( s4 );
   BST.insert( s3 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to remove ID 2\n";
   BST.remove( 2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
}

void search( void )
{
   unsigned selection;

   do {
      cout << "\nSEARCH() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: A MATCHING NODE ID IS FOUND\n";
      cout << "3.) CASE 3: SOUGHT ID IS GREATER THAN A NODE ID\n";
      cout << "4.) CASE 4: SOUGHT ID IS LESS THAN A NODE ID\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN SEARCH CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            search_case_1();
            break;
         case 2:
            search_case_2();
            break;
         case 3:
            search_case_3();
            break;
         case 4:
            search_case_4();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void search_case_1( void )
{
   cout << "TESTING SEARCH() CASE 1\n";

   Binary_search_tree BST;

   cout << "Searching for ID 1 in empty tree\n";
   BST.search( 1 );
}

void search_case_2( void )
{
   cout << "TESTING SEARCH() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Searching for ID 1\n";
   BST.search( 1 );
}

void search_case_3( void )
{
   cout << "TESTING SEARCH() CASE 3\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Searching for ID 2\n";
   BST.search( 2 );
}

void search_case_4( void )
{
   cout << "TESTING SEARCH() CASE 4\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Searching for ID 1\n";
   BST.search( 1 );
}

void maximum( void )
{
   unsigned selection;

   do {
      cout << "\nMAXIMUM() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE HAS A SINGLE NODE\n";
      cout << "3.) CASE 3: TREE HAS A RIGHT BRANCH OF HEIGHT >= 2\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN MAXIMUM CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            maximum_case_1();
            break;
         case 2:
            maximum_case_2();
            break;
         case 3:
            maximum_case_3();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void maximum_case_1( void )
{
   cout << "TESTING MAXIMUM() CASE 1\n";

   Binary_search_tree BST;

   cout << "Attempting to find the maximum ID in an empty tree\n";
   BST.maximum();
}

void maximum_case_2( void )
{
   cout << "TESTING MAXIMUM() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to find the maximum ID in tree\n";
   BST.maximum();
}

void maximum_case_3( void )
{
   cout << "TESTING MAXIMUM() CASE 3\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   Student s2( "Jacob Mills", 2, "Junior" );
   BST.insert( s1 );
   BST.insert( s2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to find the maximum ID in tree\n";
   BST.maximum();
}

void minimum( void )
{
   unsigned selection;

   do {
      cout << "\nMINIMUM() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE HAS A SINGLE NODE\n";
      cout << "3.) CASE 3: TREE HAS A LEFT BRANCH OF HEIGHT >= 2\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN MINIMUM CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            minimum_case_1();
            break;
         case 2:
            minimum_case_2();
            break;
         case 3:
            minimum_case_3();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void minimum_case_1( void )
{
   cout << "TESTING MINIMUM() CASE 1\n";

   Binary_search_tree BST;

   cout << "Attempting to find the minimum ID in empty tree\n";
   BST.minimum();
}

void minimum_case_2( void )
{
   cout << "TESTING MINIMUM() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to find the minimum ID in tree\n";
   BST.minimum();
}

void minimum_case_3( void )
{
   cout << "TESTING MINIMUM() CASE 3\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 2, "Senior" );
   Student s2( "Jacob Mills", 1, "Junior" );
   BST.insert( s1 );
   BST.insert( s2 );
   cout << "Print in order:\n";
   BST.print_inorder_traversal();
   cout << "Attempting to find the minimum ID in tree\n";
   BST.minimum();
}

void is_empty( void )
{
   unsigned selection;

   do {
      cout << "\nIS_EMPTY() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EITHER EMPTY OR NOT\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN IS_EMPTY CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            is_empty_case_1();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void is_empty_case_1( void )
{
   cout << "TESTING IS_EMPTY() CASE 1\n";

   Binary_search_tree BST;

   cout << "Testing if tree is empty\n";
   BST.is_empty();
}

void print_preorder_traversal( void )
{
   unsigned selection;

   do {
      cout << "\nPRINT_PREORDER_TRAVERSAL() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE HAS >= 1 NODES\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN PRINT_PREORDER_TRAVERSAL CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            print_preorder_traversal_case_1();
            break;
         case 2:
            print_preorder_traversal_case_2();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void print_preorder_traversal_case_1( void )
{
   cout << "TESTING PRINT_PREORDER_TRAVERSAL() CASE 1\n";

   Binary_search_tree BST;

   BST.print_preorder_traversal();
}

void print_preorder_traversal_case_2( void )
{
   cout << "TESTING PRINT_PREORDER_TRAVERSAL() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   BST.print_preorder_traversal();
}

void print_inorder_traversal( void )
{
   unsigned selection;

   do {
      cout << "\nPRINT_INORDER_TRAVERSAL() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE HAS >= 1 NODES\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN PRINT_INORDER_TRAVERSAL CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            print_inorder_traversal_case_1();
            break;
         case 2:
            print_inorder_traversal_case_2();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void print_inorder_traversal_case_1( void )
{
   cout << "TESTING PRINT_INORDER_TRAVERSAL() CASE 1\n";

   Binary_search_tree BST;

   BST.print_inorder_traversal();
}

void print_inorder_traversal_case_2( void )
{
   cout << "TESTING PRINT_INORDER_TRAVERSAL() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   BST.print_inorder_traversal();
}

void print_postorder_traversal( void )
{
   unsigned selection;

   do {
      cout << "\nPRINT_POSTORDER_TRAVERSAL() TEST CASES:\n";
      cout << "1.) CASE 1: TREE IS EMPTY\n";
      cout << "2.) CASE 2: TREE HAS >= 1 NODES\n";
      cout << "0.) QUIT\n\n";
      cout << "ENTER AN PRINT_POSTORDER_TRAVERSAL CASE TO TEST: ";

      cin >> selection;

      switch ( selection )
      {
         case 1:
            print_postorder_traversal_case_1();
            break;
         case 2:
            print_postorder_traversal_case_2();
            break;
         default:
            break;
      }
   } while ( selection != 0 );
}

void print_postorder_traversal_case_1( void )
{
   cout << "TESTING PRINT_POSTORDER_TRAVERSAL() CASE 1\n";

   Binary_search_tree BST;

   BST.print_postorder_traversal();
}

void print_postorder_traversal_case_2( void )
{
   cout << "TESTING PRINT_POSTORDER_TRAVERSAL() CASE 2\n";

   Binary_search_tree BST;

   Student s1( "William Reinhardt", 1, "Senior" );
   BST.insert( s1 );
   BST.print_postorder_traversal();
}

void run_all_tests( void )
{
   insert_case_1();
   insert_case_2();
   insert_case_3();
   insert_case_4();

   remove_case_1();
   remove_case_2();
   remove_case_3();
   remove_case_4();
   remove_case_5();
   remove_case_6();
   remove_case_7();

   search_case_1();
   search_case_2();
   search_case_3();
   search_case_4();

   maximum_case_1();
   maximum_case_2();
   maximum_case_3();

   minimum_case_1();
   minimum_case_2();
   minimum_case_3();

   is_empty_case_1();

   print_preorder_traversal_case_1();
   print_preorder_traversal_case_2();

   print_inorder_traversal_case_1();
   print_inorder_traversal_case_2();

   print_postorder_traversal_case_1();
   print_postorder_traversal_case_2();
}
