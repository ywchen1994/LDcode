#include<cstddef>
#include<cassert>
template<class T>
class CVector
{
public:
	typedef T* Iterator;
private:
	T* mem;
	size_t pSize;      //建構食慾分配的大小，容器容量
	size_t nSize;      //實際元素個數         
	Iterator pBegin;   //指向向量的第一個元素
	Iterator pEnd;     //指向向量的最後一個元素的下一位

public:
	CVector(Iterator b, Iterator e); 
	CVector(size_t s);  
	CVector();

	~CVector();

	Iterator begin();    //返回第一個元素的位址
	Iterator end();      //

	size_t size();      

	void reserve(size_t t);
	void push_back(T t);

	size_t compacity();
	T & operator [](size_t i);

};
template<class T> 
CVector<T>::CVector(Iterator b, Iterator e) :pSize(1024)
{
	this->nSize = size_t(e - b);
	this->mem = (T*)new char[nSize * sizeof(T)];

	this->pBegin = this->mem;
	this->pEnd = this->mem + this->nSize;

	for (size_t i = 0; i< this->nSize; ++i)
	{
		new (mem + i) T(*(b + i));
	}
}
template<class T>
CVector<T>::CVector(size_t s) :pSize(1024)
{
	if (s == 0)
	{
		this->mem = NULL;
		this->pBegin = NULL;
		this->pEnd = NULL;
		this->nSize = 0;
	}
	else
	{
		while (pSize < s)
			pSize *= 2;
		this->nSize = s;
		this->mem = (T*)new char[pSize * sizeof(T)];
		new (this->mem) T[s];
		this->pBegin = this->mem;
		this->pEnd = this->mem + s;
	}
}
template<class T>
CVector<T>::CVector() :pSize(1024)
{
	this->nSize = 0;
	this->mem = (T*)new char[this->pSize * sizeof(T)];
	this->pBegin = this->pEnd = mem;
}

template<class T>
CVector<T>::~CVector()
{
	for (size_t i = this->nSize; i > 0; i--)
		this->mem[i - 1].~T();
	delete[](char*)this->mem;
}
template<class T>
typename CVector<T>::Iterator CVector<T>::begin()
{
	return this->pBegin;
}
template<class T>
typename CVector<T>::Iterator CVector<T>::end()
{
	return this->pEnd;
}

template<class T>
size_t CVector<T>::size()
{
	return this->nSize;
}
template<class T>
void CVector<T>::reserve(size_t t)
{
	
	T *p = (T*)new char[t * sizeof(T)];

	if (this->nSize > 0)
		memcpy(p, this->mem, this->nSize * sizeof(T));

	delete[](char*)this->mem;
	this->mem = p;
	this->pBegin = p;
	this->pEnd = p + this->nSize;

}
template<class T>
void CVector<T>::push_back(T t)
{
	if (nSize == pSize)
	{

		pSize *= 2;
		T* p = (T*)new char[pSize * sizeof(T)];
		memcpy(p, this->mem, nSize * sizeof(T));
		delete[](char*)this->mem;

		this->mem = p;
		this->pBegin = p;
		this->pEnd = this->mem + nSize;
	}

	new (this->mem + this->nSize) T(t);
	++pEnd;
	++nSize;
}

template<class T>
size_t CVector<T>::compacity()
{
	return this->pSize;
}
template<class T>
T& CVector<T>::operator [](size_t i)
{
	return mem[i];
}