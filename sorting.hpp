#ifndef SIMPLE_DS_SORTING_HPP
#define SIMPLE_DS_SORTING_HPP

#include <cstddef>

#include <utility>

namespace sorting {
	template<typename T>
	void bubble(T* arr, std::size_t len)
	{
		for (int i = 0; i < len; i++) {
			for (int j = i; j < len - i - 1; j++) {
				if (arr[j] > arr[j + 1]) {
					std::swap(arr[j], arr[j + 1]);
				}
			}
		}
	}
}

#endif // SIMPLE_DS_SORTING_HPP
