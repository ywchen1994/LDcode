#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>
template <class T>
class Stack {
public:
	Stack() :top(0) {
	}
	~Stack() {
		while (!isEmpty()) {
			pop();
		}
		isEmpty();
	}

	void push(const T& object);
	T pop();
	const T& topElement();
	bool isEmpty();
	int size();
private:
	int _size=0;
	struct StackNode {              // linked list node
		T data;                     // data at this node
		StackNode *next;            // next node in list
       // StackNode constructor initializes both fields
		StackNode(const T& newData, StackNode *nextNode)
			: data(newData), next(nextNode) {}
	};

	// My Stack should not allow copy of entire stack
	Stack(const Stack& lhs) {}

	// My Stack should not allow assignment of one stack to another
	Stack& operator=(const Stack& rhs) {}
	StackNode *top;                 // top of stack

};

template <class T>
void Stack<T>::push(const T& obj) {
	top = new StackNode(obj, top);
	_size++;
}

template <class T>
T Stack<T>::pop() {
	if (!isEmpty()) {
		StackNode *topNode = top;
		top = top->next;
		T data = topNode->data;
		_size--;
		delete topNode;
		return data;
	}
}

template <class T>
const T& Stack<T>::topElement() {
	if (!isEmpty()) {
		return top->data;
	}
}

template <class T>
inline bool Stack<T>::isEmpty() {
	if (top == 0) {
		return true;
	}
	else {
		return false;
	}
}

template<class T>
inline int Stack<T>::size()
{
	return _size;
}

#endif 