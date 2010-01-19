#include "bst.h"

Node::Node()
{
	m_student_name = "";
	m_student_id = 0;
	m_class_standing = "";

	m_left_ptr = NULL;
	m_right_ptr = NULL;
}

Node::Node(string name, int id, string class_standing)
{
	m_student_name = name;
	m_student_id = id;
	m_class_standing = class_standing;

	m_left_ptr = NULL;
	m_right_ptr = NULL;
}

Node::Node(const Node& copy_node)
{
	this->m_student_name = copy_node.m_student_name;
	this->m_student_id = copy_node.m_student_id;
	this->m_class_standing = copy_node.m_class_standing;
}

Node::~Node()
{

}

bool Node::operator ==(const Node &the_node)
{
	if (this->m_student_id == the_node.m_student_id)
	{
		return true;
	}
	return false;
}

bool Node::operator< (const Node& the_node)
{
	if (this->m_student_id < the_node.m_student_id)
	{
		return true;
	}
	return false;
}

bool Node::operator> (const Node& the_node)
{
   	if (this->m_student_id > the_node.m_student_id)
	{
		return true;
	}
	return false;
}

bool Node::operator<= (const Node& the_node)
{
	if (this->m_student_id <= the_node.m_student_id)
	{
		return true;
	}
	return false;
}

bool Node::operator>= (const Node& the_node)
{
	if (this->m_student_id >= the_node.m_student_id)
	{
		return true;
	}
	return false;
}

bool Node::operator!= (const Node& the_node)
{
	if (this->m_student_id != the_node.m_student_id)
	{
		return true;
	}
	return false;
}

Bst::Bst()
{
	m_root_ptr = NULL;
}
/*
Bst::Bst(const Bst& the_bst)
{
	this->m_root_ptr = the_bst.m_root_ptr;
}
*/
void Bst::insert_tree(Node& the_node)
{
	insert(&m_root_ptr, the_node);
}

void Bst::insert(Node** node_ptr, Node &the_node)
{
	if(*node_ptr == NULL)
	{
		*node_ptr = &the_node;
	}
	else if (the_node < **node_ptr)
	{
		insert(&((*node_ptr)->m_left_ptr), the_node);
	}
	else if (the_node > **node_ptr)
	{
		insert(&((*node_ptr)->m_right_ptr), the_node);
	}
	else
	{
		cout << "ERROR:: Duplication\n";
	}
}

Node* Bst::search_node(int& id)
{
	if (m_root_ptr != NULL)
	{
		return search(&m_root_ptr, id);
	}
	else
	{
		cout << "MESSAGE:: Empty tree\n";
		return NULL;
	}
}

Node* Bst::search(Node** node_ptr, int& id)
{
	if (id == (*node_ptr)->m_student_id )
	{
		return &(**node_ptr);
	}
	else if (id < (*node_ptr)->m_student_id)
	{
		return search(&((*node_ptr)->m_left_ptr), id);
	}
	else if (id > (*node_ptr)->m_student_id)
	{
		return search(&((*node_ptr)->m_right_ptr), id);
	}
	else
	{
		return NULL;
	}
}

Node* Bst::minimum_node(Node* node_ptr)
{
	if(node_ptr->m_left_ptr == NULL)
	{
		return node_ptr;
	}
	return minimum_node(node_ptr->m_left_ptr);
}

Node* Bst::maximum_node(Node* node_ptr)
{
	if(node_ptr->m_right_ptr == NULL)
	{
		return node_ptr;
	}
	return maximum_node(node_ptr->m_right_ptr);
}

void Bst::in_order(Node* node_ptr)
{
	Node* curr_ptr = node_ptr;
	if(curr_ptr != NULL)
	{
		if(curr_ptr->m_left_ptr != NULL)
		{
			in_order(curr_ptr->m_left_ptr);
		}
	    cout << *curr_ptr << "\n";
		if(curr_ptr->m_right_ptr != NULL)
		{
			in_order(curr_ptr->m_right_ptr);
		}
	}
}

void Bst::pre_order(Node* node_ptr)
{
	Node* curr_ptr = node_ptr;
	if(curr_ptr != NULL)
	{
		cout << *curr_ptr << "\n";
		if(curr_ptr->m_left_ptr != NULL)
		{
			pre_order(curr_ptr->m_left_ptr);
		}
		if(curr_ptr->m_right_ptr != NULL)
		{
			pre_order(curr_ptr->m_right_ptr);
		}
	}
}

