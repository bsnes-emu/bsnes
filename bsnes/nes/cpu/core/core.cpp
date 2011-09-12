#define call(op) (this->*op)()
#define L interrupt_test();

#include "interrupt.cpp"
#include "opcodes.cpp"
#include "exec.cpp"
#include "disassembler.cpp"

#undef L
#undef call
