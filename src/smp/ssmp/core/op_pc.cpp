#ifdef SSMP_CPP

//bra
case 0x2f: {
  rd = op_readpc();
  if(0)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//beq
case 0xf0: {
  rd = op_readpc();
  if(!regs.p.z)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bne
case 0xd0: {
  rd = op_readpc();
  if(regs.p.z)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bcs
case 0xb0: {
  rd = op_readpc();
  if(!regs.p.c)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bcc
case 0x90: {
  rd = op_readpc();
  if(regs.p.c)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bvs
case 0x70: {
  rd = op_readpc();
  if(!regs.p.v)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bvc
case 0x50: {
  rd = op_readpc();
  if(regs.p.v)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bmi
case 0x30: {
  rd = op_readpc();
  if(!regs.p.n)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bpl
case 0x10: {
  rd = op_readpc();
  if(regs.p.n)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs0
case 0x03: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x01) != 0x01)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc0
case 0x13: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x01) == 0x01)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs1
case 0x23: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x02) != 0x02)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc1
case 0x33: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x02) == 0x02)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs2
case 0x43: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x04) != 0x04)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc2
case 0x53: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x04) == 0x04)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs3
case 0x63: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x08) != 0x08)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc3
case 0x73: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x08) == 0x08)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs4
case 0x83: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x10) != 0x10)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc4
case 0x93: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x10) == 0x10)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs5
case 0xa3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x20) != 0x20)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc5
case 0xb3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x20) == 0x20)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs6
case 0xc3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x40) != 0x40)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc6
case 0xd3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x40) == 0x40)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbs7
case 0xe3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x80) != 0x80)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//bbc7
case 0xf3: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x80) == 0x80)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//cbne_dp
case 0x2e: {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if(regs.a == sp)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//cbne_dpx
case 0xde: {
  dp = op_readpc();
  op_io();
  sp = op_readdp(dp + regs.x);
  rd = op_readpc();
  op_io();
  if(regs.a == sp)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//dbnz_dp
case 0x6e: {
  dp = op_readpc();
  wr = op_readdp(dp);
  op_writedp(dp, --wr);
  rd = op_readpc();
  if(wr == 0x00)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//dbnz_y
case 0xfe: {
  rd = op_readpc();
  op_io();
  regs.y--;
  op_io();
  if(regs.y == 0x00)break;
  op_io();
  op_io();
  regs.pc += (int8)rd;
} break;

//jmp_addr
case 0x5f: {
  rd  = op_readpc();
  rd |= op_readpc() << 8;
  regs.pc = rd;
} break;

//jmp_iaddrx
case 0x1f: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  dp += regs.x;
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  regs.pc = rd;
} break;

//call
case 0x3f: {
  rd  = op_readpc();
  rd |= op_readpc() << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//pcall
case 0x4f: {
  rd = op_readpc();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = 0xff00 | rd;
} break;

//tcall_0
case 0x01: {
  dp = 0xffde - (0 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_1
case 0x11: {
  dp = 0xffde - (1 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_2
case 0x21: {
  dp = 0xffde - (2 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_3
case 0x31: {
  dp = 0xffde - (3 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_4
case 0x41: {
  dp = 0xffde - (4 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_5
case 0x51: {
  dp = 0xffde - (5 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_6
case 0x61: {
  dp = 0xffde - (6 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_7
case 0x71: {
  dp = 0xffde - (7 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_8
case 0x81: {
  dp = 0xffde - (8 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_9
case 0x91: {
  dp = 0xffde - (9 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_10
case 0xa1: {
  dp = 0xffde - (10 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_11
case 0xb1: {
  dp = 0xffde - (11 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_12
case 0xc1: {
  dp = 0xffde - (12 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_13
case 0xd1: {
  dp = 0xffde - (13 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_14
case 0xe1: {
  dp = 0xffde - (14 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//tcall_15
case 0xf1: {
  dp = 0xffde - (15 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
} break;

//brk
case 0x0f: {
  rd  = op_readaddr(0xffde);
  rd |= op_readaddr(0xffdf) << 8;
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  op_writestack(regs.p);
  regs.pc = rd;
  regs.p.b = 1;
  regs.p.i = 0;
} break;

//ret
case 0x6f: {
  rd  = op_readstack();
  rd |= op_readstack() << 8;
  op_io();
  op_io();
  regs.pc = rd;
} break;

//reti
case 0x7f: {
  regs.p = op_readstack();
  rd  = op_readstack();
  rd |= op_readstack() << 8;
  op_io();
  op_io();
  regs.pc = rd;
} break;

#endif
