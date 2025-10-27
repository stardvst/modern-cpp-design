#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>

struct Chunk
{
	void Init(std::size_t blockSize, unsigned char blocks);
	void *Allocate(std::size_t blockSize);
	void Deallocate(void *p, std::size_t blockSize);
	void Release();

	unsigned char *m_data{nullptr};
	unsigned char m_firstAvailableBlock{0};
	unsigned char m_blocksAvailable{0};
};

void printChunkState(const Chunk &chunk, std::size_t blockSize);
