#include "factory.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

// Hash specialization for Loki::TypeInfo to use with std::unordered_map
namespace std
{
template <>
struct hash<Loki::TypeInfo>
{
	size_t operator()(const Loki::TypeInfo &ti) const noexcept
	{
		// Hash based on the type_info pointer address
		// type_info objects have stable addresses, so this is safe
		const std::type_info &info = ti.Get();
		return reinterpret_cast<size_t>(&info);
	}
};
} // namespace std

// Test classes for Factory
class Shape
{
public:
	virtual ~Shape() = default;
	virtual double GetArea() const = 0;
	virtual const char *GetName() const = 0;
};

class Circle : public Shape
{
public:
	Circle(double radius) : m_radius(radius)
	{
	}

	double GetArea() const override
	{
		return 3.14159 * m_radius * m_radius;
	}

	const char *GetName() const override
	{
		return "Circle";
	}

	double GetRadius() const
	{
		return m_radius;
	}

private:
	double m_radius;
};

class Rectangle : public Shape
{
public:
	Rectangle(double width, double height) : m_width(width), m_height(height)
	{
	}

	double GetArea() const override
	{
		return m_width * m_height;
	}

	const char *GetName() const override
	{
		return "Rectangle";
	}

	double GetWidth() const
	{
		return m_width;
	}

	double GetHeight() const
	{
		return m_height;
	}

private:
	double m_width;
	double m_height;
};

class Triangle : public Shape
{
public:
	Triangle(double base, double height) : m_base(base), m_height(height)
	{
	}

	double GetArea() const override
	{
		return 0.5 * m_base * m_height;
	}

	const char *GetName() const override
	{
		return "Triangle";
	}

private:
	double m_base;
	double m_height;
};

// Test classes for CloneFactory
class Animal
{
public:
	virtual ~Animal() = default;
	virtual const char *GetName() const = 0;
	virtual Animal *Clone() const = 0;
};

class Dog : public Animal
{
public:
	Dog(const std::string &name = "Buddy") : m_name(name)
	{
	}

	const char *GetName() const override
	{
		return m_name.c_str();
	}

	Animal *Clone() const override
	{
		return new Dog(m_name);
	}

	void SetName(const std::string &name)
	{
		m_name = name;
	}

private:
	std::string m_name;
};

class Cat : public Animal
{
public:
	Cat(const std::string &name = "Whiskers") : m_name(name)
	{
	}

	const char *GetName() const override
	{
		return m_name.c_str();
	}

	Animal *Clone() const override
	{
		return new Cat(m_name);
	}

	void SetName(const std::string &name)
	{
		m_name = name;
	}

private:
	std::string m_name;
};

// Factory Tests
void test_factory_register()
{
	std::cout << "=== Test 1: Factory Registration ===\n";
	Factory<Shape, int> factory;

	// Register creators
	bool result1 = factory.Register(1, []() -> Shape * { return new Circle(5.0); });
	bool result2 = factory.Register(2, []() -> Shape * { return new Rectangle(3.0, 4.0); });
	bool result3 = factory.Register(3, []() -> Shape * { return new Triangle(4.0, 3.0); });

	assert(result1 == true);
	assert(result2 == true);
	assert(result3 == true);

	// Try to register same ID again (should fail)
	bool result4 = factory.Register(1, []() -> Shape * { return new Circle(10.0); });
	assert(result4 == false);

	std::cout << "✓ Factory registration works correctly\n\n";
}

void test_factory_create()
{
	std::cout << "=== Test 2: Factory Object Creation ===\n";
	Factory<Shape, int> factory;

	factory.Register(1, []() -> Shape * { return new Circle(5.0); });
	factory.Register(2, []() -> Shape * { return new Rectangle(3.0, 4.0); });
	factory.Register(3, []() -> Shape * { return new Triangle(4.0, 3.0); });

	// Create objects
	std::unique_ptr<Shape> circle(factory.CreateObject(1));
	std::unique_ptr<Shape> rectangle(factory.CreateObject(2));
	std::unique_ptr<Shape> triangle(factory.CreateObject(3));

	assert(circle != nullptr);
	assert(rectangle != nullptr);
	assert(triangle != nullptr);

	assert(circle->GetArea() == 3.14159 * 5.0 * 5.0);
	assert(rectangle->GetArea() == 12.0);
	assert(triangle->GetArea() == 6.0);

	std::cout << "Created " << circle->GetName() << " with area: " << circle->GetArea() << "\n";
	std::cout << "Created " << rectangle->GetName() << " with area: " << rectangle->GetArea() << "\n";
	std::cout << "Created " << triangle->GetName() << " with area: " << triangle->GetArea() << "\n";
	std::cout << "✓ Factory object creation works correctly\n\n";
}

void test_factory_unknown_type()
{
	std::cout << "=== Test 3: Factory Unknown Type Exception ===\n";
	Factory<Shape, int> factory;

	factory.Register(1, []() -> Shape * { return new Circle(5.0); });

	try
	{
		factory.CreateObject(999); // Unknown ID
		std::cout << "✗ Should have thrown exception\n";
		assert(false);
	}
	catch (const DefaultFactoryError<int, Shape>::Exception &e)
	{
		std::cout << "✓ Correctly threw exception for unknown type: " << e.what() << "\n";
		assert(e.GetId() == 999);
	}

	std::cout << "✓ Factory unknown type handling works correctly\n\n";
}

