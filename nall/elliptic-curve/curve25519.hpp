#pragma once

#if defined(EC_REFERENCE)
  #include <nall/elliptic-curve/modulo25519-reference.hpp>
#else
  #include <nall/elliptic-curve/modulo25519.hpp>
#endif

namespace nall { namespace EllipticCurve {

struct Curve25519 {
  auto sharedKey(uint256_t secretKey, uint256_t basepoint = 9) const -> uint256_t {
    secretKey &= ((0_u256 - 1) >> 2) - 7;
    secretKey |= 1_u256 << 254;
    basepoint &= (0_u256 - 1) >> 1;

    point p = scalarMultiply(secretKey, modP(basepoint));
    return p.x * p.z.reciprocal();
  }

private:
  using field = Modulo25519;
  struct point { field x, z; };

  inline auto montgomeryAdd(point p, point q, field b) const -> point {
    return {
      (p.x * q.x - p.z * q.z).square(),
      (p.x * q.z - p.z * q.x).square() * b
    };
  }

  inline auto montgomeryDouble(point p) const -> point {
    field a = (p.x + p.z).square();
    field b = (p.x - p.z).square();
    field c = a - b;
    field d = a + c * 121665;
    return {a * b, c * d};
  }

  inline auto scalarMultiply(uint256_t e, field b) const -> point {
    point p{1, 0}, q{b, 1};
    for(uint n : rrange(255)) {
      bool bit = e >> n & 1;
      cswap(bit, p.x, q.x);
      cswap(bit, p.z, q.z);
      q = montgomeryAdd(p, q, b);
      p = montgomeryDouble(p);
      cswap(bit, p.x, q.x);
      cswap(bit, p.z, q.z);
    }
    return p;
  }
};

}}
