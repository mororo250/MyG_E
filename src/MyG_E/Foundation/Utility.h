#pragma once

// Get first element of parameter pack
template <class T, class ...Args>
constexpr T get_first(T const x, Args&&... args)
{
	return x;
}

// fill std::array at compile time.
template <class T, unsigned int SIZE, class ...Args, typename std::enable_if<(sizeof...(Args) == SIZE), int>::type = 0>
constexpr std::array<T, SIZE> fill_array(Args&& ... args)
{
	return std::array<T, SIZE >{ args... };
}

template <class T, unsigned int SIZE, class ...Args, typename std::enable_if<(sizeof...(Args) < SIZE && sizeof...(Args) > 0), int>::type = 0>
constexpr std::array<T, SIZE> fill_array(Args&&... args)
{
	return fill_array<T, SIZE>(args ..., get_first<T>(args ...));
}
