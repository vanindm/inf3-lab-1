#pragma once

#include <functional>
#include <optional>
#include <exception>

#include "PATypes/Sequence.h"

namespace PATypes {
	class Cardinal {
		size_t a;
		bool infinite;
		Cardinal(bool inf) : infinite(inf), a(0) {}
	public:
		Cardinal(size_t a) : a(a), infinite(false) {}
		Cardinal(const Cardinal& other) : a(other.a), infinite(other.a) {}
		Cardinal() : infinite(true), a(0) {}
		std::optional<size_t> Get() {
			if (infinite)
				return std::nullopt;
			else
				return a;
		}
		static Cardinal Infinity() { return Cardinal(); }
		bool operator==(const Cardinal& other) const {
			if (infinite && other.infinite) return true;
			if (infinite || other.infinite) return false;
			return a == other.a;
		}

		bool operator<(const Cardinal& other) const {
			if (infinite) return false;
			if (other.infinite) return true;
			return a < other.a;
		}
	};

	static Cardinal Infinity = Cardinal();

	template<class T>
	class LazySequence {
		T(*rule) = nullptr;
		MutableArraySequence<std::optional<T>> storage;
	public:
		LazySequence() : storage() {
		}

		LazySequence(T* items, int count) : storage() {
			for (size_t i = 0; i < count; ++i) {
				storage.append(items[i]);
			}
		}

		LazySequence(Sequence<T>* seq) {
			auto *enumerator = seq->getEnumerator();
			while(enumerator->GetNext()) {
				storage.append(items[i]);
			}
			delete enumerator;
		}

		LazySequence(T(*rule)(Sequence<T>*), Sequence<T>* initial) : rule(rule) {
			auto *enumerator = initial->getEnumerator();
			while(enumerator->GetNext()) {
				storage.append(items[i]);
			}
			delete enumerator;
		}
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

		template<class T2>
		LazySequence<T2>* Map(T2 (*)(T));

		template<class T2>
		T2 Reduce(T2 (*)(T2, T));

		LazySequence<T>* Where(bool (*)(T));

		LazySequence<T>* Zip(Sequence<T>* seq);
	};
}