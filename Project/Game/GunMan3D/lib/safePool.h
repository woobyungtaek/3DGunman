#pragma once

#include <Windows.h>

template<class Type>
class element
{
public:
	element();
	element(const Type& target);
	~element();

public:
	Type&			operator= (const Type& target);			// = ������ �����ε�

	Type			operator++ (int);						// ++ ���� ������ �����ε�
	const Type&		operator++ ();							// ++ ���� ������ �����ε�
	Type			operator-- (int);						// -- ���� ������ �����ε�
	const Type&		operator--();							// -- ���� ������ �����ε�

	Type&			operator += (const Type& target);		// += ������ �����ε�(type)
	Type&			operator += (const element& target);	// += ������ �����ε�(element)
	Type&			operator -= (const Type& target);		// -= ������ �����ε�(type)
	Type&			operator -= (const element& target);	// -= ������ �����ε�(element)

	Type			operator+ (const Type& target);			// + ������ �����ε�(Type)
	Type			operator+ (const element& target);		// + ������ �����ε�(element)
	Type			operator- (const Type& target);			// - ������ �����ε�(Type)
	Type			operator- (const element& target);		// - ������ �����ε�(element)

private:
	void			Use();			// ��� ���� ( �̹� ������̸� ��� )
	void			Unuse();		// ��� ��!

private:
	Type			value;			// ���� ��
	unsigned int	usLock;			// ������ ������ ���
};

template<class Type>
class safePool
{
public:
	safePool(int count);
	safePool(int count, const Type& target);
	~safePool();

public:
	element<Type>&	operator[] (int index);					// [] ������ �����ε�

public:
	int				count();		// Ǯ ���� ���

private:
	int				m_Count;		// Ǯ ����
	char*			m_MemoryPool;	// �޸� Ǯ
	element<Type>*	m_Pool;			// Ǯ �迭
};

template<class Type>
inline safePool<Type>::safePool(int count)
	: m_Count(count)
{
	m_MemoryPool = new char[sizeof(element<Type>) * m_Count];
	m_Pool = reinterpret_cast<element<Type>*>(m_MemoryPool);		// �޸� Ǯ �Ҵ�

	for (int i = 0; i < m_Count; ++i)
	{
		new (m_MemoryPool + sizeof(element<Type>) * i) element<Type>();		// placement new�� �Ҵ�� �޸� ������ ���� ����
	}
}

template<class Type>
inline safePool<Type>::safePool(int count, const Type& target)
	: safePool(count)
{
	m_MemoryPool = new char[sizeof(element<Type>) * m_Count];			// �޸� Ǯ �Ҵ�

	for (int i = 0; i < m_Count; ++i)
	{
		element<Type>* pMemory = (element<Type>*)(m_MemoryPool + (sizeof(element<Type>) * i));
		m_Pool[i] = new (pMemory) element<Type>(target);		// placement new�� �Ҵ�� �޸� ������ ���� ����
	}
}

template<class Type>
inline safePool<Type>::~safePool()
{
	for (int i = 0; i < m_Count; ++i)
	{
		m_Pool[i].~element<Type>();
	}

	delete[] m_MemoryPool;
	m_Pool = nullptr;
}

template<class Type>
inline element<Type>& safePool<Type>::operator[](int index)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return m_Pool[index];
}

template<class Type>
inline int safePool<Type>::count()
{
	return m_Count;
}

template<class Type>
inline element<Type>::element()
{
	value = Type();
	usLock = 0;
}

template<class Type>
inline element<Type>::element(const Type& target)
{
	value = Type(target);
	usLock = 0;
}

template<class Type>
inline element<Type>::~element()
{

}

template<class Type>
inline Type& element<Type>::operator=(const Type& target)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	value = target;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator++(int)
{
	Use();
	value++;
	Unuse();

	return value;
}

template<class Type>
inline const Type& element<Type>::operator++()
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	++value;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator--(int)
{
	Use();
	value--;
	Unuse();

	return value;
}

template<class Type>
inline const Type& element<Type>::operator--()
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	--value;
	Unuse();

	return value;
}

template<class Type>
inline Type& element<Type>::operator+=(const Type& target)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	value = value + target;
	Unuse();

	return value;
}

template<class Type>
inline Type& element<Type>::operator+=(const element& target)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	value = value + target.value;
	Unuse();

	return value;
}

template<class Type>
inline Type& element<Type>::operator-=(const Type& target)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	value = value - target;
	Unuse();

	return value;
}

template<class Type>
inline Type& element<Type>::operator-=(const element& target)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	Use();
	value = value - target.value;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator+(const Type& target)
{
	Use();
	value += target;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator+(const element& target)
{
	Use();
	value += target.value;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator-(const Type& target)
{
	Use();
	value -= target;
	Unuse();

	return value;
}

template<class Type>
inline Type element<Type>::operator-(const element& target)
{
	Use();
	value -= target.value;
	Unuse();

	return value;
}

template<class Type>
inline void element<Type>::Use()
{
	while (1 == InterlockedExchange(&usLock, 1))
	{

	}
}

template<class Type>
inline void element<Type>::Unuse()
{
	InterlockedCompareExchange(&usLock, 0, 1);
}