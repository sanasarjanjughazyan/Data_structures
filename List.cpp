#include <iostream>
#include <cassert>
#include <initializer_list>

template <typename T>
class List
{
	template <typename T>
	struct Node
	{
		T data;
		Node* next;
		Node(const T& data) : data(data), next(nullptr) {}
	};

	mutable size_t size;
	Node<T>* head;
public:
	template <typename T>
	class iterator
	{
		Node<T>* ptr;
	public:
		iterator(Node<T>* head) : ptr(head) {}
		T& operator* () const { assert(ptr != nullptr); return ptr->data; }
		T& operator-> () const { assert(ptr != nullptr); return ptr->data; }
		iterator& operator++ () { assert(ptr != nullptr); ptr = ptr->next; return *this; }
		iterator operator++ (int) { assert(ptr != nullptr); iterator temp = *this; ptr = ptr->next; return temp; }
		bool operator!= (const iterator& it) const { return ptr != it.ptr; }
		bool operator== (const iterator& it) const { return ptr == it.ptr; }
		void swap(iterator& it) { std::swap(ptr, it->ptr); }
	};

	List();
	List(const size_t sz, const T& elem = T());
	List(const List<T>& l);
	List(const std::initializer_list<T>& l);
	List<T>& operator= (const List<T>& l);
	~List();
	iterator<T> begin();
	iterator<T> end();
	size_t Size() const { return size; }
	void pop_front();
	iterator<T> erase(size_t ind) const;
	void push_front(const T& elem);
	iterator<T> insert(size_t ind, const T& elem) const;
	void clear();
	bool empty() const;
	bool operator== (const List<T>& l) const;
};

//..............CONSTRUCTORS......................................................
template<typename T>
List<T>::List() : size(0), head(nullptr) {}

template<typename T>
List<T>::List(const size_t sz, const T& elem) : size(sz)
{
	head = new Node<T>(elem);
	Node<T>* start = head;
	for (size_t i = 1; i < size; ++i)
	{
		start->next = new Node<T>(elem);
		start = start->next;
	}
}

template<typename T>
List<T>::List(const List<T>& l)
{
	size = l.size;
	head = new Node<T>(l.head->data);
	Node<T>* to = head;
	Node<T>* from = l.head;
	for (size_t i = 1; i < size; ++i)
	{
		to->next = new Node<T>(from->next->data);
		to = to->next;
		from = from->next;
	}
}

template<typename T>
List<T>::List(const std::initializer_list<T>& l) : size(l.size())
{
	auto it = l.begin();
	head = new Node<T>(*it);
	Node<T>* start = head;
	++it;
	for (; it != l.end(); ++it)
	{
		start->next = new Node<T>(*it);
		start = start->next;
	}
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& l)
{
	if (this == &l)
		return *this;
	clear();
	size = l.size;
	head = new Node<T>(l.head->data);
	Node<T>* to = head;
	Node<T>* from = l.head;
	for (size_t i = 1; i < size; ++i)
	{
		to->next = new Node<T>(from->next->data);
		to = to->next;
		from = from->next;
	}
	return *this;
}

template<typename T>
List<T>::~List()
{
	clear();
}

//.......................functions..............................................

template<typename T>
List<T>::iterator<T> List<T>::begin()
{
	iterator<T> it(head);
	return it;
}

template<typename T>
List<T>::iterator<T> List<T>::end()
{
	Node<T>* end = head;
	while (end != nullptr)
		end = end->next;
	iterator<T> it(end);
	return it;
}

template<typename T>
void List<T>::pop_front()
{
	assert(size > 0);
	Node<T>* temp = head;
	head = head->next;
	delete temp;
	--size;
}

template<typename T>
List<T>::iterator<T> List<T>::erase(size_t ind) const
{
	assert(ind < size);
	Node<T>* start = head;
	--ind; // for deleteing exactly ind elem (start->next)
	while (ind--)
		start = start->next;
	Node<T>* temp = start->next;
	start->next = temp->next;
	delete temp;
	--size;
	iterator<T> it(start->next); // after deleteing elem
	return it;
}

template<typename T>
void List<T>::push_front(const T & elem)
{
	Node<T>* temp = new Node<T>(elem);
	temp->next = head;
	head = temp;
	++size;
}

template<typename T>
List<T>::iterator<T> List<T>::insert(size_t ind, const T& elem) const
{
	assert(ind < size);
	Node<T>* start = head;
	while (ind--)
		start = start->next;
	Node<T>* temp = new Node<T>(elem);
	temp->next = start->next;
	start->next = temp;
	++size;
	iterator<T> it(temp);
	return it;
}

template<typename T>
void List<T>::clear()
{
	while (size)
		pop_front();
}

template<typename T>
bool List<T>::empty() const
{
	return size == 0;
}

template<typename T>
bool List<T>::operator==(const List<T>& l) const
{
	if (size != l.size)
		return false;
	Node<T>* start = head;
	Node<T>* l_start = l.head;
	for (size_t i = 0; i < size; ++i)
	{
		if (start->data != l_start->data)
			return false;
		start = start->next;
		l_start = l_start->next;
	}
	return true;
}


//....................main........................................................
int main()
{
	List<int> k = {4, 6, 3, 22, 5, 6};
	List<int> l(k);
	std::cout << (l == k) << std::endl;
	for (auto it = l.begin(); it != l.end(); ++it)
		std::cout << *(it) << " ";
	std::cout << std::endl;
	system("pause");
}
