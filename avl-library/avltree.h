#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include <assert.h>
#include "stack.h"

template <class T>
class AVLTree {

public:
	AVLTree();
	AVLTree(const AVLTree&);
	~AVLTree();
	AVLTree<T>& operator = (const AVLTree<T>& other);

	bool isEmpty() const;
	void clear();
	bool contains(const T&) const;
	void insert(const T&);
	void remove(const T&);

	/*
	 * Returns "true" if the AVL trees have exactly the same
	 * elements regardless of the order of appearance in both
	 * trees. Otherwise, it returns "false".
	 *
	 *
	 * This operator must be implemented in O(min(m,n)).				 *
	 * Where n and m are the sizes of two AVL trees to compare. *
	 *************************************************************
	 */
	bool operator == (const AVLTree<T>& other) const;

	/*
	 * This iterator is based on an inorder traversal of the
	 * current tree.
	 */
	class Iterator;
	Iterator begin() const;
	T& operator[] (const Iterator&);
	const T& operator[] (const Iterator&) const;

	/*
	 * These functions are implemented for testing and diagnostic purposes.
	 * You must not use them in your implementations, nor modify them!
	 */
	int size() const;
	int height() const;
	int balance(const T&) const;
	int get_balance(const T&) const;
	int occurrence(const T&) const;
	void display() const;

private:
	struct Node {
		Node(const T&);
		T content;
		int balance;
		Node* left;
		Node* right;
	};
	Node* root;
	
	bool sameNode(Node*, Node*);
	Node* remove(Node*&, const T&);
	Node* insert(Node*&, const T&);
	Node* balance(Node*&);
	bool compare(Node*) const;
	void clear(Node*&);
	const T* searchElem(Node*, const T&) const;
	Node* copy(Node* node);
	Node* singleLeftRotation(Node*&);
	Node* singleRightRotation(Node*&);
	Node* doubleLeftRotation(Node*&);
	Node* doubleRightRotation(Node*&);
	int size(Node*) const;
	int getBalance(Node*&);
	Node* minNode(Node*&);
	Iterator searchEqualOrPrevious(const T&) const;
	Iterator searchEqualOrNext(const T&) const;
	Iterator search(const T&) const;
	Iterator end() const;
	/*
	 * These functions are implemented for testing purposes.
	 */
	int count(Node*) const;
	Node* find(const T&) const;
	int height(Node* n) const;
	int occurrence(Node* n, const T&) const;
	void prepareDisplay(const Node*, int, int&, T*, int*) const;
public:
	class Iterator {
	public:
		Iterator(const Iterator&);
		Iterator(const AVLTree&);
		operator bool() const;
		Iterator operator++(int);
		Iterator& operator++();

	private:
		Node* current;
		const AVLTree& associated_tree;
		Stack<Node*> path;
		friend class AVLTree;
	};
};

/************ Public Functions ***************/

template <class T>
AVLTree<T>::Node::Node(const T& c) : content(c), balance(0), left(nullptr), right(nullptr) {
}

template <class T>
AVLTree<T>::AVLTree() : root(nullptr) {
}

template <class T>
AVLTree<T>::AVLTree(const AVLTree<T>& other) : root(nullptr) {
	this->operator =(other);
}

template <class T>
AVLTree<T>::~AVLTree() {
	clear();
}

template <class T>
bool AVLTree<T>::isEmpty() const {
	return root == nullptr;
}

template <class T>
void AVLTree<T>::clear() {
	clear(root);
}

template <class T>
bool AVLTree<T>::contains(const T& element) const {
	if (searchElem(root, element) == nullptr)
		return false;
	else
		return true;
}

template <class T>
void AVLTree<T>::insert(const T& e) {
	insert(root, e);
}

template <class T>
void AVLTree<T>::remove(const T& e) {
	remove(root, e);
}

template <class T>
AVLTree<T>& AVLTree<T>::operator = (const AVLTree& other) {
	if (this == &other) {
		return *this;
	}
	clear();
	root = copy(other.root);
	return *this;
}

template <class T>
bool AVLTree<T>::operator == (const AVLTree<T>& other) const {
	int found;
	Iterator iter1 = begin();
	while (!iter1.path.empty()) {
		found = 0;
		T content1 = this->operator[](iter1);
		for (Iterator iter2 = other.begin(); iter2; ++iter2) {
			T content2 = this->operator[](iter2);
			if (content1 == content2)
				found = +1;
		}
		if (found == 0) {
			return false;
		}
		++iter1;
	}
	return true;
}

