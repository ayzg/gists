#pragma once
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>

// https://xuhuisun.com/post/c++-weekly-2-constexpr-map/
template <typename Key, typename Value, std::size_t Size>
struct Map {
  std::array<std::pair<Key, Value>, Size> data;

  [[nodiscard]] constexpr Value at(const Key& key) const {
    const auto itr =
        std::find_if(std::begin(data), std::end(data),
                     [&key](const auto& v) { return v.first == key; });
    if (itr != std::end(data)) {
      return itr->second;
    } else {
      throw "Not Found";
    }
  }
};


// https://stackoverflow.com/questions/41723704/how-to-filter-a-stdinteger-sequence
template <std::size_t... s, std::size_t... t>
constexpr auto concat_sequence(std::index_sequence<s...>,
                               std::index_sequence<t...>) {
  return std::index_sequence<s..., t...>{};
}
// base case; empty sequence
template <class PredT>
constexpr auto FilterSeq(PredT&& pred, std::index_sequence<>) {
  return std::index_sequence<>{};
}

// base case; one element in the sequence
template <class PredT, std::size_t a>
constexpr auto FilterSeq(PredT&& pred, std::index_sequence<a>) {
  if constexpr (pred(a))
    return std::index_sequence<a>{};
  else
    return std::index_sequence<>{};
}

// recursive case
template <class PredT, std::size_t a, std::size_t... b>
constexpr auto FilterSeq(PredT&& pred, std::index_sequence<a, b...>) {
  return concat_sequence(
      FilterSeq(std::forward<PredT>(pred), std::index_sequence<a>{}),
      FilterSeq(std::forward<PredT>(pred), std::index_sequence<b...>{}));
}

class MethodMap {
  typedef void (*void_func_t)(void);

  std::map<std::size_t, std::pair<void_func_t, std::type_index>> func_map_;

 public:
  MethodMap() = default;
  MethodMap(
      std::initializer_list<std::pair<std::size_t, void_func_t>> init_list) {
    for (auto& elem : init_list) {
      func_map_.insert(std::make_pair(
          elem.first,
          std::make_pair(elem.second, std::type_index(typeid(elem.second)))));
    }
  }

  // copy is not allowed
  MethodMap(const MethodMap&) = delete;
  MethodMap& operator=(const MethodMap&) = delete;

  MethodMap(MethodMap&&) = default;
  MethodMap& operator=(MethodMap&&) = default;

  template <typename T>
  constexpr void insert(std::size_t func_index, T func) {
    auto func_type_index = std::type_index(typeid(func));
    func_map_.insert(std::make_pair(
        func_index, std::make_pair((void_func_t)func, func_type_index)));
  }

    // Returns the std::type_index of the function at index.
  // Returns void typeid if not found.
  std::type_index getTypeIndex(std::size_t func_index) {
    auto mapIter = func_map_.find(func_index);
    if (mapIter == func_map_.end()) {
      return std::type_index(typeid(void));
    }
    return mapIter->second.second;
  }

  template <typename T, typename... Args>
  T searchAndCall(std::size_t func_index, Args&&... args) {
    auto mapIter = func_map_.find(func_index);
    /*chk if not end*/
    auto mapVal = mapIter->second;

    auto typeCastedFun = reinterpret_cast<T (*)(Args...)>(mapVal.first);
    // auto typeCastedFun = (T(*)(Args ...))(mapVal.first);

    // compare the types is equal or not
    assert(mapVal.second == std::type_index(typeid(typeCastedFun)));
    return typeCastedFun(std::forward<Args>(args)...);
  }
};

enum myFuncIndex {
  fun1_index = 1,
  fun2_index,
  fun3_index,
  fun4_index,
  lambda_index
};

void fun1(void) { std::cout << "inside fun1\n"; }

int fun2() {
  std::cout << "inside fun2\n";
  return 2;
}

int fun3(int a) {
  std::cout << "inside fun3\n";
  return a;
}

std::vector<int> fun4() {
  std::cout << "inside fun4\n";
  std::vector<int> v(4, 100);
  return v;
}

int ExampleUse_HomogeneusFunctionContainer() {
  MethodMap a1;
  a1.insert(1, fun1);
  a1.insert(2, fun2);
  a1.insert(3, fun3);
  a1.insert(4, fun4);
  //a1.insert(lambda_index, []() { return "cool!"; });

  a1.searchAndCall<void>(1);
  int retVal = a1.searchAndCall<int>(2, 2);
  a1.searchAndCall<int>(3);
  auto temp = a1.searchAndCall<std::vector<int>>(4);

  return 0;
}