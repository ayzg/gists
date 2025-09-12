# [Note] Compile time `any_of_element` method
C++17. Compile time meta method which returns a result equal to all elements within a container. Works like so:
```
//  any_of_element(container_or_element_pack) == the_value;
static_assert(any_of_element(1, 2, 3) == 2)
static_assert(any_of_element(std::vector{1, 2, 3}) != 5
``  