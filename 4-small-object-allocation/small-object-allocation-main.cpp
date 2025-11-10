#include "chunk.h"
#include "fixed-allocator.h"
#include "small-object.h"

namespace Color
{
const char *red = "\033[31m";
const char *green = "\033[32m";
const char *yellow = "\033[33m";
const char *blue = "\033[34m";
const char *magenta = "\033[35m";
const char *cyan = "\033[36m";
const char *reset = "\033[0m";
} // namespace Color

int main()
{
	// #############  Chunk test  #############
	std::cout << Color::red << "Chunk test:\n" << Color::reset;

	Chunk chunk;

	// [1] [] [] [] # next free block index is 1
	// [2] [] [] [] # next free block index is 2
	// [3] [] [] [] # next free block index is 3 (end marker)
	chunk.Init(4, 3); // block size 4 bytes, 3 blocks
	printChunkState(chunk, 4);

	void *p1 = chunk.Allocate(4);
	printChunkState(chunk, 4);

	void *p2 = chunk.Allocate(4);
	printChunkState(chunk, 4);

	void *p3 = chunk.Allocate(4);
	printChunkState(chunk, 4);

	// will assert: no blocks available
	// void *p4 = chunk.Allocate(4);

	chunk.Deallocate(p2, 4);
	printChunkState(chunk, 4);

	void *p5 = chunk.Allocate(4); // should reuse p2
	printChunkState(chunk, 4);

	chunk.Release();
	printChunkState(chunk, 4);

	// #############  FixedAllocator test  #############
	std::cout << Color::green << "\nFixedAllocator test:\n" << Color::reset;

	FixedAllocator allocator(4);
	printFixedAllocatorState(allocator);

	// Allocate several blocks
	void *a1 = allocator.Allocate();
	printFixedAllocatorState(allocator);

	void *a2 = allocator.Allocate();
	printFixedAllocatorState(allocator);

	void *a3 = allocator.Allocate();
	printFixedAllocatorState(allocator);

	void *a4 = allocator.Allocate();
	printFixedAllocatorState(allocator);

	// will allocate a new chunk internally
	std::cout << Color::green << "\nAllocating a new chunk:\n" << Color::reset;
	void *a5 = allocator.Allocate();
	printFixedAllocatorState(allocator);

	// Deallocate some blocks
	std::cout << Color::green << "Deallocating one block:\n" << Color::reset;
	allocator.Deallocate(a2);
	printFixedAllocatorState(allocator);

	std::cout << Color::green << "Deallocating one more block:\n" << Color::reset;
	allocator.Deallocate(a4);
	printFixedAllocatorState(allocator);

	// #############  SmallObject test  #############
	std::cout << Color::blue << "\nSmallObject test:\n" << Color::reset;
	struct SmallDerived : public SmallObject
	{
		int x;
		double y;
	};

	SmallDerived *obj1 = new SmallDerived();
	SmallDerived *obj2 = new SmallDerived();
	std::cout << "Allocated SmallDerived objects at: " << obj1 << " and " << obj2 << "\n";
	delete obj1;
	delete obj2;

	return 0;
}
