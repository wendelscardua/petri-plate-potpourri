 // Copyright 2024 LLVM-MOS Project
 // Licensed under the Apache License, Version 2.0 with LLVM Exceptions.
 // See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
 // information.
  
 #ifndef _FIXED_POINT_H
 #define _FIXED_POINT_H
  
 #include <cstdint>
 #include <limits.h>
 #include <stdio.h>
 #include <type_traits>
  
 #if __cplusplus >= 202002L
 #define __fp_consteval consteval
 #else
 #define __fp_consteval constexpr
 #endif
  
 template <intmax_t IntSize, intmax_t FracSize, bool Signed = true>
 class FixedPoint;
  
 template <intmax_t IntSize, intmax_t FracSize, bool Signed> class FixedPoint {
   static __fp_consteval auto bytesForBits(intmax_t v) { return ((v + 7) / 8); }
   static constexpr auto storage_size = bytesForBits(IntSize + FracSize) * 8;
  
   // The result type for binary operations, similar to the C integer rules. The
   // larger of the integer and fractional sizes are used. If the integer sizes
   // differ, has the signedness of the larger. Otherwise, signed iff both
   // are signed.
   template <intmax_t OI, intmax_t OF, bool S>
   using BinaryResultT =
       FixedPoint<(OI > IntSize ? OI : IntSize), (OF > FracSize ? OF : FracSize),
                  (OI > IntSize   ? S
                   : OI < IntSize ? Signed
                                  : S && Signed)>;
  
 public:
   using IntType =
       std::conditional_t<Signed, _BitInt(IntSize), unsigned _BitInt(IntSize)>;
   using FracType = unsigned _BitInt(FracSize);
   using StorageType = std::conditional_t<Signed, _BitInt(storage_size),
                                          unsigned _BitInt(storage_size)>;
  
 private:
   // Underlying storage for the type
   union {
     StorageType val;
     struct {
       FracType f : FracSize;
       IntType i : IntSize;
     };
   };
  
 public:
   // Constructors
   template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
   [[clang::always_inline]] constexpr FixedPoint(T i)
       : val((StorageType)i << FracSize) {}
  
   [[clang::always_inline]] constexpr FixedPoint(IntType i, FracType f)
       : val((StorageType)i << FracSize | f) {}
  
   [[clang::always_inline]] constexpr FixedPoint(const FixedPoint &o)
       : val(o.val) {}
   [[clang::always_inline]] constexpr FixedPoint &operator=(FixedPoint o) {
     val = o.val;
     return *this;
   }
  
   template <typename T,
             std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
   [[clang::always_inline]] __fp_consteval explicit FixedPoint(T f) {
     set(f * ((StorageType)1 << FracSize));
   }
  
   // Implicit conversion. Like C/C++ integers, silently truncate on conversion
   // to a smaller type.
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint(FixedPoint<OI, OF, S> o) {
     if constexpr (FracSize > OF)
       *this = FixedPoint(o.as_i(), (FracType)o.as_f() << (FracSize - OF));
     else
       *this = FixedPoint(o.as_i(), o.as_f() >> (OF - FracSize));
   }
  
   // Direct value accessor and setter methods
  
   [[clang::always_inline]] constexpr IntType as_i() const { return i; }
   [[clang::always_inline]] constexpr FracType as_f() const { return f; }
   [[clang::always_inline]] constexpr StorageType get() const { return val; }
   [[clang::always_inline]] constexpr void set_i(IntType value) { i = value; }
   [[clang::always_inline]] constexpr void set_f(FracType value) { f = value; }
   [[clang::always_inline]] constexpr void set(StorageType value) {
     val = value;
   }
  
   template <intmax_t OI, intmax_t OF, bool S = true>
   [[clang::always_inline]] constexpr FixedPoint<OI, OF, S> as() {
     return FixedPoint<OI, OF, S>(*this);
   }
  
   [[clang::always_inline]] constexpr bool is_signed() const { return Signed; }
   [[clang::always_inline]] constexpr intmax_t bitcount() const {
     return IntSize + FracSize;
   }
   [[clang::always_inline]] constexpr intmax_t int_bitcount() const {
     return IntSize;
   }
   [[clang::always_inline]] constexpr intmax_t frac_bitcount() const {
     return FracSize;
   }
  
   // Operator overloads
  
   // Unary operators
   [[clang::always_inline]] constexpr FixedPoint operator-() const {
     FixedPoint n = *this;
     n.set(-n.get());
     return n;
   }
   [[clang::always_inline]] constexpr FixedPoint operator~() const {
     FixedPoint n = *this;
     n.set(~n.get());
     return n;
   }
  
   // Bitwise operators
   [[clang::always_inline]] constexpr FixedPoint &operator%=(FixedPoint o) {
     val %= o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator%(FixedPoint o) const {
     FixedPoint n = *this;
     n %= o;
     return n;
   }
   [[clang::always_inline]] constexpr FixedPoint &operator&=(FixedPoint o) {
     val &= o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator&(FixedPoint o) const {
     FixedPoint n = *this;
     n &= o;
     return n;
   }
   [[clang::always_inline]] constexpr FixedPoint &operator|=(FixedPoint o) {
     val |= o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator|(FixedPoint o) const {
     FixedPoint n = *this;
     n |= o;
     return n;
   }
   [[clang::always_inline]] constexpr FixedPoint &operator^=(FixedPoint o) {
     val ^= o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator^(FixedPoint o) const {
     FixedPoint n = *this;
     n ^= o;
     return n;
   }
  
   // Arithimetic operators for same sized types
   [[clang::always_inline]] constexpr FixedPoint &operator+=(FixedPoint o) {
     val += o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator+(FixedPoint o) const {
     FixedPoint n = *this;
     n += o;
     return n;
   }
  
   [[clang::always_inline]] constexpr FixedPoint &operator-=(FixedPoint o) {
     val -= o.val;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator-(FixedPoint o) const {
     FixedPoint n = *this;
     n -= o;
     return n;
   }
  
   [[clang::always_inline]] constexpr FixedPoint &operator/=(FixedPoint o) {
     val /= o.get();
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator/(FixedPoint o) const {
     FixedPoint n = *this;
     n /= o;
     return n;
   }
  
   [[clang::always_inline]] constexpr FixedPoint &operator*=(FixedPoint o) {
     // Fixed point mult is (n * m / FracSize)
     // Expand the immediate value before multiplying
     FixedPoint<IntSize * 2, FracSize, Signed> temp{*this};
     FixedPoint<IntSize * 2, FracSize, Signed> other{o};
     // Truncate the final result to fit inside our value
     val = (temp.get() * other.get()) >> FracSize;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint operator*(FixedPoint o) const {
     FixedPoint n = *this;
     n *= o;
     return n;
   }
  
   [[clang::always_inline]] constexpr FixedPoint &operator>>=(StorageType v) {
     val >>= v;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint
   operator>>(StorageType v) const {
     FixedPoint n = *this;
     n >>= v;
     return n;
   }
  
   [[clang::always_inline]] constexpr FixedPoint &operator<<=(StorageType v) {
     val <<= v;
     return *this;
   }
   [[clang::always_inline]] constexpr FixedPoint
   operator<<(StorageType v) const {
     FixedPoint n = *this;
     n <<= v;
     return n;
   }
  
   [[clang::always_inline]] FixedPoint &operator++() {
     i += 1;
     return *this;
   }
   [[clang::always_inline]] FixedPoint operator++(int) {
     FixedPoint old = *this;
     ++*this;
     return old;
   }
  
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator+(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l += r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator+=(FixedPoint<OI, OF, S> o) {
     *this = *this + o;
     return *this;
   }
  
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator-(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l -= r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator-=(FixedPoint<OI, OF, S> o) {
     *this = *this - o;
     return *this;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator%(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l %= r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator%=(FixedPoint<OI, OF, S> o) {
     *this = *this % o;
     return *this;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator^(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l ^= r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator^=(FixedPoint<OI, OF, S> o) {
     *this = *this ^ o;
     return *this;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator&(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l &= r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator&=(FixedPoint<OI, OF, S> o) {
     *this = *this & o;
     return *this;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator|(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     l |= r;
     return l;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator|=(FixedPoint<OI, OF, S> o) {
     *this = *this | o;
     return *this;
   }
  
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator*(FixedPoint<OI, OF, S> o) const {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l * r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr FixedPoint &
   operator*=(FixedPoint<OI, OF, S> o) {
     *this = *this * o;
     return *this;
   }
  
   // Comparison overloads
   [[clang::always_inline]] constexpr bool
   operator==(const FixedPoint &o) const noexcept {
     return val == o.val;
   }
   [[clang::always_inline]] constexpr bool
   operator<(const FixedPoint &o) const noexcept {
     return val < o.val;
   }
   [[clang::always_inline]] constexpr bool
   operator!=(const FixedPoint &o) const noexcept {
     return val != o.val;
   }
   [[clang::always_inline]] constexpr bool
   operator>(const FixedPoint &o) const noexcept {
     return val > o.val;
   }
   [[clang::always_inline]] constexpr bool
   operator>=(const FixedPoint &o) const noexcept {
     return val >= o.val;
   }
   [[clang::always_inline]] constexpr bool
   operator<=(const FixedPoint &o) const noexcept {
     return val <= o.val;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator==(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l == r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator<(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l < r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator!=(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l != r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator>(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l > r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator>=(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l >= r;
   }
   template <intmax_t OI, intmax_t OF, bool S>
   [[clang::always_inline]] constexpr auto
   operator<=(FixedPoint<OI, OF, S> o) const noexcept {
     BinaryResultT<OI, OF, S> l = *this;
     BinaryResultT<OI, OF, S> r = o;
     return l <= r;
   }
 };
  
 namespace fixedpoint_literals {
  
 using fs8_8 = FixedPoint<8, 8, true>;
 using fs12_4 = FixedPoint<12, 4, true>;
 using fs16_8 = FixedPoint<16, 8, true>;
 using fs8_16 = FixedPoint<8, 16, true>;
 using fs12_12 = FixedPoint<12, 12, true>;
 using fs16_16 = FixedPoint<16, 16, true>;
 using fs24_8 = FixedPoint<24, 8, true>;
  
 using fu8_8 = FixedPoint<8, 8, false>;
 using fu12_4 = FixedPoint<12, 4, false>;
 using fu16_8 = FixedPoint<16, 8, false>;
 using fu8_16 = FixedPoint<8, 16, false>;
 using fu12_12 = FixedPoint<12, 12, false>;
 using fu16_16 = FixedPoint<16, 16, false>;
 using fu24_8 = FixedPoint<24, 8, false>;
  
 using f8_8 = fs8_8;
 using f12_4 = fs12_4;
 using f16_8 = fs16_8;
 using f8_16 = fs8_16;
 using f12_12 = fs12_12;
 using f16_16 = fs16_16;
 using f24_8 = fs24_8;
  
 [[clang::always_inline]] __fp_consteval fs8_8
 operator""_s8_8(long double fixed) {
   return FixedPoint<8, 8, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs12_4
 operator""_s12_4(long double fixed) {
   return FixedPoint<12, 4, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs16_8
 operator""_s16_8(long double fixed) {
   return FixedPoint<16, 8, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs8_16
 operator""_s8_16(long double fixed) {
   return FixedPoint<8, 16, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs12_12
 operator""_s12_12(long double fixed) {
   return FixedPoint<12, 12, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs16_16
 operator""_s16_16(long double fixed) {
   return FixedPoint<16, 16, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs24_8
 operator""_s24_8(long double fixed) {
   return FixedPoint<24, 8, true>{fixed};
 }
  
 [[clang::always_inline]] __fp_consteval fu8_8
 operator""_u8_8(long double fixed) {
   return FixedPoint<8, 8, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu12_4
 operator""_u12_4(long double fixed) {
   return FixedPoint<12, 4, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu16_8
 operator""_u16_8(long double fixed) {
   return FixedPoint<16, 8, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu8_16
 operator""_u8_16(long double fixed) {
   return FixedPoint<8, 16, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu12_12
 operator""_u12_12(long double fixed) {
   return FixedPoint<12, 12, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu16_16
 operator""_u16_16(long double fixed) {
   return FixedPoint<16, 16, false>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fu24_8
 operator""_u24_8(long double fixed) {
   return FixedPoint<24, 8, false>{fixed};
 }
  
 [[clang::always_inline]] __fp_consteval fs8_8
 operator""_8_8(long double fixed) {
   return FixedPoint<8, 8, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs12_4
 operator""_12_4(long double fixed) {
   return FixedPoint<12, 4, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs16_8
 operator""_16_8(long double fixed) {
   return FixedPoint<16, 8, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs8_16
 operator""_8_16(long double fixed) {
   return FixedPoint<8, 16, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs12_12
 operator""_12_12(long double fixed) {
   return FixedPoint<12, 12, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs16_16
 operator""_16_16(long double fixed) {
   return FixedPoint<16, 16, true>{fixed};
 }
 [[clang::always_inline]] __fp_consteval fs24_8
 operator""_24_8(long double fixed) {
   return FixedPoint<24, 8, true>{fixed};
 }
 } // namespace fixedpoint_literals
  
 #endif // _FIXED_POINT_H
