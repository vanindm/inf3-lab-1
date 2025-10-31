#include <cassert>
#include <iostream>
#include <memory>

#include "PATypes/Sequence.h"
#include "PATypes/TuringMachine.h"

class MyTuringMachine : public PATypes::MultiStripTuringMachine<bool, 1UL> {
    void d() {
        if (strip[0]->Get(pointer[0]) == 0) {
            pointer[0] += 1;
        } else {
            auto newStrip = strip[0]->InsertAt(0, pointer[0]);
            delete strip[0];
            strip[0] = newStrip;
            pointer[0] += 1;
        }
    }

  public:
    MyTuringMachine(PATypes::Sequence<PATypes::LazySequence<bool>> *initial)
        : PATypes::MultiStripTuringMachine<bool, 1UL>(initial) {}
};

int main() {
    bool items[] = {0, 1, 1, 0};
    auto temp = std::unique_ptr<PATypes::LazySequence<bool>>(
        new PATypes::LazySequence<bool>(items, 4));
    PATypes::MutableArraySequence<PATypes::LazySequence<bool>> testStrips(
        temp.get(), 1);
    MyTuringMachine myMachine(&testStrips);
    while (myMachine.step()) {
    }
    PATypes::LazySequence<bool> *obtained = myMachine.getStrip(0);
    // PATypes::MultiStripTuringMachine<bool, 1UL> test(&testStrips);
    //  delete temp;
    for (int i = obtained->GetLeftmostIndex(); i <= obtained->GetRightmostIndex(); ++i) {
        std::cout << obtained->Get(i) << " ";
    }
    delete obtained;
    return 0;
}