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
/// �����t���C�e���[�^
/// </summary>
class IfIterator {
public:
	// �C�e���[�^�̌^
	using IteratorType = typename IteratorTypeHelper<Container>::type;
	// �C�e���[�^���Q�Ƃ���f�[�^�̌^
	using ValueType = typename ValueTypeHelper<Container>::type;
	// �������̌^
	using PredType = std::function<bool(ValueType&)>;

public:
	// �R���X�g���N�^�Ŏ����I��begin���Ă΂��
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
			ErrorMessage(L"�����t���C�e���[�^�ɕ��̐��𑫂����Ƃ͏o���܂���");
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
	// �Q�Ƃ���R���e�i
	Container&           m_container;
	// ������
	PredType             m_pred;
	// ���b�v����C�e���[�^
	IteratorType         m_itr;
};


template<class Container>
/// <summary>
/// �����t��const�C�e���[�^
/// </summary>
class IfConstIterator {
public:
	// �C�e���[�^�̌^
	using IteratorType = typename Container::const_iterator;
	// �C�e���[�^���Q�Ƃ���f�[�^�̌^
	using ValueType = typename const Container::value_type;
	// �������̌^
	using PredType = std::function<bool(ValueType&)>;

public:
	// �R���X�g���N�^�Ŏ����I��begin���Ă΂��
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
			ErrorMessage(L"�����t���C�e���[�^�ɕ��̐��𑫂����Ƃ͏o���܂���");
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
	// �Q�Ƃ���R���e�i
	Container&           m_container;
	// ������
	PredType             m_pred;
	// ���b�v����C�e���[�^
	IteratorType         m_itr;
};


#endif // !IF_ITERATOR_DEFINED