void test_factory_unregister()
{
	std::cout << "=== Test 4: Factory Unregistration ===\n";
	Factory<Shape, int> factory;

	factory.Register(1, []() -> Shape * { return new Circle(5.0); });
	factory.Register(2, []() -> Shape * { return new Rectangle(3.0, 4.0); });

	// Create object before unregistering
	std::unique_ptr<Shape> shape1(factory.CreateObject(1));
	assert(shape1 != nullptr);

	// Unregister
	bool result = factory.Unregister(1);
	assert(result == true);

	// Try to unregister again (should fail)
	bool result2 = factory.Unregister(1);
	assert(result2 == false);

	// Try to create unregistered type (should throw)
	try
	{
		factory.CreateObject(1);
		std::cout << "✗ Should have thrown exception\n";
		assert(false);
	}
	catch (const DefaultFactoryError<int, Shape>::Exception &e)
	{
		std::cout << "✓ Correctly threw exception after unregistration\n";
	}

	// Other registered type should still work
	std::unique_ptr<Shape> shape2(factory.CreateObject(2));
	assert(shape2 != nullptr);

	std::cout << "✓ Factory unregistration works correctly\n\n";
}

// Simple string wrapper to avoid libc++ __hash_memory linking issues
// System clang++ uses the system’s libc++ where __hash_memory is available.
// Homebrew’s LLVM may use a different libc++ build or configuration where it isn’t linked.
// Using a custom StringId wrapper with const char* avoids std::string hashing entirely.
struct StringId
{
	const char *str;
	StringId(const char *s) : str(s)
	{
	}
	bool operator==(const StringId &other) const
	{
		return str == other.str || (str && other.str && std::strcmp(str, other.str) == 0);
	}
};

namespace std
{
template <>
struct hash<StringId>
{
	size_t operator()(const StringId &sid) const noexcept
	{
		// Simple hash based on pointer address for const char*
		// This works because string literals have stable addresses
		return reinterpret_cast<size_t>(sid.str);
	}
};
} // namespace std

void test_factory_string_identifier()
{
	std::cout << "=== Test 5: Factory with String Identifier ===\n";
	Factory<Shape, StringId> factory;

	static const char *circle_id = "circle";
	static const char *rectangle_id = "rectangle";

	factory.Register(StringId(circle_id), []() -> Shape * { return new Circle(10.0); });
	factory.Register(StringId(rectangle_id), []() -> Shape * { return new Rectangle(5.0, 6.0); });

	std::unique_ptr<Shape> circle(factory.CreateObject(StringId(circle_id)));
	std::unique_ptr<Shape> rectangle(factory.CreateObject(StringId(rectangle_id)));

	assert(circle != nullptr);
	assert(rectangle != nullptr);
	assert(circle->GetArea() == 3.14159 * 10.0 * 10.0);
	assert(rectangle->GetArea() == 30.0);

	std::cout << "Created " << circle->GetName() << " with area: " << circle->GetArea() << "\n";
	std::cout << "Created " << rectangle->GetName() << " with area: " << rectangle->GetArea() << "\n";
	std::cout << "✓ Factory with string identifier works correctly\n\n";
}

// CloneFactory Tests
void test_clone_factory_register()
{
	std::cout << "=== Test 6: CloneFactory Registration ===\n";
	CloneFactory<Animal> cloneFactory;

	// Register clone creators
	bool result1 = cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	bool result2 = cloneFactory.Register(Loki::TypeInfo(typeid(Cat)), [](Animal *prototype) -> Animal * {
		if (auto *cat = dynamic_cast<Cat *>(prototype))
		{
			return new Cat(*cat);
		}
		return nullptr;
	});

	assert(result1 == true);
	assert(result2 == true);

	// Try to register same type again (should fail)
	bool result3 = cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *) -> Animal * { return new Dog(); });
	assert(result3 == false);

	std::cout << "✓ CloneFactory registration works correctly\n\n";
}

void test_clone_factory_clone()
{
	std::cout << "=== Test 7: CloneFactory Object Cloning ===\n";
	CloneFactory<Animal> cloneFactory;

	cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	cloneFactory.Register(Loki::TypeInfo(typeid(Cat)), [](Animal *prototype) -> Animal * {
		if (auto *cat = dynamic_cast<Cat *>(prototype))
		{
			return new Cat(*cat);
		}
		return nullptr;
	});

	// Create prototypes
	Dog originalDog("Max");
	Cat originalCat("Fluffy");

	// Clone objects
	std::unique_ptr<Animal> clonedDog(cloneFactory.CreateObject(&originalDog));
	std::unique_ptr<Animal> clonedCat(cloneFactory.CreateObject(&originalCat));

	assert(clonedDog != nullptr);
	assert(clonedCat != nullptr);

	// Verify cloned objects have correct type and values
	Dog *dogPtr = dynamic_cast<Dog *>(clonedDog.get());
	Cat *catPtr = dynamic_cast<Cat *>(clonedCat.get());

	assert(dogPtr != nullptr);
	assert(catPtr != nullptr);
	assert(std::string(dogPtr->GetName()) == "Max");
	assert(std::string(catPtr->GetName()) == "Fluffy");

	// Verify they are different objects
	assert(clonedDog.get() != &originalDog);
	assert(clonedCat.get() != &originalCat);

	std::cout << "Cloned Dog: " << dogPtr->GetName() << "\n";
	std::cout << "Cloned Cat: " << catPtr->GetName() << "\n";
	std::cout << "✓ CloneFactory object cloning works correctly\n\n";
}

