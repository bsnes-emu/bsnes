#pragma once

namespace nall {

struct BarrettReduction {
  BarrettReduction(uint256_t modulo) : modulo(modulo), factor((1_u1024 << 512) / modulo) {}

  //return = value % modulo
  inline auto operator()(uint512_t value) const -> uint256_t {
    uint512_t hi, lo;
    nall::mul(value, factor, hi, lo);
    uint512_t remainder = value - hi * modulo;
    return remainder < modulo ? remainder : remainder - modulo;
  }

private:
  const uint512_t modulo;
  const uint512_t factor;
};

}
