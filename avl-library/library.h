/*
* 
* library Class.
*
*/

#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "avltree.h"
#include "book.h"
#include <string>

class Library {
	/**** You are not allowed to modify the public interface of this class ******/
	/**** You are not allowed to add public functions or modify the signatures of public functions **********/
public:
	Library();
	~Library();
	Library& operator = (const Library&);

	/*
	 * Insert a Book in the library.
	 * If the Book already exists in the library, increase
	 * its "total" field with the "total" field of the Book received
	 * as a parameter.
	 */
	void insert(Book&);
	/*
	 * Return the "total" field of an object of type Book.
	 * If the Book is not in the library, return 0.
	 */
	int total(Book&) const;
	/*
	 * Return "true" if the Book is in the library,
	 * "false" otherwise.
	 */
	bool contains(const Book&) const;
	/*
	 * Search for a Book with the "isbn" field. Return
	 * the Book if it is in the library, if not,
	 * return an empty Book (default constructor of
	 * the Book class).
	 */
	Book find(unsigned long) const;
	/*
	 * Merge 2 libraries by inserting the books of the
	 * library received as a parameter into the current library.
	 */
	void merge(Library&);
	/*
	 * Return "true" if the current library has exactly
	 * the same books (based on the "==" operator of the
	 * Book class) as the library received as a parameter.
	 */
	bool operator == (const Library& other) const;


private:
	AVLTree<Book> lib;
	/**** You can add any private function you need ***********/
/**** Don't forget to explain its functionality in a comment ****/
};

Library::Library() {
}

Library::~Library() {
}

Library& Library::operator = (const Library& other) {
	lib = other.lib;
	return *this;
}

void Library::insert(Book& b) {
	lib.insert(b);
}

bool Library::contains(const Book& b) const {
	return lib.contains(b);
}

int Library::total(Book& b) const {
	if (contains(b)) {
		Book found;
		for (AVLTree<Book>::Iterator iter = lib.begin(); iter; ++iter) {
			if (lib[iter] == b) {
				found = lib[iter];
				break;
			}
		}
		return found.copies();
	}
	else
		return 0;
}

Book Library::find(unsigned long b) const {
	Book book;
	Book toFind(b);
	for (AVLTree<Book>::Iterator iter = lib.begin(); iter; ++iter) {
		Book found = lib[iter];
		if (found == toFind)
			return found;
	}
	return book;
}

void Library::merge(Library& bib) {
	for (AVLTree<Book>::Iterator iter2 = bib.lib.begin(); iter2; ++iter2) {
		insert(bib.lib[iter2]);
	}
}

bool Library::operator == (const Library& other) const {
	return lib == other.lib;
}

#endif
