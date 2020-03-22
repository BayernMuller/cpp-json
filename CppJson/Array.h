#pragma once
#include "Value.h"
#include <iostream>

class Array
{
public:
	template<class ...Args>
	Array(Args&&...Cs);

	template<class T>
	Array& operator+=(T&& value);

	//json_value& operator[](std::size_t index);

	friend std::ostream& operator<<(std::ostream& os, Array&& arr);

private:
	template<class T, class ...Args>
	void UnPacking(T&& value, Args&&...Cs);
	// Default UnPacking function.

	template<class ...Args>
	void UnPacking(const char*, Args&&...);
	// function that casts str literal to std::string.

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
inline Array& Array::operator+=(T&& value)
{
	UnPacking(value);
	return *this;
}
/*
Jsonbase::json_value& Array::operator[](std::size_t index)
{
	return m_Elements.at(index);
}
*/
template<class T, class ...Args>
inline void Array::UnPacking(T&& value, Args&&...Cs)
{
	m_Elements.push_back(value);
	UnPacking(Cs...);
}

template<class ...Args>
inline void Array::UnPacking(const char* str, Args&&...Cs)
{
	m_Elements.push_back(std::string(str));
	UnPacking(Cs...);
}

inline std::ostream& operator<<(std::ostream& os, Array&& arr)
{
	return os;
}