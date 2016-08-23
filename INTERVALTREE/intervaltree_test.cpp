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
		std::cout << "(" << intervals[i].low << ":" << intervals[i].high << ")" << "|" << i << ", ";
		tree.insert(intervals[i], i);
	}
	std::cout << std::endl << std::endl;
	tree.print();
	std::cout << std::endl << std::endl;

	Interval<int> test;
	int low = rand() % range;
	int high = rand() % range;
	(low < high) ? test = Interval<int>{ low, high } : test = Interval<int>{ high, low };
	std::vector<std::pair<const Interval<int> &, unsigned &>> *intersectList;
	std::vector<std::pair<const Interval<int> &, unsigned &>> *withinList;
	intersectList = tree.intersect(test);
	withinList = tree.within(test);
	std::cout << "finding intersects of (" << test.low << ":" << test.high << ")" << std::endl;
	for (std::pair<Interval<int>, unsigned> elm : *intersectList) {
		std::cout << "(" << elm.first.low << ":" << elm.first.high << ")" << "|" << elm.second << ", ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "finding intervals within (" << test.low << ":" << test.high << ")" << std::endl;
	for (std::pair<Interval<int>, unsigned> elm : *withinList) {
		std::cout << "(" << elm.first.low << ":" << elm.first.high << ")" << "|" << elm.second << ", ";
	}
	delete intersectList;
	delete withinList;
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