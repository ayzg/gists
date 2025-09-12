#pragma once
//#pragma once
//
//#include <array>
//#include <memory>


template<typename T,int max_size>
class stack_cbuffer {
	std::array<T, max_size> data_;
	size_t size;
	size_t offset;

	T * head_;
	T * tail_;
public:
	push(T val) {
		*head_ = val;
		head_++;
		size++;
	}

	auto&& pull() {
		auto&& ret = std::move(*tail);
		tail_++;
		size--;
		offset++;
		return std::forward(ret);
	}

	constexpr auto& operator[](size_t idx) {
		return data_[idx + offset];
	}

	constexpr auto begin() {
		auto ret = data_.begin();
		std::advance(ret, offset);
		return std::move(ret);
	}


};

auto tst() {
	stack_cbuffer<int, 3> m_buffer;
	m_buffer.push(10);
	auto is = m_buffer[1] == 10;
	m_buffer.pull();


}

//
//constexpr std::size_t get_midpoint(std::size_t size)
//{
//	if (size % 2)
//		return size / 2;
//	else
//		return (size / 2) + 1;
//}
//
//
//template<typename T,std::size_t Size, class A = std::allocator<T>>
//class Buffer
//{
//	/* Types */
//	using allocator_type = A;
//	using value_type = typename A::value_type;
//	using reference = typename A::reference;
//
//
//	using pointer = value_type * ;
//	using const_pointer = const value_type*;
//	using const_reference = const value_type&;
//	using size_type = std::size_t;
//	using difference_type = std::ptrdiff_t;
//
//
//	std::size_t t_count_;
//
//	class iterator {
//	public:
//		typedef typename A::difference_type difference_type;
//		typedef typename A::value_type value_type;
//		typedef typename A::reference reference;
//		typedef typename A::pointer pointer;
//		typedef std::random_access_iterator_tag iterator_category; //or another tag
//
//		iterator();
//		iterator(const iterator&);
//		~iterator();
//
//		iterator& operator=(const iterator&);
//		bool operator==(const iterator&) const;
//		bool operator!=(const iterator&) const;
//		bool operator<(const iterator&) const; //optional
//		bool operator>(const iterator&) const; //optional
//		bool operator<=(const iterator&) const; //optional
//		bool operator>=(const iterator&) const; //optional
//
//		iterator& operator++();
//		iterator operator++(int); //optional
//		iterator& operator--(); //optional
//		iterator operator--(int); //optional
//		iterator& operator+=(size_type); //optional
//		iterator operator+(size_type) const; //optional
//		friend iterator operator+(size_type, const iterator&); //optional
//		iterator& operator-=(size_type); //optional            
//		iterator operator-(size_type) const; //optional
//		difference_type operator-(iterator) const; //optional
//
//		reference operator*() const;
//		pointer operator->() const;
//		reference operator[](size_type) const; //optional
//	};
//	class const_iterator {
//	public:
//		typedef typename A::difference_type difference_type;
//		typedef typename A::value_type value_type;
//		typedef typename const A::reference reference;
//		typedef typename const A::pointer pointer;
//		typedef std::random_access_iterator_tag iterator_category; //or another tag
//
//		const_iterator();
//		const_iterator(const const_iterator&);
//		const_iterator(const iterator&);
//		~const_iterator();
//
//		const_iterator& operator=(const const_iterator&);
//		bool operator==(const const_iterator&) const;
//		bool operator!=(const const_iterator&) const;
//		bool operator<(const const_iterator&) const; //optional
//		bool operator>(const const_iterator&) const; //optional
//		bool operator<=(const const_iterator&) const; //optional
//		bool operator>=(const const_iterator&) const; //optional
//
//		const_iterator& operator++();
//		const_iterator operator++(int); //optional
//		const_iterator& operator--(); //optional
//		const_iterator operator--(int); //optional
//		const_iterator& operator+=(size_type); //optional
//		const_iterator operator+(size_type) const; //optional
//		friend const_iterator operator+(size_type, const const_iterator&); //optional
//		const_iterator& operator-=(size_type); //optional            
//		const_iterator operator-(size_type) const; //optional
//		difference_type operator-(const_iterator) const; //optional
//
//		reference operator*() const;
//		pointer operator->() const;
//		reference operator[](size_type) const; //optional
//	};
//
//
//	using iterator = value_type * ;
//	using const_iterator = const value_type*;
//	using reverse_iterator = std::reverse_iterator<iterator>;
//	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
//
//	constexpr size_type left_size() { return get_midpoint(Size); }
//	constexpr size_type right_size() { return Size - get_midpoint(Size); }
//
//	/* Inner Arrays */
//	std::array<value_type, left_size()> array_l_;
//	std::array<value_type, right_size()> array_r_;
//
//
//	Buffer()
//	{
//		t_count_ = 0;
//	}
//
//
//
//	/* Iterators */
//	constexpr iterator begin() noexcept { return iterator(array_l_.begin()); }
//	constexpr iterator end() noexcept {	return iterator(array_r_.end());}
//	constexpr const_iterator cbegin() noexcept { return const_iterator(array_l_.cbegin()); }
//	constexpr iterator cend() noexcept { return const_iterator(array_r_.cend()); }
//	constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(array_r_.rbegin()); }
//	constexpr reverse_iterator rend() noexcept { return reverse_iterator(array_l_.rend()); }
//	constexpr const_reverse_iterator crbegin() noexcept { return const_reverse_iterator(array_r_.crbegin()); }
//	constexpr const_reverse_iterator crend() noexcept { return const_reverse_iterator(array_l_.crend()); }
//
//	constexpr reference	front() noexcept { return reference(array_l_.front()); }
//	constexpr const_reference front() const noexcept { return const_reference(array_l_.front());}
//	constexpr reference	back() noexcept	{ return; }
//	constexpr const_reference back() const noexcept
//	{
//		return _Nm ? _AT_Type::_S_ref(_M_elems, _Nm - 1)
//			: _AT_Type::_S_ref(_M_elems, 0);
//	}
//	constexpr reference
//		operator[](size_type __n) noexcept
//	{
//		return (size() > left_size()) ? reference(array_r_[__n - left_size()]) : reference(array_l_[__n]);;
//	}
//	constexpr const_reference
//		operator[](size_type __n) const noexcept
//	{
//		return (size() > left_size() ) ? const_reference(array_r_[__n - left_size()]) : const_reference(array_l_[__n]));
//	}
//
//	constexpr reference	at(size_type __n)
//	{
//		return (size() > left_size()) ? reference(array_r_.at(__n - left_size()) : reference(array_l_.at(__n)));
//	}
//	constexpr const_reference
//		at(size_type __n) const
//	{
//		return (size() > left_size()) ? reference(array_r_.at(__n - left_size()) : reference(array_l_.at(__n)));
//	}
//
//	template<typename _Tp, std::size_t _Nm>
//	inline auto	swap(Buffer<_Tp, Size>& __one, Buffer<_Tp, Size>& __two) noexcept(noexcept(__one.swap(__two)))
//	{
//		__one.array_l_.swap(__two.array_l_);
//		__one.array_r_.swap(__two.array_r_);
//	}
//
//
//	constexpr size_type
//		size() const noexcept { return t_count_; }
//	constexpr size_type
//		max_size() const noexcept { return Size; }
//	constexpr bool
//		empty() const noexcept { return size() == 0; }
//public:
//	Buffer();
//	~Buffer();
//
//	void
//		fill(const_reference __u)
//	{
//		std::fill_n(begin(), size(), __u);
//	}
//};



