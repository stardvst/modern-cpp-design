#include "functor.h"

#include <iostream>
#include <memory>

class MyFunctorImplOneParam
{
public:
	double operator()(int a)
	{
		return 2 * a;
	}
};

class MyFunctorImplTwoParams
{
public:
	double operator()(int a, float b)
	{
		return a + b;
	}
};

double testFunctionWithThreeParams(int a, float b, double c)
{
	return a + b + c;
}

// ambiguous call if not using a cast or a wrapper
double testFunctionWithThreeParams(int a, float b, long l)
{
	return a * b * l;
}

const char *testFunctionParamConversion(double, double)
{
	static const char buffer[] = "testFunctionParamConversion called";
	return buffer;
}

class Parrot
{
public:
	void speak() const
	{
		std::cout << "Squawk!\n";
	}

	void eat()
	{
		std::cout << "Nom nom nom\n";
	}
};

int main()
{
	MyFunctorImplOneParam implOneParam;
	Functor<double, TYPELIST_1(int)> myFunctorOneParam(implOneParam);
	double result = myFunctorOneParam(42);
	std::cout << result << "\n";

	MyFunctorImplTwoParams impl;
	Functor<double, TYPELIST_2(int, float)> myFunctorTwoParams(impl);
	result = myFunctorTwoParams(42, 3.14f);
	std::cout << result << "\n";

	// handling overloaded function pointers

	using FunType = double (*)(int, float, double);
	FunType funcPtr = &testFunctionWithThreeParams;
	Functor<double, TYPELIST_3(int, float, double)> myFunctorThreeParams(funcPtr);
	result = myFunctorThreeParams(42, 3.14f, 2.71);
	std::cout << result << "\n";

	using FunType2 = double (*)(int, float, long);
	Functor<double, TYPELIST_3(int, float, long)> myFunctorThreeParams2(static_cast<FunType2>(testFunctionWithThreeParams));
	result = myFunctorThreeParams2(42, 3.14f, 2);
	std::cout << result << "\n";

	// handling implicit parameter conversions

	Functor<std::string, TYPELIST_2(int, int)> myFunctorParamConversion(&testFunctionParamConversion);
	std::string strResult = myFunctorParamConversion(42, 3);
	std::cout << strResult << "\n";

	// handling member function pointers

	Parrot parrot;
	Functor<void> speakFunctor(&parrot, &Parrot::speak);
	speakFunctor();
	Functor<void> eatFunctor(&parrot, &Parrot::eat);
	eatFunctor();
	std::cout << "\n";

	return 0;
}
