#pragma once

class Value;

class Array
{
	friend Value;
public:
	template<class ...Args>
	Array(Args&&...Cs);

	template<class T>
	Array& append(T&& value);

private:
	template<class T, class ...Args>
	void UnPacking(T&& value, Args&&...Cs);
	// Default UnPacking function.

	void UnPacking(void) {};
	// Called when Cs is empty

private:
	std::vector<Value> m_Elements;
};

template<class ...Args>
inline Array::Array(Args&& ...Cs)
{
	UnPacking(Cs...);
	// Start recursion
}

template<class T>
inline Array& Array::append(T&& value)
{
	m_Elements.push_back(value);
	return *this;
}

template<class T, class ...Args>
inline void Array::UnPacking(T&& value, Args&&...Cs)
{
	m_Elements.push_back(value);
	UnPacking(Cs...);
}
