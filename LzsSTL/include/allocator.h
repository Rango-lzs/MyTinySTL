#ifndef LZSSTL_ALLOCATOR_H_
#define LZSSTL_ALLOCATOR_H_

namespace lzs
{

template<class T>
class allocator
{

// type define
public:
	typedef T            value_type;
	typedef T*           pointer;
	typedef const T*     const_pointer;
	typedef T&           reference;
	typedef const T&     const_reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;

public:
	static T* allocate();
	static T* allocate(size_type n);

	static void deallocate(T* ptr);
	static void deallocate(T* ptr, size_type n);

};


template<class T>
T* allocator<T>::allocate()
{
	return static_cast<T*>(operator new(sizeof(T)));
}

template<class T>
T* allocator<T>::allocate(size_type n)
{
	if (n == 0) return nullptr;
	return static_cast<T*>(operator new(sizeof(T)*n));
}

template<class T>
void  allocator<T>::deallocate(T* ptr)
{
	if (ptr == nullptr) return;
	::operator delete(ptr);
}

template<class T>
void allocator<T>::deallocate(T* ptr,size_type n)
{
	if (ptr == nullptr) return;
	::operator delete[](ptr);
}



}// namespace lzs

#endif