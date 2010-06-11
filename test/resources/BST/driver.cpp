#include "bst.h"
int main()
{
	/*Node test_node("Ye Kyaw", 5, "Senior");
	Node test_two("GG", 6, "senior");
	Node test_three("cc", 7, "senior");
	if (test_two == test_three)
	{
		cout << "Node:" << test_two << "\n";
	}
	else
	{
		cout << "not equal" << "\n";
	}*/
	Bst student_tree;
	Node* new_student = new Node("Hartono", 5, "Senior");
	student_tree.insert_tree(*new_student);
	new_student = new Node("Nay Latt", 7, "Senior");
	student_tree.insert_tree(*new_student);
	new_student = new Node("Muni", 8, "Senior");
	student_tree.insert_tree(*new_student);
	new_student = new Node("Ye Kyaw", 9, "Senior");
	student_tree.insert_tree(*new_student);


	student_tree.print();
    //int tmp = 5;
	//cout << *(student_tree.search_node(tmp)) << "\n";
	int the_id = 5;
	student_tree.remove(the_id);
	student_tree.print();

	return 0;
}