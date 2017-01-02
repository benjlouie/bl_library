#include <iostream>
#include <random>
#include "Quadtree.h"

using namespace std;

void randomInsertTest(unsigned numTests, double min, double max);

int main(void)
{
	Region<double> tmp = { 0.0, 4.0, 0.0, 4.0 };
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	randomInsertTest(10, 0.0, 100.0);

	return EXIT_SUCCESS;
}

void randomInsertTest(unsigned numTests, double min, double max)
{
	Region<double> tmp = { min, max, min, max};
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<double> uni(min, max);

	for (unsigned i = 0; i < numTests; i++) {
		tmp = { uni(rng), uni(rng), uni(rng), uni(rng) };
		t.insert(tmp, data);
	}
}