#include <iostream>
#include <cassert>

#include "PATypes/LazySequence.h"

int fib(PATypes::Sequence<int>* seq) {
	return seq->get(0) + seq->get(1);
}

int main() {
	int arr[] = {1,2,3,4,5};
	PATypes::ImmutableArraySequence<int> seq(arr, 5);
	PATypes::LazySequence<int> a(&seq);
	assert(a.GetFirst() == 1);
	assert(a.Get(2) == 3);
	int arr2[] = {0,1};
	PATypes::ImmutableArraySequence<int> seq2(arr2, 2);

	// Fibonacci test
	PATypes::LazySequence<int> b(&fib, &seq2, 2);
	assert(b.Get(0) == 0);
	assert(b.Get(1) == 1);
	assert(b.Get(2) == 1);
	assert(b.Get(3) == 2);
	assert(b.Get(8) == 21);

	PATypes::LazySequence<int> *concat = a.Concat(b);
	assert(concat->Get(4) == 5);
	assert(concat->Get(5) == 0);
	assert(concat->Get(13) == 21);
	delete concat;
	return 0;
}