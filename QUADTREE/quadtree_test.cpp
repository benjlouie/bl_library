#include <iostream>
#include <random>
#include "Quadtree.h"

using namespace std;

void randomInsertTest_double(unsigned numTests, double min, double max);
void randomInsertTest_int(unsigned numTests, int min, int max);

int main(void)
{
	Region<double> tmp = { 0.0, 4.0, 0.0, 4.0 };
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	randomInsertTest_double(100, 0.0, 100.0);
	randomInsertTest_int(100, 0, 1000000);

	return EXIT_SUCCESS;
}

void randomInsertTest_double(unsigned numTests, double min, double max)
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

void randomInsertTest_int(unsigned numTests, int min, int max)
{
	Region<int> tmp = { min, max, min, max };
	int data = 0;
	Quadtree<int, int> t(tmp, data);

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min, max);

	for (unsigned i = 0; i < numTests; i++) {
		tmp = { uni(rng), uni(rng), uni(rng), uni(rng) };
		t.insert(tmp, data);
	}
}