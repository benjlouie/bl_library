#include <iostream>
#include <cstdlib>
#include <ctime>
#include "intervaltree.h"

int main()
{
	int size = 10;
	int range = 100;
	IntervalTree<int, unsigned> tree;
	Interval<int> *intervals = new Interval<int>[size];

	srand((unsigned)time(NULL));

	for (int i = 0; i < size; i++) {
		int low = rand() % range;
		int high = rand() % range;
		if (low < high) {
			intervals[i] = Interval<int>{ low, high };
		}
		else {
			intervals[i] = Interval<int>{ high, low };
		}
		std::cout << "(" << intervals[i].low << ":" << intervals[i].high << ")" << "|" << i << ", ";;
		tree.insert(intervals[i], i);
	}
	std::cout << std::endl << std::endl;
	tree.print();
	std::cout << std::endl << std::endl;

	int rem = 1;
	std::cout << "removing" << "(" << intervals[rem].low << ":" << intervals[rem].high << ")" << std::endl;
	unsigned num = 12;
	tree.remove(intervals[rem], &num);

	tree.print();
	std::cout << std::endl << std::endl;

	char c;
	std::cin >> c;
	return EXIT_SUCCESS;
}