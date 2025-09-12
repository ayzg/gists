#pragma once
#include <iostream>
#include <string>
#include <complex>
#include <type_traits>

/*
1. In this case, type T has to support operator < because a and b are compared using this operator.
2. values of type T must also be copyable in order to be returned.
*/
template<typename T>
T max(T a, T b)
{
	// if b < a then yield a else yield b
	return b < a ? a : b;
}

static void test_func_t_max()
{
	int i = 42;
	std::cout << "max(7,i):      " << ::max(7, i) << std::endl;

	double f1 = 3.4; double f2 = -6.7;
	std::cout << "max(f1,f2):    " << ::max(f1, f2) << std::endl;

	std::string s1 = "mathematics"; std::string s2 = "math";
	std::cout << "max(s1,s2):    " << ::max(s1, s2) << std::endl;

}


// class equivalent
//template<class T>
//T max(T a, T b)
//{
//
//	// if b < a then yield a else yield b
//	return b < a ? a : b;
//}


/*
Templates aren�t compiled into single entities that can handle any type.
different entities are generated from the template for every type for which the template is used



void is a valid template argument provided the resulting code is valid.
*/
template <typename T>
T foo(T*)
{

}

static void test_func_t_void_p()
{
	void * vp = nullptr;
	foo(vp);
	//foo(void*);
}



//attempt to instantiate a template for a type that doesn�t support all the operations 
std::complex<float> c1, c2;

static void test_func_t_no_type_support()
{
	std::complex<float> c1, c2;        // doesn�t provide operator < 
	//max(c1, c2);                     // ERROR at compile timed*);
}

//*******************************************************
// TmeplateArgumentDeduction

static void test_func_t_implicit_deduction()
{

	//int const c = 42; 
	// max(i, c);         // OK: T is deduced as int 
	//max(c, c);// OK: T is deduced as int int& ir = i; 
	//max(i, ir);// OK: T is deduced as int int arr[4]; 
	//foo(&i, arr);// OK: T is deduced as int*



	//max(4, 7.2);        // ERROR: T can be deduced as int or double 
	//foo("hello", s);    //ERROR: T can be deduced as char const[6]

	//SOlutions
	// 1 . cast arguments so they match
	// 2. Specify explicitly the type of T to prevent compliler from attemting deductions
	// 3. Specifty that the parameter may have diffrent types


	//Notes:
	//  type deduction does not work for default call arguments
	// you also have to declare a default argument for the template parameter

}

//*********************************************************************************
//Multiple Parameters
template <typename T1, typename T2>
T1 max(T1 a, T2 b)
{
	// if b < a then yield a else yield b
	return b < a ? a : b; // OK, but type of first argument defines return type
}

//Solutions to return type conversion
//1.Introduce a third template parameter for the return type
//2. Let the compiler find out the return type.
//3. Declare the return type to be the �common type� of the two parameter types. All these options are discussed next.


//  introduce a third template argument type to define the return type of a function template: 
template<typename T1, typename T2, typename RT>
RT max(T1 a, T2 b);
// CAVEAT:
//1. template argument deduction does not take return types into account
//2.  you have to specify the template argument list explicitly

//Solution
//  if you change the order of the template parameters in our example,
// the caller needs to specify only the return type: 
template<typename RT, typename T1, typename T2>
RT max(T1 a, T2 b);

//ex. max<double>(4 , 7.2) //OK: return type is double, T1 and T2 are deduced

//***************************************************************
// Deducing return types
// since c++ 14 auto
// example
template <typename T1, typename T2>
auto max(T1 a, T2 b)
{
	return b < a ? a : b;
}
// c++11 alternative
template <typename T1, typename T2>
auto max(T1 a, T2 b)  -> decltype(b < a ? a : b)
{
	return b < a ? a : b;
}

// is enough to do same as above
template <typename T1, typename T2>
auto max(T1 a, T2 b)  -> decltype(true ? a : b)
{
	return b < a ? a : b;
} //drawback: return type could be reference type 

// solution using type decayed from t
template <typename T1, typename T2>
auto max(T1 a, T2 b)  -> typename std::decay<decltype(true ? a : b)>::type
{
	return b < a ? a : b;
}


//************************************************************************
// Returning types as common types
//.� std::common_type<>::type yields the �common type� of two (or more) different types passed as template arguments
//EXAMPLE

template <typename T1, typename T2>
std::common_type_t<T1, T2> max(T1 a, T2 b)
{
	return b < a ? a : b;
}

// usage of common type
//template <typename T1, typename T2>
//typename std::common_type<T1, T2>::type;// since c++11

//c++ 14 equivalent
//std::common_type_t<T1,T2>


//***********************************************
//Default Template Arguments

//1.  can use operator?: but can only use types
//requires that we are able to call default constructors for the passed types
//ca use std::declval but makes declaration verbose section 11.2.3 pg 166 for example
template <typename T1, typename T2, typename RT = std::decay_t<decltype(true ? T1() : T2())>>
RT max(T1 a, T2 b)
{
	return b < a ? a : b;
}

//2. use std::common_type<> trait
template <typename T1, typename T2, typename RT = std::common_type_t<T1, T2>>
RT max(T1 a, T2 b)
{
	return b < a ? a : b;
}
//we have the problem that we have to specify three types to be able to specify the return type only.
//best to let compiler deduce
// section 26.5.1 pg 621 for detailed solution

//*****************************************************************************************************
// Function Template Overloading

//SECTION 1.5 OVERLOADING FUNCTION tEMPLATES
//For details of partial specialization, see Section 16.4 on page 347.
