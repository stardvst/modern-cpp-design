#include "fixed-allocator.h"

constexpr int TOTAL_CHUNK_LENGTH = 12;

void printFixedAllocatorState(const FixedAllocator &allocator)
{
	std::cout << "Block size: " << allocator.m_blockSize << "\n";
	std::cout << "Number of blocks: " << static_cast<int>(allocator.m_numBlocks) << "\n";
	std::cout << "Number of chunks: " << allocator.m_chunks.size() << "\n";

	for (std::size_t i = 0; i < allocator.m_chunks.size(); ++i)
	{
		std::cout << "Chunk " << i << ":\n";
		printChunkState(allocator.m_chunks[i], allocator.m_blockSize);
	}
}

FixedAllocator::FixedAllocator(std::size_t blockSize) : m_blockSize(blockSize)
{
	assert(m_blockSize > 0);

	std::size_t numBlocks = TOTAL_CHUNK_LENGTH / m_blockSize;
	if (numBlocks > UCHAR_MAX)
		numBlocks = UCHAR_MAX;
	else if (numBlocks == 0)
		numBlocks = 8 * m_blockSize;

	m_numBlocks = static_cast<unsigned char>(numBlocks);
	assert(m_numBlocks == numBlocks);
}

void *FixedAllocator::Allocate()
{
	if (!m_allocChunk || m_allocChunk->m_blocksAvailable == 0)
	{
		auto it = m_chunks.begin();
		while (true)
		{
			if (it == m_chunks.end())
			{
				// all chunks are full; create a new one
				Chunk newChunk;
				newChunk.Init(m_blockSize, m_numBlocks);

				m_chunks.reserve(m_chunks.size() + 1);
				m_chunks.push_back(std::move(newChunk));
				m_allocChunk = &m_chunks.back();
				m_deallocChunk = &m_chunks.back();
				break;
			}
			if (it->m_blocksAvailable > 0)
			{
				// foud a chunk with available blocks
				m_allocChunk = &(*it);
				break;
			}
			++it;
		}
	}

	assert(m_allocChunk);
	assert(m_allocChunk->m_blocksAvailable > 0);

	return m_allocChunk->Allocate(m_blockSize);
}

void FixedAllocator::Deallocate(void *p)
{
	assert(!m_chunks.empty());
	assert(&m_chunks.front() <= m_deallocChunk);
	assert(&m_chunks.back() >= m_deallocChunk);

	m_deallocChunk = VicinityFind(p);
	assert(m_deallocChunk);

	DoDeallocate(p);
}
Chunk *FixedAllocator::VicinityFind(void *p)
{
	assert(!m_chunks.empty());
	assert(m_deallocChunk);

	const std::size_t chunkLength = m_numBlocks * m_blockSize;

	Chunk *lo = m_deallocChunk;
	Chunk *hi = m_deallocChunk + 1;
	Chunk *loBound = &m_chunks.front();
	Chunk *hiBound = &m_chunks.back() + 1;

	// Special case: m_deallocChunk is the last in the array
	if (hi == hiBound)
		hi = nullptr;

  // the loop continues until we find the chunk containing p
  // or until all chunks have been checked
	while (true)
	{
		if (lo)
		{
      // is p in lo chunk?
			if (p >= lo->m_data && p < lo->m_data + chunkLength)
				return lo;

      // move lo pointer to previous chunk or null
			if (lo == loBound)
				lo = nullptr;
			else
				--lo;
		}

		if (hi)
		{
      // is p in hi chunk?
			if (p >= hi->m_data && p < hi->m_data + chunkLength)
				return hi;

      // move hi pointer to next chunk or null
			if (++hi == hiBound)
				hi = nullptr;
		}
	}
	assert(false);
	return nullptr;
}

void FixedAllocator::DoDeallocate(void *p)
{
	assert(m_deallocChunk->m_data <= p);
	assert(m_deallocChunk->m_data + m_numBlocks * m_blockSize > p);

	// call into the chunk, will adjust the inner list but won't release memory
	m_deallocChunk->Deallocate(p, m_blockSize);

	if (m_deallocChunk->m_blocksAvailable == m_numBlocks)
	{
		// m_deallocChunk is completely free, should we release it?

		Chunk &lastChunk = m_chunks.back();

		if (&lastChunk == m_deallocChunk)
		{
			// check if we have two last chunks empty
			if (m_chunks.size() > 1 && m_deallocChunk[-1].m_blocksAvailable == m_numBlocks)
			{
				// Two free chunks, discard the last one
				lastChunk.Release();
				m_chunks.pop_back();
				m_allocChunk = m_deallocChunk = &m_chunks.front();
			}
			return;
		}

		if (lastChunk.m_blocksAvailable == m_numBlocks)
		{
			// Two free blocks, discard one
			lastChunk.Release();
			m_chunks.pop_back();
			m_allocChunk = m_deallocChunk;
		}
		else
		{
			// move the empty chunk to the end
			std::swap(*m_deallocChunk, lastChunk);
			m_allocChunk = &m_chunks.back();
		}
	}
}

FixedAllocator::~FixedAllocator()
{
	for (auto &chunk : m_chunks)
		chunk.Release();
}


std::size_t FixedAllocator::GetBlockSize() const
{
	return m_blockSize;
}
