#pragma once
#ifndef IF_ITERATOR_DEFINED
#define IF_ITERATOR_DEFINED


#include "ErrorMessage.h"


template<class Container>
struct IteratorTypeHelper {
	using type = typename decltype(std::declval<Container>().begin());
};

template<class Container>
struct ValueTypeHelper {
	using type = typename Container::value_type;
};

template<class Container>
struct ValueTypeHelper<const Container> {
	using type = typename const Container::value_type;
};


template<class Container>
/// <summary>
/// 条件付きイテレータ
/// </summary>
class IfIterator {
public:
	// イテレータの型
	using IteratorType = typename IteratorTypeHelper<Container>::type;
	// イテレータが参照するデータの型
	using ValueType = typename ValueTypeHelper<Container>::type;
	// 条件式の型
	using PredType = std::function<bool(ValueType&)>;

public:
	// コンストラクタで自動的にbeginが呼ばれる
	IfIterator(Container& container, const PredType& pred)
		: m_container(container)
		, m_pred(pred) {
		m_itr = std::find_if(m_container.begin(), m_container.end(), m_pred);
	}

public:
	IfIterator& operator=(const IfIterator& from) {
		m_container = from.m_container;
		m_pred = from.m_pred;
		m_itr = from.m_itr;
		return *this;
	}

	IfIterator& operator++() {
		m_itr = std::find_if(std::next(m_itr), m_container.end(), m_pred);
		return *this;
	}

	IfIterator operator++(int) {
		IfIterator itr = *this;
		m_itr = std::find_if(std::next(m_itr), m_container.end(), m_pred);
		return itr;
	}

	IfIterator operator+(int i) {
		if (i < 0) {
			ErrorMessage(L"条件付きイテレータに負の数を足すことは出来ません");
		}
		IfIterator itr = *this;
		while (i > 0) {
			++itr;
			--i;
		}
		return itr;
	}

	ValueType& operator*() const noexcept {
		return m_itr.operator*();
	}

	ValueType* operator->() const noexcept {
		return m_itr.operator->();
	}

	bool operator==(const IfIterator& other) {
		return m_itr == other.m_itr;
	}

	bool operator!=(const IfIterator& other) {
		return m_itr != other.m_itr;
	}

	bool operator==(const IteratorType& other) {
		return m_itr == other;
	}

	bool operator!=(const IteratorType& other) {
		return m_itr != other;
	}


public:
	IteratorType& Raw() { return m_itr; }

private:
	// 参照するコンテナ
	Container&           m_container;
	// 条件式
	PredType             m_pred;
	// ラップするイテレータ
	IteratorType         m_itr;
};


template<class Container>
/// <summary>
/// 条件付きconstイテレータ
/// </summary>
class IfConstIterator {
public:
	// イテレータの型
	using IteratorType = typename Container::const_iterator;
	// イテレータが参照するデータの型
	using ValueType = typename const Container::value_type;
	// 条件式の型
	using PredType = std::function<bool(ValueType&)>;

public:
	// コンストラクタで自動的にbeginが呼ばれる
	IfConstIterator(Container& container, const PredType& pred)
		: m_container(container)
		, m_pred(pred) {
		m_itr = std::find_if(m_container.cbegin(), m_container.cend(), m_pred);
	}

public:
	IfConstIterator& operator=(const IfConstIterator& from) {
		m_container = from.m_container;
		m_pred = from.m_pred;
		m_itr = from.m_itr;
		return *this;
	}

	IfConstIterator& operator++() {
		m_itr = std::find_if(std::next(m_itr), m_container.cend(), m_pred);
		return *this;
	}

	IfConstIterator operator++(int) {
		IfConstIterator itr = *this;
		m_itr = std::find_if(std::next(m_itr), m_container.cend(), m_pred);
		return itr;
	}

	IfConstIterator operator+(int i) {
		if (i < 0) {
			ErrorMessage(L"条件付きイテレータに負の数を足すことは出来ません");
		}
		IfConstIterator itr = *this;
		while (i > 0) {
			++itr;
			--i;
		}
		return itr;
	}

	ValueType& operator*() const noexcept {
		return m_itr.operator*();
	}

	ValueType* operator->() const noexcept {
		return m_itr.operator->();
	}

	bool operator==(const IfConstIterator& other) {
		return m_itr == other.m_itr;
	}

	bool operator!=(const IfConstIterator& other) {
		return m_itr != other.m_itr;
	}

	bool operator==(const IteratorType& other) {
		return m_itr == other;
	}

	bool operator!=(const IteratorType& other) {
		return m_itr != other;
	}

public:
	IteratorType& Raw() { return m_itr; }

private:
	// 参照するコンテナ
	Container&           m_container;
	// 条件式
	PredType             m_pred;
	// ラップするイテレータ
	IteratorType         m_itr;
};


#endif // !IF_ITERATOR_DEFINED
