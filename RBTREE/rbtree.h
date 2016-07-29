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
	T* remove(const K &key);
	T* operator[](const K &key);

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

	typename RBTree<K,T>::Node *grandparent(Node *n);
	typename RBTree<K, T>::Node *uncle(Node *n);
	void rotate_right(Node *n);
	void rotate_left(Node *n);
	void insert_case1(Node *n);
	void insert_case2(Node *n);
	void insert_case3(Node *n);
	void insert_case4(Node *n);
	void insert_case5(Node *n);
};


template <typename K, typename T>
RBTree<K, T>::RBTree(void)
{
	size_ = 0;
	root_ = nullptr;
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
	if (g == NULL)
		return NULL; // No grandparent means no uncle
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
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

	//rotate right
	*parentLink = right;
	n->right = right->left;
	n->parent = right;
	right->parent = parent;
	right->left = n;
}

template <typename K, typename T>
void RBTree<K, T>::insert(const K &key, const T &data)
{
	Node *n = new Node{ key, data, RBTree::Node::RED, nullptr, nullptr, nullptr };

	Node *prev = nullptr;
	Node *cur = root_;
	Node **prevLink = &root_;
	//BST insert at leaf
	while (cur) {
		prev = cur;
		if (key > cur->key) {
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
T* RBTree<K, T>::remove(const K &key)
{
	return nullptr;
}

template <typename K, typename T>
T* RBTree<K, T>::operator[](const K &key)
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

		if (cur) {
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
