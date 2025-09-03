#pragma once
//#pragma once
//#include <cstddef>
//
//template<int N>
//struct Factorial
//{
//	static const int value = N * Factorial<N - 1>::value;
//};
//
//template<>
//struct Factorial<1>
//{
//	static const int value = 1;
//};
//
////Non const returns self
//template<typename T >
//struct removeConst {
//	typedef T type;               // (1)
//};
//
////Specialize for const T
//template<typename T >
//struct removeConst<const T> {
//	typedef T type;               // (1)
//};
//
//
////Here are today's rules for template definitions:
//
////T.60: Minimize a template�s context dependencies -
////It would be optimal but not always manageable if a template operates only on its arguments. 
//template<typename C>
//void sort(C& c)
//{
//	std::sort(begin(c), end(c)); // necessary and useful dependency
//}
//
//template<typename Iter>
//Iter algo(Iter first, Iter last) {
//	for (; first != last; ++first) {
//		auto x = sqrt(*first); // potentially surprising dependency: which sqrt()?
//		helper(first, x);      // potentially surprising dependency:
//							   // helper is chosen based on first and x    
//	}
//}
////T.61 : Do not over - parameterize members(SCARY)
///*
//he acronym SCARY describes assignments and initializations that
//are Seemingly erroneous (appearing Constrained by conflicting generic 
//parameters), but Actually work with the Right implementation (unconstrained
//bY the conflict due to minimized dependencies).
//I hope, you get it for the details see N2911. 
//In order not to bore you, here is the obvious idea applied to the standard 
//container iterator: they have no dependency to the container's, 
//key_compare, hasher, key_equal, or allocator types.*/
//
////T.62 : Place non - dependent class template members in a non - templated base class
////put the functionality of the template which does not depend on the template parameters in a non-templated base class.
//template <typename T, std::size_t N>
//class Array1 {
//public:
//	Array() = default;
//	std::size_t getSize() const {
//		return N;
//	}
//private:
//	T elem[N];
//};
//
//// method getSize is the same except for type parameter N.
////Let me refactor the code and declare a class template Array which depends on the type parameter 
//template<typename T>
//class ArrayBase {
//protected:
//	ArrayBase(std::size_t n) : size(n) {}
//	std::size_t getSize() const {
//		return size;
//	};
//private:
//	std::size_t size;
//};
//
//template<typename T, std::size_t n>
//class Array2 : private ArrayBase<T> {
//public:
//	Array() : ArrayBase<T>(n) {}
//	std::size_t getSize() const {
//		return  ArrayBase<T>::getSize();
//	}
//private:
//	T data[n];
//};