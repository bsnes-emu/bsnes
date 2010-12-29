#include "registers.hpp"
void (CPU::*opcode_table[256])();
void (CPU::*opcode_table_cb[256])();
void initialize_opcode_table();

void op_unknown();

void op_cb();

//8-bit load commands
template<unsigned x, unsigned y> void op_ld_r_r();
template<unsigned x> void op_ld_r_n();
template<unsigned x> void op_ld_r_hl();
void op_ld_hl_n();
void op_ld_nn_a();
void op_ld_a_ffn();
void op_ld_ffn_a();
void op_ld_ffc_a();
void op_ldi_a_hl();
void op_ldd_hl_a();

//16-bit load commands
template<unsigned x> void op_ld_rr_nn();
template<unsigned x> void op_push_rr();
template<unsigned x> void op_pop_rr();

//8-bit arithmetic commands
template<unsigned x> void op_add_a_r();
template<unsigned x> void op_and_r();
void op_and_n();
template<unsigned x> void op_xor_r();
template<unsigned x> void op_or_r();
void op_cp_n();
template<unsigned x> void op_inc_r();
template<unsigned x> void op_dec_r();
void op_cpl();

//16-bit arithmetic commands
template<unsigned x> void op_add_hl_rr();
template<unsigned x> void op_inc_rr();
template<unsigned x> void op_dec_rr();

//rotate/shift commands
template<unsigned x> void op_swap_r();

//single-bit commands
template<unsigned b, unsigned x> void op_bit_n_r();
template<unsigned b, unsigned x> void op_set_n_r();
template<unsigned b, unsigned x> void op_res_n_r();

//control commands
void op_nop();
void op_di();
void op_ei();

//jump commands
void op_jp_nn();
void op_jp_hl();
template<unsigned x, bool y> void op_jr_f_n();
void op_call_nn();
void op_ret();
template<unsigned n> void op_rst_n();

//disassembler.cpp
string disassemble(uint16 pc);
string disassemble_opcode(uint16 pc);
string disassemble_opcode_cb(uint16 pc);
