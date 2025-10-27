#pragma once

#include "chunk.h"
#include <cstddef>
#include <vector>

class FixedAllocator
{
public:
	explicit FixedAllocator(std::size_t blockSize);
	~FixedAllocator();

	void *Allocate();
	void Deallocate(void *p);

	std::size_t GetBlockSize() const;

private:
	Chunk *VicinityFind(void *p);
	void DoDeallocate(void *p);

	std::size_t m_blockSize{0};
	unsigned char m_numBlocks{0};

	using Chunks = std::vector<Chunk>;
	Chunks m_chunks;

	Chunk *m_allocChunk{nullptr};
	Chunk *m_deallocChunk{nullptr};

	// for ensuring proper copy/move semantics
	// mutable const FixedAllocator *m_prev{nullptr};
	// mutable const FixedAllocator *m_next{nullptr};

	friend void printFixedAllocatorState(const FixedAllocator &allocator);
};