template <class T>
typename AVLTree<T>::Iterator AVLTree<T>::begin() const {
	Iterator iter(*this);
	iter.current = root;
	if (iter.current != nullptr) {
		while (iter.current->left != nullptr) {
			iter.path.push(iter.current);
			iter.current = iter.current->left;
		}
	}
	return iter;
}

template <class T>
T& AVLTree<T>::operator[](const Iterator& i) {
	Iterator found = searchEqualOrPrevious(i.current->content);
	return found.current->content;
}

template <class T>
const T& AVLTree<T>::operator[](const Iterator& i) const {
	Iterator found = searchEqualOrPrevious(i.current->content);
	return found.current->content;
}

/************ Private Functions ***************/

/*

Returns a pointer to the new root node after removing the node
containing the object of type T passed in parameters
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::remove(Node*& node, const T& e)
{
	/* Traverse the tree using recursion and find the node
	whose content is equal to the object e passed in parameter*/
	if (node == nullptr)
		return node;
	if (e < node->content)
	{
		node->left = remove(node->left, e);
	}
	else if (e > node->content)
	{
		node->right = remove(node->right, e);

	}
	/* Once found, disconnect the node from the tree and remove it
	while keeping the child nodes */
	else
	{
		if (node->left == nullptr || node->right == nullptr)
		{
			Node* temp = node->left ? node->left : node->right;
			if (temp == nullptr) {
				temp = node;
				node = nullptr;
			}
			else {
				*node = *temp;
			}
			delete temp;
		}
		else
		{
			Node* temp = minNode(node->right);


			node->content = temp->content;

			node->right = remove(node->right,
				temp->content);
		}
	}

	if (node == nullptr) return node;
	/* Call the balance method to balance the node */
	node = balance(node);

	return node;
}

/*

Returns a pointer to the node with the smallest value (content)
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::minNode(Node*& node)
{
	Node* current = node;
	/* Traverse downwards to find the leftmost leaf */
	while (current->left != nullptr)
		current = current->left;

	return current;
}

/*

Returns true if the two trees are equal
*/
template <class T>
bool AVLTree<T>::compare(Node* node) const
{
	if (node) {
		if (!compare(node->left)) {
			return false;
		}
		if (!compare(node->right)) {
			return false;
		}
		if (!contains(node->content)) {
			return false;
		}
	}
	return true;
}
/*
 * Returns true if the two nodes passed as parameters are equal
 *
*/
template <class T>
bool AVLTree<T>::sameNode(Node* node1, Node* node2)
{
	if (!node1 && !node2)
		return true;
	if (!node1 || !node2)
		return false;

	if (node1->data == node2->data)
	{
		bool left = sameNode(node1->left, node2->left);
		bool right = sameNode(node1->right, node2->right);
		return (left && right);
	}
	else
		return false;
}

/*
 * Returns the balance factor of the node passed as parameter
 *
*/
template <class T>
int AVLTree<T>::getBalance(Node*& node) {
	return (size(node->left) - size(node->right));
}


/*
 * Returns the size of a node
 *
*/
template <class T>
int AVLTree<T>::size(Node* n) const {
	if (n == nullptr)
		return 0;
	int left = size(n->left);
	int right = size(n->right);
	return 1 + (left < right ? right : left);
}

/*
 * Returns a pointer to the node passed as parameter after balancing it
 * if its balance factor is different from the values -1, 0, and 1
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::balance(Node*& node) {

	node->balance = size(node->left) - size(node->right);

	int balanceFactor = getBalance(node);

	if (balanceFactor > 1 && getBalance(node->left) >= 0)
		return singleRightRotation(node);

	if (balanceFactor > 1 && getBalance(node->left) < 0)
	{
		node->left = singleLeftRotation(node->left);
		return singleRightRotation(node);
	}

	if (balanceFactor < -1 && getBalance(node->right) <= 0)
		return singleLeftRotation(node);

	if (balanceFactor < -1 && getBalance(node->right) > 0)
	{
		node->right = singleRightRotation(node->right);
		return singleLeftRotation(node);
	}
	return node;
}
/*
 * Returns a pointer to the new root node
 * after inserting the object 'e' in the correct place (node)
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::insert(Node*& node, const T& e) {
	if (node == nullptr) {
		node = new Node(e);
	}
	else if (e < node->content) {
		node->left = insert(node->left, e);
	}
	else if (e > node->content) {
		node->right = insert(node->right, e);
	}
	else {
		node->content = e;
		return node;
	}
	node = balance(node);
	return node;
}

/*
 * Returns a pointer to the content of the node equal to the object 'element'
 * passed as parameter
 *
*/
template <class T>
const T* AVLTree<T>::searchElem(Node* node, const T& element) const {
	if (node == nullptr) {
		return nullptr;
	}
	if (element == node->content) {
		return &(node->content);
	}
	if (element < node->content) {
		return searchElem(node->left, element);
	}
	else {
		return searchElem(node->right, element);
	}
}

