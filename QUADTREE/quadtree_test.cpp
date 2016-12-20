#include <iostream>
#include "Quadtree.h"

using namespace std;

int main(void)
{
	Region<int> tmp = { INT_MIN, INT_MAX, INT_MIN, INT_MAX };
	int data = 0;
	Quadtree<int, int> t(tmp, data);

	return EXIT_SUCCESS;
}