#include "smart-ptr.h"

#include <cassert>
#include <iostream>

// Test class for more complex scenarios
class TestClass
{
public:
	int value;
	TestClass(int v) : value(v)
	{
	}
};

void test_basic_default()
{
	std::cout << "=== Test 1: Basic Default SmartPtr ===\n";
	SmartPtr<int> sp(new int(5));
	assert(*sp == 5);
	*sp = 10;
	assert(*sp == 10);
	std::cout << "Value: " << *sp << "\n";
	std::cout << "✓ Basic default smart pointer works\n\n";
}

void test_reference_counting()
{
	std::cout << "=== Test 2: Reference Counting (RefCounted) ===\n";
	SmartPtr<int, RefCounted> sp1(new int(42));
	{
		SmartPtr<int, RefCounted> sp2 = sp1; // Copy constructor
		assert(*sp1 == 42);
		assert(*sp2 == 42);
		*sp2 = 100;
		assert(*sp1 == 100); // Both point to same object
		std::cout << "After copy, both point to same value: " << *sp1 << "\n";
	} // sp2 goes out of scope, but object should still exist
	assert(*sp1 == 100); // sp1 still valid
	std::cout << "✓ Reference counting works correctly\n\n";
}

void test_no_check_policy()
{
	std::cout << "=== Test 3: NoCheck Policy ===\n";
	SmartPtr<int, RefCounted, DisallowConversion, NoCheck> sp(new int(7));
	assert(*sp == 7);
	*sp = 77;
	assert(*sp == 77);
	std::cout << "Value: " << *sp << "\n";
	std::cout << "✓ NoCheck policy works\n\n";
}

void test_reject_null_policy()
{
	std::cout << "=== Test 4: RejectNull Policy ===\n";
	try
	{
		SmartPtr<int, RefCounted, DisallowConversion, RejectNull> sp(nullptr);
		std::cout << "✗ Should have thrown exception\n";
		assert(false);
	}
	catch (const NullPointerException &e)
	{
		std::cout << "✓ Correctly rejected null pointer: " << e.what() << "\n";
	}

	// Valid pointer should work
	SmartPtr<int, RefCounted, DisallowConversion, RejectNull> sp(new int(99));
	assert(*sp == 99);
	std::cout << "✓ Valid pointer works: " << *sp << "\n\n";
}

void test_conversion_policies()
{
	std::cout << "=== Test 5: Conversion Policies ===\n";

	// DisallowConversion (default) - explicit construction required
	SmartPtr<int, RefCounted, DisallowConversion> sp1(new int(1));
	std::cout << "DisallowConversion: " << *sp1 << "\n";

	// AllowConversion - implicit construction allowed
	SmartPtr<int, RefCounted, AllowConversion> sp2(new int(2));
	std::cout << "AllowConversion: " << *sp2 << "\n";

	std::cout << "✓ Conversion policies work\n\n";
}

void test_destructive_copy()
{
	std::cout << "=== Test 6: DestructiveCopy Policy ===\n";
	SmartPtr<int, DestructiveCopy> sp1(new int(50));
	assert(*sp1 == 50);

	SmartPtr<int, DestructiveCopy> sp2 = sp1; // Destructive copy
	assert(*sp2 == 50);
	// sp1 should now be null (destructive copy moved ownership)
	assert(!sp1); // Should be null after destructive copy
	std::cout << "After destructive copy:\n";
	std::cout << "  sp1 is null: " << (!sp1 ? "true" : "false") << "\n";
	std::cout << "  sp2 value: " << *sp2 << "\n";
	std::cout << "✓ Destructive copy works\n\n";
}

void test_custom_type()
{
	std::cout << "=== Test 7: Custom Type ===\n";
	SmartPtr<TestClass> sp(new TestClass(123));
	assert(sp->value == 123);
	sp->value = 456;
	assert(sp->value == 456);
	std::cout << "Custom class value: " << sp->value << "\n";
	std::cout << "✓ Custom types work\n\n";
}

void test_arrow_operator()
{
	std::cout << "=== Test 8: Arrow Operator ===\n";
	SmartPtr<TestClass> sp(new TestClass(789));
	assert(sp->value == 789);
	sp->value = 999;
	assert((*sp).value == 999);
	std::cout << "Arrow operator value: " << sp->value << "\n";
	std::cout << "✓ Arrow operator works\n\n";
}

void test_multiple_copies()
{
	std::cout << "=== Test 9: Multiple Copies (Reference Counting) ===\n";
	SmartPtr<int, RefCounted> sp1(new int(111));
	SmartPtr<int, RefCounted> sp2 = sp1;
	SmartPtr<int, RefCounted> sp3 = sp2;
	*sp3 = 222;
	assert(*sp1 == 222);
	assert(*sp2 == 222);
	assert(*sp3 == 222);
	std::cout << "All three pointers share value: " << *sp1 << "\n";
	std::cout << "✓ Multiple copies work correctly\n\n";
}

void test_null_check()
{
	std::cout << "=== Test 10: Null Check Operator ===\n";
	SmartPtr<int, RefCounted, DisallowConversion, NoCheck> sp1;
	assert(!sp1); // Should be null
	std::cout << "Null pointer check: " << (!sp1 ? "null" : "not null") << "\n";

	SmartPtr<int, RefCounted, DisallowConversion, NoCheck> sp2(new int(333));
	assert(!!sp2); // Should not be null
	std::cout << "Valid pointer check: " << (!sp2 ? "null" : "not null") << "\n";
	std::cout << "✓ Null check operator works\n\n";
}

int main()
{
	std::cout << "Running SmartPtr Test Suite\n";
	std::cout << "===========================\n\n";

	test_basic_default();
	test_reference_counting();
	test_no_check_policy();
	test_reject_null_policy();
	test_conversion_policies();
	test_destructive_copy();
	test_custom_type();
	test_arrow_operator();
	test_multiple_copies();
	test_null_check();

	std::cout << "===========================\n";
	std::cout << "All tests passed! ✓\n";

	return 0;
}
