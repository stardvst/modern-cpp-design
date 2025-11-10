#include "small-object-allocator.h"
#include "fixed-allocator.h"
#include <algorithm>

namespace
{

struct FixedAllocatorSizeComparator
{
	bool operator()(const FixedAllocator &allocator, std::size_t numBytes) const
	{
		return allocator.GetBlockSize() < numBytes;
	}
};

} // namespace

SmallObjectAllocator::SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
	: m_chunkSize(chunkSize), m_maxObjectSize(maxObjectSize)
{
}

void *SmallObjectAllocator::Allocate(std::size_t numBytes)
{
	// if the requested size is larger than maxObjectSize, use global new
	if (numBytes > m_maxObjectSize)
		return operator new(numBytes);

	// check if there is a previous allocator that can return the requested size
	if (m_lastAllocator && m_lastAllocator->GetBlockSize() == numBytes)
		return m_lastAllocator->Allocate();

	// find an allocator which has blocks larger than the requested size
	auto poolIt = std::lower_bound(m_pool.begin(), m_pool.end(), numBytes, FixedAllocatorSizeComparator{});
	if (poolIt == m_pool.end() || poolIt->GetBlockSize() != numBytes)
	{
		poolIt = m_pool.insert(poolIt, FixedAllocator{numBytes});
		m_lastDeallocator = &(*m_pool.begin());
	}
	m_lastAllocator = &(*poolIt);
	return m_lastAllocator->Allocate();
}

void SmallObjectAllocator::Deallocate(void *p, std::size_t numBytes)
{
	// if the size is larger than maxObjectSize, use global delete
	if (numBytes > m_maxObjectSize)
		return operator delete(p);

	// check if there is a previous deallocator that matches the size (fast path)
	if (m_lastDeallocator && m_lastDeallocator->GetBlockSize() == numBytes)
	{
		m_lastDeallocator->Deallocate(p);
		return;
	}

	// find the allocator that matches the size
	auto poolIt = std::lower_bound(m_pool.begin(), m_pool.end(), numBytes, FixedAllocatorSizeComparator{});
	assert(poolIt != m_pool.end());
	assert(poolIt->GetBlockSize() == numBytes);
	m_lastDeallocator = &(*poolIt);
	m_lastDeallocator->Deallocate(p);
	// if no matching allocator is found, it's an error (should not happen if allocation/deallocation are paired correctly)
}
