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
	static_assert(!std::is_same<bool, T>::value, "vector<bool> is not supported!");
public:
	//vector 的嵌套型别定义
	typedef lzs::allocator<T> allocator_type;
	typedef lzs::allocator<T> data_allocator;

	typedef  typename allocator_type::value_type  value_type;

};
#endif