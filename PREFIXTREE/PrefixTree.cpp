#include "PrefixTree.h"


PrefixTree::PrefixTree()
{
	root_.endpoint = false;
}

PrefixTree::~PrefixTree()
{
	//TODO: recursively delete tree
}

void PrefixTree::insert(char *str)
{
	std::string s = str;
	insert(s);
}

void PrefixTree::insert(const char *str)
{
	std::string s = str;
	insert(s);
}

void PrefixTree::insert(std::string &str)
{
	Node *cur = &root_;
	size_t len = str.size();
	size_t strIndex = 0;

	while (strIndex < len) { //TODO: can probably take out comparison here
		size_t splitPos = split(cur->prefix, str, 0, strIndex);
		strIndex += splitPos;

		if (splitPos < cur->prefix.length()) {
			//insert in current node
			insertInNode(cur, splitPos, str, strIndex);
			return;
		}
		if (strIndex >= len) { //TODO: check this
			//str ends with cur node
			cur->endpoint = true;
			return;
		}

		size_t endPos = 0;
		if (childSearch(cur->children, str[strIndex], &endPos)) {
			cur = cur->children[endPos];
		}
		else {
			//add new node onto current (at endPos)
			insertAfterNode(cur, endPos, str, strIndex);
			return;
		}
	}
}

void PrefixTree::insertInNode(Node *cur, size_t splitPos, std::string &str, size_t startInd)
{
	//add node for split
	Node *splitRight = new Node;
	//move cur down to new node
	splitRight->children.swap(cur->children);
	splitRight->prefix = cur->prefix.substr(splitPos);
	splitRight->endpoint = cur->endpoint;
	cur->prefix.erase(splitPos);
	cur->endpoint = false;

	if (startInd >= str.length()) {//TODO: check this, should only be possible to be ==
		cur->children.push_back(splitRight);
		//str ends at current node
		cur->endpoint = true;
		return;
	}

	//need another node for rest of str
	Node *strRight = new Node;
	strRight->prefix = str.substr(startInd);
	strRight->endpoint = true;

	//push elms on in correct order (keep sorted)
	if (strRight->prefix[0] > splitRight->prefix[0]) {
		cur->children.push_back(splitRight);
		cur->children.push_back(strRight);
	}
	else {
		cur->children.push_back(strRight);
		cur->children.push_back(splitRight);
	}
}

void PrefixTree::insertAfterNode(Node *cur, size_t insPos, std::string &str, size_t strInd)
{
	Node *add = new Node;
	add->prefix = str.substr(strInd);

	auto it = cur->children.begin();
	it += insPos;
	cur->children.insert(it, add); //insert node in correct sorted position
}

// finds where str2 splits from str1 (num from given starting points)
size_t split(std::string &str1, std::string &str2, size_t str1Ind, size_t str2Ind)
{
	size_t str1Len = str1.length() - str1Ind;
	size_t str2Len = str2.length() - str2Ind;

	if (str1Len == 0 || str2Len == 0) {
		return 0;
	}

	size_t remainingLen = str1Len > str2Len ? str2Len : str1Len;
	unsigned count = 0;
	for (; count < remainingLen; count++, str1Ind++, str2Ind++) {
		if (str1[str1Ind] != str2[str2Ind]) {
			break;
		}
	}
	return count;
}

bool PrefixTree::childSearch(std::vector<Node *> &children, char key, size_t *endPos)
{
	size_t left = 0;
	size_t right = children.size();
	size_t mid = 0;

	if (children.empty()) {
		if (endPos) {
			*endPos = 0;
		}
		return false;
	}

	while (left < right) {
		mid = left + (right - left) / 2;
		char midChar = children[mid]->prefix[0];

		if (key < midChar) {
			right = mid;
		}
		else if (key > midChar) {
			left = mid + 1;
		}
		else {
			if (endPos) {
				*endPos = mid;
			}
			return true;
		}
	}

	if (endPos) {
		*endPos = mid;
	}
	return false;
}

bool PrefixTree::contains(std::string &str)
{
	return find(str) != nullptr;
}

//TODO: redo so it works with compressed nodes (like in insert)
PrefixTree::Node *PrefixTree::find(std::string &str)
{
	Node *cur = &root_;
	size_t len = str.size();

	for (size_t strIndex = 0; strIndex < len; strIndex++) {
		size_t endPos = 0;
		if (childSearch(cur->children, str[strIndex], &endPos)) {
			cur = cur->children[endPos];
		}
		else {
			return nullptr;
		}
	}

	return cur;
}


//TODO: remove debug func
void PrefixTree::print()
{
	std::queue<Node *> q;
	q.push(&root_);

	while (q.size() > 0) {
		size_t len = q.size();
		for (unsigned i = 0; i < len; i++) {
			Node *cur = q.front();
			q.pop();

			if (cur == nullptr) {
				std::cout << " | ";
				continue;
			}

			for (Node *i : cur->children) {
				q.push(i);
			}
			if (cur->children.size() > 0) {
				q.push(nullptr);
			}

			std::cout << cur->prefix;
			if (cur->endpoint) {
				std::cout << "*";
			}
			if (cur->children.size() == 0) {
				std::cout << ".";
			}
			if (q.front()){
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}