#ifndef NALL_RANDOM_HPP
#define NALL_RANDOM_HPP

namespace nall {
  //pseudo-random number generator
  inline unsigned prng() {
    static unsigned n = 0;
    return n = (n >> 1) ^ (((n & 1) - 1) & 0xedb88320);
  }

  struct random_cyclic {
    unsigned seed;
    inline unsigned operator()() {
      return seed = (seed >> 1) ^ (((seed & 1) - 1) & 0xedb88320);
    }
    random_cyclic() : seed(0) {}
  };
}

#endif
