#pragma once

namespace nall {

//attempt to keep Natural / Integer types when performing a math operation against another Natural / Integer or primitive type
//try not to lose any data through truncation (eg Natural<32> + Natural<32> -> Natural<64>)
//complex SFINAE is needed to prevent the creation of ambiguous operator overloads

#define Type PrimitiveType
#define Compatible PrimitiveCompatible
#define CastExpand PrimitiveCastExpand
#define CastShrink PrimitiveCastShrink
#define CastDouble PrimitiveCastDouble
#define CastLarger PrimitiveCastLarger
#define CastLesser PrimitiveCastLesser

template<typename T> struct Type {
  using type = T;
  static inline constexpr uint bits = 8 * sizeof(T);
};

template<> struct Type<Boolean> {
  using type = typename Boolean::type;
  static inline constexpr uint bits = Boolean::bits();
};

template<int Precision> struct Type<Natural<Precision>> {
  using type = typename Natural<Precision>::type;
  static inline constexpr uint bits = Natural<Precision>::bits();
};

template<int Precision> struct Type<Integer<Precision>> {
  using type = typename Integer<Precision>::type;
  static inline constexpr uint bits = Integer<Precision>::bits();
};

template<int Precision> struct Type<Real<Precision>> {
  using type = typename Real<Precision>::type;
  static inline constexpr uint bits = Real<Precision>::bits();
};

//compatible: SFINAE operator enable

template<typename LHS, typename RHS, typename = void> struct Compatible { static const bool value = false; };

template<int LHS, typename RHS> struct Compatible<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { static const bool value = true; };
template<typename LHS, int RHS> struct Compatible<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { static const bool value = true; };
template<int LHS, int RHS> struct Compatible<Natural<LHS>, Natural<RHS>> { static const bool value = true; };

template<int LHS, typename RHS> struct Compatible<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { static const bool value = true; };
template<typename LHS, int RHS> struct Compatible<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { static const bool value = true; };
template<int LHS, int RHS> struct Compatible<Integer<LHS>, Integer<RHS>> { static const bool value = true; };

//expand: LHS+RHS bits

template<typename LHS, typename RHS, typename = void> struct CastExpand { using type = void; };

template<int LHS, typename RHS> struct CastExpand<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Natural<LHS + Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastExpand<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Natural<Type<LHS>::bits + RHS>; };
template<int LHS, int RHS> struct CastExpand<Natural<LHS>, Natural<RHS>> { using type = Natural<LHS + RHS>; };

template<int LHS, typename RHS> struct CastExpand<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Integer<LHS + Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastExpand<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Integer<Type<LHS>::bits + RHS>; };
template<int LHS, int RHS> struct CastExpand<Integer<LHS>, Integer<RHS>> { using type = Integer<LHS + RHS>; };

//shrink: LHS-RHS bits

template<typename LHS, typename RHS, typename = void> struct CastShrink { using type = void; };

template<int LHS, typename RHS> struct CastShrink<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Natural<LHS - Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastShrink<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Natural<Type<LHS>::bits - RHS>; };
template<int LHS, int RHS> struct CastShrink<Natural<LHS>, Natural<RHS>> { using type = Natural<LHS - RHS>; };

template<int LHS, typename RHS> struct CastShrink<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Integer<LHS - Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastShrink<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Integer<Type<LHS>::bits - RHS>; };
template<int LHS, int RHS> struct CastShrink<Integer<LHS>, Integer<RHS>> { using type = Integer<LHS - RHS>; };

//double: 1+max(LHS,RHS) bits

template<typename LHS, typename RHS, typename = void> struct CastDouble { using type = void; };

template<int LHS, typename RHS> struct CastDouble<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Natural<1 + (LHS >= Type<RHS>::bits ? LHS : Type<RHS>::bits)>; };
template<typename LHS, int RHS> struct CastDouble<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Natural<1 + (Type<LHS>::bits >= RHS ? Type<LHS>::bits : RHS)>; };
template<int LHS, int RHS> struct CastDouble<Natural<LHS>, Natural<RHS>> { using type = Natural<1 + (LHS >= RHS ? LHS : RHS)>; };

template<int LHS, typename RHS> struct CastDouble<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Integer<1 + (LHS >= Type<RHS>::bits ? LHS : Type<RHS>::bits)>; };
template<typename LHS, int RHS> struct CastDouble<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Integer<1 + (Type<LHS>::bits >= RHS ? Type<LHS>::bits : RHS)>; };
template<int LHS, int RHS> struct CastDouble<Integer<LHS>, Integer<RHS>> { using type = Integer<1 + (LHS >= RHS ? LHS : RHS)>; };

//larger: max(LHS,RHS) bits

template<typename LHS, typename RHS, typename = void> struct CastLarger { using type = void; };