void Bst::post_order(Node* curr_ptr)
{
	//Node* curr_ptr = node_ptr;
	if(curr_ptr != NULL)
	{
		if(curr_ptr->m_left_ptr != NULL)
		{
			post_order(curr_ptr->m_left_ptr);
		}
		if(curr_ptr->m_right_ptr != NULL)
		{
			post_order(curr_ptr->m_right_ptr);
		}
		cout << *curr_ptr << "\n";
	}
}

bool Bst::remove(int& id)
{
	return remove_node(&m_root_ptr, id);
}

bool Bst::remove_node(Node** node_ptr, int& id)
{
	if( (*node_ptr)->m_student_id == id )
	{
		if (((*node_ptr)->m_right_ptr != NULL) && ((*node_ptr)->m_left_ptr != NULL))
		{
			if (delete_min(&((*node_ptr)->m_right_ptr)))
			{
				(*node_ptr)->m_student_name = tmp_std_name;
				(*node_ptr)->m_student_id = tmp_std_id;
				(*node_ptr)->m_class_standing = tmp_cls_standing;
				return true;
			}
			else
			{
				cout << "ERROR:: Delete Node error\n";
				return false;
			}
		}

		else if (((*node_ptr)->m_right_ptr == NULL) && ((*node_ptr)->m_left_ptr != NULL))
		{
			delete((*node_ptr)->m_right_ptr);
			Node* old_node_ptr = (*node_ptr);
			(*node_ptr) = ((*node_ptr)->m_left_ptr);
			delete(old_node_ptr);
			return true;
		}

		else if (((*node_ptr)->m_right_ptr != NULL) && ((*node_ptr)->m_left_ptr == NULL))
		{
			delete((*node_ptr)->m_left_ptr);
			Node* old_node_ptr = (*node_ptr);
			(*node_ptr) = ((*node_ptr)->m_right_ptr);
			delete(old_node_ptr);
			return true;
		}

		else if (((*node_ptr)->m_right_ptr == NULL) && ((*node_ptr)->m_left_ptr == NULL))
		{
			delete((*node_ptr)->m_left_ptr);
			delete((*node_ptr)->m_right_ptr);
			(*node_ptr) = NULL;
			return true;
		}
	}
	else if (((*node_ptr)->m_student_id) < id)
	{
		return remove_node((&((*node_ptr)->m_right_ptr)), id);
	}
	else
	{
		return remove_node((&((*node_ptr)->m_left_ptr)), id);
	}
}

bool Bst::delete_min(Node** node_ptr)
{
	if ((*node_ptr) != NULL)
	{
		if((*node_ptr)->m_right_ptr == NULL)
		{
			return delete_min(&((*node_ptr)->m_left_ptr));
		}

		if ((*node_ptr)->m_right_ptr == NULL)
		{
			tmp_std_name = (*node_ptr)->m_student_name;
			tmp_std_id = (*node_ptr)->m_student_id;
			tmp_cls_standing = (*node_ptr)->m_class_standing;

			delete((*node_ptr)->m_left_ptr);
			delete((*node_ptr)->m_right_ptr);
			(*node_ptr) = NULL;
			return true;
		}

		else if ((*node_ptr)->m_right_ptr != NULL)
		{
			tmp_std_name = (*node_ptr)->m_student_name;
			tmp_std_id = (*node_ptr)->m_student_id;
			tmp_cls_standing = (*node_ptr)->m_class_standing;

			Node* old_node_ptr = (*node_ptr);
			(*node_ptr) = ((*node_ptr)->m_right_ptr);
			delete(old_node_ptr->m_left_ptr);
			delete(old_node_ptr);
			return true;
		}
	}
	else
	{
		cout << "Empty Tree\n";
		return false;
	}
}

void Bst::print()
{
	cout << "\n\nStudent List\n";
	cout << "------------\n";
	in_order(m_root_ptr);
	//cout << "Minimum\n";
	//cout << "-------\n";
	//cout << *(minimum_node(m_root_ptr)) << "\n";
	//cout << "Miximum\n";
	//cout << "-------\n";
	//cout << *(maximum_node(m_root_ptr)) << "\n";
}
