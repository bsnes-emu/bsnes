#ifdef CPU_CPP

#include "disassembler.cpp"

unsigned opcode_counter = 0;

void CPU::op_unknown() {
  uint8 opcode = bus.read(--r[PC]);
  print(
    "CPU: unknown opcode [", hex<2>(opcode), "]\n",
    "af:", hex<4>(r[AF]), " bc:", hex<4>(r[BC]), " de:", hex<4>(r[DE]), " hl:", hex<4>(r[HL]), " ",
    "sp:", hex<4>(r[SP]), " pc:", hex<4>(r[PC]), "\n",
    "ly:", decimal<3, ' '>(lcd.status.ly), " exec:", opcode_counter, "\n"
  );
  while(true) scheduler.exit();
}

void CPU::op_cb() {
  uint8 opcode = op_read(r[PC]++);
  (this->*opcode_table_cb[opcode])();
}

//8-bit load commands

template<unsigned x, unsigned y> void CPU::op_ld_r_r() {
  r[x] = r[y];
}

template<unsigned x> void CPU::op_ld_r_n() {
  r[x] = op_read(r[PC]++);
}

template<unsigned x> void CPU::op_ld_r_hl() {
  r[x] = op_read(r[HL]);
}

void CPU::op_ld_hl_n() {
  op_write(r[HL], op_read(r[PC]++));
}

void CPU::op_ld_nn_a() {
  uint8 lo = op_read(r[PC]++);
  uint8 hi = op_read(r[PC]++);
  op_write((hi << 8) | (lo << 0), r[A]);
}

void CPU::op_ld_a_ffn() {
  r[A] = op_read(0xff00 + op_read(r[PC]++));
}

void CPU::op_ld_ffn_a() {
  op_write(0xff00 + op_read(r[PC]++), r[A]);
}

void CPU::op_ld_ffc_a() {
  op_write(0xff00 + r[C], r[A]);
}

void CPU::op_ldi_a_hl() {
  r[A] = op_read(r[HL]);
  r[HL]++;
}

void CPU::op_ldd_hl_a() {
  op_write(r[HL], r[A]);
  r[HL]--;
}

//16-bit load commands

template<unsigned x> void CPU::op_ld_rr_nn() {
  r[x]  = op_read(r[PC]++) << 0;
  r[x] |= op_read(r[PC]++) << 8;
}

template<unsigned x> void CPU::op_push_rr() {
  op_write(--r[SP], r[x] >> 8);
  op_write(--r[SP], r[x] >> 0);
  op_io();
}

template<unsigned x> void CPU::op_pop_rr() {
  r[x]  = op_read(r[SP]++) << 0;
  r[x] |= op_read(r[SP]++) << 8;
}

//8-bit arithmetic commands

template<unsigned x> void CPU::op_add_a_r() {
  uint16 rb = (r[A] + r[x]);
  uint16 rn = (r[A] & 0x0f) + (r[x] & 0x0f);
  r[A] = rb;
  r.f.z = (uint8)rb == 0;
  r.f.n = 0;
  r.f.h = rn > 0x0f;
  r.f.c = rb > 0xff;
}

template<unsigned x> void CPU::op_and_r() {
  r[A] &= r[x];
  r.f.z = r[A] == 0;
  r.f.n = 0;
  r.f.h = 1;
  r.f.c = 0;
}

void CPU::op_and_n() {
  r[A] &= op_read(r[PC]++);
  r.f.z = r[A] == 0;
  r.f.n = 0;
  r.f.h = 1;
  r.f.c = 0;
}

template<unsigned x> void CPU::op_xor_r() {
  r[A] ^= r[x];
  r.f.z = r[A] == 0;
  r.f.n = 0;
  r.f.h = 0;
  r.f.c = 0;
}

template<unsigned x> void CPU::op_or_r() {
  r[A] |= r[x];
  r.f.z = r[A] == 0;
  r.f.n = 0;
  r.f.h = 0;
  r.f.c = 0;
}

void CPU::op_cp_n() {
  uint8 data = op_read(r[PC]++);
  uint16 rb = (r[A] - data);
  uint16 rn = (r[A] & 0x0f) - (data & 0x0f);
  r.f.z = (uint8)rb == 0;
  r.f.n = 1;
  r.f.h = rn > 0x0f;
  r.f.c = rb > 0xff;
}

