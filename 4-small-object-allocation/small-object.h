#pragma once

#include "../6-singletons/8-singleton-holder/singleton-holder.h" // for SingletonHolder
#include "small-object-allocator.h"

#include <cstddef> // for std::size_t
#include <new>	   // for ::operator new, ::operator delete

using SmallObjectAllocatorType = SingletonHolder<SmallObjectAllocator>;

class SmallObject
{
public:
	SmallObject() = default;
	virtual ~SmallObject() = default;

	static void *operator new(std::size_t size);
	static void operator delete(void *p, std::size_t size);
};

inline void *SmallObject::operator new(std::size_t size)
{
	return SmallObjectAllocatorType::getInstance().Allocate(size);
}

inline void SmallObject::operator delete(void *p, std::size_t size)
{
	SmallObjectAllocatorType::getInstance().Deallocate(p, size);
}
