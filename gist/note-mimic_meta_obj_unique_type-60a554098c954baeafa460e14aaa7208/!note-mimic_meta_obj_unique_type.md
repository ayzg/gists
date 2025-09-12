# [Concept] `Mimic` meta object. A type that is unique every time - but equivalent to its own type.
C++20.
Mimic is a meta object. Mimics a type and its operations but is a dirrent
type. Like a distinct typedef. UNIQUE_TYPE macro is a shortcut for a decltype
of a void lambda which is unique every time.

"A type that is unique every time - but equivalent to its own type."