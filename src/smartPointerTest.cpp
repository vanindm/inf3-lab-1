#include "SmartPointers/SmartPtr.h"
#include <cassert>
#include <string>

int main() {
	SmartPointers::SmartPtr<std::string> strTest(new std::string("abcd"));
	{
		SmartPointers::SmartPtr<std::string> inheritanceTest(strTest);
		assert(inheritanceTest == strTest);
		assert(inheritanceTest.getCount() == 2);
		SmartPointers::SmartPtr<std::string> assignTest = inheritanceTest;
		assert(assignTest.getCount() == 3);
	}
	assert(strTest.getCount() == 1);
	return 0;
}