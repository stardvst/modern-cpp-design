#pragma once

#include "../6-singletons/8-singleton-holder/singleton-holder.h" // for SingletonHolder
#include "../6-singletons/8-singleton-holder/threading-models.h" // for ThreadingModel
#include "small-object-allocator.h"

#include <cstddef> // for std::size_t
#include <new>	   // for ::operator new, ::operator delete

using SmallObjectAllocatorType = SingletonHolder<SmallObjectAllocator>;

template <template <class> class ThreadingModel>
class SmallObject : public ThreadingModel<SmallObject<ThreadingModel>>
{
public:
	SmallObject() = default;
	virtual ~SmallObject() = default;

	static void *operator new(std::size_t size);
	static void operator delete(void *p, std::size_t size);
};

template <template <class> class ThreadingModel>
inline void *SmallObject<ThreadingModel>::operator new(std::size_t size)
{
	typename ThreadingModel<SmallObject<ThreadingModel>>::Lock guard;
	return SmallObjectAllocatorType::getInstance().Allocate(size);
}

template <template <class> class ThreadingModel>
inline void SmallObject<ThreadingModel>::operator delete(void *p, std::size_t size)
{
	typename ThreadingModel<SmallObject<ThreadingModel>>::Lock guard;
	SmallObjectAllocatorType::getInstance().Deallocate(p, size);
}
