#pragma once

#include <nall/arithmetic/barrett.hpp>

namespace nall { namespace EllipticCurve {

static const uint256_t P = (1_u256 << 255) - 19;
static const uint256_t L = (1_u256 << 252) + 27742317777372353535851937790883648493_u256;

static BarrettReduction modP{P};
static BarrettReduction modL{L};

struct Modulo25519 : uint256_t {
  using type = Modulo25519;
  using uint256_t::uint256_t;

  alwaysinline auto operator()() const -> uint256_t {
    return *this;
  }

  alwaysinline auto operator-() const -> type {
    return P.operator-(*this);
  }

  template<typename T> alwaysinline auto operator+(const T& rhs) const -> type {
    auto lhs = (uint512_t)*this + rhs;
    if(lhs >= P) lhs -= P;
    return lhs;
  }

  template<typename T> alwaysinline auto operator-(const T& rhs) const -> type {
    auto lhs = (uint512_t)*this;
    if(lhs < rhs) lhs += P;
    return lhs - rhs;
  }

  template<typename T> alwaysinline auto operator*(const T& rhs) const -> type {
    uint256_t hi, lo;
    nall::mul(*this, rhs, hi, lo);
    return modP(uint512_t{hi, lo});
  }

  alwaysinline auto square() const -> type {
    uint256_t hi, lo;
    nall::square(*this, hi, lo);
    return modP(uint512_t{hi, lo});
  }

  inline auto expmod(uint256_t e) const -> type {
    type x = 1;
    for(auto n : rrange(256)) {
      x = x.square();
      if(e >> n & 1) x = operator*(x);
    }
    return x;
  }

  inline auto reciprocal() const -> type {
    return expmod(P - 2);
  }

  inline auto squareRoot() const -> type {
    static const type i = type(2).expmod((P - 1) >> 2);  //i = sqrt(-1)
    type x = expmod((P + 3) >> 3);
    if(operator!=(x.square())) x = x * i;
    if(x & 1) x = -x;
    return x;
  }
};

inline auto cmove(bool bit, Modulo25519& lhs, const Modulo25519& rhs) -> void {
  if(bit) lhs = rhs;
}

inline auto cswap(bool bit, Modulo25519& lhs, Modulo25519& rhs) -> void {
  if(bit) swap(lhs, rhs);
}

}}