template<unsigned x> void CPU::op_inc_r() {
  r[x]++;
  r.f.z = r[x] == 0;
  r.f.n = 0;
  r.f.h = (r[x] & 0x0f) == 0x00;
}

template<unsigned x> void CPU::op_dec_r() {
  r[x]--;
  r.f.z = r[x] == 0;
  r.f.n = 1;
  r.f.h = (r[x] & 0x0f) == 0x0f;
}

void CPU::op_cpl() {
  r[A] ^= 0xff;
  r.f.n = 1;
  r.f.h = 1;
}

//16-bit arithmetic commands

template<unsigned x> void CPU::op_add_hl_rr() {
  uint32 rb = (r[HL] + r[x]);
  uint32 rn = (r[HL] & 0xfff) + (r[x] & 0xfff);
  r[HL] = rb;
  r.f.n = 0;
  r.f.h = rn > 0x0fff;
  r.f.c = rb > 0xffff;
}

template<unsigned x> void CPU::op_inc_rr() {
  r[x]++;
}

template<unsigned x> void CPU::op_dec_rr() {
  r[x]--;
}

//rotate/shift commands

template<unsigned x> void CPU::op_swap_r() {
  r[x] = (r[x] << 4) | (r[x] >> 4);
  r.f.z = r[x] == 0;
  r.f.n = 0;
  r.f.h = 0;
  r.f.c = 0;
}

//single-bit commands

template<unsigned b, unsigned x> void CPU::op_bit_n_r() {
  r.f.z = (r[x] & (1 << b)) == 0;
  r.f.n = 0;
  r.f.h = 1;
}

template<unsigned b, unsigned x> void CPU::op_set_n_r() {
  r[x] |= 1 << b;
}

template<unsigned b, unsigned x> void CPU::op_res_n_r() {
  r[x] &= ~(1 << b);
}

//control commands

void CPU::op_nop() {
}

void CPU::op_di() {
  status.ime = 0;
}

void CPU::op_ei() {
  status.ime = 1;
}

//jump commands

void CPU::op_jp_nn() {
  uint8 lo = op_read(r[PC]++);
  uint8 hi = op_read(r[PC]++);
  r[PC] = (hi << 8) | (lo << 0);
  op_io();
}

void CPU::op_jp_hl() {
  r[PC] = r[HL];
}

template<unsigned x, bool y> void CPU::op_jr_f_n() {
  int8 n = op_read(r[PC]++);
  if(r.f[x] == y) {
    r[PC] += n;
    op_io();
  }
}

void CPU::op_call_nn() {
  uint16 dest = r[PC] + 2;
  op_write(--r[SP], dest >> 8);
  op_write(--r[SP], dest >> 0);
  uint8 lo = op_read(r[PC]++);
  uint8 hi = op_read(r[PC]++);
  r[PC] = (hi << 8) | (lo << 0);
  op_io();
}

void CPU::op_ret() {
  uint8 lo = op_read(r[SP]++);
  uint8 hi = op_read(r[SP]++);
  r[PC] = (hi << 8) | (lo << 0);
  op_io();
}

template<unsigned n> void CPU::op_rst_n() {
  op_write(--r[SP], r[PC] >> 8);
  op_write(--r[SP], r[PC] >> 0);
  r[PC] = n;
  op_io();
}

//opcode tables

