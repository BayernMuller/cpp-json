#pragma once


class Array
{
public:
	template<class ...Args>
	Array(Args&&...Cs);

	template<class T>
	Array& operator+=(T&& value);
	friend std::ostream& operator<<(std::ostream& os, Array&& arr);

private:
	template<class T, class ...Args>
	void UnPacking(T&& value, Args&&... Cs);
	void UnPacking(void) {}; // Called when Cs is empty

private:
	std::vector<std::any> m_Elements;
};

template<class ...Args>
inline Array::Array(Args&& ...Cs)
{
	UnPacking(Cs...);
}

template<class T>
inline Array& Array::operator+=(T&& value)
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

inline std::ostream& operator<<(std::ostream& os, Array&& arr)
{
	/*
		How I do it? 
	*/
	return os;
}