/*
 * Returns a pointer to the new subtree root node after
 * performing a single right rotation
 * This rotation is performed when a new node is inserted
 * in the right child of the right subtree.
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::singleRightRotation(Node*& subtreeRoot) {
	Node* temp = subtreeRoot->left;
	Node* a = temp->right;
	temp->right = subtreeRoot;
	subtreeRoot->left = a;
	subtreeRoot->balance = size(subtreeRoot->left) - size(subtreeRoot->right);
	temp->balance = size(temp->left) - size(temp->right);
	return temp;
}
/*
 * Returns a pointer to the new root node after performing a single left rotation
 * This rotation is performed when a new node is inserted as the left child of the left subtree.
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::singleLeftRotation(Node*& subtreeRoot) {
	Node* temp = subtreeRoot->right;

	Node* a = temp->left;

	temp->left = subtreeRoot;
	subtreeRoot->right = a;

	subtreeRoot->balance = size(subtreeRoot->left) - size(subtreeRoot->right);
	temp->balance = size(temp->left) - size(temp->right);

	return temp;
}


/*
 * Returns a pointer to the new root node after performing a double right then left rotation
 * This rotation is performed when a new node is inserted as the right child of the left subtree.
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::doubleLeftRotation(Node*& subtreeRoot) {
	subtreeRoot->left = rotationRightSimple(subtreeRoot->left);
	return singleLeftRotation(subtreeRoot);
}


/*
 * Returns a pointer to the new root node after performing a double left then right rotation
 * This rotation is performed when a new node is inserted as the left child of the right subtree.
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::doubleRightRotation(Node*& subtreeRoot) {
	subtreeRoot->right = singleLeftRotation(subtreeRoot->right);
	return rotationRightSimple(subtreeRoot);
}

/*
 * Helper function, recursively traverses the tree starting from the root node passed as parameter
 * and frees the memory of the nodes.
 *
*/
template <class T>
void AVLTree<T>::clear(Node*& node) {

	if (node != nullptr) {
		clear(node->right);
		clear(node->left);
		delete(node);
	}

	node = nullptr;
}
/*
 * Returns a pointer to the root node of the tree after copying all the nodes
 * of the current tree in the same order. Returns NULL if the node passed as a
 * parameter points to a null object.
 *
*/
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::copy(Node* node) {
	if (node != nullptr) {
		Node* copyNode = new Node(node->content);
		copyNode->balance = node->balance;
		Node* left = copy(node->left);
		Node* right = copy(node->right);
		copyNode->left = left;
		copyNode->right = right;
		return copyNode;
	}
	else {
		return nullptr;
	}
}

/*
 * Returns an object of type Iterator positioned on the element preceding
 * the element e passed as a parameter in the current tree.
 *
*/
template <class T>
typename AVLTree<T>::Iterator AVLTree<T>::searchEqualOrPrevious(const T& e) const {
	Node* last = nullptr;
	Node* n = root;
	while (n) {
		if (e < n->content) {
			n = n->left;
		}
		else if (e > n->content) {
			last = n;
			n = n->right;
		}
		else {
			return search(e);
		}
	}
	if (last != nullptr)
		return search(last->content);
	return Iterator(*this);
}

/*
 * Returns an object of type Iterator positioned on the element e to search.
*/
template <class T>
typename AVLTree<T>::Iterator AVLTree<T>::search(const T& e) const {
	Iterator iter(*this);
	Node* n = root;
	while (n) {
		if (e < n->content) {
			iter.path.push(n);
			n = n->left;
		}
		else if (e > n->content) {
			n = n->right;
		}
		else {
			iter.current = n;
			return iter;
		}
	}
	iter.path.clear();
	return iter;
}

