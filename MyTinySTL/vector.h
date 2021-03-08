#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

#include "iterator.h"
#include "memory.h"

namespace mystl {

// 模板类: vector
// 接受两个参数，参数一代表数据类型，参数二代表空间配置器类型，缺省使用 mystl 的 alloc
template<class T,class Alloc =mystl::alloc>
class vector
{
public:
	//vector 嵌套型别定义
	typedef T		value_type;
	typedef Alloc	allocator_type;
	typedef value_type*		pointer;
	typedef const value_type* const_pointer;
	typedef value_type&		reference;
	typedef const value_type&		const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t		difference_type;

	typedef value_type*		iterator; //vector 可以直接使用原生指针来当迭代器
	typedef const value_type* const_iterator;
	typedef mystl::reverse_iterator<iterator>   reverse_iterator;
	typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

	typedef mystl::allocator<T, Alloc>  data_allocator;
	allocator_type get_allocator() { return allocator_type(); }

private:

	iterator start_;
	iterator finish_;
	iterator end_of_storage_;
public:
	vector():start_(nullptr),finish_(nullptr),end_of_storage_(nullptr){}
	explicit vector(size_type n) { __fill_initialize(n, value_type()); }
	vector(size_type n, const value_type &value) { __fill_initialize(n, value); }

	template<class InputIterator>
	vector(InputIterator first, InputIterator last);

	vector(const vector &rhs);
	vector(vector &&rhs);

	vector& operator =(const vector &rhs);
	vector& operator=(vector&& rhs);

	~vector() { __destroy_and_deallocate(); }
//迭代器相关
public:
	iterator	begin() { return start_; }
	const_iterator begin() const { return start_; }
	iterator	end() { return finish_; }
	const_iterator end() const { return finish_; }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
// 容量相关
	bool empty() const { return begin() == end(); }
	size_type size() const { return static_cast<size_type>(end() - begin()); }
	size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }
	size_type capacity() const { return static_cast<size_type>(end_of_storage_ - start_); }
	void shrink_to_fit();
	void reserve(size_type n);
//访问元素
	reference operator [](difference_type n) { return *(begin() + n); }
	const_reference operator [](difference_type n) const { return *(begin() + n); }
	reference at(difference_type n) { return (*this)[n]; }
	const_reference at(difference_type n) const { return (*this)[n]; }
	reference front() { return *begin(); }
	const_reference front() const { return *(begin()); }
	
	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }
	pointer data() { return begin(); }
	const_pointer data() const { return begin(); }

//修改容器操作
	void assign(size_type n) { __fill_assign(n, value_type()); }
	void assign(size_type n, const value_type& value) { __fill_assign(n, value); }
	template<class InputIterator>
	void assign(InputIterator first, InputIterator  last);
	void push_back(const value_type& value);
	void pop_back();

	iterator insert(iterator position);
	iterator insert(iterator position, const value_type& value);
	void insert(iterator position, size_type n, const value_type &value);
	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);
	void clear() { erase(begin(), end()); }

	void resize(size_type new_size) { resize(new_size, 0); }//
	void resize(size_type new_size, const value_type& value);
	//void reverse() { mystl::reverse(begin(), end()); }
	//void   reverse() { mystl::reverse(begin(), end()); }
	void swap(vector& rhs);
private:
	//私有成员
	template<class Integer>
	void __vector_initialize(Integer n, Integer value, __true_type);

	template<class InputIterator>
	void __vector_initialize(InputIterator first, InputIterator last, __false_type);

	void __destroy_and_deallocate();
	void __fill_initialize(size_type n, const value_type &value);

	template<class InputIterator>
	void __range_initialize(InputIterator first, InputIterator last);

	template<class Integer>
	void __assign_dispatch(Integer n, Integer value, __true_type);

 	template<class InputIterator>
	void __assign_dispatch(InputIterator n, InputIterator last, __false_type);

	void __fill_assign(size_type n, const value_type& value);

	template <class InputIterator>
	void __range_assign(InputIterator first, InputIterator last, input_iterator_tag);
	
	template <class ForwardIterator>
	void __range_assign(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
	
	void __insert_aux(iterator position, const value_type& value);
	
	template <class Integer>
	void __insert_dispatch(iterator position, Integer n, Integer x, __true_type);
	
	template <class InputIterator>
	void __insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type);
	
	void __fill_insert(iterator position, size_type n, const value_type& value);
	
	template <class InputIterator>
	void __range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);
	
	template <class ForwardIterator>
	void __range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

};

/****************************Method Implementation************************************************/
//constructor
template<class T,class Alloc>
template<class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
	typedef typename __is_integer<InputIterator>::is_integer Integer;
	__vector_initialize(first, last, Integer());
}

