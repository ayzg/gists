// SPDX-License-Identifier: Apache-2.0
// Copyright(c) Anton Yashchenko 2025
// Created : 2024/10/21

#include <array>
#include <compare>
#include <iostream>
#include <type_traits>
#include <utility>
struct cstring {
  const char* str{nullptr};
};

template <class EnumT, EnumT FirstV, EnumT LastV, class AssociatedDataEntryT,
          auto... AssociatedDataEntryVs>
  requires ((std::is_scoped_enum_v<EnumT>))
struct contig_enum {
  using under_t = std::underlying_type_t<EnumT>;
  using enum_t = EnumT;
  static constexpr enum_t first_val = FirstV;
  static constexpr enum_t last_val = LastV;
  static constexpr under_t size =
      std::to_underlying(LastV) - std::to_underlying(FirstV) + 1;

  static constexpr enum_t to_enum(under_t e) { return static_cast<enum_t>(e); }
  static constexpr under_t to_under(enum_t e) { return std::to_underlying(e); }

  static constexpr std::array<enum_t, size> values_arr = []() {
    std::array<enum_t, size> ret;
    auto nextval = first_val;
    for (auto& entry : ret) {
      entry = nextval;
      nextval = static_cast<enum_t>(static_cast<under_t>(nextval) + 1);
    }
    return ret;
  }();
  static constexpr std::array<AssociatedDataEntryT, size> associated = {
      AssociatedDataEntryVs...};

  constexpr bool in_range() { return val >= first_val && val <= last_val; }

  constexpr void clamp() {
    if (val >= first_val) val = first_val;
    if (val <= last_val) val = last_val;
  }

  constexpr under_t under() { return std::to_underlying(val); }

  constexpr contig_enum begin() { return *this; }
  constexpr contig_enum end() { return contig_enum{static_cast<EnumT>(size)}; }
  bool operator!=(contig_enum other) { return other.val != val; }
  bool operator!=(enum_t other) { return other != val; }
  bool operator!=(under_t other) { return other != under(); }
  constexpr bool operator<=>(contig_enum other) {
    return under() <=> other.under();
  }
  constexpr bool operator<=>(under_t other) { return under() <=> other; }
  constexpr bool operator<=>(enum_t other) {
    return under() <=> to_under(other);
  }
  contig_enum& operator++() {
    val = to_enum(under() + 1);
    return *this;
  }
  contig_enum operator+(under_t rhs) { return to_enum(under() + rhs); }
  contig_enum operator-(under_t rhs) { return to_enum(under() - rhs); }

  contig_enum operator*() { return *this; }

  contig_enum operator[](size_t idx) { return to_enum(idx); }
  contig_enum operator[](enum_t eidx) { return to_under(eidx); }
  EnumT val{};
};

enum class eTest : size_t { kGood, kUgly, kBad };

int main() {
  contig_enum<eTest, eTest::kGood, eTest::kBad, cstring, cstring{"hello"}>
      MY_ENUM{};

  return 0;
}