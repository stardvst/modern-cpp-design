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

class MyFunctorImplThreeParams
{
public:
	double operator()(int a, float b, double c)
	{
		return a + b + c;
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

	MyFunctorImplThreeParams implThreeParams;
	Functor<double, TYPELIST_3(int, float, double)> myFunctorThreeParams(implThreeParams);
	result = myFunctorThreeParams(42, 3.14f, 2.71);
	std::cout << result << "\n";

	return 0;
}
