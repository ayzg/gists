# [Note] Apply a callable object to a `std::tuple<Ts...>`.
C++20. Demostrates how to apply a callable to a generic tuple.
``` cpp
  auto value_tuple = std::make_tuple(5, 5.f, std::string("hello"));
  auto accumulate = [](auto... args) { return (args + ...); };
  auto add_tuple_elements = apply_callable_to_tuple(accumulate, value_tuple);
```