#include <string>
#include <cassert>

#include "SmartPointers/MemorySpan.h"

int main() {
	SmartPointers::MemorySpan<int> intArray(11);
	*(intArray.Locate(10)) = 10;
	*(intArray.Locate(9)) = 1337;
	assert(*(intArray.Locate(10)) == 10);
	auto ptr = intArray.Locate(10);
	ptr = ptr - 1;
	assert(*(ptr) == 1337);
	return 0;
}