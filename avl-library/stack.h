/*
 * Stack Class.
 *
 */

#ifndef __STACK_H__
#define __STACK_H__

template <class T>
class Stack {
public:
    Stack();
    Stack(Stack<T>& other);
    ~Stack();

    void push(const T&);
    T pop();
    bool empty() const;
    void clear();
    Stack<T>& operator = (const Stack<T>& other);

private:
    class Cell {
    public:
        Cell(const T& e, Cell* n);
        T content;
        Cell* next;
    };

    Cell* top;
};

#include "assert.h"

template <class T>
Stack<T>::Stack() {
    top = nullptr;
}

template <class T>
Stack<T>::Stack(Stack<T>& other) {
    if (other.top != nullptr) {
        top = new Cell(other.top->content, nullptr);
        Cell* cp = other.top;
        Cell* c = top;

        while (cp->next != nullptr) {
            c->next = new Cell(cp->next->content, nullptr);
            c = c->next;
            cp = cp->next;
        }
    }
    else
        top = nullptr;
}

template <class T>
Stack<T>::~Stack() {
    clear();
}

template <class T>
void Stack<T>::clear() {
    while (!empty())
        pop();
}

template <class T>
Stack<T>::Cell::Cell(const T& e, Cell* n) : next(n) {
    content = e;
}

template <class T>
void Stack<T>::push(const T& e) {
    top = new Cell(e, top);
    assert(top);
}

template <class T>
T Stack<T>::pop() {
    assert(top);
    Cell c(*top);
    delete top;
    top = c.next;
    return c.content;
}

template <class T>
bool Stack<T>::empty() const {
    return top == nullptr;
}

template <class T>
Stack<T>& Stack<T>::operator = (const Stack<T>& other) {
    if (this == &other)
        return *this;

    clear();

    if (other.top != nullptr) {
        top = new Cell(other.top->content, nullptr);
        Cell* cp = other.top;
        Cell* c = top;

        while (cp->next != nullptr) {
            c->next = new Cell(cp->next->content, nullptr);
            c = c->next;
            cp = cp->next;
        }
    }
    return *this;
}

#endif