//copoy constructor
template<class T,class Alloc>
vector<T, Alloc>::vector(const vector& rhs)
{
	__range_initialize(rhs.start_, rhs.finish_);
}

//move constructor
template<class T,class Alloc>
vector<T, Alloc>::vector(vector&&rhs)
	:start_(rhs.start_)
	, finish_(rhs.finish_)
	, end_of_storage_(rhs.end_of_storage_)
{
	rhs.start_ = nullptr;
	rhs.finish_ = nullptr;
	rhs.end_of_storage_ = nullptr;
}

// copy assign operator
template<class T,class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator =(const vector& rhs)
{

}

// move assign operator
// 移动赋值操作符
template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& rhs) {
	if (this != &rhs) {
		__destroy_and_deallocate();  // 释放原资源
		start_ = rhs.start_;
		finish_ = rhs.finish_;
		end_of_storage_ = rhs.end_of_storage_;
		rhs.start_ = nullptr;
		rhs.finish_ = nullptr;
		rhs.end_of_storage_ = nullptr;
	}
	return *this;
}

//free the extra memory
template<class T, class Alloc>
void vector< T, Alloc>::shrink_to_fit()
{
	if (finish_ != end_of_storage_)
	{
		data_allocator::deallocate(finish_ + 1, end_of_storage_ - finish_);
		end_of_storage_ = finish_;
	}
}

//reserved capacity ,reallocate when not enough
template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n)
{
	if (capacity() < n)
	{
		const auto old_size = size();
		auto tmp = data_allocator::allocate(n);
		mystl::uninitialized_copy(start_, finish_, tmp);
		__destroy_and_deallocate();
		start_ = tmp;
		finish_ = tmp + old_size;
		end_of_storage_ = tmp + n;
	}
}

//initial assign

template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last)
{
typedef typename __is_integer<InputIterator>::is_integer Integer;
__assign_dispatch(first, last, Integer());
}

//insert element to the end
template<class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type &value)
{
	if (finish_ != end_of_storage_)
	{
		mystl::construct(finish_, value);
		++finish_;
	}
	else
	{
		__insert_aux(end(), value);
	}
}

//pop the end element 
template<class T, class Alloc>
void vector<T, Alloc>::pop_back()
{
	--finish_;
	mystl::destroy(finish_);
}

//insert an element at position 
template<class T, class Alloc>
typename vector<T,Alloc>::iterator 
vector<T, Alloc>::insert(iterator position, const value_type &value)
{
	auto n = position - begin();
	if (finish_ != end_of_storage_ && position == end())
	{
		mystl::construct(finish_, value);
		++finish_;
	}
	else
	{
		__insert_aux(position, value);
	}
	return begin() + n;
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator position)
{
	auto n = position - begin();
	if (finish_ != end_of_storage_ && position == end())
	{
		mystl::construct(finish_);
		++finish_;
	}
	else
	{
		__insert_aux(position,value_type());
	}
	return begin() + n;
}

// insert n elements at position
template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value)
{
	__fill_insert(position, n, value);
}

//insert an range[first,last) at position
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
{
	typedef typename __is_integer<InputIterator>::is_integer Integer;
	__insert_dispatch(position, first, last, Integer());
}

//delete element at position
template<class T,class Alloc>
typename vector<T,Alloc>::iterator
vector<T, Alloc>::erase(iterator position)
{
	if (position + 1 != end())
		mystl::copy(position + 1, finish_, position);
	--finish_;
	mystl::destroy(finish_);
	return position;
}

//删除[first,last)区间的元素
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator first, iterator last)
{
	auto i = mystl::copy(last, finish_, first);
	mystl::destroy(i, finish_);
	finish_ = finish_ - (last - first);
	return first;
}

//rsize vector
template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size, const value_type &value)
{
	if (new_size < size())
		erase(begin() + new_size, end());
	else
		insert(end(), new_size - size(), value);
}

//与另一个vector交换
template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc> &rhs)
{
	if (this != &rhs)
	{
		mystl::swap(start_, rhs.start_);
		mystl::swap(finish_, rhs.finish_);
		mystl::swap(end_of_storage_, rhs.end_of_storage_);
	}
}

