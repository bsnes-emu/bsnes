struct ARM {
  #include "registers.hpp"
  #include "arm.hpp"
  #include "thumb.hpp"
  #include "disassembler.hpp"
  virtual void step(unsigned clocks) = 0;

  void power();
};