template<int LHS, typename RHS> struct CastLarger<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Natural<LHS >= Type<RHS>::bits ? LHS : Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastLarger<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Natural<Type<LHS>::bits >= RHS ? Type<LHS>::bits : RHS>; };
template<int LHS, int RHS> struct CastLarger<Natural<LHS>, Natural<RHS>> { using type = Natural<LHS >= RHS ? LHS : RHS>; };

template<int LHS, typename RHS> struct CastLarger<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Integer<LHS >= Type<RHS>::bits ? LHS : Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastLarger<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Integer<Type<LHS>::bits >= RHS ? Type<LHS>::bits : RHS>; };
template<int LHS, int RHS> struct CastLarger<Integer<LHS>, Integer<RHS>> { using type = Integer<LHS >= RHS ? LHS : RHS>; };

//lesser: min(LHS,RHS) bits

template<typename LHS, typename RHS, typename = void> struct CastLesser { using type = void; };

template<int LHS, typename RHS> struct CastLesser<Natural<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Natural<LHS <= Type<RHS>::bits ? LHS : Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastLesser<LHS, Natural<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Natural<Type<LHS>::bits <= RHS ? Type<LHS>::bits : RHS>; };
template<int LHS, int RHS> struct CastLesser<Natural<LHS>, Natural<RHS>> { using type = Natural<LHS <= RHS ? LHS : RHS>; };

template<int LHS, typename RHS> struct CastLesser<Integer<LHS>, RHS, enable_if_t<is_integral_v<RHS>>> { using type = Integer<LHS <= Type<RHS>::bits ? LHS : Type<RHS>::bits>; };
template<typename LHS, int RHS> struct CastLesser<LHS, Integer<RHS>, enable_if_t<is_integral_v<LHS>>> { using type = Integer<Type<LHS>::bits <= RHS ? Type<LHS>::bits : RHS>; };
template<int LHS, int RHS> struct CastLesser<Integer<LHS>, Integer<RHS>> { using type = Integer<LHS <= RHS ? LHS : RHS>; };

#define TP template<typename LHS, typename RHS, typename = enable_if_t<Compatible<LHS, RHS>::value>>

#define Source typename Type<LHS>::type
#define Expand typename CastExpand<LHS, RHS>::type
#define Shrink typename CastShrink<LHS, RHS>::type
#define Double typename CastDouble<LHS, RHS>::type
#define Larger typename CastLarger<LHS, RHS>::type
#define Lesser typename CastLesser<LHS, RHS>::type

#define TLE typename Type<typename CastExpand<LHS, RHS>::type>::type
#define TLN typename Type<LHS>::type
#define TRN typename Type<RHS>::type

#if 1
TP inline auto operator *(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  * (TRN)rhs}; }
TP inline auto operator /(const LHS& lhs, const RHS& rhs) -> Source { return {(TLN)lhs  / (TRN)rhs}; }
TP inline auto operator %(const LHS& lhs, const RHS& rhs) -> Source { return {(TLN)lhs  % (TRN)rhs}; }
TP inline auto operator +(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  + (TRN)rhs}; }
TP inline auto operator -(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  - (TRN)rhs}; }
TP inline auto operator<<(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs << (TRN)rhs}; }
TP inline auto operator>>(const LHS& lhs, const RHS& rhs) -> Source { return {(TLN)lhs >> (TRN)rhs}; }
TP inline auto operator &(const LHS& lhs, const RHS& rhs) -> Lesser { return {(TLN)lhs  & (TRN)rhs}; }
TP inline auto operator ^(const LHS& lhs, const RHS& rhs) -> Larger { return {(TLN)lhs  ^ (TRN)rhs}; }
TP inline auto operator |(const LHS& lhs, const RHS& rhs) -> Larger { return {(TLN)lhs  | (TRN)rhs}; }
#endif

#if 0
TP inline auto operator *(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  * (TRN)rhs}; }
TP inline auto operator /(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  / (TRN)rhs}; }
TP inline auto operator %(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  % (TRN)rhs}; }
TP inline auto operator +(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  + (TRN)rhs}; }
TP inline auto operator -(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  - (TRN)rhs}; }
TP inline auto operator<<(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs << (TRN)rhs}; }
TP inline auto operator>>(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs >> (TRN)rhs}; }
TP inline auto operator &(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  & (TRN)rhs}; }
TP inline auto operator ^(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  ^ (TRN)rhs}; }
TP inline auto operator |(const LHS& lhs, const RHS& rhs) -> Expand { return {(TLE)lhs  | (TRN)rhs}; }
#endif

#undef TP

#undef Source
#undef Expand
#undef Shrink
#undef Double
#undef Larger
#undef Lesser

#undef TLE
#undef TLN
#undef TRN

#undef Type
#undef Compatible
#undef CastExpand
#undef CastShrink
#undef CastDouble
#undef CastLarger
#undef CastLesser

}
