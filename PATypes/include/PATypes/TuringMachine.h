#pragma once

#include <optional>

#include "LazySequence.h"

namespace PATypes {
	template<class A, size_t n>
	class MultiStripTuringMachine {
		LazySequence<A> strip[n];
		int pointer[n];
		bool accepted = false;
		void (*d)(MultiStripTuringMachine<A, n>*);
	public:
		MultiStripTuringMachine(Sequence<LazySequence<A>>* initial, void (*d)(MultiStripTuringMachine<A, n>*)) : d(d) {
			for (size_t i = 0; i < n; ++i) {
				LazySequence<A> current = initial->get(i);
				strip[i] = current;
				pointer[i] = 0;
			}
		}
		void step() {
			d(this);
		}
		bool hasFinished() const {
			for (size_t i = 0; i < n; ++i) {
				try {
					strip[i].get(pointer[i]);
				} catch (std::out_of_range& err) {
					return true;
				}
				return false;
			}
		}
		LazySequence<A>* getStrip(size_t i) {
			if (i < n) {
				return new LazySequence<A>(strip[i]);
			} else {
				throw std::logic_error("attempt to return nonexisting strip");
			}
		}
	};
}