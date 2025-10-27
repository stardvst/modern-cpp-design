#include "small-object-allocator.h"
#include "fixed-allocator.h"

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

SmallObjAllocator::SmallObjAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
	: m_chunkSize(chunkSize), m_maxObjectSize(maxObjectSize)
{
}

void *SmallObjAllocator::Allocate(std::size_t numBytes)
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
