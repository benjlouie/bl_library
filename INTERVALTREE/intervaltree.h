//insert/delete cases used straight from wikipedia

#ifndef BL_INTERVALTREE_H
#define BL_INTERVALTREE_H

#include <functional>
#include <vector>

//TODO: remove debug dependencies
#include <iostream>
#include <queue>

//TODO: check if some allocator needs to be passed in
//TODO: make iterator for class

template<typename K>
struct Interval {
	K low;
	K high;

	inline void operator= (const Interval<K> &rhs){ this->low = rhs.low; this->high = rhs.high; }
};
template<typename K> inline bool operator==(const Interval<K> &lhs, const Interval<K> &rhs){ return (lhs.low == rhs.low) && (lhs.high == rhs.high); }
template<typename K> inline bool operator!=(const Interval<K> &lhs, const Interval<K> &rhs){ return !operator==(lhs, rhs); }
template<typename K> inline bool operator< (const Interval<K> &lhs, const Interval<K> &rhs){ return lhs.low < rhs.low; }
template<typename K> inline bool operator> (const Interval<K> &lhs, const Interval<K> &rhs){ return  operator< (rhs, lhs); }
template<typename K> inline bool operator<=(const Interval<K> &lhs, const Interval<K> &rhs){ return !operator> (lhs, rhs); }
template<typename K> inline bool operator>=(const Interval<K> &lhs, const Interval<K> &rhs){ return !operator< (lhs, rhs); }


template <typename K, typename T>
class IntervalTree
{
public:
	IntervalTree(void);
	~IntervalTree(void);

	size_t size(void);
	size_t count(const Interval<K> &key, const T *dataCmp = nullptr);
	void clear(void);
	bool empty(void);
	T& insert(const Interval<K> &key, const T &data);
	void remove(const Interval<K> &key, const T *dataCmp = nullptr);
	std::vector<std::pair<const Interval<K> &, T&>> *intersect(const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> *list = nullptr);
	std::vector<std::pair<const Interval<K> &, T&>> *within(const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> *list = nullptr);
	T& operator[](const Interval<K> &key);

	//TODO: remove debug method
	void print(void);

private:
	struct Node {
		Interval<K> key;
		K max;
		T data;
		enum Color { RED, BLACK } color;
		Node *parent;
		Node *left;
		Node *right;
	};

	size_t size_;
	Node *root_;
	Node leaf_;

	size_t count_(Node *root, const Interval<K> &key, const T *dataCmp = nullptr);
	Node *peek(Node *root, const Interval<K> &key, const T *dataCmp = nullptr);
	Node *grandparent(Node *n);
	Node *uncle(Node *n);
	Node *sibling(Node *n);
	Node *predecessor(Node *n);
	Node *successor(Node *n);
	void rotate_right(Node *n);
	void rotate_left(Node *n);
	bool is_leaf(Node *n);
	void replace_node(Node *n, Node *replacement);
	void updateMax(Node *n);

	void insert_case1(Node *n);
	void insert_case2(Node *n);
	void insert_case3(Node *n);
	void insert_case4(Node *n);
	void insert_case5(Node *n);
	void remove_case0(Node *n);
	void remove_case1(Node *n);
	void remove_case2(Node *n);
	void remove_case3(Node *n);
	void remove_case4(Node *n);
	void remove_case5(Node *n);
	void remove_case6(Node *n);

	std::vector<std::pair<const Interval<K> &, T&>> *intersect_(Node *root, const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> &intervals);
	std::vector<std::pair<const Interval<K> &, T&>> *within_(Node *root, const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> &intervals);
	void foreach_postorder(Node *root, std::function<void(Node *)> func);
};


template <typename K, typename T>
IntervalTree<K, T>::IntervalTree(void)
{
	size_ = 0;
	root_ = &leaf_;
	leaf_.color = Node::BLACK;
	leaf_.left = nullptr;
	leaf_.right = nullptr;
}

template <typename K, typename T>
IntervalTree<K, T>::~IntervalTree(void)
{
	clear();
}

template <typename K, typename T>
size_t IntervalTree<K, T>::size(void)
{
	return size_;
}

template <typename K, typename T>
size_t IntervalTree<K, T>::count(const Interval<K> &key, const T *dataCmp)
{
	return count_(root_, key, dataCmp);
}

//TODO: this method can be optimized
template <typename K, typename T>
size_t IntervalTree<K, T>::count_(Node *root, const Interval<K> &key, const T *dataCmp)
{
	Node *cur = peek(root, key);
	if (cur == &leaf_) {
		return 0;
	}

	if (key == cur->key && (dataCmp == nullptr || cur->data == *dataCmp)) {
		return (1 + count_(cur->left, key, dataCmp) + count_(cur->right, key, dataCmp));
	}
	return count_(cur->left, key, dataCmp) + count_(cur->right, key, dataCmp);
}

