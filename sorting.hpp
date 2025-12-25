#ifndef SIMPLE_DS_SORTING_HPP
#define SIMPLE_DS_SORTING_HPP

#include <cstddef>
#include <stack>

#include <utility>

namespace sorting {
  template<typename T>
  concept Comparable = requires (T a, T b) {
    a < b;
    a > b;
    a == b;
  };

  template<Comparable T>
  void bubble(T* arr, std::size_t len)
  {
    if (arr == nullptr) return;

    for (int i = 0; i < len; i++) {
      for (int j = i; j < len - i - 1; j++) {
        if (arr[j] > arr[j + 1]) {
          std::swap(arr[j], arr[j + 1]);
        }
      }
    }
  }

  template<Comparable T>
  void quick(T* arr, std::size_t len)
  {
    if (arr == nullptr || len <= 1) return ;

    std::stack<std::pair<T*, std::size_t>> stack;
    stack.emplace(arr, len);

    while (!stack.empty()) {
      std::pair<T*, std::size_t> top = stack.top();
      stack.pop();

      T* start = top.first;
      std::size_t length = top.second;

      if (length <= 1) continue;

      T pivot = start[0];
      std::size_t left = 0;
      std::size_t right = length - 1;

      while (left < right) {
        while (left < right && start[right] >= pivot) --right;
        if (left < right) start[left++] = start[right];

        while (left < right && start[left] <= pivot) ++left;
        if (left < right) start[right--] = start[left];
      }

      start[left] = pivot;
      stack.emplace(start, left);
      stack.emplace(start + left + 1, length - left - 1);
    }
  }
}

#endif // SIMPLE_DS_SORTING_HPP
