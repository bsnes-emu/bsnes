#pragma once

namespace Emulator {

struct Random {
  enum class Entropy : uint { None, Low, High };

  auto entropy(Entropy entropy) -> void {
    settings.entropy = entropy;
    seed();
  }

  auto seed(maybe<uint32> seed = nothing, maybe<uint32> sequence = nothing) -> void {
    if(!seed) seed = (uint32)clock();
    if(!sequence) sequence = 0;
    seed32(seed(), sequence());
  }

  auto operator()() -> uint64 {
    return iterate64();
  }

  auto bias(uint64 bias) -> uint64 {
    if(settings.entropy == Entropy::None) return bias;
    return operator()();
  }

  auto bound(uint64 bound) -> uint64 {
    uint64 threshold = -bound % bound;
    while(true) {
      uint64 result = iterate64();
      if(result >= threshold) return result % bound;
    }
  }

  auto serialize(serializer& s) -> void {
    s.integer((uint&)settings.entropy);
    s.integer(ram.state);
    s.integer(ram.increment);
    s.integer(pcg.state);
    s.integer(pcg.increment);
  }

private:
  auto seed32(uint32 seed, uint32 sequence) -> void {
    switch(settings.entropy) {

    case Entropy::None: {
      break;
    }

    case Entropy::Low: {
      ram.state = seed;
      ram.increment = 0;
      break;
    }

    case Entropy::High: {
      pcg.state = 0;
      pcg.increment = sequence << 1 | 1;
      iterate32();
      pcg.state += seed;
      iterate32();
      break;
    }

    }
  }

  auto iterate32() -> uint32 {
    switch(settings.entropy) {

    case Entropy::None: {
      return 0;
    }

    case Entropy::Low: {
      uint64 result = 0;
      if(ram.increment.bit(4 + ram.state.bits(0,1))) result = ~0;
      ram.increment++;
      return result;
    }

    case Entropy::High: {
      uint64 state = pcg.state;
      pcg.state = state * 6364136223846793005ull + pcg.increment;
      uint32 xorshift = (state >> 18 ^ state) >> 27;
      uint32 rotate = state >> 59;
      return xorshift >> rotate | xorshift << (-rotate & 31);
    }

    }
  }

  auto iterate64() -> uint64 {
    return (uint64)iterate32() << 32 | (uint64)iterate32() << 0;
  }

  struct Settings {
    Entropy entropy = Entropy::High;
  } settings;

  struct RAM {  //Entropy::Low
    uint64 state;
    uint64 increment;
  } ram;

  struct PCG {  //Entropy::High
    uint64 state;
    uint64 increment;
  } pcg;
};

}