#include <memory>
#include <array>
#include <vector>

template<typename size_type>
static inline constexpr size_type get_midpoint(size_type size)
{
	if (size % 2)
		return size / 2;
	else
		return (size / 2) + 1;
}

template<typename size_type>
static inline constexpr size_type left_size(size_type S) { return get_midpoint(S); }

template<typename size_type>
static inline constexpr size_type right_size(size_type S) { return S - get_midpoint(S); }


template <class T, typename std::allocator<T>::size_type S, class A = std::allocator<T> >
class lin_buffer {
public:
	typedef A allocator_type;
	typedef typename A::value_type							value_type;
	typedef typename A::reference							reference;
	typedef typename A::const_reference						const_reference;
	typedef typename A::pointer								pointer;
	typedef typename A::const_pointer					const_pointer;
	typedef typename A::difference_type						difference_type;
	typedef typename A::size_type							size_type;
	typedef typename std::array<value_type, left_size(S)>	left_array_t;
	typedef typename std::array<value_type, right_size(S)>	right_array_t;
	typedef typename std::array<value_type, left_size(S)>::iterator		left_array_it;
	typedef typename std::array<value_type, right_size(S)>::iterator	right_array_it;

	left_array_t array_l_;
	right_array_t array_r_;
	std::vector<pointer> buffer_array_;

