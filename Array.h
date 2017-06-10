
#ifndef ARRAY_H
#define ARRAY_H

#include "Tools.h"

//-----------------------------------------------------------------------------
template <class T>
class Array
{
private:
	T * _data;
	size_t _size;

	void clear ();

public:
	Array () { _data = NULL; _size = 0; }
	virtual ~Array () { clear(); }
	size_t size () const { return _size; }

	Array (size_t);
	void reserve (size_t);
	Array (size_t, int);
	void reserve (size_t, int);

	Array (const Array &);
	Array & operator= (const Array &);

	T get (size_t i) const { return _data[i]; }
	void set (T in, size_t i) { _data[i] = in; } 

	void print () const;
};

//-----------------------------------------------------------------------------
template <class T>
void Array<T>::print () const
{
	for(size_t i=0; i<_size; i++) { cout << _data[i] << "\n"; }
	cout << "\n";
}

//-----------------------------------------------------------------------------
template <class T>
Array<T>::Array (const Array & arr)
{
	_size = arr.size();
	_data = new T[_size +1];

	for(size_t i=0; i<_size; i++) { _data[i] = arr.get(i); }
}

//-----------------------------------------------------------------------------
template <class T>
Array<T> & Array<T>::operator= (const Array & arr)
{
	clear();
	if(arr.size() == 0) { return *this; }

	_size = arr.size();
	_data = new T[_size +1];

	for(size_t i=0; i<_size; i++) { _data[i] = arr.get(i); }
	
	return *this;
}

//-----------------------------------------------------------------------------
template <class T>
Array<T>::Array (size_t size, int val)
{
	_size = size;
	_data = new T[_size +1];

	for(size_t i=0; i<_size; i++) { _data[i] = val; }
}

//-----------------------------------------------------------------------------
template <class T>
void Array<T>::reserve (size_t size, int val)
{
	clear();
	_size = size;
	_data = new T[_size +1];
	for(size_t i=0; i<_size; i++) { _data[i] = val; }
}

//-----------------------------------------------------------------------------
template <class T>
Array<T>::Array (size_t size)
{
	_size = size;
	_data = new T[_size +1];
}

//-----------------------------------------------------------------------------
template <class T>
void Array<T>::reserve (size_t size)
{
	clear();
	_size = size;
	_data = new T[_size +1];
}

//-----------------------------------------------------------------------------
template <class T>
void Array<T>::clear ()
{
	if(_data != NULL) { delete [] _data; _data = NULL; }
	_size = 0;
}

#endif