template <typename K, typename T>
void IntervalTree<K, T>::clear(void)
{
	foreach_postorder(root_, [](Node *cur){ delete cur; });
	root_ = &leaf_;
	size_ = 0;
}

template <typename K, typename T>
bool IntervalTree<K, T>::empty(void)
{
	return !size_;
}

/* HELPER section */
template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::peek(Node *root, const Interval<K> &key, const T *dataCmp)
{
	Node *cur = root;

	while (cur != &leaf_) {
		if (key < cur->key) {
			cur = cur->left;
		}
		else if (key > cur->key) {
			cur = cur->right;
		}
		else { //key.high may not equal cur->key.high, be aware
			Node *ret = cur;
			if (dataCmp && cur->data != *dataCmp) {
				//dataCmp not equal, check subtrees (left first)
				ret = peek(cur->left, key, dataCmp);
				if (ret == &leaf_) {
					ret = peek(cur->right, key, dataCmp);
				}
			}
			return ret;
		}
	}
	return &leaf_;
}

template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::grandparent(Node *n)
{
	if (n && n->parent) {
		return n->parent->parent;
	}
	else {
		return nullptr;
	}
}

template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::uncle(Node *n)
{
	Node *g = grandparent(n);
	if (g == nullptr)
		return nullptr; // No grandparent means no uncle
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
}

template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::sibling(Node *n)
{
	if (!n || !n->parent) {
		return nullptr;
	}
	if (n == n->parent->left) {
		return n->parent->right;
	}
	else {
		return n->parent->left;
	}
}

template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::predecessor(Node *n)
{
	if (n->left == &leaf_) {
		return nullptr;
	}

	Node *cur = n->left;
	while (cur->right != &leaf_) {
		cur = cur->right;
	}

	return cur;
}

template <typename K, typename T>
typename IntervalTree<K, T>::Node *
IntervalTree<K, T>::successor(Node *n)
{
	if (n->right == &leaf_) {
		return nullptr;
	}

	Node *cur = n->right;
	while (cur->left != &leaf_) {
		cur = cur->left;
	}

	return cur;
}

template <typename K, typename T>
void IntervalTree<K, T>::rotate_right(Node *n)
{
	Node *left = n->left;
	Node *parent = nullptr;
	Node **parentLink;

	//get parentLink
	if (n->parent) {
		parent = n->parent;
		if (n->parent->left == n) {
			parentLink = &n->parent->left;
		}
		else {
			parentLink = &n->parent->right;
		}
	}
	else {
		parentLink = &root_;
	}

	//rotate right
	*parentLink = left;
	n->left = left->right;
	n->parent = left;
	left->parent = parent;
	if (left->right != &leaf_) {
		left->right->parent = n;
	}
	left->right = n;
	updateMax(n);
	updateMax(left);
}

template <typename K, typename T>
void IntervalTree<K, T>::rotate_left(Node *n)
{
	Node *right = n->right;
	Node *parent = nullptr;
	Node **parentLink;

	//get parentLink
	if (n->parent) {
		parent = n->parent;
		if (n->parent->left == n) {
			parentLink = &n->parent->left;
		}
		else {
			parentLink = &n->parent->right;
		}
	}
	else {
		parentLink = &root_;
	}

	//rotate left
	*parentLink = right;
	n->right = right->left;
	n->parent = right;
	right->parent = parent;
	if (right->left != &leaf_) {
		right->left->parent = n;
	}
	right->left = n;
	updateMax(n);
	updateMax(right);
}

template <typename K, typename T>
bool IntervalTree<K, T>::is_leaf(Node *n)
{
	return n == &leaf_;
}

template <typename K, typename T>
void IntervalTree<K, T>::replace_node(Node *n, Node *replacement)
{
	if (!n->parent) {
		root_ = replacement;
		replacement->parent = nullptr;
		return;
	}

	replacement->parent = n->parent;
	if (n->parent->left == n) {
		n->parent->left = replacement;
	}
	else {
		n->parent->right = replacement;
	}
}

//TODO: this may be traversing more than necessary, think about it more
template <typename K, typename T>
void IntervalTree<K, T>::updateMax(Node *n)
{
	if (n == &leaf_) {
		return;
	}

	n->max = n->key.high;
	bool changed = false;
	if (n->left != &leaf_ && n->left->max > n->max) {
		n->max = n->left->max;
		changed = true;
	}
	if (n->right != &leaf_ && n->right->max > n->max) {
		n->max = n->right->max;
		changed = true;
	}

	if (n->parent && (changed || (n->max > n->parent->max))) {
		updateMax(n->parent);
	}
}

