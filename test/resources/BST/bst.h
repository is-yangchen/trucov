#ifndef BST_H
#define BST_H

// SYSTEM INCLUDE
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::ostream;

class Node
{
	friend class Bst;
	/// FRIEND FUNCTIONS
	/// overloading operator <<
	friend ostream &operator<< ( ostream & out, const Node& the_node )
	{
		out << "Student Name:\t" << the_node.m_student_name << "\n"
			<< "Student ID:\t" << the_node.m_student_id << "\n"
			<< "Class Standing:\t" << the_node.m_class_standing << "\n";
		return out;
	}

	/// PUBLIC MEMBERS
	public:
		/// constructor
		Node();

		/// Overloaded Constructor
        Node(string, int id, string);

		/// copy constructor
        Node(const Node& copy_node);

		/// default constructor
		~Node();

		/// Overloading operator ==
		bool operator==(const Node& the_node);

		/// Overloading operator <
		bool operator< (const Node& the_node);

		/// Overloading operator >
		bool operator> (const Node& the_node);
   
		/// Overloading <=
        bool operator<= (const Node& the_node);

		/// Overloading >=
		bool operator>= (const Node& the_node);

		/// Overloading !=
		bool operator!= (const Node& the_node);

	/// PRIVATE MEMBERS
	private:
		string m_student_name;
		int m_student_id;
		string m_class_standing;
		Node* m_left_ptr;
		Node* m_right_ptr;
};
class Bst
{
	friend class Node;
	public:	
		Bst();
        void insert_tree(Node& the_node);
		Node* search_node(int& id);
		Node* minimum_node(Node*);
		Node* maximum_node(Node*);
		bool remove(int& id);
		void print();

	private:
		Node* m_root_ptr;
		void insert(Node** node_ptr, Node& the_node);
		Node* search(Node** node_ptr, int& id);

		bool remove_node(Node** node_ptr, int& id);
		bool delete_min(Node** node_ptr);
		void in_order(Node*);
		void post_order(Node*);
		void pre_order(Node*);

		string tmp_std_name;
		int tmp_std_id;
		string tmp_cls_standing;
};
#endif
