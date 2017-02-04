#include <iostream>
#include <random>
#include "Quadtree.h"

using namespace std;

void randomInsertTest_double(unsigned numTests, double min, double max);
void randomInsertTest_int(unsigned numTests, int min, int max);
void randomRemoveTest_double(unsigned numTests, double min, double max);
void randomWithinTest_double(unsigned numRegions, double min, double max);

int main(void)
{
	Region<double> tmp = { 0.0, 4.0, 0.0, 4.0 };
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	randomInsertTest_double(100, 0.0, 100.0);
	randomInsertTest_int(100, 0, 1000000);
	randomRemoveTest_double(100, 0.0, 100.0);
	for (int i = 0; i < 100; i++) {
		randomWithinTest_double(100, 0.0, 100.0);
	}

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

void randomRemoveTest_double(unsigned numTests, double min, double max)
{
	Region<double> tmp = { min, max, min, max };
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<double> uni(min, max);

	double removeChance = uni(rng) / max;
	std::vector<std::pair<Region<double>, double>> removeList;
	unsigned numRemoved = 0;

	//insert regions
	for (unsigned i = 0; i < numTests; i++) {
		tmp = { uni(rng), uni(rng), uni(rng), uni(rng) };
		t.insert(tmp, data);
		if (uni(rng) / max < removeChance) {
			removeList.push_back(std::make_pair(tmp, data));
			numRemoved++;
		}
		data++;
	}

	//remove regions
	size_t actualRemoved = 0;
	for (auto removeRegion : removeList) {
		actualRemoved += t.remove(removeRegion.first, removeRegion.second);
	}
	if (actualRemoved == numRemoved) {
		std::cout << actualRemoved << " regions removed\n";
	}
	else {
		std::cout << "only " << actualRemoved << " of " << numRemoved << " were removed\n";
	}

	//ensure removed regions were actually removed
	std::vector<std::pair<Region<double>, double>> remainingRegions =  t.getRegionsAll();
	for (auto region : remainingRegions) {
		for (auto removedRegion : removeList) {
			if (removedRegion.first == region.first
				&& removedRegion.second == region.second) {
				Region<double> tmp = region.first;
				//found region that shouldn't be there
				std::cout << "region: {" << tmp.lowRow_ << "," << tmp.highRow_ << "," << tmp.lowCol_ << "," << tmp.highCol_ << "}:" << region.second << "wasn't removed\n";
			}
		}
	}
}

void randomWithinTest_double(unsigned numRegions, double min, double max)
{
	Region<double> tmp = { min, max, min, max };
	double data = 0.0;
	Quadtree<double, double> t(tmp, data);

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<double> uni(min, max);

	Region<double> withinRegion = { uni(rng), uni(rng), uni(rng), uni(rng) };
	std::vector<std::pair<Region<double>, double>> withinList;

	//insert regions
	for (unsigned i = 0; i < numRegions; i++) {
		tmp = { uni(rng), uni(rng), uni(rng), uni(rng) };
		if (tmp.within(withinRegion)) {
			withinList.push_back(std::make_pair(tmp, data));
		}
		t.insert(tmp, data);
		data++;
	}

	//test that within regions were properly found
	std::vector<std::pair<Region<double>, double>> foundWithin = t.getRegionsWithin(withinRegion);
	for (auto foundIt = foundWithin.begin(); foundIt != foundWithin.end(); ) {
		bool removed = false;
		for (auto withinIt = withinList.begin(); withinIt != withinList.end(); withinIt++) {
			if (withinIt->first == foundIt->first && withinIt->second == foundIt->second) {
				withinList.erase(withinIt);
				foundIt = foundWithin.erase(foundIt);
				removed = true;
				break;
			}
		}
		if (!removed) {
			foundIt++;
		}
	}

	//print errors if any
	if (withinList.size() != 0) {
		std::cout << "withinList not empty: (" << withinList.size() <<")\n";
		for (auto region : withinList) {
			Region<double> tmp = region.first;
			std::cout << "{" << tmp.lowRow_ << "," << tmp.highRow_ << "," << tmp.lowCol_ << "," << tmp.highCol_ << "}:" << region.second << "\n";
		}
		std::cout << "\n";
	}
	if (foundWithin.size() != 0) {
		std::cout << "foundWithin not empty: (" << withinList.size() << ")\n";;
		for (auto region : foundWithin) {
			Region<double> tmp = region.first;
			std::cout << "{" << tmp.lowRow_ << "," << tmp.highRow_ << "," << tmp.lowCol_ << "," << tmp.highCol_ << "}:" << region.second << "\n";
		}
		std::cout << "\n";
	}
}