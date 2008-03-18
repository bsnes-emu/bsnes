#ifdef BPPU_CPP

/*****
 * Color Addition / Subtraction
 * Thanks to blargg for the optimized algorithms
 *****/
inline uint16 bPPU::addsub(uint32 x, uint32 y, bool halve) {
  if(!regs.color_mode) {
    if(!halve) {
    uint sum   = x + y;
    uint carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {
  uint diff   = x - y + 0x8420;
  uint borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return   (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

#endif //ifdef BPPU_CPP
