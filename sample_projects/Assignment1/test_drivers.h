// William Reinhardt
// Cpt S 422 Fall '09
// Assignment 1

#ifndef TEST_DRIVERS_H
#define TEST_DRIVERS_H

// SYSTEM INCLUDES

#include <iostream>

// LOCAL INCLUDES

#include "binary_search_tree.h"

// FORWARD REFERENCES

void test_menu( void );

void insert( void );
void insert_case_1( void );
void insert_case_2( void );
void insert_case_3( void );
void insert_case_4( void );

void remove( void );
void remove_case_1( void );
void remove_case_2( void );
void remove_case_3( void );
void remove_case_4( void );
void remove_case_5( void );
void remove_case_6( void );
void remove_case_7( void );

void search( void );
void search_case_1( void );
void search_case_2( void );
void search_case_3( void );
void search_case_4( void );

void maximum( void );
void maximum_case_1( void );
void maximum_case_2( void );
void maximum_case_3( void );

void minimum( void );
void minimum_case_1( void );
void minimum_case_2( void );
void minimum_case_3( void );

void is_empty( void );
void is_empty_case_1( void );

void print_preorder_traversal( void );
void print_preorder_traversal_case_1( void );
void print_preorder_traversal_case_2( void );

void print_inorder_traversal( void );
void print_inorder_traversal_case_1( void );
void print_inorder_traversal_case_2( void );

void print_postorder_traversal( void );
void print_postorder_traversal_case_1( void );
void print_postorder_traversal_case_2( void );

void run_all_tests( void );

#endif