void test_clone_factory_unknown_type()
{
	std::cout << "=== Test 8: CloneFactory Unknown Type Exception ===\n";
	CloneFactory<Animal> cloneFactory;

	cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	// Try to clone unregistered type
	Cat unregisteredCat("Test");
	try
	{
		cloneFactory.CreateObject(&unregisteredCat);
		std::cout << "✗ Should have thrown exception\n";
		assert(false);
	}
	catch (const DefaultFactoryError<Loki::TypeInfo, Animal>::Exception &e)
	{
		std::cout << "✓ Correctly threw exception for unknown type\n";
	}

	std::cout << "✓ CloneFactory unknown type handling works correctly\n\n";
}

void test_clone_factory_null_prototype()
{
	std::cout << "=== Test 9: CloneFactory Null Prototype ===\n";
	CloneFactory<Animal> cloneFactory;

	cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	// Try to clone with null prototype
	Animal *nullAnimal = nullptr;
	Animal *result = cloneFactory.CreateObject(nullAnimal);
	assert(result == nullptr);

	std::cout << "✓ CloneFactory correctly handles null prototype\n\n";
}

void test_clone_factory_unregister()
{
	std::cout << "=== Test 10: CloneFactory Unregistration ===\n";
	CloneFactory<Animal> cloneFactory;

	cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	cloneFactory.Register(Loki::TypeInfo(typeid(Cat)), [](Animal *prototype) -> Animal * {
		if (auto *cat = dynamic_cast<Cat *>(prototype))
		{
			return new Cat(*cat);
		}
		return nullptr;
	});

	// Clone before unregistering
	Dog dog("Buddy");
	std::unique_ptr<Animal> cloned1(cloneFactory.CreateObject(&dog));
	assert(cloned1 != nullptr);

	// Unregister
	bool result = cloneFactory.Unregister(Loki::TypeInfo(typeid(Dog)));
	assert(result == true);

	// Try to unregister again (should fail)
	bool result2 = cloneFactory.Unregister(Loki::TypeInfo(typeid(Dog)));
	assert(result2 == false);

	// Try to clone unregistered type (should throw)
	try
	{
		cloneFactory.CreateObject(&dog);
		std::cout << "✗ Should have thrown exception\n";
		assert(false);
	}
	catch (const DefaultFactoryError<Loki::TypeInfo, Animal>::Exception &e)
	{
		std::cout << "✓ Correctly threw exception after unregistration\n";
	}

	// Other registered type should still work
	Cat cat("Whiskers");
	std::unique_ptr<Animal> cloned2(cloneFactory.CreateObject(&cat));
	assert(cloned2 != nullptr);

	std::cout << "✓ CloneFactory unregistration works correctly\n\n";
}

void test_clone_factory_independence()
{
	std::cout << "=== Test 11: CloneFactory Object Independence ===\n";
	CloneFactory<Animal> cloneFactory;

	cloneFactory.Register(Loki::TypeInfo(typeid(Dog)), [](Animal *prototype) -> Animal * {
		if (auto *dog = dynamic_cast<Dog *>(prototype))
		{
			return new Dog(*dog);
		}
		return nullptr;
	});

	Dog originalDog("Rex");
	std::unique_ptr<Animal> clonedDog(cloneFactory.CreateObject(&originalDog));

	// Modify original
	originalDog.SetName("Modified");

	// Clone should be independent (not affected by original modification)
	Dog *dogPtr = dynamic_cast<Dog *>(clonedDog.get());
	assert(dogPtr != nullptr);
	assert(std::string(dogPtr->GetName()) == "Rex"); // Should still be original name

	std::cout << "Original dog name: " << originalDog.GetName() << "\n";
	std::cout << "Cloned dog name: " << dogPtr->GetName() << "\n";
	std::cout << "✓ Cloned objects are independent of originals\n\n";
}

int main()
{
	std::cout << "Running Factory and CloneFactory Test Suite\n";
	std::cout << "==========================================\n\n";

	// Factory tests
	test_factory_register();
	test_factory_create();
	test_factory_unknown_type();
	test_factory_unregister();
	test_factory_string_identifier();

	// CloneFactory tests
	test_clone_factory_register();
	test_clone_factory_clone();
	test_clone_factory_unknown_type();
	test_clone_factory_null_prototype();
	test_clone_factory_unregister();
	test_clone_factory_independence();

	std::cout << "==========================================\n";
	std::cout << "All tests passed! ✓\n";

	return 0;
}
