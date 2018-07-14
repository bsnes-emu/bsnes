#pragma once

#include <nall/hash/sha512.hpp>
#if defined(EC_REFERENCE)
  #include <nall/elliptic-curve/modulo25519-reference.hpp>
#else
  #include <nall/elliptic-curve/modulo25519.hpp>
#endif

namespace nall { namespace EllipticCurve {

struct Ed25519 {
  Ed25519() {
    field y = field(4) * field(5).reciprocal();
    field x = recoverX(y);
    point B{x, y, 1, x * y};
    for(uint n : range(253)) {
      Bscalar[n] = B;
      B = edwardsDouble(B);
    }
  }

  auto publicKey(uint256_t privateKey) const -> uint256_t {
    auto H = uint512_t{Hash::SHA512(to_vector(privateKey)).output()};
    auto a = clamp(H);
    auto A = compress(scalarMultiplyB(modL(a)));
    return A;
  }

  auto sign(const vector<uint8_t>& message, uint256_t privateKey) const -> uint512_t {
    auto H = uint512_t{Hash::SHA512(to_vector(privateKey)).output()};
    auto a = clamp(H);
    auto A = compress(scalarMultiplyB(modL(a)));

    Hash::SHA512 hash1;
    hash1.input(to_vector(upper(H)));
    hash1.input(message);
    auto r = uint512_t{hash1.output()};
    auto R = compress(scalarMultiplyB(modL(r)));

    Hash::SHA512 hash2;
    hash2.input(to_vector(R));
    hash2.input(to_vector(A));
    hash2.input(message);
    uint512_t k = modL(uint512_t{hash2.output()});
    uint256_t S = modL(k * a + r);

    return uint512_t(S) << 256 | R;
  }

  auto verify(const vector<uint8_t>& message, uint512_t signature, uint256_t publicKey) const -> bool {
    auto R = decompress(lower(signature));
    auto A = decompress(publicKey);
    if(!R || !A) return false;
    uint256_t S = upper(signature);

    Hash::SHA512 hash;
    hash.input(to_vector(lower(signature)));
    hash.input(to_vector(publicKey));
    hash.input(message);
    auto r = uint512_t{hash.output()};

    auto p = scalarMultiplyB(modL(S));
    auto q = edwardsAdd(R(), scalarMultiply(modL(r), A()));
    if(!onCurve(p) || !onCurve(q)) return false;
    if(p.x * q.z - q.x * p.z) return false;
    if(p.y * q.z - q.y * p.z) return false;
    return true;
  }

private:
  using field = Modulo25519;
  struct point { field x, y, z, t; };
  point Bscalar[253];
  const field D = -field(121665) * field(121666).reciprocal();

  inline auto clamp(uint256_t p) const -> uint256_t {
    p &= ((0_u256 - 1) >> 2) - 7;
    p |= 1_u256 << 254;
    return p;
  }

  inline auto recoverX(field y) const -> field {
    field y2 = y.square();
    field x = ((y2 - 1) * (D * y2 + 1).reciprocal()).squareRoot();
    return x() & 1 ? -x : x;
  }

  inline auto onCurve(point p) const -> bool {
    if(!p.z) return false;
    if(p.x * p.y != p.z * p.t) return false;
    if(p.y.square() - p.x.square() - p.z.square() - p.t.square() * D) return false;
    return true;
  }

  inline auto decompress(uint256_t c) const -> maybe<point> {
    field y = c & (1_u256 << 255) - 1;
    field x = recoverX(y);
    if(c >> 255) x = -x;
    point p{x, y, 1, x * y};
    if(!onCurve(p)) return nothing;
    return p;
  }

  inline auto compress(point p) const -> uint256_t {
    field r = p.z.reciprocal();
    field x = p.x * r;
    field y = p.y * r;
    return (x() & 1) << 255 | (y() & ((0_u256 - 1) >> 1));
  }

  inline auto edwardsDouble(point p) const -> point {
    field a = p.x.square();
    field b = p.y.square();
    field c = p.z.square();
    field d = -a;
    field e = (p.x + p.y).square() - a - b;
    field g = d + b;
    field f = g - (c + c);
    field h = d - b;
    return {e * f, g * h, f * g, e * h};
  }

  inline auto edwardsAdd(point p, point q) const -> point {
    field a = (p.y - p.x) * (q.y - q.x);
    field b = (p.y + p.x) * (q.y + q.x);
    field c = (p.t + p.t) * q.t * D;
    field d = (p.z + p.z) * q.z;
    field e = b - a;
    field f = d - c;
    field g = d + c;
    field h = b + a;
    return {e * f, g * h, f * g, e * h};
  }

  inline auto scalarMultiply(uint512_t e, point q) const -> point {
    point p{0, 1, 1, 0}, c;
    for(uint n : reverse(range(253))) {
      p = edwardsDouble(p);
      c = edwardsAdd(p, q);
      bool bit = e >> n & 1;
      cmove(bit, p.x, c.x);
      cmove(bit, p.y, c.y);
      cmove(bit, p.z, c.z);
      cmove(bit, p.t, c.t);
    }
    return p;
  }

  inline auto scalarMultiplyB(uint512_t e) const -> point {
    point p{0, 1, 1, 0}, c;
    for(uint n : reverse(range(253))) {
      bool bit = e >> n & 1;
      c = edwardsAdd(p, Bscalar[n]);
      cmove(bit, p.x, c.x);
      cmove(bit, p.y, c.y);
      cmove(bit, p.z, c.z);
      cmove(bit, p.t, c.t);
    }
    return p;
  }
};

}}
