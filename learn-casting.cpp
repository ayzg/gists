#include <iostream>
#include <memory>

using namespace std;

class A {
public:
	int a_;
};
class B : public A {};
class C : A {}; // private inheritance
class D {};

void func1(int * x) {
	(*x)++;
	cout << "In func1: x = " << *x << endl;
	cout << "typeid(x)" << typeid(x).name() << endl;
}

void func2(const int * x) {
	//(*x)++; Does not work because const
	cout << "In func1: x = " << *x << endl;
	cout << "typeid(x)" << typeid(x).name() << endl;
}

int main()
{
	//static casting ex.(float to int)
	float f = 3.14;
	int i = (int)f; //original method
	int ii = static_cast<int>(f); // safer method - checks to make sure flaot can bceome int
	//dynamic cast is used for the conversion of polymorphic types.
	B b; // instance of b
	A * a = dynamic_cast<A*>(&b); // always wants a pointer,casts to a shared pointer 
	if (a) cout << "b can be casted to an A*\n";
	//compiler wont allow the code below
	C c;
	//A * a = dynamic_cast<A*>(&c);  // C and A have a private inheritance, &c cannot be seen

	shared_ptr<B> b1(new B);
	if (dynamic_cast<A*>(b1.get())) cout << "b1 is an A*\n"; // has to return regular pointer
	if (dynamic_pointer_cast<A>(b1)) cout << "b1 is an A*\n"; // has to return shared pointer

	//const_cast
	int x = 42;
	const int * a1 = &x;
	//func1(a1); wont work cause const
	func1(const_cast<int*>(a1)); // Takes away the 'constantness' of a1
	cout << "typeid(a1) after const cast = " << typeid(a1).name() << endl;

	//reinterpret_cast - can take two completley diffrent objects and cast them to eachother
	D * d = reinterpret_cast<D*>(&b); // D does not inherit B
	//D has nothing to do with B but allows casting using reinterpret cast
	//Use with caution


	unique_ptr<int> up;
	unique_ptr<int> up1();
	unique_ptr<int> up2(new int(42));
	//unique_ptr<int> up3(up2); not allowed copy constructor
	//up3 = up2; not allowed copy assingment

	shared_ptr<int> sp;
	shared_ptr<int> sp1();
	shared_ptr<int> sp2(new int(42));
	shared_ptr<int> sp3(sp2);
	sp = sp3;
	shared_ptr<int> sp4 = sp2;
	cout << "Use count sp3 = " << sp3.use_count() << endl; //Calls destructor when use_count() reaches 0



}