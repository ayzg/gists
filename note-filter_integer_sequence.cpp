#include <algorithm>
#include <array>
#include <functional>
#include <string_view>
#include <utility>
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

int main() {
  static_assert(
      std::is_same_v<std::index_sequence<0, 2, 4, 6, 8>,
                     decltype(FilterSeq(
                         [](auto&& a) { return (a % 2) == 0; },
                         std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>{}))>,
      "Integer sequences should be equal");

  return 0;
}