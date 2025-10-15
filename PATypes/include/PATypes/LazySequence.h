#pragma once

#include <functional>
#include <optional>
#include <exception>

#include "PATypes/Set.h"
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
		class LazyStorage {
			size_t index;
			T val;
		public:
			LazyStorage(size_t index, T val) : index(index), val(val) {}
			size_t getIndex() { return index; }
			T getValue() {return val;}
			int operator<(const LazyStorage& b){
				return index < b.index;
			}
			int operator<=(const LazyStorage& b){
				return index <= b.index;
			}
			int operator==(const LazyStorage& b){
				return index == b.index;
			}
		};
		T (*rule)(Sequence<T>*) = nullptr;
		// На самом деле словарь (map)
		Set<LazyStorage> storage;
		bool IsInfinite = false;
		size_t stride;
		bool IsCalculated(size_t index) {
			return storage.contains({index, T()});
		}
		T Calculate(size_t index) {
			if (IsCalculated(index)) {
				return storage.getByItem({index, 0}).getValue();
			} else {
				MutableListSequence<T> toCalculate;
				for (size_t i = 0; i < stride; ++i) {
					toCalculate.append(Calculate(index - i - 1));
				}
				T newValue = rule(&toCalculate);
				storage.insert({index, newValue});
				return newValue;
			}
		}
	public:
		LazySequence() : storage() {
		}

		LazySequence(T* items, int count) : storage() {
			for (size_t i = 0; i < count; ++i) {
				storage.insert({i, items[i]});
			}
		}

		LazySequence(Sequence<T>* seq) {
			auto *enumerator = seq->getEnumerator();
			size_t i = 0;
			while(enumerator->moveNext()) {
				storage.insert({i, enumerator->current()});
				++i;
			}
			delete enumerator;
		}

		LazySequence(T(*rule)(Sequence<T>*), Sequence<T>* initial, size_t count) : rule(rule), stride(count) {
			auto *enumerator = initial->getEnumerator();
			size_t i = 0;
			while(enumerator->moveNext()) {
				storage.insert({i, enumerator->current()});
				++i;
			}
			delete enumerator;
		}
		LazySequence(const LazySequence<T>& list);
		T GetFirst() {
			return storage.getByItem({0, 0}).getValue();
		}
		T GetLast() {
			if (GetLength().Get() == std::nullopt) {
				throw std::out_of_range("attempt to get last element of infinite sequence");
			} else {
				return storage.getByItem({GetLength().Get() - 1, 0}).getValue();
			}
		}
		T Get(int index) {
			return Calculate(index);
		}
		LazySequence<T>* GetSubSequence(int startindex, int endindex);
		Cardinal GetLength() const {
			if (IsInfinite) {
				return Cardinal();
			} else {
				// GET LENGTH TODO
			}
		}
		size_t GetMaterializedCount() const {
			return storage.GetLength();
		}

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