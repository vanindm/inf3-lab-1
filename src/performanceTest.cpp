#include <iostream>
#include <fstream>
#include <chrono>
#include <cassert>
#include <unistd.h>

#include "SmartPointers/SmartPtr.h"
#include "SmartPointers/ShrdPtr.h"
#include "SmartPointers/MemorySpan.h"

#include <DynamicArray.h>

void process_mem_usage(size_t& resident_set)
{
    //vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE); // in case x86-64 is configured to use 2MB pages
    //vm_usage = vsize / 1024.0;
    resident_set = rss ;//* page_size_kb;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		std::fprintf(stderr, "Wrong number of arguments!");
		return 1;
	}

	size_t AMOUNT = atoi(argv[1]);

	char testPath[] = "../contrib/testdata.input"; 

	std::ifstream inputDataStream;

	inputDataStream.open(testPath);

	if (inputDataStream.fail()) {
		throw std::runtime_error("couldn't open file");
	}

	int* items = new int[AMOUNT];
	for (size_t i = 0; i < AMOUNT; ++i) {
		inputDataStream >> items[i];
	}


	std::chrono::_V2::steady_clock::time_point start;
	std::chrono::_V2::steady_clock::time_point end;

//-------------------------------------------------------------------

	size_t rss_before;
	process_mem_usage(rss_before);

	start = std::chrono::steady_clock::now();
	
	SmartPtrDynamicArray<int> array(AMOUNT, items);
	
	end = std::chrono::steady_clock::now();
	size_t rss_after;
	process_mem_usage(rss_after);
	
	std::chrono::duration<double> elapsed = end - start;
	
	//std::cout << "SmartPtr DynamicArray Allocation\n";
	std::cout << elapsed.count() << " : " << (rss_after - rss_before) << "\n";

//-------------------------------------------------------------------

	process_mem_usage(rss_before);

	start = std::chrono::steady_clock::now();
	
	UnqPtrDynamicArray<int> unqArray(AMOUNT, items);
	
	end = std::chrono::steady_clock::now();
	process_mem_usage(rss_after);
	elapsed = end - start;
	
	//std::cout << "UnqPtr DynamicArray Allocation\n";
	std::cout << elapsed.count() << " : " << (rss_after - rss_before) << "\n";


//-------------------------------------------------------------------

	process_mem_usage(rss_before);

	start = std::chrono::steady_clock::now();

	MemorySpanDynamicArray<int> arrayMemorySpan(AMOUNT, items);

	end = std::chrono::steady_clock::now();

	process_mem_usage(rss_after);
	
	elapsed = end - start;

	//std::cout << "MemorySpan DynamicArray Allocation\n";
	std::cout << elapsed.count() << " : " << (rss_after - rss_before) << "\n";

//-------------------------------------------------------------------
	delete[] items;
	return 0;
}