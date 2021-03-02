#ifndef MYTINYSTL_ALLOCATOR_H_
#define MYTINYSTL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于对象内存的分配与释放

#include "alloc.h"    

namespace mystl {

// 分配器类提供内存分配和释放接口 
// 需要实现 allocate() 和 deallocate()
// 无需实例化  allocate()方法为static 方法 

// allocator 是否需要定义内嵌型别类型？ 如需要 ，应用场景是什么？

template <class T, class Alloc>
class allocator
{
public:
	typedef T            value_type;
	typedef T*           pointer;
	typedef const T*     const_pointer;
	typedef T&           reference;
	typedef const T&     const_reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
public:

	static T* allocate()
	{
		return static_cast<T*>(Alloc::allocate(sizeof(T)));
	}

	static T* allocate(size_type n)
	{
		if (n == 0) return nullptr;
		return static_cast<T*>(Alloc::allocate(n*sizeof(T)));
	}

	static void deallocate(T* ptr)
	{
		if (nullptr == ptr) return;
		Alloc::deallocate(ptr, sizeof(T));
	}

	static void deallocate(T* ptr,size_type n)
	{
		if (nullptr == ptr) return;
		Alloc::deallocate(ptr, n*sizeof(T));
	}

};

} // namespace mystl
#endif // !MYTINYSTL_ALLOCATOR_H_