//vector initialize函数
template <class T, class Alloc>
template <class Integer>
void vector<T, Alloc>::__vector_initialize(Integer n, Integer value, __true_type)
{
	__fill_initialize(static_cast<size_type>(n), static_cast<T>(value));
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__vector_initialize(InputIterator first, InputIterator last, __false_type)
{
	__range_initialize(first,last);
}

template <class T, class Alloc>
void vector<T, Alloc>::__fill_initialize(size_type n, const value_type &value)
{
	start_ = data_allocator::allocate(n);
	finish_ = mystl::uninitialized_fill_n(start_, n, value);
	end_of_storage_ = finish_;
}

template <class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::__range_initialize(InputIterator first, InputIterator last)
{
	auto n = last - first;
	start_ = data_allocator::allocate(static_cast<size_type>(n));
	finish_ = mystl::uninitialized_copy(first,last,start_);
	end_of_storage_ = finish_;
}

// __destroy_and_deallocate 函数
template <class T, class Alloc>
void vector<T, Alloc>::__destroy_and_deallocate() {
	mystl::destroy(start_, finish_);
	data_allocator::deallocate(start_, end_of_storage_ - start_);
}

// __assign_dispatch 函数
template <class T, class Alloc>
template <class Integer>
void vector<T, Alloc>::__assign_dispatch(Integer n, Integer value, __true_type) {
	__fill_assign(static_cast<size_type>(n), static_cast<T>(value));
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__assign_dispatch(InputIterator first, InputIterator last, __false_type) {
	__range_assign(first, last, iterator_category(first));
}

// __fill_assign 函数
template <class T, class Alloc>
void vector<T, Alloc>::__fill_assign(size_type n, const value_type& value) {
	if (n > capacity()) {
		vector<T, Alloc> tmp(n, value);
		tmp.swap(*this);
	}
	else if (n > size()) {
		mystl::fill(begin(), end(), value);
		finish_ = mystl::uninitialized_fill_n(finish_, n - size(), value);
	}
	else {
		erase(mystl::fill_n(start_, n, value), finish_);
	}
}

// __range_assign 函数
template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__range_assign(InputIterator first, InputIterator last, input_iterator_tag) {
	auto cur = begin();
	for (; first != last && cur != end(); ++first, ++cur) {
		*cur = *first;
	}
	if (first == last)               // 如果[first, last)区间内的元素复制完成
		erase(cur, end());           // 删除多余的元素
	else
		insert(end(), first, last);  // 否则插入区间剩余元素
}

template <class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::__range_assign(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	auto len = distance(first, last);
	if (static_cast<size_type>(len) > capacity()) {               // 如果区间长度大于容器容量
		__destroy_and_deallocate();                               // 销毁原来的容器
		__range_initialize(first, last);                          // 重新分配空间并复制区间
	}
	else if (size() >= static_cast<size_type>(len)) {             // 如果区间长度小于等于容器大小
		auto new_finish = mystl::copy(first, last, start_);       // 复制区间到起始处
		mystl::destroy(new_finish, finish_);                      // 销毁多余的元素
		finish_ = new_finish;
	}
	else {                                                        // 如果区间长度大于容器大小并且小于容器容量
		auto mid = first;
		advance(mid, size());                                     // 以容器大小把区间分为前后段
		mystl::copy(first, mid, start_);                          // 先复制前一段区间
		finish_ = mystl::uninitialized_copy(mid, last, finish_);  // 再复制后一段区间
	}
}

// __insert_aux 函数
template <class T, class Alloc>
void vector<T, Alloc>::__insert_aux(iterator position, const value_type& value) {
	if (finish_ != end_of_storage_) {                              // 如果还有备用空间
		mystl::construct(finish_, *(finish_ - 1));
		++finish_;
		mystl::copy_backward(position, finish_ - 2, finish_ - 1);  // 后移一位
		auto value_type = value;
		*position = value_type;                                    // 将 position 位置元素修改为新值
	}
	else {                                                         // 备用空间不足
		const auto old_size = size();
		// 如果原大小为 0，则配置 1 个元素大小，否则配置原来的大小的两倍
		const auto len = old_size == 0 ? 1 : (old_size << 1);
		auto new_start = data_allocator::allocate(len);
		auto new_finish = new_start;
		try {
			// 将原 vector 数据拷贝到新的 vector
			new_finish = mystl::uninitialized_copy(start_, position, new_start);
			mystl::construct(new_finish, value);                   // 插入新元素
			++new_finish;
			// 将剩余元素拷贝到新的 vector
			new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
		}
		catch (...) {
			mystl::destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
		}
		__destroy_and_deallocate();                                // 析构释放原 vector
		start_ = new_start;                                        // 调整迭代器，指向新的 vector
		finish_ = new_finish;
		end_of_storage_ = new_start + len;
	}
}

// __insert_dispatch 函数
template <class T, class Alloc>
template <class Integer>
void vector<T, Alloc>::__insert_dispatch(iterator position, Integer n, Integer x, __true_type) {
	__fill_insert(position, static_cast<size_type>(n), static_cast<T>(x));
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__insert_dispatch(iterator position, InputIterator first,
	InputIterator last, __false_type) {
	__range_insert(position, first, last, mystl::iterator_category(first));
}

// __fill_insert 函数
template <class T, class Alloc>
void vector<T, Alloc>::__fill_insert(iterator position, size_type n, const value_type& value) {
	if (n != 0) {
		if (static_cast<size_type>(end_of_storage_ - finish_) >= n) {
			// 如果备用空间大于等于增加的空间
			auto value_copy = value;
			const auto after_elems = finish_ - position;                            // 插入点后的元素个数
			auto old_finish = finish_;
			if (static_cast<size_type>(after_elems) > n) {                          // 插入点后元素较多
				mystl::uninitialized_copy(finish_ - n, finish_, finish_);           // 复制原元素
				finish_ += n;
				mystl::copy_backward(position, old_finish - n, old_finish);         // 复制剩余元素
				mystl::fill_n(position, n, value_copy);                             // 填充新元素
			}
			else {
				mystl::uninitialized_fill_n(finish_, n - after_elems, value_copy);  // 填充新元素
				finish_ += n - after_elems;
				mystl::uninitialized_copy(position, old_finish, finish_);           // 复制原元素
				finish_ += after_elems;
				mystl::fill(position, old_finish, value_copy);                      // 填充新元素
			}
		}
		else {                                                                      // 备用空间不足
																					// 新长度在 旧长度的两倍，旧长度 + 新增元素个数 中取较大值
			const auto old_size = size();
			const auto len = old_size + mystl::max(old_size, n);
			auto new_start = data_allocator::allocate(len);                         // 配置新的 vector 空间
			auto new_finish = new_start;
			try {
				// 先将旧 vector 插入点前的元素复制到新 vector 空间
				new_finish = mystl::uninitialized_copy(start_, position, new_start);
				// 将新增元素加入新空间
				new_finish = mystl::uninitialized_fill_n(new_finish, n, value);
				// 将旧 vector 插入点后的元素复制到新空间
				new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
			}
			catch (...) {
				mystl::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
			}
			__destroy_and_deallocate();  // 析构释放原 vector
			start_ = new_start;          // 调整迭代器，指向新的 vector
			finish_ = new_finish;
			end_of_storage_ = start_ + len;
		}
	}
}

// __range_insert 函数
template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__range_insert(iterator position, InputIterator first,
	InputIterator last, input_iterator_tag) {
	for (; first != last; ++first) {  // 一个一个元素插入
		position = insert(position, *first);
		++position;
	}
}

template <class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::__range_insert(iterator position, ForwardIterator first,
	ForwardIterator last, forward_iterator_tag) {
	if (first != last) {
		auto n = distance(first, last);
		if ((end_of_storage_ - finish_) >= n) {           // 如果备用空间大小足够
			const auto after_elems = finish_ - position;  // position 后面的元素个数
			auto old_finish = finish_;
			if (after_elems > n) {                        // position 后面的元素较多
														  // 先把最后 n 个元素复制到 finish_ 为起始的位置
				mystl::uninitialized_copy(finish_ - n, finish_, finish_);
				finish_ += n;
				mystl::copy_backward(position, old_finish - n, old_finish);
				mystl::copy(first, last, position);       // 将要插入的元素复制到 position 处
			}
			else {                                        // position 后面的元素较少
				auto mid = first;
				advance(mid, after_elems);                // 以 after_elems 把要插入的区间分为前后段
														  // 把后段复制到 finish_ 为起始的位置
				mystl::uninitialized_copy(mid, last, finish_);
				finish_ += n - after_elems;
				// 原元素复制到 finish_ 为起始的位置
				mystl::uninitialized_copy(position, old_finish, finish_);
				finish_ += after_elems;
				mystl::copy(first, mid, position);        // 复制前半段到 position 位置
			}
		}
		else {                                            // 备用空间不足
														  // 新长度在 旧长度的两倍，旧长度 + 新增元素个数 中取较大值
			const auto old_size = size();
			const auto len = old_size + mystl::max(old_size, static_cast<size_type>(n));
			auto new_start = data_allocator::allocate(len);
			auto new_finish = new_start;
			try {
				// 原 vector 的前半段复制到新的 vector 空间
				new_finish = mystl::uninitialized_copy(start_, position, new_start);
				// 把插入区间复制到新 vector 空间尾端
				new_finish = mystl::uninitialized_copy(first, last, new_finish);
				// 原 vector 的后半段复制到新的 vector 空间尾端
				new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
			}
			catch (...) {
				mystl::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
			}
			__destroy_and_deallocate();  // 析构释放原 vector
			start_ = new_start;          // 调整迭代器，指向新的 vector
			finish_ = new_finish;
			end_of_storage_ = start_ + len;
		}
	}
}

// 重载比较操作符
template <class T, class Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template <class T, class Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Alloc>
inline bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return rhs < lhs;
}

template <class T, class Alloc>
inline bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(rhs < lhs);
}

template <class T, class Alloc>
inline bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T, class Alloc>
inline void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
	lhs.swap(rhs);
}
} // namespace mystl
#endif // !MYTINYSTL_VECTOR_H_

