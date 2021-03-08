#ifndef MYTINYSTL_LIST_H_
#define MYTINYSTL_LIST_H_

// 这个头文件包含了一个模板类 list
// list : 双向链表

#include "iterator.h"
#include "memory.h"

namespace mystl {

//list 节点定义
template<class T>
struct __list_node {
	T data;
	__list_node<T*> prev;
	__list_node<T*> next;
	__list_node(T value=0,__list_node<T>* p=nullptr,__list_node<T>* n=nullptr)
		:data(value)
		,prev(p)
		,next(n)
	{}
};

//list 的迭代器
template<class T, class Ref,class Ptr>
struct __list_iterator : public iterator<bidirectional_iterator_tag, T>
{
	typedef __list_iterator<T, T&, T*> iterator;
	typedef __list_iterator<T, const T&, const T*> const_iterator;
	typedef __list_iterator<T, Ref, Ptr> self;

	typedef T		value_type;
	typedef Ptr		pointer;
	typedef Ref		reference;
	typedef size_t	size_type;
	typedef ptrdiff_t difference_type;

	typedef __lsit_node<T>*    link_type;
	link_type node_;

	//构造函数
	__list_iterator() {}
	__list_iterator(link_type x):node_(x){}
	__list_iterator(const iterator& rhs):node_(rhs.node_){}

	//操作符重载
	reference operator*() const { return (*node_).data; }
	pointer operator->()  const { return &(operator*()); }  // 这里为什么这样写？

	self& operator++ ()
	{
		node_ = node_->next;
		return *this;
	}

	self operator ++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self operator--()
	{
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	bool operator== (const self& rhs) const { return node_ == rhs.node_; }
	bool operator!= (const self&rhs)  const { return node_ != rhs.node_; }
};

//模板类list
template<class T,class Alloc =mystl::alloc>
class list
{
public:
	//list 嵌套型别定义
	typede T		value_type;
	typedef Alloc   allocator_type;
	typedef value_type*  pointer;
	typedef const value_type*  const_pointer;
	typedef value_type&    reference;
	typedef const value_type&   const_reference;
	typedef size_t   size_type;
	typedef ptrdiff_t   difference_type;

	typedef __list_iterator<T, T&, T*>  iterator;
	typedef __list_iterator<T, const T&, const T*> const_iterator;
	typedef mystl::reverse_iterator<iterator>  reverse_iterator;
	typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef __list_node<T*>   link_type;

	typedef mystl::allocator<__list_node<T>, Alloc>  data_allocate;
	allocator_type  get_allocator() { return allocator_type(); }

private:
	link_type node_;   //尾端空节点 ，代表整个list

public:
	//构造相关

	list() { __list_initialize(); }



	explicit list(size_type n);
	list(size_type n, const T& value);

	template<class InputIterator>
	list(InputIterator first, InputIterator last);

	list(const list& rhs);
	list(list && rhs);

	list& operator=(const list& rhs);
	list& operator=(list&& rhs);
	~list();
public:
	//迭代器相关

	iterator    begin() { return node_->next; }
	const_iterator begin() const { return node_->next; }
	iterator   end() { return node_; }
	const_iterator end() const { return node_; }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	//容相关

	bool empty() const { return node_->next == node_; }
	size_type size() const { return static_cast<size_type> (distance(begin(), end())); }
	size_type max_size() const { return static_cast<size_type>(-1); }
	//访问元素相关

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }

	reference back() { return *(--end()); }
	const_reference back() const { return *(--end()); }

	// 调整容器相关
	void assign(size_type n) { __fill_assign(n, value_type()); }
	void assign(size_type n, const value_type& value) { __fill_assign(n, value); }

	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);
	iterator insert(iterator position) { return insert(position, value_type()); }

	template<class T, class Alloc /*=mystl::alloc*/>
	iterator mystl::list<T, Alloc>::insert(iterator position, value_type& value)
	{

	}

	iterator insert(iterator position, value_type& value);
	
