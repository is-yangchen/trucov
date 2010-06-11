// William Reinhardt
// Cpt S 422 Fall '09
// Assignment 1

///////////////////////////////////////////////////////////////////////////////
//  file: binary_search_tree.h
//
//  Defines the methods of the Binary search tree class
///////////////////////////////////////////////////////////////////////////////

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

// SYSTEM INCLUDES

#include <iostream>

class Student
{
public:
   explicit Student( std::string name, unsigned id, std::string standing )
      : m_name( name ), m_id( id ), m_class( standing )
   {
   }

   void print( void ) const
   {
      std::cout << "Name:  " << m_name << "\n";
      std::cout << "ID:    " << m_id   << "\n";
      std::cout << "Class: " << m_class << "\n";
   }

   std::string m_name;
   unsigned m_id;
   std::string m_class;
};


class Binary_node;

// Defines a binary search tree
class Binary_search_tree
{
public:

   // PUBLIC METHODS

   explicit Binary_search_tree( void ) : m_root( NULL )
   {
   }

   ~Binary_search_tree()
   {
      make_empty();
   }

   void insert( const Student & student );
   void remove( const unsigned id );
   void search( const unsigned id );
   void maximum( void ) const;
   void minimum( void ) const;
   const bool is_empty( void ) const;
   void print_preorder_traversal( void );
   void print_inorder_traversal( void );
   void print_postorder_traversal( void );

private:

   // PRIVATE METHODS
   void insert( const Student & student, Binary_node * & t );
   void remove( const unsigned id, Binary_node * & t );
   void search( const unsigned id, Binary_node * t );
   const Binary_node * maximum( Binary_node * t ) const;
   const Binary_node * minimum( Binary_node * t ) const;
   void print_preorder_traversal( Binary_node * t );
   void print_inorder_traversal( Binary_node * t );
   void print_postorder_traversal( Binary_node * t );
   void make_empty( void );
   void make_empty( Binary_node * & t );

   // PRIVATE MEMBERS

   // The binary search tree's root node pointer
   Binary_node * m_root;

}; // End class Binary_search_tree

/// @class Binary_search_tree
///
/// @brief
/// Implements a binary search tree node
class Binary_node
{
friend class Binary_search_tree;
public:
   // Binary_node constructor
   explicit Binary_node( const Student & s ) : data( s ), left( NULL ), right( NULL )
   {
   }

private:
   Student data;
   Binary_node * left;
   Binary_node * right;
};

#endif