void CPU::initialize_opcode_table() {
  for(unsigned n = 0; n < 256; n++) opcode_table[n] = &CPU::op_unknown;
  for(unsigned n = 0; n < 256; n++) opcode_table_cb[n] = &CPU::op_unknown;

  opcode_table[0x00] = &CPU::op_nop;
  opcode_table[0x01] = &CPU::op_ld_rr_nn<BC>;
  opcode_table[0x03] = &CPU::op_inc_rr<BC>;
  opcode_table[0x05] = &CPU::op_dec_r<B>;
  opcode_table[0x06] = &CPU::op_ld_r_n<B>;
  opcode_table[0x09] = &CPU::op_add_hl_rr<BC>;
  opcode_table[0x0b] = &CPU::op_dec_rr<BC>;
  opcode_table[0x0c] = &CPU::op_inc_r<C>;
  opcode_table[0x0d] = &CPU::op_dec_r<C>;
  opcode_table[0x0e] = &CPU::op_ld_r_n<C>;
  opcode_table[0x11] = &CPU::op_ld_rr_nn<DE>;
  opcode_table[0x13] = &CPU::op_inc_rr<DE>;
  opcode_table[0x16] = &CPU::op_ld_r_n<D>;
  opcode_table[0x19] = &CPU::op_add_hl_rr<DE>;
  opcode_table[0x1b] = &CPU::op_dec_rr<DE>;
  opcode_table[0x1e] = &CPU::op_ld_r_n<E>;
  opcode_table[0x20] = &CPU::op_jr_f_n<ZF, 0>;
  opcode_table[0x21] = &CPU::op_ld_rr_nn<HL>;
  opcode_table[0x23] = &CPU::op_inc_rr<HL>;
  opcode_table[0x26] = &CPU::op_ld_r_n<H>;
  opcode_table[0x29] = &CPU::op_add_hl_rr<HL>;
  opcode_table[0x2a] = &CPU::op_ldi_a_hl;
  opcode_table[0x2b] = &CPU::op_dec_rr<HL>;
  opcode_table[0x2e] = &CPU::op_ld_r_n<L>;
  opcode_table[0x2f] = &CPU::op_cpl;
  opcode_table[0x31] = &CPU::op_ld_rr_nn<SP>;
  opcode_table[0x32] = &CPU::op_ldd_hl_a;
  opcode_table[0x33] = &CPU::op_inc_rr<SP>;
  opcode_table[0x36] = &CPU::op_ld_hl_n;
  opcode_table[0x39] = &CPU::op_add_hl_rr<SP>;
  opcode_table[0x3b] = &CPU::op_dec_rr<SP>;
  opcode_table[0x3e] = &CPU::op_ld_r_n<A>;
  opcode_table[0x40] = &CPU::op_ld_r_r<B, B>;
  opcode_table[0x41] = &CPU::op_ld_r_r<B, C>;
  opcode_table[0x42] = &CPU::op_ld_r_r<B, D>;
  opcode_table[0x43] = &CPU::op_ld_r_r<B, E>;
  opcode_table[0x44] = &CPU::op_ld_r_r<B, H>;
  opcode_table[0x45] = &CPU::op_ld_r_r<B, L>;
  opcode_table[0x46] = &CPU::op_ld_r_hl<B>;
  opcode_table[0x47] = &CPU::op_ld_r_r<B, A>;
  opcode_table[0x48] = &CPU::op_ld_r_r<C, B>;
  opcode_table[0x49] = &CPU::op_ld_r_r<C, C>;
  opcode_table[0x4a] = &CPU::op_ld_r_r<C, D>;
  opcode_table[0x4b] = &CPU::op_ld_r_r<C, E>;
  opcode_table[0x4c] = &CPU::op_ld_r_r<C, H>;
  opcode_table[0x4d] = &CPU::op_ld_r_r<C, L>;
  opcode_table[0x4e] = &CPU::op_ld_r_hl<C>;
  opcode_table[0x4f] = &CPU::op_ld_r_r<C, A>;
  opcode_table[0x50] = &CPU::op_ld_r_r<D, B>;
  opcode_table[0x51] = &CPU::op_ld_r_r<D, C>;
  opcode_table[0x52] = &CPU::op_ld_r_r<D, D>;
  opcode_table[0x53] = &CPU::op_ld_r_r<D, E>;
  opcode_table[0x54] = &CPU::op_ld_r_r<D, H>;
  opcode_table[0x55] = &CPU::op_ld_r_r<D, L>;
  opcode_table[0x56] = &CPU::op_ld_r_hl<D>;
  opcode_table[0x57] = &CPU::op_ld_r_r<D, A>;
  opcode_table[0x58] = &CPU::op_ld_r_r<E, B>;
  opcode_table[0x59] = &CPU::op_ld_r_r<E, C>;
  opcode_table[0x5a] = &CPU::op_ld_r_r<E, D>;
  opcode_table[0x5b] = &CPU::op_ld_r_r<E, E>;
  opcode_table[0x5c] = &CPU::op_ld_r_r<E, H>;
  opcode_table[0x5d] = &CPU::op_ld_r_r<E, L>;
  opcode_table[0x5e] = &CPU::op_ld_r_hl<E>;
  opcode_table[0x5f] = &CPU::op_ld_r_r<E, A>;
  opcode_table[0x60] = &CPU::op_ld_r_r<H, B>;
  opcode_table[0x61] = &CPU::op_ld_r_r<H, C>;
  opcode_table[0x62] = &CPU::op_ld_r_r<H, D>;
  opcode_table[0x63] = &CPU::op_ld_r_r<H, E>;
  opcode_table[0x64] = &CPU::op_ld_r_r<H, H>;
  opcode_table[0x65] = &CPU::op_ld_r_r<H, L>;
  opcode_table[0x66] = &CPU::op_ld_r_hl<H>;
  opcode_table[0x67] = &CPU::op_ld_r_r<H, A>;
  opcode_table[0x68] = &CPU::op_ld_r_r<L, B>;
  opcode_table[0x69] = &CPU::op_ld_r_r<L, C>;
  opcode_table[0x6a] = &CPU::op_ld_r_r<L, D>;
  opcode_table[0x6b] = &CPU::op_ld_r_r<L, E>;
  opcode_table[0x6c] = &CPU::op_ld_r_r<L, H>;
  opcode_table[0x6d] = &CPU::op_ld_r_r<L, L>;
  opcode_table[0x6e] = &CPU::op_ld_r_hl<L>;
  opcode_table[0x6f] = &CPU::op_ld_r_r<L, A>;
  opcode_table[0x78] = &CPU::op_ld_r_r<A, B>;
  opcode_table[0x79] = &CPU::op_ld_r_r<A, C>;
  opcode_table[0x7a] = &CPU::op_ld_r_r<A, D>;
  opcode_table[0x7b] = &CPU::op_ld_r_r<A, E>;
  opcode_table[0x7c] = &CPU::op_ld_r_r<A, H>;
  opcode_table[0x7d] = &CPU::op_ld_r_r<A, L>;
  opcode_table[0x7e] = &CPU::op_ld_r_hl<E>;
  opcode_table[0x7f] = &CPU::op_ld_r_r<A, A>;
  opcode_table[0x80] = &CPU::op_add_a_r<B>;
  opcode_table[0x81] = &CPU::op_add_a_r<C>;
  opcode_table[0x82] = &CPU::op_add_a_r<D>;
  opcode_table[0x83] = &CPU::op_add_a_r<E>;
  opcode_table[0x84] = &CPU::op_add_a_r<H>;
  opcode_table[0x85] = &CPU::op_add_a_r<L>;
  opcode_table[0x87] = &CPU::op_add_a_r<A>;
  opcode_table[0xa0] = &CPU::op_and_r<B>;
  opcode_table[0xa1] = &CPU::op_and_r<C>;
  opcode_table[0xa2] = &CPU::op_and_r<D>;
  opcode_table[0xa3] = &CPU::op_and_r<E>;
  opcode_table[0xa4] = &CPU::op_and_r<H>;
  opcode_table[0xa5] = &CPU::op_and_r<L>;
  opcode_table[0xa7] = &CPU::op_and_r<A>;
  opcode_table[0xa8] = &CPU::op_xor_r<B>;
  opcode_table[0xa9] = &CPU::op_xor_r<C>;
  opcode_table[0xaa] = &CPU::op_xor_r<D>;
  opcode_table[0xab] = &CPU::op_xor_r<E>;
  opcode_table[0xac] = &CPU::op_xor_r<H>;
  opcode_table[0xad] = &CPU::op_xor_r<L>;
  opcode_table[0xaf] = &CPU::op_xor_r<A>;
  opcode_table[0xb0] = &CPU::op_or_r<B>;
  opcode_table[0xb1] = &CPU::op_or_r<C>;
  opcode_table[0xb2] = &CPU::op_or_r<D>;
  opcode_table[0xb3] = &CPU::op_or_r<E>;
  opcode_table[0xb4] = &CPU::op_or_r<H>;
  opcode_table[0xb5] = &CPU::op_or_r<L>;
  opcode_table[0xb7] = &CPU::op_or_r<A>;
  opcode_table[0xc1] = &CPU::op_pop_rr<BC>;
  opcode_table[0xc3] = &CPU::op_jp_nn;
  opcode_table[0xc5] = &CPU::op_push_rr<BC>;
  opcode_table[0xc7] = &CPU::op_rst_n<0x00>;
  opcode_table[0xc9] = &CPU::op_ret;
  opcode_table[0xcb] = &CPU::op_cb;
  opcode_table[0xcd] = &CPU::op_call_nn;
  opcode_table[0xcf] = &CPU::op_rst_n<0x08>;
  opcode_table[0xd1] = &CPU::op_pop_rr<DE>;
  opcode_table[0xd5] = &CPU::op_push_rr<DE>;
  opcode_table[0xd7] = &CPU::op_rst_n<0x10>;
  opcode_table[0xdf] = &CPU::op_rst_n<0x18>;
  opcode_table[0xe0] = &CPU::op_ld_ffn_a;
  opcode_table[0xe1] = &CPU::op_pop_rr<HL>;
  opcode_table[0xe2] = &CPU::op_ld_ffc_a;
  opcode_table[0xe5] = &CPU::op_push_rr<HL>;
  opcode_table[0xe6] = &CPU::op_and_n;
  opcode_table[0xe7] = &CPU::op_rst_n<0x20>;
  opcode_table[0xe9] = &CPU::op_jp_hl;
  opcode_table[0xea] = &CPU::op_ld_nn_a;
  opcode_table[0xef] = &CPU::op_rst_n<0x28>;
  opcode_table[0xf0] = &CPU::op_ld_a_ffn;
  opcode_table[0xf1] = &CPU::op_pop_rr<AF>;
  opcode_table[0xf3] = &CPU::op_di;
  opcode_table[0xf5] = &CPU::op_push_rr<AF>;
  opcode_table[0xf7] = &CPU::op_rst_n<0x30>;
  opcode_table[0xfb] = &CPU::op_ei;
  opcode_table[0xfe] = &CPU::op_cp_n;
  opcode_table[0xff] = &CPU::op_rst_n<0x38>;

  opcode_table_cb[0x37] = &CPU::op_swap_r<A>;
  opcode_table_cb[0x40] = &CPU::op_bit_n_r<0, B>;
  opcode_table_cb[0x41] = &CPU::op_bit_n_r<0, C>;
  opcode_table_cb[0x42] = &CPU::op_bit_n_r<0, D>;
  opcode_table_cb[0x43] = &CPU::op_bit_n_r<0, E>;
  opcode_table_cb[0x44] = &CPU::op_bit_n_r<0, H>;
  opcode_table_cb[0x45] = &CPU::op_bit_n_r<0, L>;
  opcode_table_cb[0x47] = &CPU::op_bit_n_r<0, A>;
  opcode_table_cb[0x48] = &CPU::op_bit_n_r<1, B>;
  opcode_table_cb[0x49] = &CPU::op_bit_n_r<1, C>;
  opcode_table_cb[0x4a] = &CPU::op_bit_n_r<1, D>;
  opcode_table_cb[0x4b] = &CPU::op_bit_n_r<1, E>;
  opcode_table_cb[0x4c] = &CPU::op_bit_n_r<1, H>;
  opcode_table_cb[0x4d] = &CPU::op_bit_n_r<1, L>;
  opcode_table_cb[0x4f] = &CPU::op_bit_n_r<1, A>;
  opcode_table_cb[0x50] = &CPU::op_bit_n_r<2, B>;
  opcode_table_cb[0x51] = &CPU::op_bit_n_r<2, C>;
  opcode_table_cb[0x52] = &CPU::op_bit_n_r<2, D>;
  opcode_table_cb[0x53] = &CPU::op_bit_n_r<2, E>;
  opcode_table_cb[0x54] = &CPU::op_bit_n_r<2, H>;
  opcode_table_cb[0x55] = &CPU::op_bit_n_r<2, L>;
  opcode_table_cb[0x57] = &CPU::op_bit_n_r<2, A>;
  opcode_table_cb[0x58] = &CPU::op_bit_n_r<3, B>;
  opcode_table_cb[0x59] = &CPU::op_bit_n_r<3, C>;
  opcode_table_cb[0x5a] = &CPU::op_bit_n_r<3, D>;
  opcode_table_cb[0x5b] = &CPU::op_bit_n_r<3, E>;
  opcode_table_cb[0x5c] = &CPU::op_bit_n_r<3, H>;
  opcode_table_cb[0x5d] = &CPU::op_bit_n_r<3, L>;
  opcode_table_cb[0x5f] = &CPU::op_bit_n_r<3, A>;
  opcode_table_cb[0x60] = &CPU::op_bit_n_r<4, B>;
  opcode_table_cb[0x61] = &CPU::op_bit_n_r<4, C>;
  opcode_table_cb[0x62] = &CPU::op_bit_n_r<4, D>;
  opcode_table_cb[0x63] = &CPU::op_bit_n_r<4, E>;
  opcode_table_cb[0x64] = &CPU::op_bit_n_r<4, H>;
  opcode_table_cb[0x65] = &CPU::op_bit_n_r<4, L>;
  opcode_table_cb[0x67] = &CPU::op_bit_n_r<4, A>;
  opcode_table_cb[0x68] = &CPU::op_bit_n_r<5, B>;
  opcode_table_cb[0x69] = &CPU::op_bit_n_r<5, C>;
  opcode_table_cb[0x6a] = &CPU::op_bit_n_r<5, D>;
  opcode_table_cb[0x6b] = &CPU::op_bit_n_r<5, E>;
  opcode_table_cb[0x6c] = &CPU::op_bit_n_r<5, H>;
  opcode_table_cb[0x6d] = &CPU::op_bit_n_r<5, L>;
  opcode_table_cb[0x6f] = &CPU::op_bit_n_r<5, A>;
  opcode_table_cb[0x70] = &CPU::op_bit_n_r<6, B>;
  opcode_table_cb[0x71] = &CPU::op_bit_n_r<6, C>;
  opcode_table_cb[0x72] = &CPU::op_bit_n_r<6, D>;
  opcode_table_cb[0x73] = &CPU::op_bit_n_r<6, E>;
  opcode_table_cb[0x74] = &CPU::op_bit_n_r<6, H>;
  opcode_table_cb[0x75] = &CPU::op_bit_n_r<6, L>;
  opcode_table_cb[0x77] = &CPU::op_bit_n_r<6, A>;
  opcode_table_cb[0x78] = &CPU::op_bit_n_r<7, B>;
  opcode_table_cb[0x79] = &CPU::op_bit_n_r<7, C>;
  opcode_table_cb[0x7a] = &CPU::op_bit_n_r<7, D>;
  opcode_table_cb[0x7b] = &CPU::op_bit_n_r<7, E>;
  opcode_table_cb[0x7c] = &CPU::op_bit_n_r<7, H>;
  opcode_table_cb[0x7d] = &CPU::op_bit_n_r<7, L>;
  opcode_table_cb[0x7f] = &CPU::op_bit_n_r<7, A>;
  opcode_table_cb[0x80] = &CPU::op_res_n_r<0, B>;
  opcode_table_cb[0x81] = &CPU::op_res_n_r<0, C>;
  opcode_table_cb[0x82] = &CPU::op_res_n_r<0, D>;
  opcode_table_cb[0x83] = &CPU::op_res_n_r<0, E>;
  opcode_table_cb[0x84] = &CPU::op_res_n_r<0, H>;
  opcode_table_cb[0x85] = &CPU::op_res_n_r<0, L>;
  opcode_table_cb[0x87] = &CPU::op_res_n_r<0, A>;
  opcode_table_cb[0x88] = &CPU::op_res_n_r<1, B>;
  opcode_table_cb[0x89] = &CPU::op_res_n_r<1, C>;
  opcode_table_cb[0x8a] = &CPU::op_res_n_r<1, D>;
  opcode_table_cb[0x8b] = &CPU::op_res_n_r<1, E>;
  opcode_table_cb[0x8c] = &CPU::op_res_n_r<1, H>;
  opcode_table_cb[0x8d] = &CPU::op_res_n_r<1, L>;
  opcode_table_cb[0x8f] = &CPU::op_res_n_r<1, A>;
  opcode_table_cb[0x90] = &CPU::op_res_n_r<2, B>;
  opcode_table_cb[0x91] = &CPU::op_res_n_r<2, C>;
  opcode_table_cb[0x92] = &CPU::op_res_n_r<2, D>;
  opcode_table_cb[0x93] = &CPU::op_res_n_r<2, E>;
  opcode_table_cb[0x94] = &CPU::op_res_n_r<2, H>;
  opcode_table_cb[0x95] = &CPU::op_res_n_r<2, L>;
  opcode_table_cb[0x97] = &CPU::op_res_n_r<2, A>;
  opcode_table_cb[0x98] = &CPU::op_res_n_r<3, B>;
  opcode_table_cb[0x99] = &CPU::op_res_n_r<3, C>;
  opcode_table_cb[0x9a] = &CPU::op_res_n_r<3, D>;
  opcode_table_cb[0x9b] = &CPU::op_res_n_r<3, E>;
  opcode_table_cb[0x9c] = &CPU::op_res_n_r<3, H>;
  opcode_table_cb[0x9d] = &CPU::op_res_n_r<3, L>;
  opcode_table_cb[0x9f] = &CPU::op_res_n_r<3, A>;
  opcode_table_cb[0xa0] = &CPU::op_res_n_r<4, B>;
  opcode_table_cb[0xa1] = &CPU::op_res_n_r<4, C>;
  opcode_table_cb[0xa2] = &CPU::op_res_n_r<4, D>;
  opcode_table_cb[0xa3] = &CPU::op_res_n_r<4, E>;
  opcode_table_cb[0xa4] = &CPU::op_res_n_r<4, H>;
  opcode_table_cb[0xa5] = &CPU::op_res_n_r<4, L>;
  opcode_table_cb[0xa7] = &CPU::op_res_n_r<4, A>;
  opcode_table_cb[0xa8] = &CPU::op_res_n_r<5, B>;
  opcode_table_cb[0xa9] = &CPU::op_res_n_r<5, C>;
  opcode_table_cb[0xaa] = &CPU::op_res_n_r<5, D>;
  opcode_table_cb[0xab] = &CPU::op_res_n_r<5, E>;
  opcode_table_cb[0xac] = &CPU::op_res_n_r<5, H>;
  opcode_table_cb[0xad] = &CPU::op_res_n_r<5, L>;
  opcode_table_cb[0xaf] = &CPU::op_res_n_r<5, A>;
  opcode_table_cb[0xb0] = &CPU::op_res_n_r<6, B>;
  opcode_table_cb[0xb1] = &CPU::op_res_n_r<6, C>;
  opcode_table_cb[0xb2] = &CPU::op_res_n_r<6, D>;
  opcode_table_cb[0xb3] = &CPU::op_res_n_r<6, E>;
  opcode_table_cb[0xb4] = &CPU::op_res_n_r<6, H>;
  opcode_table_cb[0xb5] = &CPU::op_res_n_r<6, L>;
  opcode_table_cb[0xb7] = &CPU::op_res_n_r<6, A>;
  opcode_table_cb[0xb8] = &CPU::op_res_n_r<7, B>;
  opcode_table_cb[0xb9] = &CPU::op_res_n_r<7, C>;
  opcode_table_cb[0xba] = &CPU::op_res_n_r<7, D>;
  opcode_table_cb[0xbb] = &CPU::op_res_n_r<7, E>;
  opcode_table_cb[0xbc] = &CPU::op_res_n_r<7, H>;
  opcode_table_cb[0xbd] = &CPU::op_res_n_r<7, L>;
  opcode_table_cb[0xbf] = &CPU::op_res_n_r<7, A>;
  opcode_table_cb[0xc0] = &CPU::op_set_n_r<0, B>;
  opcode_table_cb[0xc1] = &CPU::op_set_n_r<0, C>;
  opcode_table_cb[0xc2] = &CPU::op_set_n_r<0, D>;
  opcode_table_cb[0xc3] = &CPU::op_set_n_r<0, E>;
  opcode_table_cb[0xc4] = &CPU::op_set_n_r<0, H>;
  opcode_table_cb[0xc5] = &CPU::op_set_n_r<0, L>;
  opcode_table_cb[0xc7] = &CPU::op_set_n_r<0, A>;
  opcode_table_cb[0xc8] = &CPU::op_set_n_r<1, B>;
  opcode_table_cb[0xc9] = &CPU::op_set_n_r<1, C>;
  opcode_table_cb[0xca] = &CPU::op_set_n_r<1, D>;
  opcode_table_cb[0xcb] = &CPU::op_set_n_r<1, E>;
  opcode_table_cb[0xcc] = &CPU::op_set_n_r<1, H>;
  opcode_table_cb[0xcd] = &CPU::op_set_n_r<1, L>;
  opcode_table_cb[0xcf] = &CPU::op_set_n_r<1, A>;
  opcode_table_cb[0xd0] = &CPU::op_set_n_r<2, B>;
  opcode_table_cb[0xd1] = &CPU::op_set_n_r<2, C>;
  opcode_table_cb[0xd2] = &CPU::op_set_n_r<2, D>;
  opcode_table_cb[0xd3] = &CPU::op_set_n_r<2, E>;
  opcode_table_cb[0xd4] = &CPU::op_set_n_r<2, H>;
  opcode_table_cb[0xd5] = &CPU::op_set_n_r<2, L>;
  opcode_table_cb[0xd7] = &CPU::op_set_n_r<2, A>;
  opcode_table_cb[0xd8] = &CPU::op_set_n_r<3, B>;
  opcode_table_cb[0xd9] = &CPU::op_set_n_r<3, C>;
  opcode_table_cb[0xda] = &CPU::op_set_n_r<3, D>;
  opcode_table_cb[0xdb] = &CPU::op_set_n_r<3, E>;
  opcode_table_cb[0xdc] = &CPU::op_set_n_r<3, H>;
  opcode_table_cb[0xdd] = &CPU::op_set_n_r<3, L>;
  opcode_table_cb[0xdf] = &CPU::op_set_n_r<3, A>;
  opcode_table_cb[0xe0] = &CPU::op_set_n_r<4, B>;
  opcode_table_cb[0xe1] = &CPU::op_set_n_r<4, C>;
  opcode_table_cb[0xe2] = &CPU::op_set_n_r<4, D>;
  opcode_table_cb[0xe3] = &CPU::op_set_n_r<4, E>;
  opcode_table_cb[0xe4] = &CPU::op_set_n_r<4, H>;
  opcode_table_cb[0xe5] = &CPU::op_set_n_r<4, L>;
  opcode_table_cb[0xe7] = &CPU::op_set_n_r<4, A>;
  opcode_table_cb[0xe8] = &CPU::op_set_n_r<5, B>;
  opcode_table_cb[0xe9] = &CPU::op_set_n_r<5, C>;
  opcode_table_cb[0xea] = &CPU::op_set_n_r<5, D>;
  opcode_table_cb[0xeb] = &CPU::op_set_n_r<5, E>;
  opcode_table_cb[0xec] = &CPU::op_set_n_r<5, H>;
  opcode_table_cb[0xed] = &CPU::op_set_n_r<5, L>;
  opcode_table_cb[0xef] = &CPU::op_set_n_r<5, A>;
  opcode_table_cb[0xf0] = &CPU::op_set_n_r<6, B>;
  opcode_table_cb[0xf1] = &CPU::op_set_n_r<6, C>;
  opcode_table_cb[0xf2] = &CPU::op_set_n_r<6, D>;
  opcode_table_cb[0xf3] = &CPU::op_set_n_r<6, E>;
  opcode_table_cb[0xf4] = &CPU::op_set_n_r<6, H>;
  opcode_table_cb[0xf5] = &CPU::op_set_n_r<6, L>;
  opcode_table_cb[0xf7] = &CPU::op_set_n_r<6, A>;
  opcode_table_cb[0xf8] = &CPU::op_set_n_r<7, B>;
  opcode_table_cb[0xf9] = &CPU::op_set_n_r<7, C>;
  opcode_table_cb[0xfa] = &CPU::op_set_n_r<7, D>;
  opcode_table_cb[0xfb] = &CPU::op_set_n_r<7, E>;
  opcode_table_cb[0xfc] = &CPU::op_set_n_r<7, H>;
  opcode_table_cb[0xfd] = &CPU::op_set_n_r<7, L>;
  opcode_table_cb[0xff] = &CPU::op_set_n_r<7, A>;

  unsigned missing = 0;
  for(unsigned n = 0; n < 256; n++) {
    if(opcode_table[n] == &CPU::op_unknown) missing++;
    if(opcode_table_cb[n] == &CPU::op_unknown) missing++;
  }

  print("CPU opcodes: ", 512 - missing, " implemented, ", missing, " remaining.\n");
}

#endif
