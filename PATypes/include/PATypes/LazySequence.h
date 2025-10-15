#pragma once

#include <functional>
#include <optional>
#include <exception>

#include "PATypes/Sequence.h"

namespace PATypes {
	class Cardinal {
		size_t a;
		bool infinite;
	public:
		Cardinal(size_t a) : a(a), infinite(false) {}
		Cardinal() : infinite(true) {}
		std::optional<size_t> Get() {
			if (infinite)
				return std::nullopt;
			else
				return a;
		}
	};

	static Cardinal Infinity = Cardinal();

	template<class T, class T2 = T>
	class LazySequence {
		class Generator {
			LazySequence<T>* parent;
			std::function<T>(Sequence<T>*) rule;
		public:
			Generator(LazySequence<T>* parent, std::function<T>(Sequence<T>*) rule) : rule(rule), parent(parent) {}
			Generator(LazySequence<T>* parent, size_t index, Sequence<T>* rule) : parent(parent), rule(rule) {}
			bool HasNext() const {
				try {
					Cardinal length = parent->GetLength();
					if (length.Get() == std::nullopt) {
						return false;
					} else {
						try {

						} catch (std::exception& e) {

						}
					}
				}
			}
			T GetNext() {
				if (hasNext()) {
					return rule();
				} else {
					throw std::out_of_range("Generator exhausted");
				}
			}
		};
		Generator& currentGenerator;
		ArraySequence<std::optional<T>> storage;
	public:
		LazySequence();
		LazySequence(T* items, int count) : currentGenerator(this, [](){}), storage() {

		}
		LazySequence(Sequence<T>* seq);
		LazySequence(T(*rule)(Sequence<T>*), Sequence<T>* initial);
		LazySequence(const LazySequence<T>& list);
		T GetFirst();
		T GetLast();
		T Get(int index);
		LazySequence<T>* GetSubSequence(int startindex, int endindex);
		Cardinal GetLength() const;
		size_t GetMaterializedCount() const;

		LazySequence<T>* Append(T item);
		LazySequence<T>* Prepend(T item);
		LazySequence<T>* InsertAt(T item, int index);
		LazySequence<T>* Concat(LazySequence<T> *list);
		LazySequence<T2>* Map(T2 (*)(T));
		T2 Reduce(T2 (*)(T2, T));
		LazySequence<T>* Where(bool (*)(T));
		LazySequence<T>* Zip(Sequence<T>* seq);
	};
}