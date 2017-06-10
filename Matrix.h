
#ifndef MATRIX_H
#define MATRIX_H

#include "Tools.h"

//-----------------------------------------------------------------------------
template <class T>
class Matrix
{
private:
	T ** _data;
	size_t _sizeX;
	size_t _sizeY;

	void clear ();

public:
	Matrix () { _data = NULL; _sizeX = 0; _sizeY = 0; }
	virtual ~Matrix() { clear(); }
	size_t size(size_t i) { return i==0 ? _sizeX : _sizeY; }

	Matrix (size_t, size_t);
	void reserve (size_t, size_t);
	Matrix (size_t, size_t, int);
	void reserve (size_t, size_t, int);

	Matrix (const Matrix &);
	Matrix & operator= (const Matrix &);

	T get (size_t i, size_t j) const { return _data[i][j]; }
	void set (T x, size_t i, size_t j) { _data[i][j] = x; }

	void print () const;
};

//-----------------------------------------------------------------------------
template <class T>
void Matrix<T>::print () const
{
	for(size_t i=0; i<_sizeX; i++) 
	{
		for(size_t j=0; j<_sizeY; j++) { cout << _data[i][j] << "\n"; }
		cout << "\n";
	}
	cout << "\n";
}

//-----------------------------------------------------------------------------
template <class T>
Matrix<T>::Matrix (const Matrix & mat)
{
	_sizeX = mat.size(0);
	_sizeY = mat.size(1);
	
	_data = new T*[_sizeX +1];
	for(size_t i=0; i<_sizeX; i++)
	{ 
		_data[i] = new T[_sizeY +1];
		for(size_t j=0; j<_sizeY; j++) { _data[i][j] = mat.get(i,j); }
	}
}

//-----------------------------------------------------------------------------
template <class T>
Matrix<T> & Matrix<T>::operator= (const Matrix & mat)
{
	clear();
	if(mat.size(0) == 0 ) { return *this; }

	_sizeX = mat.size(0);
	_sizeY = mat.size(1);
	
	_data = new T*[_sizeX +1];
	for(size_t i=0; i<_sizeX; i++)
	{ 
		_data[i] = new T[_sizeY +1];
		for(size_t j=0; j<_sizeY; j++) { _data[i][j] = mat.get(i,j); }
	}
	
	return *this;
}

//-----------------------------------------------------------------------------
template <class T>
Matrix<T>::Matrix (size_t x, size_t y, int val)
{
	_sizeX = x;
	_sizeY = y;
	
	_data = new T*[x +1];
	for(size_t i=0; i<x; i++)
	{ 
		_data[i] = new T[y +1];
		for(size_t j=0; j<y; j++) { _data[i][j] = val; }
	}
}

//-----------------------------------------------------------------------------
template <class T>
void Matrix<T>::reserve (size_t x, size_t y, int val)
{
	clear();
	_sizeX = x;
	_sizeY = y;
	
	_data = new T*[x +1];
	for(size_t i=0; i<x; i++) 
	{ 
		_data[i] = new T[y +1];
		for(size_t j=0; j<y; j++) { _data[i][j] = val; }
	}
}

//-----------------------------------------------------------------------------
template <class T>
Matrix<T>::Matrix (size_t x, size_t y)
{
	_sizeX = x;
	_sizeY = y;
	
	_data = new T*[x +1];
	for(size_t i=0; i<x; i++) { _data[i] = new T[y +1]; }
}

//-----------------------------------------------------------------------------
template <class T>
void Matrix<T>::reserve (size_t x, size_t y)
{
	clear();
	_sizeX = x;
	_sizeY = y;
	
	_data = new T*[x +1];
	for(size_t i=0; i<x; i++) { _data[i] = new T[y +1]; }
}

//-----------------------------------------------------------------------------
template <class T>
void Matrix<T>::clear ()
{
	for(size_t i=0; i<_sizeX; i++) 
	{ delete [] _data[i]; _data[i] = NULL; }
	delete [] _data; _data = NULL;

	_sizeX = 0;
	_sizeY = 0;
}

#endif