/* INSERT section */
template <typename K, typename T>
T& IntervalTree<K, T>::insert(const Interval<K> &key, const T &data)
{
	Node *n = new Node{ key, key.high, data, Node::RED, nullptr, &leaf_, &leaf_ };

	Node *prev = nullptr;
	Node *cur = root_;
	Node **prevLink = &root_;
	//BST insert at leaf
	while (cur != &leaf_) {
		prev = cur;
		if (key >= cur->key) {
			prevLink = &cur->right;
			cur = cur->right;
		}
		else {
			prevLink = &cur->left;
			cur = cur->left;
		}
	}

	//insert node
	n->parent = prev;
	*prevLink = n;

	insert_case1(n);
	updateMax(n);
	size_++;

	return n->data;
}

template <typename K, typename T>
void IntervalTree<K, T>::insert_case1(Node *n)
{
	if (n->parent == nullptr) {
		n->color = Node::BLACK;
	}
	else {
		insert_case2(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::insert_case2(Node *n)
{
	if (n->parent->color == Node::BLACK) {
		return; /* Tree is still valid */
	}
	else {
		insert_case3(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::insert_case3(Node *n)
{
	Node *g;
	Node *u = uncle(n);

	if ((u != nullptr) && (u->color == Node::RED)) {
		n->parent->color = Node::BLACK;
		u->color = Node::BLACK;
		g = grandparent(n);
		g->color = Node::RED;
		insert_case1(g);
	}
	else {
		insert_case4(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::insert_case4(Node *n)
{
	Node *g = grandparent(n);

	if ((n == n->parent->right) && (n->parent == g->left)) {
		rotate_left(n->parent);
		n = n->left;

	}
	else if ((n == n->parent->left) && (n->parent == g->right)) {
		rotate_right(n->parent);
		n = n->right;
	}
	insert_case5(n);
}

template <typename K, typename T>
void IntervalTree<K, T>::insert_case5(Node *n)
{
	Node *g = grandparent(n);

	n->parent->color = Node::BLACK;
	g->color = Node::RED;
	if (n == n->parent->left)
		rotate_right(g);
	else
		rotate_left(g);
}

/* REMOVE section */
template <typename K, typename T>
void IntervalTree<K, T>::remove(const Interval<K> &key, const T *dataCmp)
{
	Node *cur = peek(root_, key, dataCmp);

	if (cur != &leaf_) {
		//BST delete, then correct
		Node *removeNode = cur;
		if (cur->left != &leaf_) {
			removeNode = predecessor(cur);
		}
		else if (cur->right != &leaf_) {
			removeNode = successor(cur);
		}
		//swap replacement(removeNode) with cur
		if (removeNode != cur) {
			cur->key = removeNode->key;
			cur->data = removeNode->data;
		}

		remove_case0(removeNode);
		updateMax(cur);
		size_--;
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case0(Node *n)
{
	// Precondition: n has at most one non-leaf child.
	Node *child = is_leaf(n->right) ? n->left : n->right;
	
	replace_node(n, child);
	if (n->color == Node::BLACK) {
		if (child->color == Node::RED) {
			child->color = Node::BLACK;
		}
		else {
			remove_case1(child);
		}
	}

	delete n;
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case1(Node *n)
{
	if (n->parent) {
		remove_case2(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case2(Node *n)
{
	Node *s = sibling(n);

	if (s->color == Node::RED) {
		n->parent->color = Node::RED;
		s->color = Node::BLACK;
		if (n == n->parent->left) {
			rotate_left(n->parent);
		}
		else {
			rotate_right(n->parent);
		}
	}
	remove_case3(n);
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case3(Node *n)
{
	Node *s = sibling(n);

	if ((n->parent->color == Node::BLACK)
		&& (s->color == Node::BLACK)
		&& (s->left->color == Node::BLACK)
		&& (s->right->color == Node::BLACK)) {
		s->color = Node::RED;
		remove_case1(n->parent);
	}
	else {
		remove_case4(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case4(Node *n)
{
	Node *s = sibling(n);

	if ((n->parent->color == Node::RED)
		&& (s->color == Node::BLACK)
		&& (s->left->color == Node::BLACK)
		&& (s->right->color == Node::BLACK)) {
		s->color = Node::RED;
		n->parent->color = Node::BLACK;
	}
	else {
		remove_case5(n);
	}
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case5(Node *n)
{
	Node *s = sibling(n);

	if (s->color == Node::BLACK) {
		if ((n == n->parent->left)
			&& (s->right->color == Node::BLACK)
			&& (s->left->color == Node::RED)) {
			s->color = Node::RED;
			s->left->color = Node::BLACK;
			rotate_right(s);
		}
		else if ((n == n->parent->right)
			&& (s->left->color == Node::BLACK)
			&& (s->right->color == Node::RED)) {
			s->color = Node::RED;
			s->right->color = Node::BLACK;
			rotate_left(s);
		}
	}
	remove_case6(n);
}

template <typename K, typename T>
void IntervalTree<K, T>::remove_case6(Node *n)
{
	Node *s = sibling(n);

	s->color = n->parent->color;
	n->parent->color = Node::BLACK;

	if (n == n->parent->left) {
		s->right->color = Node::BLACK;
		rotate_left(n->parent);
	}
	else {
		s->left->color = Node::BLACK;
		rotate_right(n->parent);
	}
}

/* ACCESS section */

//TODO: check intersect/within and make sure the booleans work (x:x) intersects (x:x) and is within (x:x)

//TODO: elements of vector can still be changed (like with iterator), maybe copy instead of ref? (could be memory heavy)
template <typename K, typename T>
std::vector<std::pair<const Interval<K> &, T&>> *
IntervalTree<K, T>::intersect(const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> *intervals)
{
	std::vector<std::pair<const Interval<K> &, T&>> *list = intervals;
	if (list == nullptr) {
		list = new std::vector<std::pair<const Interval<K> &, T&>>;
	}

	list = intersect_(root_, key, *list);
	return list;
}

template <typename K, typename T>
std::vector<std::pair<const Interval<K> &, T&>> *
IntervalTree<K, T>::intersect_(Node *root, const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> &intervals)
{
	if (root == &leaf_) {
		return &intervals;
	}

	if (key.low <= root->max) {
		intersect_(root->left, key, intervals);
	}
	if (key.low <= root->key.high && key.high >= root->key.low) {
		intervals.push_back(std::pair<const Interval<K> &, T&>(root->key, root->data));
	}
	if (key.high >= root->key.low) {
		intersect_(root->right, key, intervals);
	}

	return &intervals;
}

template <typename K, typename T>
std::vector<std::pair<const Interval<K> &, T&>> *
IntervalTree<K, T>::within(const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> *intervals)
{
	std::vector<std::pair<const Interval<K> &, T&>> *list = intervals;
	if (list == nullptr) {
		list = new std::vector<std::pair<const Interval<K> &, T&>>;
	}

	list = within_(root_, key, *list);
	return list;
}

template <typename K, typename T>
std::vector<std::pair<const Interval<K> &, T&>> *
IntervalTree<K, T>::within_(Node *root, const Interval<K> &key, std::vector<std::pair<const Interval<K> &, T&>> &intervals)
{
	if (root == &leaf_) {
		return &intervals;
	}

	if (key.low <= root->max && key.low <= root->key.low) { //TODO: check 2nd test
		within_(root->left, key, intervals);
	}
	if (key.low <= root->key.low && key.high >= root->key.high) {
		intervals.push_back(std::pair<const Interval<K> &, T&>(root->key, root->data));
	}
	if (key.high >= root->key.low) { //TODO: need extra test here? think more about intervals within others
		within_(root->right, key, intervals);
	}

	return &intervals;
}

template <typename K, typename T>
T& IntervalTree<K, T>::operator[](const Interval<K> &key)
{
	Node *node = peek(root_, key);
	if (node != &leaf_) {
		//key exists
		return node->data;
	}

	T data; //TODO: does this work on types with complex allocation?
	return insert(key, data);
}

template <typename K, typename T>
void IntervalTree<K, T>::foreach_postorder(Node *root, std::function<void(Node *)> func)
{
	if (root == &leaf_) {
		return;
	}
	foreach_postorder(root->left, func);
	foreach_postorder(root->right, func);
	func(root);
}



//TODO: remove debug method
template <typename K, typename T>
void IntervalTree<K, T>::print(void)
{
	std::queue<Node *> q;
	q.push(root_);

	while (q.size() > 0) {
		int size = q.size();
		for (int i = 0; i < size; i++) {
			Node *cur = q.front();
			q.pop();

			if (cur != &leaf_) {
				std::cout << "(" << cur->key.low << ":" << cur->key.high << ")" << cur->max << "|" << cur->data << ", ";
				q.push(cur->left);
				q.push(cur->right);
			}
			else {
				std::cout << "NULL, ";
			}
		}
		std::cout << std::endl;
	}
}

#endif // !BL_IntervalTree_H
