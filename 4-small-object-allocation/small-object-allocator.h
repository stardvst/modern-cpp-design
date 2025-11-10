#pragma once

#include <cstddef>
#include <vector>

class FixedAllocator;

// This class is capable of allocating objects of ANY SIZE efficiently.
// It does this by keeping a pool of fixed-size allocators that are
// used to allocate objects of a specific size.
class SmallObjectAllocator
{
public:
	SmallObjectAllocator() = default;
	explicit SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize);
	SmallObjectAllocator(const SmallObjectAllocator &) = delete;
	SmallObjectAllocator &operator=(const SmallObjectAllocator &) = delete;

	void *Allocate(std::size_t numBytes);
	void Deallocate(void *p, std::size_t numBytes);

private:
	using AllocatorPool = std::vector<FixedAllocator>;
	AllocatorPool m_pool;
	std::size_t m_chunkSize{0};
	std::size_t m_maxObjectSize{0};
	FixedAllocator *m_lastAllocator{nullptr};
	FixedAllocator *m_lastDeallocator{nullptr};
};
