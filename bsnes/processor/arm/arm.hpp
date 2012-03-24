#ifndef PROCESSOR_ARM_HPP
#define PROCESSOR_ARM_HPP

namespace Processor {

//ARMv3, ARMv4TM

struct ARM {
  enum : unsigned { Byte = 8, Half = 16, Word = 32 };
  #include "registers.hpp"
  #include "instructions-arm.hpp"
  #include "instructions-thumb.hpp"
  #include "disassembler.hpp"
  virtual void step(unsigned clocks) = 0;
  virtual uint32 bus_read(uint32 addr, uint32 size) = 0;
  virtual void bus_write(uint32 addr, uint32 size, uint32 word) = 0;

  void power();
  void vector(uint32 addr, Processor::Mode mode);

  void serialize(serializer&);
};

}

#endif
