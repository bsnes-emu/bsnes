#include "registers.hpp"
void (CPU::*opcode_table[256])();
void initialize_opcode_table();

void op_unknown();

//8-bit load commands
template<unsigned x> void op_ld_r_n();
void op_ldd_hl_a();

//16-bit load commands
template<unsigned x, unsigned y> void op_ld_rr_nn();

//8-bit arithmetic commands
template<unsigned x> void op_xor_r();
template<unsigned x> void op_dec_r();

//control commands
void op_nop();

//jump commands
void op_jp_nn();
template<unsigned x, bool y> void op_jr_f_n();

//disassembler.cpp
string disassemble(uint16 pc);
string disassemble_opcode(uint16 pc);
