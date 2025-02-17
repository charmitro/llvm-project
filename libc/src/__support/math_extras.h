//===-- Mimics llvm/Support/MathExtras.h ------------------------*- C++ -*-===//
// Provides useful math functions.
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MATH_EXTRAS_H
#define LLVM_LIBC_SRC___SUPPORT_MATH_EXTRAS_H

#include "src/__support/CPP/type_traits.h"
#include "src/__support/macros/attributes.h" // LIBC_INLINE
#include "src/__support/macros/config.h"     // LIBC_HAS_BUILTIN

namespace LIBC_NAMESPACE {

// Add with carry
template <typename T> struct SumCarry {
  T sum;
  T carry;
};

// This version is always valid for constexpr.
template <typename T>
LIBC_INLINE constexpr cpp::enable_if_t<
    cpp::is_integral_v<T> && cpp::is_unsigned_v<T>, SumCarry<T>>
add_with_carry_const(T a, T b, T carry_in) {
  T tmp = a + carry_in;
  T sum = b + tmp;
  T carry_out = (sum < b) + (tmp < a);
  return {sum, carry_out};
}

// This version is not always valid for constepxr because it's overriden below
// if builtins are available.
template <typename T>
LIBC_INLINE cpp::enable_if_t<cpp::is_integral_v<T> && cpp::is_unsigned_v<T>,
                             SumCarry<T>>
add_with_carry(T a, T b, T carry_in) {
  return add_with_carry_const<T>(a, b, carry_in);
}

#if LIBC_HAS_BUILTIN(__builtin_addc)
// https://clang.llvm.org/docs/LanguageExtensions.html#multiprecision-arithmetic-builtins

template <>
LIBC_INLINE SumCarry<unsigned char>
add_with_carry<unsigned char>(unsigned char a, unsigned char b,
                              unsigned char carry_in) {
  SumCarry<unsigned char> result{0, 0};
  result.sum = __builtin_addcb(a, b, carry_in, &result.carry);
  return result;
}

template <>
LIBC_INLINE SumCarry<unsigned short>
add_with_carry<unsigned short>(unsigned short a, unsigned short b,
                               unsigned short carry_in) {
  SumCarry<unsigned short> result{0, 0};
  result.sum = __builtin_addcs(a, b, carry_in, &result.carry);
  return result;
}

template <>
LIBC_INLINE SumCarry<unsigned int>
add_with_carry<unsigned int>(unsigned int a, unsigned int b,
                             unsigned int carry_in) {
  SumCarry<unsigned int> result{0, 0};
  result.sum = __builtin_addc(a, b, carry_in, &result.carry);
  return result;
}

template <>
LIBC_INLINE SumCarry<unsigned long>
add_with_carry<unsigned long>(unsigned long a, unsigned long b,
                              unsigned long carry_in) {
  SumCarry<unsigned long> result{0, 0};
  result.sum = __builtin_addcl(a, b, carry_in, &result.carry);
  return result;
}

template <>
LIBC_INLINE SumCarry<unsigned long long>
add_with_carry<unsigned long long>(unsigned long long a, unsigned long long b,
                                   unsigned long long carry_in) {
  SumCarry<unsigned long long> result{0, 0};
  result.sum = __builtin_addcll(a, b, carry_in, &result.carry);
  return result;
}

#endif // LIBC_HAS_BUILTIN(__builtin_addc)

// Subtract with borrow
template <typename T> struct DiffBorrow {
  T diff;
  T borrow;
};

// This version is always valid for constexpr.
template <typename T>
LIBC_INLINE constexpr cpp::enable_if_t<
    cpp::is_integral_v<T> && cpp::is_unsigned_v<T>, DiffBorrow<T>>
sub_with_borrow_const(T a, T b, T borrow_in) {
  T tmp = a - b;
  T diff = tmp - borrow_in;
  T borrow_out = (diff > tmp) + (tmp > a);
  return {diff, borrow_out};
}

// This version is not always valid for constepxr because it's overriden below
// if builtins are available.
template <typename T>
LIBC_INLINE cpp::enable_if_t<cpp::is_integral_v<T> && cpp::is_unsigned_v<T>,
                             DiffBorrow<T>>
sub_with_borrow(T a, T b, T borrow_in) {
  return sub_with_borrow_const<T>(a, b, borrow_in);
}

#if LIBC_HAS_BUILTIN(__builtin_subc)
// https://clang.llvm.org/docs/LanguageExtensions.html#multiprecision-arithmetic-builtins

template <>
LIBC_INLINE DiffBorrow<unsigned char>
sub_with_borrow<unsigned char>(unsigned char a, unsigned char b,
                               unsigned char borrow_in) {
  DiffBorrow<unsigned char> result{0, 0};
  result.diff = __builtin_subcb(a, b, borrow_in, &result.borrow);
  return result;
}

template <>
LIBC_INLINE DiffBorrow<unsigned short>
sub_with_borrow<unsigned short>(unsigned short a, unsigned short b,
                                unsigned short borrow_in) {
  DiffBorrow<unsigned short> result{0, 0};
  result.diff = __builtin_subcs(a, b, borrow_in, &result.borrow);
  return result;
}

template <>
LIBC_INLINE DiffBorrow<unsigned int>
sub_with_borrow<unsigned int>(unsigned int a, unsigned int b,
                              unsigned int borrow_in) {
  DiffBorrow<unsigned int> result{0, 0};
  result.diff = __builtin_subc(a, b, borrow_in, &result.borrow);
  return result;
}

template <>
LIBC_INLINE DiffBorrow<unsigned long>
sub_with_borrow<unsigned long>(unsigned long a, unsigned long b,
                               unsigned long borrow_in) {
  DiffBorrow<unsigned long> result{0, 0};
  result.diff = __builtin_subcl(a, b, borrow_in, &result.borrow);
  return result;
}

template <>
LIBC_INLINE DiffBorrow<unsigned long long>
sub_with_borrow<unsigned long long>(unsigned long long a, unsigned long long b,
                                    unsigned long long borrow_in) {
  DiffBorrow<unsigned long long> result{0, 0};
  result.diff = __builtin_subcll(a, b, borrow_in, &result.borrow);
  return result;
}

#endif // LIBC_HAS_BUILTIN(__builtin_subc)

} // namespace LIBC_NAMESPACE

#endif // LLVM_LIBC_SRC___SUPPORT_MATH_EXTRAS_H
