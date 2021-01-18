#ifndef LZSSTL_VECTOR_H_
#define LZSSTL_VECTOR_H_

#include <initializer_list>

#include "iterator.h"
//#include "memory.h"
//#include "util.h"
//#include "exceptdef.h"

template<class T>
class vector
{
	//static_assert(!std::is_same<bool, T>::value, "vector<bool> is not supported!");
public:
	//vector 的嵌套型别定义
	typedef lzs::allocator<T> allocator_type;
	typedef lzs::allocator<T> data_allocator;

	typedef typename allocator_type::value_type       value_type;
	typedef typename allocator_type::pointer	      pointer;
	typedef typename allocator_type::const_pointer    const_pointer;
	typedef typename allocator_type::reference	      reference;
	typedef typename allocator_type::const_reference  const_reference;
	typedef typename allocator_type::size_type	      size_type;
	typedef typename allocator_type::difference_type  difference_type;

	typedef value_type*                                iterator;
	typedef const value_type*                          const_iterator;

	allocator_type get_allocator() { return data_allocator(); }

private:
	iterator begin_;
	iterator end_;
	iterator cap_;

};
#endif