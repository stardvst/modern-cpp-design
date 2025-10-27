#include "chunk.h"

void printChunkState(const Chunk &chunk, std::size_t blockSize)
{
	std::cout << "Chunk state:\n";
	std::cout << " firstAvailableBlock: " << static_cast<int>(chunk.m_firstAvailableBlock) << "\n";
	std::cout << " blocksAvailable: " << static_cast<int>(chunk.m_blocksAvailable) << "\n";
	std::cout << " Blocks:\n";

	if (!chunk.m_data)
	{
		std::cout << " (no data - chunk released)\n";
		std::cout << "-------------------------\n";
		return;
	}

	unsigned char *p = chunk.m_data;
	for (unsigned char i = 0; i != chunk.m_blocksAvailable + (chunk.m_firstAvailableBlock == 0 ? 0 : 1); ++i)
	{
		for (std::size_t j = 0; j < blockSize; ++j)
		{
			if (j == 0)
			{
				std::cout << "[" << static_cast<int>(*p) << "] ";
			}
			else
			{
				std::cout << "[] ";
			}
			++p;
		}
		std::cout << "\n";
	}

	std::cout << "-------------------------\n";
}

void Chunk::Init(std::size_t blockSize, unsigned char blocks)
{
	assert(blockSize > 0);
	assert(blocks > 0);
	assert((blockSize * blocks) / blockSize == blocks); // overflow check

	m_data = new unsigned char[blockSize * blocks];
	m_firstAvailableBlock = 0;
	m_blocksAvailable = blocks;

	unsigned char *p = m_data;
	for (unsigned char i = 0; i != blocks; ++i)
	{
		*p = i + 1;
		p += blockSize;
	}
}

void *Chunk::Allocate(std::size_t blockSize)
{
	assert(m_blocksAvailable > 0);

	unsigned char *result = m_data + (m_firstAvailableBlock * blockSize);
	m_firstAvailableBlock = *result;
	--m_blocksAvailable;

	return result;
}

void Chunk::Deallocate(void *p, std::size_t blockSize)
{
	assert(p >= m_data);

	unsigned char *block = static_cast<unsigned char *>(p);

	// alignment check
	assert((block - m_data) % blockSize == 0);

	*block = m_firstAvailableBlock;
	m_firstAvailableBlock = static_cast<unsigned char>((block - m_data) / blockSize);

	// truncation check
	assert(m_firstAvailableBlock == (block - m_data) / blockSize);

	++m_blocksAvailable;
}

void Chunk::Release()
{
	std::exchange(m_data, nullptr);
}
