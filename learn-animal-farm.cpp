// Anton Yashchenko
// Dec. 7, 2017
// Animal Farm, a look at inheritance and polymorphism 
// with shared_ptr

#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <vector>
#include <typeinfo>			// i can use typeid
#include <cassert>

using namespace std;

class Animal
{
public:
	using pointer_type = shared_ptr<Animal>;
	//using pointer_type1 = shared_ptr<Dog>;

public:

	// after using virtual keyword, the getName function will be overrided in every derived class
	// virtual = polymorphism
	// when the write a virtaul function, we need a virtual destructor
	virtual ~Animal() {}

	//virtual void eat() = 0;	//pure virtual 
};

// class Canis : Animal			// default to private if nothing 
class Canis : public Animal {};				// canis is-a animal 


class Felis : public Animal {
public:
	using pointer_type = std::shared_ptr<Felis>;
	virtual void purr() const { cout << "Purr..." << endl; }
};		// felis is-a animal

class Domesticus : public Felis {
};		// Domesicus is-a Felis and is-a animal

class Persian : public Domesticus {
	void eat() { cout << "Eating rabbit... mmm\n"; }
};		// Persian is-a Domesicus, is-a Felis and is-a animal

class Dog : public Canis {
	void eat() { cout << "Eating dogbites... mmm\n"; }
};

class Wolf : public Canis {
public:
	void eat() { cout << "Eating foxes... mmm\n"; }
};

class BullDog : public Dog {
	void eat() { cout << "Eating rabbit... mmm\n"; }
};

// return Animal slices
// return Animal::pointer_type 'slurps'
Animal::pointer_type animal_factory()
{
	switch (rand() % 3)
	{
		// we use return a shard_ptr to instead of return an object
		// return Animal slices
		// return an animal object
		//case 0: return Wolf();
		//case 1: return Persian();
		//case 2: return BullDog();
		//default: return Animal(); 

		// will return a shared pointer
		// return Animal::pointer_type 'slurps'
		// create a new shared_prt of Wolf and return the shared pointer
	case 0: return Animal::pointer_type(new Wolf);
	case 1: return Animal::pointer_type(new Persian);
	case 2: return Animal::pointer_type(new BullDog);
#if #define(_DEBUG)
			assert(false && "This should never happen");
	default: return Animal::pointer_type(new Animal);
#endif
	}

}

//Dog::pointer_type1 animal_factory1()
//{
//	switch (rand() % 3)
//	{
//		// we use return a shard_ptr to instead of return an object
//
//		// return an animal object
//		//case 0: return Wolf();
//		//case 1: return Persian();
//		//case 2: return BullDog();
//		//default: return Animal(); 
//
//		// will return a shared pointer
//		// create a new shared_prt of Wolf and return the shared pointer
//	case 0: return Dog::pointer_type1(new Wolf);
//	case 1: return Dog::pointer_type1(new Persian);
//	case 2: return Dog::pointer_type1(new BullDog);
//	default: return Dog::pointer_type1(new Animal);
//	}
//
//}

/** Make a new smart-pointer managed Token object. */
// todo: make<T>
// For BOOST_AUTO_TEST_CASE(token_test)
// empty token
template <typename T> inline Animal::pointer_type make()
{
	return Animal::pointer_type(new T);
}

/** Make a new smart-pointer managed Token object with constructor parameter. */
// todo: make<U,T>
// param cast to T
template <typename T, typename U> inline Animal::pointer_type make(U const& param)
{
	return Animal::pointer_type(new T(param));
}


/** Compare two tokens for same value. */
// todo: operator==(Token,Token)
inline bool operator== (Animal::pointer_type const& lhs, Animal::pointer_type const& rhs)
{
#if !defined(NDEBUG)
	// compare between two strings
	auto l = typeid(lhs).name();
	auto r = typeid(rhs).name();
#endif
	return l == r;
}


/** Test for family membership. */
// todo: is<T>(U)
// check what the token is 
// BOOST_CHECK(is<Token>(i));		to check if i is a Token 
// BOOST_CHECK(is<Operand>(i));		to check if i is an operand
template <typename CAST_TYPE, typename ORIGINAL_TYPE>
inline bool is(ORIGINAL_TYPE const& token)
{
	// dynamic_cast can only be used with pointers and references to classes
	// .get() convert a shared pointer to a regular ptr
	// dynamic_cast returns a nullptr if it is not able to
	// properly cast up the inheritance tree
	// if cannot cast, it will give you a nullptr
	return dynamic_cast<CAST_TYPE const*>(token.get()) != nullptr;
}



/** Test for family membership. */
// todo: is<T>(U*)
// dynamic_cast is only work on the regular pointer 
// the tknPtr is a regular pointer
// check the regular pointer(tknPtr) is a <CAST_TYPE> pointer
template <typename CAST_TYPE, typename ORIGINAL_TYPE>
inline bool is(ORIGINAL_TYPE const* tknPtr)
{
	return dynamic_cast<CAST_TYPE const*>(tknPtr) != nullptr;
}



