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
		Cardinal(bool inf) : a(0), infinite(inf) {}
	public:
		Cardinal(size_t a) : a(a), infinite(false) {}
		Cardinal(const Cardinal& other) : a(other.a), infinite(other.a) {}
		Cardinal() : a(0), infinite(true) {}
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
			int index;
			T val;
		public:
			LazyStorage(int index, T val) : index(index), val(val) {}
			int getIndex() { return index; }
			T getValue() {return val;}
			int operator<(const LazyStorage& b) const {
				return index < b.index;
			}
			int operator<=(const LazyStorage& b) const {
				return index <= b.index;
			}
			int operator==(const LazyStorage& b) const {
				return index == b.index;
			}
		};
		T (*rule)(Sequence<T>*) = nullptr;
		// На самом деле словарь (map)
		Set<LazyStorage> storage;

		bool IsInfinite = false;
		size_t stride;
		int rightmost_index;
		int leftmost_index;
		void _clearStorage(int index) {
			std::function<bool(T)> clearFunction = [index](const T& a){ return a.index < index;};
			storage(clearFunction, storage);
		}
		bool IsCalculated(int index) {
			return storage.contains({index, T()});
		}
		T Calculate(int index) {
			if (IsCalculated(index)) {
				return storage.getByItem({index, 0}).getValue();
			} else {
				if (!IsInfinite && (index < leftmost_index || index > rightmost_index)) {
					throw std::out_of_range("outside of finite sequence");
				}
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
		LazySequence() : storage(), leftmost_index(0), rightmost_index(0) {
		}

		LazySequence(T* items, int count) : storage(), leftmost_index(0), rightmost_index(count - 1) {
			for (int i = 0; i < count; ++i) {
				storage.insert({i, items[i]});
			}
		}

		LazySequence(Sequence<T>* seq) : rightmost_index(seq->getLength()), leftmost_index(0) {
			auto *enumerator = seq->getEnumerator();
			int i = 0;
			while(enumerator->moveNext()) {
				storage.insert({i, enumerator->current()});
				++i;
			}
			delete enumerator;
		}

		LazySequence(T(*rule)(Sequence<T>*), Sequence<T>* initial, size_t count) : rule(rule), stride(count), leftmost_index(0), rightmost_index(0), IsInfinite(true) {
			auto *enumerator = initial->getEnumerator();
			int i = 0;
			while(enumerator->moveNext()) {
				storage.insert({i, enumerator->current()});
				++i;
			}
			delete enumerator;
		}
		LazySequence(const LazySequence<T>& copy) : storage(copy.storage), IsInfinite(copy.IsInfinite), rule(copy.rule), stride(copy.stride), leftmost_index(copy.leftmost_index), rightmost_index(copy.rightmost_index) {
		}
		T GetFirst() {
			return storage.getByItem({leftmost_index, 0}).getValue();
		}
		T GetLast() {
			if (GetLength().Get() == std::nullopt) {
				throw std::out_of_range("attempt to get last element of infinite sequence");
			} else {
				return storage.getByItem({rightmost_index, 0}).getValue();
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
				return Cardinal(rightmost_index - leftmost_index + 1);
			}
		}
		size_t GetMaterializedCount() const {
			return storage.GetLength();
		}

		LazySequence<T>* Append(T item) {
			++rightmost_index;
			storage.insert({rightmost_index, item});
		}
		LazySequence<T>* Prepend(T item) {
			--leftmost_index;
			storage.insert({leftmost_index, item});
		}
		LazySequence<T>* InsertAt(T item, int index) {
			_clearStorage(index);
			storage.insert({index, item});
		}
		LazySequence<T>* Concat(LazySequence<T> &list) {
			LazySequence<T>* newSequence;
			if (this->IsInfinite)
				newSequence = new LazySequence<T>(*this);
			else {
				newSequence = new LazySequence<T>(*this);
				newSequence->rule = list.rule;
				newSequence->stride = list.stride;
				newSequence->IsInfinite = list.IsInfinite;
				if (list.IsInfinite) {
					for (int i = list.leftmost_index; i < list.leftmost_index + list.stride; ++i) {
						newSequence->Append(list.Get(i));
					}
				} else {
					for (int i = list.leftmost_index; i < list.rightmost_index; ++i) {
						newSequence->Append(list.Get(i));
					}

				}
			}
			return newSequence;
		}

		template<class T2>
		LazySequence<T2>* Map(T2 (*)(T));

		template<class T2>
		T2 Reduce(T2 (*)(T2, T));

		LazySequence<T>* Where(bool (*)(T));

		LazySequence<T>* Zip(Sequence<T>* seq);
	};
}