/*
 * Book Class.
 *
 */

#ifndef __BOOK_H__
#define __BOOK_H__

#include <sstream>
#include <ostream>

using namespace std;

class Book {
    /**** You are not allowed to modify the public interface of this class ******/
    /**** You are therefore not allowed to add public functions or **********/
    /**** modify the signatures of public functions ********************************/
public:
    /*
     * Constructor with the following parameters (in this order):
     * 		isbn 		(default value: 0)
     * 		author	(default value: "")
     * 		title		(default value: "")
     * 		total		(default value: 0)
     */
    Book(unsigned long, std::string, std::string, int);
    /*
     * Constructor with a string in the following format:
     * 		title;isbn;author;total
     *
     * For examples, see the file "exemple_librairie_a.txt".
     */
    Book(std::string&);
    Book(const Book&);
    ~Book();
    /*
     * Assignment operator. The "title" and "author" fields
     * must be overwritten.
     * If the Book parameter has the same "isbn" field as the
     * current Book, then the "total" field of the current
     * Book will be incremented by the value of the "total"
     * field of the Book parameter. If not, the "isbn" and
     * "total" fields will also be overwritten.
     */
    Book& operator = (const Book&);

    /*
     * Returns the "total" field.
     */
    int copies() const;
    /*
     * Comparison operators. They are based on the "isbn" field.
     * Two Books are equal if their "isbn" fields are equal. The
     * same goes for the other operators.
     */
    bool operator == (const Book& other) const;
    bool operator != (const Book& other) const;
    bool operator < (const Book& other) const;
    bool operator > (const Book& other) const;

private:
    unsigned long isbn;
    std::string author;
    std::string title;
    int total;
    /**** You can add any necessary private function ***********/
    /**** Don't forget to explain its operation in a comment ****/
    Book& copy(const Book&);

    friend std::ostream& operator << (std::ostream&, const Book&);
};

Book::Book(unsigned long i = 0, std::string a = "", std::string t = "", int s = 0) {
    isbn = i;
    author = a;
    title = t;
    total = s;
}

Book::Book(std::string& ligne) {
    std::string::size_type pos = ligne.find(';');
    std::string t = ligne.substr(0, pos);
    title = t;
    std::string restOfString1 = ligne.substr(pos + 1);

    std::string::size_type pos1 = restOfString1.find(';');
    std::string isbnString = restOfString1.substr(0, pos1);
    unsigned long i = stoull(isbnString);
    isbn = i;
    std::string restOfString2 = restOfString1.substr(pos1 + 1);

    std::string::size_type pos2 = restOfString2.find(';');
    std::string a = restOfString2.substr(0, pos2);
    author = a;
    std::string totalString = restOfString2.substr(pos2 + 1);

    int s = stoi(totalString);
    total = s;
}

Book::Book(const Book& l) {
    title = l.title;
    author = l.author;
    total = l.total;
    isbn = l.isbn;
}

Book::~Book() {
}

int Book::copies() const {
    return this->total;
}

Book& Book::operator = (const Book& other) {
    if (this == &other) {
        return *this;
    }
    this->title = other.title;
    this->author = other.author;
    if (this->isbn == other.isbn) {
        this->total = this->total + other.total;
        return *this;
    }
    return copy(other);
}

/*
 * Returns a pointer to the current Book object
 * after copying the field values of the passed object
 * as current member fields.
 *
*/
Book& Book::copy(const Book& l) {
    title = l.title;
    author = l.author;
    total = l.total;
    isbn = l.isbn;
    return *this;
}

bool Book::operator == (const Book& other) const {
    return this->isbn == other.isbn;
}

bool Book::operator != (const Book& other) const {
    return this->isbn != other.isbn;
}

bool Book::operator < (const Book& other) const {
    return this->isbn < other.isbn;
}

bool Book::operator > (const Book& other) const {
    return this->isbn > other.isbn;
}

std::ostream& operator << (std::ostream& os, const Book& l) {
    os << l.isbn << " [ copies : " << l.total << " ]\n\t" << l.author << " - " << l.title;
    return os;
}

#endif

