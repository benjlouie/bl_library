#include <iostream>
#include <cstdlib>
#include <ctime>
#include "rbtree.h"

int main()
{
	RBTree<int, int> tree;

	srand((unsigned)time(NULL));

	unsigned size = 10;
	unsigned numRemoved = 0;
	unsigned *nums = new unsigned[size];
	unsigned *removed = new unsigned[size];

	//insertion
	for (unsigned i = 0; i < size; i++) {
		nums[i] = rand() % 100;
		tree.insert(nums[i], nums[i]);
		std::cout << nums[i] << ", ";
	}
	std::cout << std::endl << std::endl;

	//deletion
	numRemoved = rand() % (size + 1);
	for (unsigned i = 0; i < numRemoved; i++) {
		removed[i] = nums[rand() % size];
		tree.remove(removed[i]);
		std::cout << removed[i] << ", ";
	}
	std::cout << std::endl << std::endl;

	delete[] nums;
	delete[] removed;

	tree.print();

	char c;
	std::cin >> c;
	return EXIT_SUCCESS;
}