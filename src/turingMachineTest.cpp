#include <cassert>
#include <iostream>

#include "PATypes/TuringMachine.h"

void d(PATypes::MultiStripTuringMachine<bool, 1UL> *self) {}

int main() {
    bool items[] = {0, 1, 1, 0};
    auto temp = new PATypes::LazySequence<bool>(items, 4);
    PATypes::MutableArraySequence<PATypes::LazySequence<bool>> testStrips(temp,
                                                                          1);
    PATypes::MultiStripTuringMachine<bool, 1UL> test(&testStrips, d);
    delete temp;
    return 0;
}