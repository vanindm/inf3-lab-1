#include "SmartPointers/ShrdPtr.h"
#include <string>
#include <cassert>

int main() {
	SmartPointers::UnqPtr<std::string> stringTest(new std::string("abcd"));
	SmartPointers::ShrdPtr<std::string> sharedTest(stringTest);
	assert(sharedTest->at(0) == 'a');
	assert(stringTest->at(1) == 'b');
	sharedTest->at(1) = 'C';
	assert(stringTest->at(1) == 'C');
	return 0;
}