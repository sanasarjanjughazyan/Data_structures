#include <iostream>

template <typename T>
class U_ptr
{
	T* ptr;
public:
	U_ptr() : ptr(nullptr) {}
	explicit U_ptr(T* p) : ptr(p) {}
	U_ptr(U_ptr&& u) 
	{
		ptr = std::move(u.ptr);
		u.ptr = nullptr;
	}
	~U_ptr() { delete ptr; ptr = nullptr; }
	U_ptr& operator= (U_ptr&& u) 
	{ 
		if (this == &u)
			return *this;
		delete ptr;
		ptr = std::move(u.ptr);
		u.ptr = nullptr;
		return *this; 
	}
	U_ptr(const U_ptr&) = delete;
	U_ptr& operator= (const U_ptr&) = delete;
	T* operator-> () const { return ptr; }
	T& operator* () const { return *ptr; }
};

template <typename T>
U_ptr<T> make_unique(const T& data)
{
	return U_ptr<T>(new T(data));
}

struct A
{
	double d = 10;
	int i = 9;
};

int main()
{
	U_ptr<int> q(new int(9));
	U_ptr<int> p;
	p = std::move(q);
	U_ptr<int> k = make_unique<int>(5);
	std::cout << *(k) << " " << *(p) <<  std::endl;
	//std::cout << p->d << " " << p->i << std::endl;
}