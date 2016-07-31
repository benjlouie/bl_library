//insert/delete cases used straight from wikipedia

#ifndef BL_RBTREE_H
#define BL_RBTREE_H

//TODO: remove debug dependence
#include <queue>
#include <iostream>

//TODO: use const for K and T
//TODO: check if some allocator needs to be passed in

template <typename K, typename T>
class RBTree
{
public:
	RBTree(void);
	~RBTree(void);

	void insert(const K &key, const T &data);
	void remove(const K &key);
	T operator[](const K &key);

	//TODO: remove debug method
	void print(void);

protected:
	struct Node {
		K key;
		T data;
		enum Color { RED, BLACK } color;
		Node *parent;
		Node *left;
		Node *right;
	};

private:
	size_t size_;
	Node *root_;
	Node leaf_;

	typename RBTree<K,T>::Node *grandparent(Node *n);
	typename RBTree<K, T>::Node *uncle(Node *n);
	typename RBTree<K, T>::Node *sibling(Node *n);
	typename RBTree<K, T>::Node *predecessor(Node *n);
	typename RBTree<K, T>::Node *successor(Node *n);
	void rotate_right(Node *n);
	void rotate_left(Node *n);
	bool is_leaf(Node *n);
	void replace_node(Node *n, Node *replacement);

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
};


template <typename K, typename T>
RBTree<K, T>::RBTree(void)
{
	size_ = 0;
	root_ = &leaf_;
	leaf_.color = Node::BLACK;
	leaf_.left = nullptr;
	leaf_.right = nullptr;
}

template <typename K, typename T>
RBTree<K, T>::~RBTree(void)
{
	//TODO: free recursively
}

template <typename K, typename T>
typename RBTree<K, T>::Node *
RBTree<K, T>::grandparent(Node *n)
{
	if (n && n->parent) {
		return n->parent->parent;
	}
	else {
		return nullptr;
	}
}

template <typename K, typename T>
typename RBTree<K, T>::Node *
RBTree<K, T>::uncle(Node *n)
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
typename RBTree<K, T>::Node *
RBTree<K, T>::sibling(Node *n)
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
typename RBTree<K, T>::Node *
RBTree<K, T>::predecessor(Node *n)
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
typename RBTree<K, T>::Node *
RBTree<K, T>::successor(Node *n)
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
void RBTree<K, T>::rotate_right(Node *n)
{
	Node *left = n->left;
	Node *parent = nullptr;
	Node **parentLink; //TODO: check this

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
}

template <typename K, typename T>
void RBTree<K, T>::rotate_left(Node *n)
{
	Node *right = n->right;
	Node *parent = nullptr;
	Node **parentLink; //TODO: check this

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
}

template <typename K, typename T>
bool RBTree<K, T>::is_leaf(Node *n)
{
	return n == &leaf_;
}

template <typename K, typename T>
void RBTree<K, T>::replace_node(Node *n, Node *replacement)
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



template <typename K, typename T>
void RBTree<K, T>::insert(const K &key, const T &data)
{
	Node *n = new Node{ key, data, RBTree::Node::RED, nullptr, &leaf_, &leaf_ };

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
}

template <typename K, typename T>
void RBTree<K, T>::insert_case1(Node *n)
{
	if (n->parent == nullptr) {
		n->color = Node::BLACK;
	}
	else {
		insert_case2(n);
	}
}

template <typename K, typename T>
void RBTree<K, T>::insert_case2(Node *n)
{
	if (n->parent->color == Node::BLACK) {
		return; /* Tree is still valid */
	}
	else {
		insert_case3(n);
	}
}

template <typename K, typename T>
void RBTree<K, T>::insert_case3(Node *n)
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
void RBTree<K, T>::insert_case4(Node *n)
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
void RBTree<K, T>::insert_case5(Node *n)
{
	Node *g = grandparent(n);

	n->parent->color = Node::BLACK;
	g->color = Node::RED;
	if (n == n->parent->left)
		rotate_right(g);
	else
		rotate_left(g);
}

template <typename K, typename T>
void RBTree<K, T>::remove(const K &key)
{
	Node *cur = root_;

	while (cur != &leaf_) {
		if (key < cur->key) {
			cur = cur->left;
		}
		else if (key > cur->key) {
			cur = cur->right;
		}
		else {
			break;
		}
	}

	if (cur != &leaf_) {
		//BST delete, then correct
		Node *removeNode = cur;
		if (cur->left != &leaf_) {
			removeNode = predecessor(cur);
			//removeNode->parent->right = removeNode->left;
		}
		else if (cur->right != &leaf_) {
			removeNode = successor(cur);
			//removeNode->parent->left = removeNode->right;
		}
		//swap replacement(removeNode) with cur
		if (removeNode != cur) {
			cur->key = removeNode->key;
			cur->data = removeNode->data;
		}

		remove_case0(removeNode);
	}
}

//TODO: check this method
template <typename K, typename T>
void RBTree<K, T>::remove_case0(Node *n)
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
void RBTree<K, T>::remove_case1(Node *n)
{
	if (n->parent) {
		remove_case2(n);
	}
}

template <typename K, typename T>
void RBTree<K, T>::remove_case2(Node *n)
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
void RBTree<K, T>::remove_case3(Node *n)
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
void RBTree<K, T>::remove_case4(Node *n)
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
void RBTree<K, T>::remove_case5(Node *n)
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
void RBTree<K, T>::remove_case6(Node *n)
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

template <typename K, typename T>
T RBTree<K, T>::operator[](const K &key)
{
	return nullptr;
}



//TODO: remove debug method
template <typename K, typename T>
void RBTree<K, T>::print(void)
{
	std::queue<Node *> q;
	q.push(root_);

	while (q.size() > 0) {
		Node *cur = q.front();
		q.pop();

		if (cur != &leaf_) {
			std::cout << cur->key << ", ";
			q.push(cur->left);
			q.push(cur->right);
		}
		else {
			std::cout << "NULL, ";
		}
	}
}

#endif // !BL_RBTREE_H