	class
		iterator {
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename A::reference reference;
		typedef typename A::pointer pointer;
		typedef std::random_access_iterator_tag iterator_category; //or another tag

		pointer it_;

		iterator() = default;
		iterator(const iterator& other)
		{

		};
		~iterator() = default;

		iterator& operator=(const iterator& other) {

		};
		bool operator==(const iterator& other) const
		{

		};
		bool operator!=(const iterator& other) const
		{

		};
		//bool operator<(const iterator&) const; //optional
		//bool operator>(const iterator&) const; //optional
		//bool operator<=(const iterator&) const; //optional
		//bool operator>=(const iterator&) const; //optional

		//iterator& operator++() { (size() != left_size(S)) ? iterator(array_r_[__n - left_size()]) : iterator(array_l_[__n]) };
		//iterator operator++(int); //optional
		//iterator& operator--(); //optional
		//iterator operator--(int); //optional
		//iterator& operator+=(size_type); //optional
		//iterator operator+(size_type) const; //optional
		//friend iterator operator+(size_type, const iterator&); //optional
		//iterator& operator-=(size_type); //optional            
		//iterator operator-(size_type) const; //optional
		//difference_type operator-(iterator) const; //optional

		//reference operator*() const;
		//pointer operator->() const {};
		//reference operator[](size_type) const; //optional
	};




	class const_iterator {
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename const A::reference reference;
		typedef typename const A::pointer pointer;
		typedef std::random_access_iterator_tag iterator_category; //or another tag

		//const_iterator();
		//const_iterator(const const_iterator&);
		//const_iterator(const iterator&);
		//~const_iterator();

		//const_iterator& operator=(const const_iterator&);
		//bool operator==(const const_iterator&) const;
		//bool operator!=(const const_iterator&) const;
		//bool operator<(const const_iterator&) const; //optional
		//bool operator>(const const_iterator&) const; //optional
		//bool operator<=(const const_iterator&) const; //optional
		//bool operator>=(const const_iterator&) const; //optional

		//const_iterator& operator++();
		//const_iterator operator++(int); //optional
		//const_iterator& operator--(); //optional
		//const_iterator operator--(int); //optional
		//const_iterator& operator+=(size_type); //optional
		//const_iterator operator+(size_type) const; //optional
		//friend const_iterator operator+(size_type, const const_iterator&); //optional
		//const_iterator& operator-=(size_type); //optional            
		//const_iterator operator-(size_type) const; //optional
		//difference_type operator-(const_iterator) const; //optional

		//reference operator*() const;
		//pointer operator->() const;
		//reference operator[](size_type) const; //optional
	};

	//typedef std::reverse_iterator<iterator> reverse_iterator; //optional
	//typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

	//lin_buffer();
	//lin_buffer(const lin_buffer&);
	//~lin_buffer();

	//lin_buffer& operator=(const lin_buffer&);
	//bool operator==(const lin_buffer&) const;
	//bool operator!=(const lin_buffer&) const;
	//bool operator<(const lin_buffer&) const; //optional
	//bool operator>(const lin_buffer&) const; //optional
	//bool operator<=(const lin_buffer&) const; //optional
	//bool operator>=(const lin_buffer&) const; //optional

	//iterator begin();
	//const_iterator begin() const;
	//const_iterator cbegin() const;
	//iterator end();
	//const_iterator end() const;
	//const_iterator cend() const;
	//reverse_iterator rbegin(); //optional
	//const_reverse_iterator rbegin() const; //optional
	//const_reverse_iterator crbegin() const; //optional
	//reverse_iterator rend(); //optional
	//const_reverse_iterator rend() const; //optional
	//const_reverse_iterator crend() const; //optional

	//reference front(); //optional
	//const_reference front() const; //optional
	//reference back(); //optional
	//const_reference back() const; //optional
	//template<class ...Args>
	//void emplace_front(Args&&...); //optional
	//template<class ...Args>
	//void emplace_back(Args&&...); //optional
	//void push_front(const T&); //optional
	//void push_front(T&&); //optional
	//void push_back(const T&); //optional
	//void push_back(T&&); //optional
	//void pop_front(); //optional
	//void pop_back(); //optional
	//reference operator[](size_type); //optional
	//const_reference operator[](size_type) const; //optional
	//reference at(size_type); //optional
	//const_reference at(size_type) const; //optional

	//template<class ...Args>
	//iterator emplace(const_iterator, Args&&...); //optional
	//iterator insert(const_iterator, const T&); //optional
	//iterator insert(const_iterator, T&&); //optional
	//iterator insert(const_iterator, size_type, T&); //optional
	//template<class iter>
	//iterator insert(const_iterator, iter, iter); //optional
	//iterator insert(const_iterator, std::initializer_list<T>); //optional
	//iterator erase(const_iterator); //optional
	//iterator erase(const_iterator, const_iterator); //optional
	//void clear(); //optional
	//template<class iter>
	//void assign(iter, iter); //optional
	//void assign(std::initializer_list<T>); //optional
	//void assign(size_type, const T&); //optional

	//void swap(lin_buffer&);
	//size_type size() const;
	//size_type max_size() const;
	//bool empty() const;

	//A get_allocator() const; //optional
};
//template <class T, class A = std::allocator<T> >
//void swap(lin_buffer<T, A>&, lin_buffer<T, A>&); //optional