/*
 * Returns an object of type Iterator pointing to the end node of the
 * current tree.
*/
template <class T>
typename AVLTree<T>::Iterator AVLTree<T>::end() const {
	return Iterator(*this);
}
/************ Iterator ***************/

template <class T>
AVLTree<T>::Iterator::Iterator(const AVLTree& a) : associated_tree(a), current(nullptr) {
}

template <class T>
AVLTree<T>::Iterator::Iterator(const Iterator& i) : associated_tree(i.associated_tree), current(i.current) {
}

template <class T>
typename AVLTree<T>::Iterator AVLTree<T>::Iterator::operator++(int) {
	assert(current);
	Node* next = current->right;
	while (next) {
		path.push(next);
		next = next->left;
	}
	if (!path.empty())
		current = path.pop();
	else
		current = nullptr;
	return *this;
}

template <class T>
typename AVLTree<T>::Iterator& AVLTree<T>::Iterator::operator++() {
	assert(current);
	Node* next = current->right;
	while (next) {
		path.push(next);
		next = next->left;
	}
	if (!path.empty())
		current = path.pop();
	else
		current = nullptr;
	return *this;
}

template <class T>
AVLTree<T>::Iterator::operator bool() const {
	return current != nullptr;
}

/************ Test Functions ***************/

#include <climits>

template <class T>
int AVLTree<T>::size() const {
	return count(root);
}

template <class T>
int AVLTree<T>::height() const {
	return height(root);
}

template <class T>
int AVLTree<T>::balance(const T& e) const {
	int bal = INT_MIN;
	if (contains(e)) {
		Node* n = find(e);
		bal = height(n->left) - height(n->right);
	}
	return bal;
}

template <class T>
int AVLTree<T>::get_balance(const T& e) const {
	int bal = INT_MIN;
	if (contains(e)) {
		Node* n = find(e);
		bal = n->balance;
	}
	return bal;
}

template <class T>
int AVLTree<T>::occurrence(const T& e) const {
	return occurrence(root, e);
}

template <class T>
int AVLTree<T>::count(Node* n) const {
	if (n == nullptr)
		return 0;
	return 1 + count(n->left) + count(n->right);
}

template <class T>
typename AVLTree<T>::Node* AVLTree<T>::find(const T& e) const {
	Node* n = root;
	while (n != nullptr && n->content != e) {
		if (n->content > e)
			n = n->left;
		else
			n = n->right;
	}
	return n;
}

template <class T>
int AVLTree<T>::height(Node* n) const {
	if (n == nullptr)
		return 0;
	int l = height(n->left);
	//std::cout << "l " << l << std::endl;
	int r = height(n->right);
	//std::cout << "r " << r << std::endl;
	return 1 + (l < r ? r : l);
}

template <class T>
int AVLTree<T>::occurrence(Node* n, const T& e) const {
	int o = 0;
	if (n != nullptr) {
		if (n->content == e)
			o++;
		o += occurrence(n->left, e) + occurrence(n->right, e);
	}
	return o;
}
#include <iostream>

template <class T>
void AVLTree<T>::display() const {
	std::cout << "Content of the tree (";
	int n = size();
	std::cout << n << " nodes)\n";
	T* elements = new T[n];
	int* depths = new int[n];
	n = 0;
	prepareDisplay(root, 0, n, elements, depths);
	for (int p = 0;; p++) {
		bool lastDepth = true;
		for (int i = 0; i < n; i++) {
			if (depths[i] == p) {
				std::cout << elements[i];
				lastDepth = false;
			}
			std::cout << '\t';
		}
		std::cout << '\n';
		if (lastDepth) break;
	}
	delete[] depths;
	delete[] elements;
	std::cout << "-------------" << std::endl;
}

template <class T>
void AVLTree<T>::prepareDisplay(const Node* n, int depth, int& index, T* elements, int* depths) const {
	if (n == nullptr) return;
	prepareDisplay(n->left, depth + 1, index, elements, depths);
	elements[index] = n->data;
	depths[index] = depth;
	index++;
	prepareDisplay(n->right, depth + 1, index, elements, depths);
}

#endif

