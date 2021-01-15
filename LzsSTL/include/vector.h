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
	static_assert(!is_same(bool, T), "vector<bool> is not supported!");
};
#endif