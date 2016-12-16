#ifndef BL_PREFIXTREE_H
#define BL_PREFIXTREE_H

#include <vector>
#include <string>
//TODO: remove debug dependencies
#include <iostream>
#include <queue>

//TODO: maybe allow user data to be stored with a word? (turn into template?)
class PrefixTree
{
public:
	PrefixTree();
	~PrefixTree();

	void insert(char *str);
	void insert(const char *str);
	void insert(std::string &str);
	bool contains(std::string &str);
	//void remove(std::string &str); //TODO: indicate was there or not? have to recondense after removal, think about it more

	//TODO: remove debug func
	void print();

private:
	struct Node {
		bool endpoint;
		std::string prefix;
		std::vector<Node *> children;
	} root_;

	void insertInNode(Node *cur, size_t splitPos, std::string &str, size_t startInd);
	void insertAfterNode(Node *cur, size_t insPos, std::string &str, size_t strInd);
	bool childSearch(std::vector<Node *> &children, char key, size_t *endPos = nullptr);
	Node *find(std::string &str);
};

//TODO: put into some library
size_t split(std::string &str1, std::string &str2, size_t str1Ind = 0, size_t str2Ind = 0);

#endif // !BL_INTERVALTREE_H
