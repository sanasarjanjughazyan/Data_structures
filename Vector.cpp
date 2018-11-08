#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <exception>

template<typename T>
class vector;

namespace implementation_details
{
	template<typename T>
	class iterator
	{
		size_t index;
		vector<T>* myvec = nullptr;
	public:
		iterator() {}
		iterator(vector<T>& v, size_t index) : myvec(&v), index(index) {}
		iterator& operator++ () { ++index; return *this; }  
		iterator operator++ (int) { iterator temp = *this; ++*this; return temp; } 
		iterator& operator-- () { --index; return *this; }
		iterator operator-- (int) { iterator temp = *this; --index; return temp; }
		iterator operator+ (const int n) const { iterator temp = *this;  temp.index += n; return temp; }  
		iterator operator- (const int n) const { iterator temp = *this;  temp.index -= n; return temp; }
		int operator- (const iterator& i) const { return index - i.index; } 
		iterator& operator+= (const int n) { return *this = *this + n; }   
		iterator& operator-= (const int n) { return *this = *this - n; }
		bool operator!= (const iterator& i) const { return index != i.index; }
		bool operator== (const iterator& i) const { return index == i.index; }
		T& operator* () { return (*myvec)[index]; }
		T* operator-> () { return &myvec[index]; } 
	};
}

//class Vector.......................................................................................

template<typename T>
class vector 
{
	size_t Size;
	size_t maxSize = 1 + Size * 2;
	T* arr;
	
	void swap(vector<T>& v)
	{
		std::swap(maxSize, v.maxSize);
		std::swap(Size, v.Size);
		std::swap(arr, v.arr);
	}

	vector(vector<T>& v, size_t new_cap) : Size(v.Size), maxSize(new_cap), arr(new T[new_cap])
	{
		for (size_t i = 0; i < Size; ++i)
			arr[i] = v[i];
	}
public:
	typedef implementation_details::iterator<T> iterator;
	vector(size_t sz = 0, const T& val = T());
	vector(std::initializer_list<T> l);
	vector(vector<T>& v);
	T& operator[] (const size_t i);
	const T& operator[] (const size_t i) const;
	T& operator= (const vector<T>& v);
	bool operator> (const vector<T>& v) const;
	bool operator< (const vector<T>& v) const;
	bool operator== (const vector<T>& v) const;
	bool operator!= (const vector<T>& v) const;
	T& at(const size_t i);
	T& at(const size_t i) const;
	void resize(const size_t sz, const T& val = T());
	void reserve(const size_t val);
	size_t size() const;
	size_t capasity() const;
	void pop_back();
	iterator erase(const size_t i);
	void push_back(const T& val);
	iterator insert(const size_t i, const T& val);
	void clear();
	bool Isempty() const { return Size == 0; }
	iterator begin();
	iterator end();
	~vector() { delete[] arr; }
};

//main..............................................................................................

int main()
{
	vector<int> v = {4, 7, 6, 5, 3};
	auto it = v.begin();
	std::cout << *it << std::endl;
	for (int i : v)
		std::cout << i << " ";
	
}

// Constructors.....................................................................................

template<typename T>
vector<T>::vector(size_t sz, const T& val) : Size(sz), arr(new T[maxSize])
{
	for (size_t i = 0; i < Size; ++i)
		arr[i] = val;
}

template<typename T>
vector<T>::vector(std::initializer_list<T> l) : Size(l.size()), arr(new T[Size])
{
	std::copy(l.begin(), l.end(), arr);
}

template<typename T>
vector<T>::vector(vector<T>& v) : Size(v.Size), arr(new T[maxSize])
{
	for (size_t i = 0; i < Size; ++i)
		arr[i] = v[i];
}


// Operators.........................................................................................

template<typename T>
T & vector<T>::operator[](const size_t i)  
{
	return arr[i];
}

template<typename T>
const T & vector<T>::operator[](const size_t i) const
{
	return arr[i];
}



template<typename T>
T & vector<T>::operator=(const vector<T>& v)     
{
	if (this == &v)
		return *this;
	vector<T>(v).swap(*this);    
	return *this;
}

template<typename T>
bool vector<T>::operator>(const vector<T>& v) const   
{
	for (size_t i = 0; i < Size; ++i)
		if (arr[i] < v.arr[i])
			return false;
	if (Size < v.Size)
		return false;
	return true;
}

template<typename T>
bool vector<T>::operator<(const vector<T>& v) const
{
	return v > *this;
}

template<typename T>
bool vector<T>::operator==(const vector<T>& v) const  
{
	if (Size != v.Size)
		return false;
	for (size_t i = 0; i < Size; ++i)
		if (arr[i] != v.arr[i])
			return false;
	return true;
}

template<typename T>
bool vector<T>::operator!=(const vector<T>& v) const
{
	return !(v == *this);
}

//Metods.............................................................................................

template<typename T>
T & vector<T>::at(const size_t i)   
{
		if (i >= Size)
			throw std::out_of_range("Out Of Range!!");
		return arr[i];
}

template<typename T>
T & vector<T>::at(const size_t i) const
{
	if (i >= Size)
		throw std::out_of_range("Out Of Range!!");
	return arr[i];
}

template<typename T>
void vector<T>::resize(const size_t sz, const T& val)
{
	if (Size >= sz)
		while (Size != sz)
			pop_back();
	else
	{
		vector<T> temp(sz, val);
		for (size_t i = 0; i < Size; ++i)
			temp.arr[i] = arr[i];
		swap(temp);
	}
}

template<typename T>
void vector<T>::reserve(const size_t val)
{
	vector temp(*this, val);
	swap(temp);
}

template<typename T>
size_t vector<T>::size() const
{
	return Size;                             
}

template<typename T>
size_t vector<T>::capasity() const
{
	return maxSize;
}

template<typename T>
void vector<T>::pop_back()
{
		--Size;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(const size_t i)
{
	try {
		if (i >= Size)
			throw std::out_of_range ("Out Of Range!!  Wrong index");
		if (i == Size - 1)
		{
			pop_back();
			iterator it(*this, Size);
			return it;
		}
		else
		{
			for (size_t j = i; j < Size; ++j)
				arr[j] = arr[j + 1];
			--Size;
		}
		iterator it(*this, i);
		return it;
	}
	catch(std::out_of_range& err)
	{
		std::cout << err.what() << std::endl;
	}
}

template<typename T>
void vector<T>::push_back(const T& val)
{
	if (Size == maxSize)
	{
		maxSize *= 2;
		T* newarr = new T[maxSize];
		for (size_t i = 0; i < Size; ++i)
			newarr[i] = arr[i];
		newarr[Size++] = val;
		delete[] arr;
		arr = newarr;
	}
	else
	{
		++Size;
		arr[Size - 1] = val;
	}
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(const size_t i, const T & val)   
{
	if (i > Size)
		throw std::out_of_range("Out Of Range!! Wrong index");
	if (i == Size)
	{
		push_back(val);
		iterator t(*this, Size - 1);
		return t;
	}
	else
	{
		++Size;
		for (size_t j = Size - 1; j >= i; --j)
		{
			if (j == i)
				arr[j] = val;
			else
				arr[j] = arr[j - 1];
		}
	}
	iterator it(*this, i);
	return it;
}

template<typename T>
void vector<T>::clear()
{
	Size = 0;
}

template<typename T>
typename vector<T>::iterator vector<T>::begin()
{
	iterator i(*this, 0);
	return i;
}

template<typename T>
typename vector<T>::iterator vector<T>::end()
{
	iterator i(*this, Size);
	return i;
}



