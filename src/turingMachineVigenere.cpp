#include <cassert>
#include <iostream>
#include <memory>

#include "PATypes/Sequence.h"
#include "PATypes/TuringMachine.h"

inline int mod(int i, int n) {
	return (i % n + n) % n;
}

class VigenereEncryptTuringMachine : public PATypes::MultiStripTuringMachine<char, 4> {
    void d() {
		while (strip[2]->Get(pointer[2]) < strip[0]->Get(pointer[0])) {
			++pointer[2];
		}
		while (strip[2]->Get(pointer[2]) > strip[0]->Get(pointer[0])) {
			--pointer[2];
		}

		while (strip[3]->Get(pointer[3]) < strip[1]->Get(pointer[1])) {
			pointer[2] = (pointer[2] + 1) % (int)(strip[2]->GetLength().Get().value());
			++pointer[3];
		}

		auto newStrip = strip[0]->InsertAt(strip[2]->Get(pointer[2]), pointer[0]);

		delete strip[0];

		strip[0] = newStrip;
		pointer[0] += 1;
		pointer[1] = (pointer[1] + 1) % (int)(strip[1]->GetLength().Get().value());
		pointer[2] = 0;
		pointer[3] = 0;
    }

  public:
    VigenereEncryptTuringMachine(PATypes::Sequence<PATypes::LazySequence<char>> *initial)
        : PATypes::MultiStripTuringMachine<char, 4>(initial) {}
};

class VigenereDecryptTuringMachine : public PATypes::MultiStripTuringMachine<char, 4> {
    void d() {
		while (strip[2]->Get(pointer[2]) < strip[0]->Get(pointer[0])) {
			++pointer[2];
		}
		while (strip[2]->Get(pointer[2]) > strip[0]->Get(pointer[0])) {
			--pointer[2];
		}

		while (strip[3]->Get(pointer[3]) < strip[1]->Get(pointer[1])) {
			pointer[2] = mod((pointer[2] - 1), (int)(strip[2]->GetLength().Get().value()));
			pointer[3] += 1;
		}

		auto newStrip = strip[0]->InsertAt(strip[2]->Get(pointer[2]), pointer[0]);

		delete strip[0];

		strip[0] = newStrip;
		pointer[0] -= 1;
		pointer[1] = mod((pointer[1] - 1), (int)(strip[1]->GetLength().Get().value()));
		pointer[2] = 0;
		pointer[3] = 0;
    }

  public:
    VigenereDecryptTuringMachine(PATypes::Sequence<PATypes::LazySequence<char>> *initial)
        : PATypes::MultiStripTuringMachine<char, 4>(initial) {
			pointer[0] = initial->get(0).GetLength().Get().value() - 1;
			pointer[1] = mod((initial->get(0).GetLength().Get().value() - 1), initial->get(1).GetLength().Get().value());
		}
};

int main() {
    char plaintext[] = "TESTMESSAGE";
	char key[] = "KEY";
	char alph[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char buff[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    PATypes::MutableArraySequence<PATypes::LazySequence<char>> encryptStrips;
	encryptStrips.append(PATypes::LazySequence<char>(plaintext, 11));
	encryptStrips.append(PATypes::LazySequence<char>(key, 3));
	encryptStrips.append(PATypes::LazySequence<char>(alph, 26));
	encryptStrips.append(PATypes::LazySequence<char>(buff, 26));
    VigenereEncryptTuringMachine encryptMachine(&encryptStrips);
    while (encryptMachine.step()) {
    }
	PATypes::LazySequence<char> *obtained = encryptMachine.getStrip(0);
	for (int i = obtained->GetLeftmostIndex(); i <= obtained->GetRightmostIndex(); ++i) {
		std::cout << obtained->Get(i) << " ";
	}
	std::cout << "\n";
	delete obtained;
	PATypes::MutableArraySequence<PATypes::LazySequence<char>> decryptStrips;
	decryptStrips.append(*std::unique_ptr<PATypes::LazySequence<char>>(encryptMachine.getStrip(0)));
	decryptStrips.append(*std::unique_ptr<PATypes::LazySequence<char>>(encryptMachine.getStrip(1)));
	decryptStrips.append(*std::unique_ptr<PATypes::LazySequence<char>>(encryptMachine.getStrip(2)));
	decryptStrips.append(*std::unique_ptr<PATypes::LazySequence<char>>(encryptMachine.getStrip(3)));
	VigenereDecryptTuringMachine decryptMachine(&decryptStrips);
	while (decryptMachine.step()) {
    }
	obtained = decryptMachine.getStrip(0);
	for (int i = obtained->GetLeftmostIndex(); i <= obtained->GetRightmostIndex(); ++i) {
		std::cout << obtained->Get(i) << " ";
	}
	delete obtained;
    return 0;
}