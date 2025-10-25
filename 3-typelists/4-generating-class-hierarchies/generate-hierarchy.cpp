#include "generate-hierarchy.h"
#include "field-helpers.h"
#include "typelist.h"

#include <iostream>
#include <string>

template <class T>
struct Holder
{
	T value;
};

int main()
{
	// Holder<int>, Holder<std::string>, Holder<double>
	using Info =
		GenerateHierarchy<TYPELIST_3(int, std::string, double), Holder>;

	Info info;
	static_cast<Holder<int> &>(info).value = 42;
	std::cout << static_cast<Holder<int> &>(info).value << "\n";
	std::cout << static_cast<Holder<std::string> &>(info).value << "\n";
	std::cout << static_cast<Holder<double> &>(info).value << "\n";

	std::cout << "---\n";
	std::cout << Field<int>(info).value << "\n";
	std::cout << Field<std::string>(info).value << "\n";
	std::cout << Field<double>(info).value << "\n";

	std::cout << "---\n";
	std::cout << Field<0>(info).value << "\n";
	std::cout << Field<1>(info).value << "\n";
	std::cout << Field<2>(info).value << "\n";

	return 0;
}
