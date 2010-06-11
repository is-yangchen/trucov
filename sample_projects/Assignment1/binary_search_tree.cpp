// William Reinhardt
// Cpt S 422 Fall '09
// Assignment 1

///////////////////////////////////////////////////////////////////////////////
//  file: binary_search_tree.cpp
//
//  Implements the methods of the Binary search tree class
///////////////////////////////////////////////////////////////////////////////

// HEADER FILE

#include "binary_search_tree.h"

// USING STATEMENTS

using std::cout;
using std::endl;

// Inserts a student object onto the tree
void Binary_search_tree::insert( const Student & student )
{
   insert ( student, m_root );
}

// Traverses the tree until the appropriate node location is found and then
// creates a new node with the passed in student object
void Binary_search_tree::insert( const Student & student, Binary_node * & t )
{
   if ( NULL == t )
   {
      t = new Binary_node( student );
   }
   else if ( student.m_id < t->data.m_id )
   {
      insert( student, t->left );
   }
   else if ( student.m_id > t->data.m_id )
   {
      insert(student, t->right );
   }
   else
   {
      cout << "Student ID #" << student.m_id << " already exists!\n";
   }
}

// Removes the node with the passed in student ID if it exists
void Binary_search_tree::remove( const unsigned id )
{
   remove( id, m_root );
}

// Traverses the tree and removes the node with the passed in student id
// if found
void Binary_search_tree::remove( const unsigned id, Binary_node * & t )
{
   if ( NULL != t )
   {
      if ( id == t->data.m_id )
      {
         if ( NULL != t->left && NULL != t->right )
         {
            Binary_node * tmp = t->right;

            while ( NULL != tmp->left )
            {
               tmp = tmp->left;
            }

            t->data.m_name = tmp->data.m_name;
            t->data.m_id = tmp->data.m_id;
            t->data.m_class = tmp->data.m_class;
            remove( tmp->data.m_id, t->right );
         }
         else if ( NULL != t->left )
         {
            Binary_node * tmp = t;
            t = t->left;
            delete tmp;
            tmp = NULL;
         }
         else if ( NULL != t->right )
         {
            Binary_node * tmp = t;
            t = t->right;
            delete tmp;
            tmp = NULL;
         }
         else
         {
            delete t;
            t = NULL;
         }
      }
      else if ( id < t->data.m_id )
      {
         remove( id, t->left );
      }
      else
      {
         remove( id, t->right );
      }
   }
}

// Searches the tree for a node w/ the specified id
void Binary_search_tree::search( const unsigned id )
{
   search( id, m_root );
}

// Searches tree for a node w/ the specified id
void Binary_search_tree::search( const unsigned id,
   Binary_node * t )
{
   if ( NULL != t )
   {
      if ( id == t->data.m_id )
      {
         t->data.print();
      }
      else if ( id < t->data.m_id )
      {
         search( id, t->left );
      }
      else
      {
         search( id, t->right );
      }
   }
   else
   {
      cout << "ID #" << id
           << " does not exist.\n";
   }
}

// Finds the node w/ the maximum student ID
void Binary_search_tree::maximum( void ) const
{
   if ( ! is_empty() )
   {
      const Binary_node * node = maximum( m_root );
      node->data.print();
   }
   else
   {
      cout << "The tree is empty.\n";
   }
}

// Traverses tree along right branch and returns the node with the
// maximum student ID
const Binary_node * Binary_search_tree::maximum( Binary_node * t ) const
{
   while ( NULL != t->right )
   {
      t = t->right;
   }

   return t;
}

// Finds the node w/ the minimum student ID
void Binary_search_tree::minimum( void ) const
{
   if ( ! is_empty() )
   {
      const Binary_node * node = minimum( m_root );
      node->data.print();
   }
   else
   {
      cout << "The tree is empty.\n";
   }
}

// Traverses tree along left branch and returns the node with the
// minimum student ID
const Binary_node * Binary_search_tree::minimum( Binary_node * t ) const
{
   while ( NULL != t->left )
   {
      t = t->left;
   }

   return t;
}

// Returns true if the tree is empty
const bool Binary_search_tree::is_empty( void ) const
{
   return ( m_root == NULL );
}

// Prints the contents of the tree in preorder traversal
void Binary_search_tree::print_preorder_traversal( void )
{
   if ( ! is_empty() )
   {
      print_preorder_traversal( m_root );
   }
   else
   {
      cout << "The tree is empty.\n";
   }
}

void Binary_search_tree::print_preorder_traversal( Binary_node * t )
{
   if ( NULL != t )
   {
      t->data.print();
      print_preorder_traversal( t->left );
      print_preorder_traversal( t->right );
   }
}

// Prints the contents of the tree in in-order traversal
void Binary_search_tree::print_inorder_traversal( void )
{
   if ( ! is_empty() )
   {
      print_inorder_traversal( m_root );
   }
   else
   {
      cout << "The tree is empty.\n";
   }
}

void Binary_search_tree::print_inorder_traversal( Binary_node * t )
{
   if ( NULL != t )
   {
      print_inorder_traversal( t->left );
      t->data.print();
      print_inorder_traversal( t->right );
   }
}

// Prints the contents of the tree in post-order traversal
void Binary_search_tree::print_postorder_traversal( void )
{
   if ( ! is_empty() )
   {
      print_postorder_traversal( m_root );
   }
   else
   {
      cout << "The tree is empty.\n";
   }
}

void Binary_search_tree::print_postorder_traversal( Binary_node * t )
{
   if ( NULL != t )
   {
      print_postorder_traversal( t->left );
      print_postorder_traversal( t->right );
      t->data.print();
   }
}

// Clears the contents of the tree
void Binary_search_tree::make_empty( void )
{
   make_empty( m_root );
}

void Binary_search_tree::make_empty( Binary_node * & t )
{
   if ( t != NULL )
   {
      make_empty( t->left );
      make_empty( t->right );
      delete t;
      t = NULL;
   }
}
