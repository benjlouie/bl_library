#include <iostream>
#include <fstream>
#include "PrefixTree.h"

bool test_insert(unsigned numStrings, unsigned maxStringLength);
std::string rand_string(const unsigned length);

int main(int argc, char **argv)
{
	PrefixTree t;

	test_insert(50, 5);

	//TODO: read in file of all dictionary words

	return EXIT_SUCCESS;
}

bool test_insert(unsigned numStrings, unsigned maxStringLength)
{
	PrefixTree t;
	std::vector<std::string> saved;
	for (unsigned i = 0; i < numStrings; i++) {
		std::string randString = rand_string(rand() % (maxStringLength + 1));
		t.insert(randString);
		saved.push_back(randString);
	}

	t.print();

	for (unsigned i = 0; i < numStrings; i++) {
		if (!t.contains(saved[i])) {
			std::cout << "couldn't find \"" + saved[i] + "\"\n\n";
			return false;
		}
	}

	std::cout << "success\n\n";
	return true;
}

std::string rand_string(const unsigned length)
{
	static const char alphanum[] =
		//"0123456789"
		//"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string s;
	s.reserve(length);
	for (unsigned i = 0; i < length; i++) {
		s.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
	}

	return s;
}