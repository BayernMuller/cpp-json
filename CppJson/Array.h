#pragma once
#include <vector>
#include <any>
#include <iostream>

class Array
{
public:
	template<class ...Args>
	Array(Args&&...Cs);

private:
	template<class T, class ...Args>
	void UnPacking(T&& value, Args&&... Cs);
	void UnPacking() {};

private:
	std::vector<std::any> m_Elements;
};

template<class ...Args>
inline Array::Array(Args&& ...Cs)
{
	UnPacking(Cs...);
}

template<class T, class ...Args>
inline void Array::UnPacking(T&& value, Args&&...Cs)
{
	m_Elements.push_back(value);
	UnPacking(Cs...);
}