	template<class InputIterator>
	void insert(iterator position,InputIterator first, InputIterator last);

	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);

	void clear();
	void push_front(const value_type & value) { insert(begin(), value); }
	void push_back(const value_type & value) { insert(end(), value); } //why not end--

	void pop_front() { erase(begin()); }
	void pop_back() { iterator tmp = end(); erase(--tmp); }

	void resize(size_type new_size) { resize(new_size, value_type()); }

	template<class T, class Alloc /*=mystl::alloc*/>
	void mystl::list<T, Alloc>::resize(size_type new_size, const value_type & value)
	{

	}

	void resize(size_type new_size, const value_type & value);

	void swap(list& rhs);

	//list 相关操作
	void splice(iterator position, list& x);
	void splice(iterator position, list&, iterator i);
	void splice(iterator position, list& x, iterator first, iterator last);
	void remove(const value_type& value);
	template<class Predicate>
	void remove_if(Predicate pred);
	void unique();
	void merge(list& x);
	void sort();
	void reverse();
private:
	link_type __get_node();
	void __put_node(link_type p);
	link_type __create_node(const value_type & value);
	void __destroy_node(link_type p);
	void __list_initialize();
	void __fill_assign(size_type n, const value_type & value);

	template<class Integer>
	void __assign_dispatch(Integer n, Integer value, __true_type);

	template<class InputIterator>
	void __assign_dispatch(InputIterator first, InputIterator last , __false_type);
	void __fill_insert(iterator position, size_type n, cosnt value_type& value);

	template <class Integer>
	void      __insert_dispatch(iterator position, Integer n, Integer value, __true_type);
	template <class InputIterator>
	void      __insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type);

	void __transfer(iterator position, iterator first, iterator last);
};

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__transfer(iterator position, iterator first, iterator last)
{

}

template <class InputIterator>
void mystl::list::__insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type)
{

}

template <class Integer>
void mystl::list::__insert_dispatch(iterator position, Integer n, Integer value, __true_type)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__fill_insert(iterator position, size_type n, cosnt value_type& value)
{

}

template<class InputIterator>
void mystl::list::__assign_dispatch(InputIterator first, InputIterator last, __false_type)
{

}

template<class Integer>
void mystl::list::__assign_dispatch(Integer n, Integer value, __true_type)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__fill_assign(size_type n, const value_type & value)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__list_initialize()
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__destroy_node(link_type p)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
link_type mystl::list<T, Alloc>::__create_node(const value_type & value)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::__put_node(link_type p)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
link_type mystl::list<T, Alloc>::__get_node()
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::merge(list& x)
{

}

template<class Predicate>
void mystl::list::remove_if(Predicate pred)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::remove(const value_type& value)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::splice(iterator position, list& x, iterator first, iterator last)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::splice(iterator position, list&, iterator i)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::splice(iterator position, list& x)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
void mystl::list<T, Alloc>::swap(list& rhs)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
iterator mystl::list<T, Alloc>::erase(iterator first, iterator last)
{

}

template<class T, class Alloc /*=mystl::alloc*/>
iterator mystl::list<T, Alloc>::erase(iterator position)
{

}



template<class T, class Alloc>
template<class InputIterator>
inline void list<T, Alloc>::assign(InputIterator first, InputIterator last)
{
}

template<class T, class Alloc>
template<class InputIterator>
inline void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
{
}

template<class T, class Alloc /*=mystl::alloc*/>
mystl::list<T, Alloc>::list(list && rhs)
{

}

template<class T, class Alloc>
inline list & list<T, Alloc>::operator=(const list & rhs)
{
	// TODO: 在此处插入 return 语句
}

template<class T, class Alloc>
inline list & list<T, Alloc>::operator=(list && rhs)
{
	// TODO: 在此处插入 return 语句
}

template<class T, class Alloc /*=mystl::alloc*/>
mystl::list<T, Alloc>::list(const list& rhs)
{
	__list_initialize();
	insert(begin(), rhs.begin(), rhs.end());
}

template<class T, class Alloc /*=mystl::alloc*/>
mystl::list<T, Alloc>::list(size_type n, const T& value)
{

}

template<class T, class Alloc>
template<class InputIterator>
inline list<T, Alloc>::list(InputIterator first, InputIterator last)
{
}
} // namespace mystl
#endif // !MYTINYSTL_LIST_H_

