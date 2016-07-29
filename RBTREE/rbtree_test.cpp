#include <iostream>
#include "rbtree.h"

int main()
{
	RBTree<int, int> tree;
	tree.insert(8, 8);
	tree.insert(7, 7);
	tree.insert(6, 6);
	tree.insert(5, 5);
	tree.insert(4, 4);
	tree.insert(3, 3);
	tree.insert(2, 2);
	tree.insert(1, 1);

	tree.print();

	char c;
	std::cin >> c;
	return EXIT_SUCCESS;
}