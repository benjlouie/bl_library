#include <iostream>
#include <cstdlib>
#include <ctime>
#include "rbtree.h"

int main()
{
	RBTree<int, int> tree;

	srand((unsigned)time(NULL));

	unsigned size = 10;
	unsigned range = 10;
	unsigned numRemoved = 0;
	unsigned *nums = new unsigned[size];
	unsigned *removed = new unsigned[size];

	//insertion
	for (unsigned i = 0; i < size; i++) {
		nums[i] = rand() % range;
		tree.insert(nums[i], nums[i]);
		std::cout << nums[i] << ", ";
	}
	std::cout << std::endl;

	int key =  nums[rand() % size];
	size_t elmCount = tree.count(key);
	std::cout << key << " appears " << elmCount << " times" << std::endl;
	size_t secondCount = 0;
	for (unsigned i = 0; i < size; i++) {
		if (nums[i] == key) {
			secondCount++;
		}
	}
	if (secondCount == elmCount) {
		std::cout << "CORRECT" << std::endl;
	}
	else {
		std::cout << "NOT CORRECT" << std::endl;
	}
	std::cout << std::endl;

	//deletion
	numRemoved = rand() % (size + 1);
	std::cout << "removing ";
	for (unsigned i = 0; i < numRemoved; i++) {
		removed[i] = nums[rand() % size];
		tree.remove(removed[i]);
		std::cout << removed[i] << ", ";
	}
	std::cout << std::endl;

	delete[] nums;
	delete[] removed;

	tree.clear();
	std::cout << "size after clear: " << tree.size() << std::endl;

	char c;
	std::cin >> c;
	return EXIT_SUCCESS;
}