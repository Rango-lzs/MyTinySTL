#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

// 这个头文件用于基本的迭代器设计，包含了一些模板结构体与全局函数，

#include <cstddef>

namespace mystl {

struct input_iterator_tag{};  //只读迭代器
struct output_iterator_tag{}; //只写迭代
struct forward_iterator_tag: public input_iterator_tag{}; //前向迭代器
struct bidirectional_iterator_tag: public forward_iterator_tag{}; // 双向迭代器
struct random_access_iterator_tag: public bidirectional_iterator_tag {}; //随机迭代器


//iterator 泛型模板
template <class Category, class T,class Distance=ptrdiff_t,
         class Pointer=T*,class Reference=T&>
struct iterator
{
	typedef Category    iterator_category;
	typedef T           value_type;
	typedef Distance    difference_type;
	typedef Pointer     pointer;
	typedef Reference   reference;
};

//迭代器特性萃取

template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type        value_type;
	typedef typename Iterator::difference_type   difference_type;
	typedef typename Iterator::pointer           pointer;
	typedef typename Iterator::reference        reference;
 };

template <class T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag   iterator_category;
	typedef T                     value_type;
	typedef T*                    pointer;
	typedef T&                    reference;
	typedef ptrdiff_t             difference_type;
};

template <class T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag   iterator_category;
	typedef T                     value_type;
	typedef const T*                    pointer;
	typedef const T&                    reference;
	typedef ptrdiff_t             difference_type;
};


// 萃取迭代器category的函数
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
	typedef typename iterator_traits<Iterator>::iterator_category Category;
	return iterator_traits<Iterator>::iterator_category();
	//return Category();
}

//萃取迭代器的distance_type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
	return static_cast<iterator_traits<Iterator>::difference_type*>(0);
}

//萃取迭代器的value_type

template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
	return static_cast<iterator_traits<Iterator>::value_type*>(0);
}

//计算两个迭代器的距离
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return _distance(first, last, iterator_category(first));
}

// 根据迭代器类型进行distance函数重载
//input_iterator_tag 版本

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
_distance(InputIterator first, InputIterator last, input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first!=last)
	{
		++first;
		++n;
	}
	return n;
}

template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
_distance(RandomAccessIterator first,RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}


//advance 重载函数

template<class InputIterator ,class Distance>
inline void advance(InputIterator &i, Distance n)
{
	_advance(i, n, iterator_category(i));
}

//input_iterator_tag 版
template <class InputIterator, class Distance>
inline void _advance(InputIterator &i, Distance n, input_iterator_tag)
{
	while (n--) ++i;
}

//bidrectional_iterator_tag 版
template<class BidirectionalIterator,class Distance>
inline void _advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}

//random_access_iterator_tag版
template<class RandomAccessIterator, class Distance>
inline void _advace(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
{
	i += n;
}


} // namespace mystl

#include "reverse_iterator.h"    // 包含 reverse_iterator

#endif // !MYTINYSTL_ITERATOR_H_

