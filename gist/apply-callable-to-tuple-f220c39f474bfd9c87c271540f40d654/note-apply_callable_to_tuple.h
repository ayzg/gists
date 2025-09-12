#pragma once
#include <concepts>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeindex>

template <typename T>
concept Addable = requires(T Value) { Value + Value; };

template <typename T>
concept Stringable = requires(T Value) { std::to_string(Value); };

// Overload + operator so it can be implicitly added to scalars in the test().
template <typename T>
static inline auto operator+(T lhs, std::string rhs)
  requires((not std::is_same_v<T, std::string>) && Addable<T> && Stringable<T>)
{
  // Todo: custom to-string method
  return std::to_string(lhs) + rhs;
}

template <typename T>
static inline auto operator+(std::string lhs, T rhs)
  requires((not std::is_same_v<T, std::string>) && Addable<T> && Stringable<T>)
{
  // Todo: custom to-string method
  return lhs + std::to_string(rhs);
}


//  The actual function expanding the tuple:
template <typename CallableT, typename... ValTs>
auto apply_callable_impl(CallableT&& callable, ValTs... v) {
  return callable(v...);
}

// The interface to call. Sadly, it needs to dispatch to another function
// to deduce the sequence of indices created from std::make_index_sequence<N>
template <typename CallableT>
auto apply_callable(CallableT&& callable, auto&&... args) {
  return apply_callable_impl(std::forward<CallableT>(callable),
                             std::forward<decltype(args)>(args)...);
}

template <class CallableT, class Tuple, std::size_t... Is>
auto apply_callable_to_tuple_impl(CallableT&& callable, const Tuple& tup,
                                  std::index_sequence<Is...>) {
  return apply_callable(callable, std::get<Is>(tup)...);
}

template <class CallableT, class... Args>
auto apply_callable_to_tuple(CallableT&& callable,
                             const std::tuple<Args...>& t) {
  return apply_callable_to_tuple_impl(
      std::forward<CallableT>(callable),
      std::forward<const std::tuple<Args...>&>(t),
      std::index_sequence_for<Args...>{});
}

int test() {
  auto value_tuple = std::make_tuple(5, 5.f, std::string("hello"));
  auto accumulate = [](auto... args) { return (args + ...); };
  auto add_tuple_elements = apply_callable_to_tuple(accumulate, value_tuple);
  std::cout << add_tuple_elements << std::endl;
};