/** Convert to subclass type. */
// todo: convert<T>(U)
// convert from a shared_ptr and another shared_ptr 
template<typename CONVERTED_TYPE>
inline typename CONVERTED_TYPE::pointer_type
convert(Animal::pointer_type const& tkn)
{
	// dynamic_pointer_cast are for shared_ptr and converts it to
	// another shared_ptr
	// dynamic_cast works on the shared pointer 
	return std::dynamic_pointer_cast<CONVERTED_TYPE>(tkn);
}


// with polymorphism pass-by-reference does not slice it slurps 
ostream& operator<<(ostream& os, Animal const& a)
{
	os << typeid(a).name() + 6;
	return os;
}

int main()
{
	// current time, cause using srand(), so everytime the time is different  
	srand((unsigned)time(0));
	Animal * a = new Animal;
	//Animal * b = new Dog;
	//cout << a->getName() << endl;
	//cout << b->getName() << endl;

	for (int i = 0; i < 10; ++i)
	{

		// animal_factory returns an animal so slicing object 
		// and only outputs animals 
		// losing Wolf, BullDog and Persian version
		// this along with a virtual getName will allow slurping 
		auto a = animal_factory();
		// shared_ptr<Animal> a = animal_factory();
		// shared_ptr<Dog> d = animal_factory1();

		cout << *a << endl;			//deference a shard_ptr i got Animal
									//cout << a->getName() << endl;
	}

	cout << endl;
	Persian cat;
	cout << cat << endl;
	// calling another get_name in the class above use .operator
	// knhow that Felis has no getName() so cal the Animal::getName()
	/*cout << "Felis::getName() = " << cat.Felis::getName() << endl;
	cout << "Domesticus::getName() = " << cat.Domesticus::getName() << endl;
	cout << "Animal::getName() = " << cat.Animal::getName() << endl;*/

	// in order to get all cats in, we need to create a class reference instead of just creating an object
	Animal slicedCat = cat;
	// the getName in side the Persian class will not there, instead of that the getName() inside the Animal class will be called 
	cout << "slicedCat = " << slicedCat << endl;
	Animal& slurpCat = cat;
	cout << "slurpCat = " << slurpCat << endl;

	// more typeid stuff
	cout << "typeid(int).name() = " << typeid(int).name() << endl;
	double const* p = nullptr;
	cout << "typeid(p).name() = " << typeid(p).name() << endl;
	// deference will lose const
	cout << "typeid(*p).name() = " << typeid(*p).name() << endl;

	Animal::pointer_type ptr(new Wolf);
	cout << "typeid(prt).name() = " << typeid(ptr).name() << endl;
	cout << "typeid(*prt).name() = " << typeid(*ptr).name() << endl;

	if (typeid(*ptr) == typeid(Wolf))
		cout << "typeid said: It's a wolf" << endl;
	else
		cout << "typeid said: It's NOT a wolf" << endl;
	if (typeid(*ptr) == typeid(Canis))
		cout << "typeid said: It's a Canis" << endl;
	else
		cout << "typeid said: It's NOT a Canis" << endl;
	// typeid only compares the final types				Animal -> Canis -> Wolf

	// reinterpret cast forces a pointer type change 
	// force the Wolf to Canis 
	// this is dangerous, dont do this unless you know what you are doing 
	// Canis * pCanis = reinterpret_cast<Canis *>(ptr.get());

	Canis * pCanis = dynamic_cast<Canis *>(ptr.get());
	if (pCanis)
		cout << "dynamic cast says: It's a Canis!\n";
	else
		cout << "dynamic cast says: It's NOT a Canis!\n";

	if (dynamic_cast<Felis *>(ptr.get()))
		cout << "dynamic cast says: It's a Felis!\n";
	else
		cout << "dynamic cast says: It's NOT a Felis!\n";

	// C++ 11
	if (auto sp = dynamic_pointer_cast<Canis>(ptr))
		cout << "dynamic cast says: It's a Canis!\n";
	else
		cout << "dynamic cast says: It's NOT a Canis!\n";

	//looking at the purr
	cout << "Testing purrs......." << endl;
	//if the animal is a Felis, tell it to purr
	shared_ptr<Persian> spPersian(new Persian());
	if (Felis * pCat = dynamic_cast<Felis*>(spPersian.get()))
		pCat->purr();
	if (Felis::pointer_type pCat = dynamic_pointer_cast<Felis>(spPersian))
		pCat->purr();


	// Now testing the functions from Project2, maek, is, == 
	cout << "Now using make function\n";
	auto sp = make<Wolf>();
	cout << *sp << endl;
	auto sp1 = make<BullDog>();
	cout << *sp1 << endl;
	Animal::pointer_type sp3 = make<Persian>();
	cout << *sp3 << endl;

	if (is<Wolf>(sp))
	{
		cout << "sp is a Wolf\n";
	}
	else
	{
		cout << "sp is NOT a Wolf\n";
	}

	if (is<Canis>(sp))
	{
		cout << "sp is a Canis\n";
	}
	else
	{
		cout << "sp is NOT a Canis\n";
	}

	if (is<Felis>(sp))
	{
		cout << "sp is a Felis\n";
	}
	else
	{
		cout << "sp is NOT a Felis\n";
	}

	auto wolf = make<Wolf>();
	if (wolf == sp)
		cout << "Wolf and sp are the same\n";
}