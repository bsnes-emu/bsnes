void SMP::op_io() {
}

uint8 SMP::op_read(uint16 addr) {
  if((addr & 0xfff0) == 0x00f0) return mmio_read(addr);
  if((addr & 0xffc0) == 0xffc0 && status.iplrom_enable) return iplrom[addr & 0x3f];
  return apuram[addr];
}

void SMP::op_write(uint16 addr, uint8 data) {
  if((addr & 0xfff0) == 0x00f0) mmio_write(addr, data);
  apuram[addr] = data;  //all writes go to RAM, even MMIO writes
}

//TODO:
//* untaken conditional branches should subtract from opcode's cycle overhead

void SMP::op_step() {
  #define op_readpc() op_read(regs.pc++)
  #define op_readdp(addr) op_read((regs.p.p << 8) + addr)
  #define op_writedp(addr, data) op_write((regs.p.p << 8) + addr, data)
//#define op_readsp() op_read(0x0100 | ++regs.sp)
//#define op_writesp(data) op_write(0x0100 | regs.sp--, data)
  #define op_readsp() stackram[++regs.sp]
  #define op_writesp(data) stackram[regs.sp--] = data
  static unsigned rd, wr, dp, sp, ya, bit;

  unsigned opcode = op_readpc();

  switch(opcode) {

  case 0x00:  //nop
    op_io();
    break;

  case 0xef:  //sleep
  case 0xff:  //stop
    regs.pc--;
    break;

//
// batch
//

  case 0x01: case 0x11: case 0x21: case 0x31: case 0x41: case 0x51: case 0x61: case 0x71:
  case 0x81: case 0x91: case 0xa1: case 0xb1: case 0xc1: case 0xd1: case 0xe1: case 0xf1:  //tcall vector
    dp  = 0xffde - ((opcode >> 4) << 1);
    rd  = op_read(dp + 0) << 0;
    rd |= op_read(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writesp(regs.pc >> 8);
    op_writesp(regs.pc >> 0);
    regs.pc = rd;
    break;

  case 0x02: case 0x22: case 0x42: case 0x62: case 0x82: case 0xa2: case 0xc2: case 0xe2:  //setB $dp
    dp = op_readpc();
    rd = op_readdp(dp) | (1 << (opcode >> 5));
    op_writedp(dp, rd);
    break;

  case 0x12: case 0x32: case 0x52: case 0x72: case 0x92: case 0xb2: case 0xd2: case 0xf2:  //clrB $dp
    dp = op_readpc();
    rd = op_readdp(dp) &~ (1 << (opcode >> 5));
    op_writedp(dp, rd);
    break;

  case 0x03: case 0x23: case 0x43: case 0x63: case 0x83: case 0xa3: case 0xc3: case 0xe3:  //bbsB $dp,$rr
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & (1 << (opcode >> 5))) == 0) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0x13: case 0x33: case 0x53: case 0x73: case 0x93: case 0xb3: case 0xd3: case 0xf3:  //bbcB $dp,$rr
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & (1 << (opcode >> 5))) != 0) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0x20: op_io(); regs.p.p = 0; break;  //clrp
  case 0x40: op_io(); regs.p.p = 1; break;  //setp
  case 0x60: op_io(); regs.p.c = 0; break;  //clrc
  case 0x80: op_io(); regs.p.c = 1; break;  //setc
  case 0xe0: op_io(); regs.p.v = 0; break;  //clrv
  //
  case 0xa0: op_io(); op_io(); regs.p.i = 1; break;  //ei
  case 0xc0: op_io(); op_io(); regs.p.i = 0; break;  //di
  //
  case 0xed: op_io(); op_io(); regs.p.c = !regs.p.c; break;  //notc

//
// jump
//

#define OP_BRANCH_IF(condition) \
  rd = op_readpc(); \
  if((condition) == false) break; \
  op_io(); \
  op_io(); \
  regs.pc += (int8)rd; \
  break;
  case 0x10: OP_BRANCH_IF(regs.p.n == 0);  //bpl $rr
  case 0x30: OP_BRANCH_IF(regs.p.n == 1);  //bmi $rr
  case 0x50: OP_BRANCH_IF(regs.p.v == 0);  //bvc $rr
  case 0x70: OP_BRANCH_IF(regs.p.v == 1);  //bvs $rr
  case 0x90: OP_BRANCH_IF(regs.p.c == 0);  //bcc $rr
  case 0xb0: OP_BRANCH_IF(regs.p.c == 1);  //bcs $rr
  case 0xd0: OP_BRANCH_IF(regs.p.z == 0);  //bne $rr
  case 0xf0: OP_BRANCH_IF(regs.p.z == 1);  //beq $rr

  case 0x0f:  //brk
    rd  = op_read(0xffde) << 0;
    rd |= op_read(0xffdf) << 8;
    op_io();
    op_io();
    op_writesp(regs.pc >> 8);
    op_writesp(regs.pc >> 0);
    op_writesp(regs.p);
    regs.pc = rd;
    regs.p.b = 1;
    regs.p.i = 0;
    break;

  case 0x1f:  //jmp ($aaaa,x)
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    dp += regs.x;
    rd  = op_read(dp + 0) << 0;
    rd |= op_read(dp + 1) << 8;
    regs.pc = rd;
    break;

  case 0x2f:  //bra $rr
    rd = op_readpc();
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0x3f:  //call $addr
    rd  = op_readpc() << 0;
    rd |= op_readpc() << 8;
    op_io();
    op_io();
    op_io();
    op_writesp(regs.pc >> 8);
    op_writesp(regs.pc >> 0);
    regs.pc = rd;
    break;

  case 0x4f:  //pcall $ff00+$dp
    rd = op_readpc();
    op_io();
    op_io();
    op_writesp(regs.pc >> 8);
    op_writesp(regs.pc >> 0);
    regs.pc = 0xff00 | rd;
    break;

  case 0x5f:  //jmp $addr
    rd  = op_readpc() << 0;
    rd |= op_readpc() << 8;
    regs.pc = rd;
    break;

  case 0x6f:  //ret
    rd  = op_readsp() << 0;
    rd |= op_readsp() << 8;
    op_io();
    op_io();
    regs.pc = rd;
    break;

  case 0x7f:  //reti
    regs.p = op_readsp();
    rd  = op_readsp() << 0;
    rd |= op_readsp() << 8;
    op_io();
    op_io();
    regs.pc = rd;
    break;

  case 0x2e:  //cbne $dp,$rr
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if(regs.a == sp) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0x6e:  //dbnz $dp,$rr
    dp = op_readpc();
    wr = op_readdp(dp);
    op_writedp(dp, --wr);
    rd = op_readpc();
    if(wr == 0) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0xde:  //cbne $dp+x,$rr
    dp = op_readpc();
    op_io();
    sp = op_readdp(dp + regs.x);
    rd = op_readpc();
    op_io();
    if(regs.a == sp) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

  case 0xfe:  //dbz y,$rr
    rd = op_readpc();
    op_io();
    regs.y--;
    op_io();
    if(regs.y == 0) break;
    op_io();
    op_io();
    regs.pc += (int8)rd;
    break;

//
// stack
//

#define OP_PUSH_REG(source) \
  op_io(); \
  op_io(); \
  op_writesp(source); \
  break;
  case 0x0d: OP_PUSH_REG(regs.p);  //push p
  case 0x2d: OP_PUSH_REG(regs.a);  //push a
  case 0x4d: OP_PUSH_REG(regs.x);  //push x
  case 0x6d: OP_PUSH_REG(regs.y);  //push y

#define OP_POP_REG(target) \
  op_io(); \
  op_io(); \
  target = op_readsp(); \
  break;
  case 0x8e: OP_POP_REG(regs.p);  //pop p
  case 0xae: OP_POP_REG(regs.a);  //pop a
  case 0xce: OP_POP_REG(regs.x);  //pop x
  case 0xee: OP_POP_REG(regs.y);  //pop y

//
// ALU
//

  case 0x9e:  //div ya,x
    op_io(); op_io(); op_io(); op_io();
    op_io(); op_io(); op_io(); op_io();
    op_io(); op_io(); op_io();
    ya = regs.ya;
    regs.p.v = !!(regs.y >= regs.x);
    regs.p.h = !!((regs.y & 15) >= (regs.x & 15));
    if(regs.y < (regs.x << 1)) {
      regs.a = ya / regs.x;
      regs.y = ya % regs.x;
    } else {
      regs.a = 255    - (ya - (regs.x << 9)) / (256 - regs.x);
      regs.y = regs.x + (ya - (regs.x << 9)) % (256 - regs.x);
    }
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xcf:  //mul ya
    op_io(); op_io(); op_io(); op_io();
    op_io(); op_io(); op_io(); op_io();
    ya = regs.y * regs.a;
    regs.a = ya;
    regs.y = ya >> 8;
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    break;

//
// read
//

#define OP_READ_REG_DP(func, target) \
  dp = op_readpc(); \
  rd = op_readdp(dp); \
  target = func(target, rd); \
  break;
  case 0x04: OP_READ_REG_DP(fn_or,  regs.a);  //or  a,$dp
  case 0x24: OP_READ_REG_DP(fn_and, regs.a);  //and a,$dp
  case 0x44: OP_READ_REG_DP(fn_eor, regs.a);  //eor a,$dp
  case 0x64: OP_READ_REG_DP(fn_cmp, regs.a);  //cmp a,$dp
  case 0x84: OP_READ_REG_DP(fn_adc, regs.a);  //adc a,$dp
  case 0xa4: OP_READ_REG_DP(fn_sbc, regs.a);  //sbc a,$dp
  //
  case 0x3e: OP_READ_REG_DP(fn_cmp, regs.x);  //cmp x,$dp
  case 0x7e: OP_READ_REG_DP(fn_cmp, regs.y);  //cmp y,$dp

#define OP_READ_REG_ADDR(func, target) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  rd  = op_read(dp); \
  target = func(target, rd); \
  break;
  case 0x05: OP_READ_REG_ADDR(fn_or,  regs.a);  //or  a,$addr
  case 0x25: OP_READ_REG_ADDR(fn_and, regs.a);  //and a,$addr
  case 0x45: OP_READ_REG_ADDR(fn_eor, regs.a);  //eor a,$addr
  case 0x65: OP_READ_REG_ADDR(fn_cmp, regs.a);  //cmp a,$addr
  case 0x85: OP_READ_REG_ADDR(fn_adc, regs.a);  //adc a,$addr
  case 0xa5: OP_READ_REG_ADDR(fn_sbc, regs.a);  //sbc a,$addr
  //
  case 0x1e: OP_READ_REG_ADDR(fn_cmp, regs.x);  //cmp x,$addr
  case 0x5e: OP_READ_REG_ADDR(fn_cmp, regs.y);  //cmp y,$addr

#define OP_READ_A_IX(func) \
  op_io(); \
  rd = op_readdp(regs.x); \
  regs.a = func(regs.a, rd); \
  break;
  case 0x06: OP_READ_A_IX(fn_or );  //or  a,(x)
  case 0x26: OP_READ_A_IX(fn_and);  //and a,(x)
  case 0x46: OP_READ_A_IX(fn_eor);  //eor a,(x)
  case 0x66: OP_READ_A_IX(fn_cmp);  //cmp a,(x)
  case 0x86: OP_READ_A_IX(fn_adc);  //adc a,(x)
  case 0xa6: OP_READ_A_IX(fn_sbc);  //sbc a,(x)

#define OP_READ_A_IDPX(func) \
  dp  = op_readpc() + regs.x; \
  op_io(); \
  sp  = op_readdp(dp + 0) << 0; \
  sp |= op_readdp(dp + 1) << 8; \
  rd  = op_read(sp); \
  regs.a = func(regs.a, rd); \
  break;
  case 0x07: OP_READ_A_IDPX(fn_or );  //or  a,($dp+x)
  case 0x27: OP_READ_A_IDPX(fn_and);  //and a,($dp+x)
  case 0x47: OP_READ_A_IDPX(fn_eor);  //eor a,($dp+x)
  case 0x67: OP_READ_A_IDPX(fn_cmp);  //cmp a,($dp+x)
  case 0x87: OP_READ_A_IDPX(fn_adc);  //adc a,($dp+x)
  case 0xa7: OP_READ_A_IDPX(fn_sbc);  //sbc a,($dp+x)

#define OP_READ_REG_CONST(func, target) \
  rd = op_readpc(); \
  target = func(target, rd); \
  break;
  case 0x08: OP_READ_REG_CONST(fn_or,  regs.a);  //or  a,#$ss
  case 0x28: OP_READ_REG_CONST(fn_and, regs.a);  //and a,#$ss
  case 0x48: OP_READ_REG_CONST(fn_eor, regs.a);  //eor a,#$ss
  case 0x68: OP_READ_REG_CONST(fn_cmp, regs.a);  //cmp a,#$ss
  case 0x88: OP_READ_REG_CONST(fn_adc, regs.a);  //adc a,#$ss
  case 0xa8: OP_READ_REG_CONST(fn_sbc, regs.a);  //sbc a,#$ss
  //
  case 0xad: OP_READ_REG_CONST(fn_cmp, regs.y);  //cmp y,#$ss
  case 0xc8: OP_READ_REG_CONST(fn_cmp, regs.x);  //cmp x,#$ss

#define OP_READ_DP_DP(func) \
  sp = op_readpc(); \
  rd = op_readdp(sp); \
  dp = op_readpc(); \
  wr = op_readdp(dp); \
  wr = func(wr, rd);
  case 0x09: OP_READ_DP_DP(fn_or ); op_writedp(dp, wr); break;  //or  $dp,$dp
  case 0x29: OP_READ_DP_DP(fn_and); op_writedp(dp, wr); break;  //and $dp,$dp
  case 0x49: OP_READ_DP_DP(fn_eor); op_writedp(dp, wr); break;  //eor $dp,$dp
  case 0x69: OP_READ_DP_DP(fn_cmp); op_io();            break;  //cmp $dp,$dp
  case 0x89: OP_READ_DP_DP(fn_adc); op_writedp(dp, wr); break;  //adc $dp,$dp
  case 0xa9: OP_READ_DP_DP(fn_sbc); op_writedp(dp, wr); break;  //sbc $dp,$dp  

#define OP_READ_A_DPX(func) \
  dp = op_readpc(); \
  op_io(); \
  rd = op_readdp(dp + regs.x); \
  regs.a = func(regs.a, rd); \
  break;
  case 0x14: OP_READ_A_DPX(fn_or );  //or  a,$dp+x
  case 0x34: OP_READ_A_DPX(fn_and);  //and a,$dp+x
  case 0x54: OP_READ_A_DPX(fn_eor);  //eor a,$dp+x
  case 0x74: OP_READ_A_DPX(fn_cmp);  //cmp a,$dp+x
  case 0x94: OP_READ_A_DPX(fn_adc);  //adc a,$dp+x
  case 0xb4: OP_READ_A_DPX(fn_sbc);  //sbc a,$dp+x

#define OP_READ_A_ADDRR(func, source) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  op_io(); \
  rd = op_read(dp + source); \
  regs.a = func(regs.a, rd); \
  break;
  case 0x15: OP_READ_A_ADDRR(fn_or , regs.x);  //or  a,$dp+x
  case 0x16: OP_READ_A_ADDRR(fn_or , regs.y);  //or  a,$dp+y
  case 0x35: OP_READ_A_ADDRR(fn_and, regs.x);  //and a,$dp+x
  case 0x36: OP_READ_A_ADDRR(fn_and, regs.y);  //and a,$dp+y
  case 0x55: OP_READ_A_ADDRR(fn_eor, regs.x);  //eor a,$dp+x
  case 0x56: OP_READ_A_ADDRR(fn_eor, regs.y);  //eor a,$dp+y
  case 0x75: OP_READ_A_ADDRR(fn_cmp, regs.x);  //cmp a,$dp+x
  case 0x76: OP_READ_A_ADDRR(fn_cmp, regs.y);  //cmp a,$dp+y
  case 0x95: OP_READ_A_ADDRR(fn_adc, regs.x);  //adc a,$dp+x
  case 0x96: OP_READ_A_ADDRR(fn_adc, regs.y);  //adc a,$dp+y
  case 0xb5: OP_READ_A_ADDRR(fn_sbc, regs.x);  //sbc a,$dp+x
  case 0xb6: OP_READ_A_ADDRR(fn_sbc, regs.y);  //sbc a,$dp+y

#define OP_READ_A_IDPY(func) \
  dp  = op_readpc(); \
  op_io(); \
  sp  = op_readdp(dp + 0) << 0; \
  sp |= op_readdp(dp + 1) << 8; \
  rd  = op_read(sp + regs.y); \
  regs.a = func(regs.a, rd); \
  break;
  case 0x17: OP_READ_A_IDPY(fn_or );  //or  a,($dp)+y
  case 0x37: OP_READ_A_IDPY(fn_and);  //and a,($dp)+y
  case 0x57: OP_READ_A_IDPY(fn_eor);  //eor a,($dp)+y
  case 0x77: OP_READ_A_IDPY(fn_cmp);  //cmp a,($dp)+y
  case 0x97: OP_READ_A_IDPY(fn_adc);  //adc a,($dp)+y
  case 0xb7: OP_READ_A_IDPY(fn_sbc);  //sbc a,($dp)+y

#define OP_READ_DP_CONST(func) \
  rd = op_readpc(); \
  dp = op_readpc(); \
  wr = op_readdp(dp); \
  wr = func(wr, rd);
  case 0x18: OP_READ_DP_CONST(fn_or ); op_writedp(dp, wr); break;  //or  $dp,#$ss
  case 0x38: OP_READ_DP_CONST(fn_and); op_writedp(dp, wr); break;  //and $dp,#$ss
  case 0x58: OP_READ_DP_CONST(fn_eor); op_writedp(dp, wr); break;  //eor $dp,#$ss
  case 0x78: OP_READ_DP_CONST(fn_cmp); op_io();            break;  //cmp $dp,#$ss
  case 0x98: OP_READ_DP_CONST(fn_adc); op_writedp(dp, wr); break;  //adc $dp,#$ss
  case 0xb8: OP_READ_DP_CONST(fn_sbc); op_writedp(dp, wr); break;  //sbc $dp,#$ss

#define OP_READ_IX_IY(func) \
  op_io(); \
  rd = op_readdp(regs.y); \
  wr = op_readdp(regs.x); \
  wr = func(wr, rd);
  case 0x19: OP_READ_IX_IY(fn_or ); op_writedp(regs.x, wr); break;  //or  (x),(y)
  case 0x39: OP_READ_IX_IY(fn_and); op_writedp(regs.x, wr); break;  //and (x),(y)
  case 0x59: OP_READ_IX_IY(fn_eor); op_writedp(regs.x, wr); break;  //eor (x),(y)
  case 0x79: OP_READ_IX_IY(fn_cmp); op_io();                break;  //cmp (x),(y)
  case 0x99: OP_READ_IX_IY(fn_adc); op_writedp(regs.x, wr); break;  //adc (x),(y)
  case 0xb9: OP_READ_IX_IY(fn_sbc); op_writedp(regs.x, wr); break;  //sbc (x),(y)

#define OP_READ_YA_DP(func) \
  dp  = op_readpc(); \
  rd  = op_readdp(dp + 0) << 0; \
  op_io(); \
  rd |= op_readdp(dp + 1) << 8; \
  regs.ya = func(regs.ya, rd); \
  break;
  case 0x7a: OP_READ_YA_DP(fn_addw);  //addw $dp
  case 0x9a: OP_READ_YA_DP(fn_subw);  //subw $dp

//
// move
//

#define OP_MOV_REG_REG(target, source) \
  op_io(); \
  target = source; \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0); \
  break;
  case 0x5d: OP_MOV_REG_REG(regs.x, regs.a );  //mov x,a
  case 0x7d: OP_MOV_REG_REG(regs.a, regs.x );  //mov a,x
  case 0x9d: OP_MOV_REG_REG(regs.x, regs.sp);  //mov x,sp
  case 0xdd: OP_MOV_REG_REG(regs.a, regs.y );  //mov a,y
  case 0xfd: OP_MOV_REG_REG(regs.y, regs.a );  //mov y,a

#define OP_MOV_REG_CONST(target) \
  target = op_readpc(); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0); \
  break;
  case 0x8d: OP_MOV_REG_CONST(regs.y);  //mov y,#$ss
  case 0xcd: OP_MOV_REG_CONST(regs.x);  //mov x,#$ss
  case 0xe8: OP_MOV_REG_CONST(regs.a);  //mov a,#$ss

#define OP_MOV_DP_REG(source) \
  dp = op_readpc(); \
  op_readdp(dp); \
  op_writedp(dp, source); \
  break;
  case 0xc4: OP_MOV_DP_REG(regs.a);  //mov $dp,a
  case 0xcb: OP_MOV_DP_REG(regs.y);  //mov $dp,y
  case 0xd8: OP_MOV_DP_REG(regs.x);  //mov $dp,x

#define OP_MOV_ADDR_REG(source) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  op_read(dp); \
  op_write(dp, source); \
  break;
  case 0xc5: OP_MOV_ADDR_REG(regs.a);  //mov $addr,a
  case 0xc9: OP_MOV_ADDR_REG(regs.x);  //mov $addr,x
  case 0xcc: OP_MOV_ADDR_REG(regs.y);  //mov $addr,y

#define OP_MOV_DPR_REG(target, source) \
  dp  = op_readpc(); \
  op_io(); \
  dp += source; \
  op_readdp(dp); \
  op_writedp(dp, target); \
  break;
  case 0xd4: OP_MOV_DPR_REG(regs.a, regs.x);  //mov $dp+a,x
  case 0xd9: OP_MOV_DPR_REG(regs.x, regs.y);  //mov $dp+x,y
  case 0xdb: OP_MOV_DPR_REG(regs.y, regs.x);  //mov $dp+y,x

#define OP_MOV_ADDRR_A(source) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  op_io(); \
  dp += source; \
  op_read(dp); \
  op_write(dp, regs.a); \
  break;
  case 0xd5: OP_MOV_ADDRR_A(regs.x);  //mov $addr+x,a
  case 0xd6: OP_MOV_ADDRR_A(regs.y);  //mov $addr+y,a

#define OP_MOV_REG_DP(target) \
  sp = op_readpc(); \
  target = op_readdp(sp); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0); \
  break;
  case 0xe4: OP_MOV_REG_DP(regs.a);  //mov a,$dp
  case 0xeb: OP_MOV_REG_DP(regs.y);  //mov y,$dp
  case 0xf8: OP_MOV_REG_DP(regs.x);  //mov x,$dp

#define OP_MOV_REG_ADDR(target) \
  sp  = op_readpc() << 0; \
  sp |= op_readpc() << 8; \
  target = op_read(sp); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0); \
  break;
  case 0xe5: OP_MOV_REG_ADDR(regs.a);  //mov a,$addr
  case 0xe9: OP_MOV_REG_ADDR(regs.x);  //mov x,$addr
  case 0xec: OP_MOV_REG_ADDR(regs.y);  //mov y,$addr

  case 0xe6:  //mov a,(x)
    op_io();
    regs.a = op_readdp(regs.x);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

#define OP_MOV_REG_DPR(target, source) \
  sp = op_readpc(); \
  op_io(); \
  target = op_readdp(sp + source); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0); \
  break;
  case 0xf4: OP_MOV_REG_DPR(regs.a, regs.x);  //mov a,$dp+x
  case 0xf9: OP_MOV_REG_DPR(regs.x, regs.y);  //mov x,$dp+y
  case 0xfb: OP_MOV_REG_DPR(regs.y, regs.x);  //mov y,$dp+x

#define OP_MOV_A_ADDRR(source) \
  sp  = op_readpc() << 0; \
  sp |= op_readpc() << 8; \
  op_io(); \
  regs.a = op_read(sp + source); \
  regs.p.n = (regs.a & 0x80); \
  regs.p.z = (regs.a == 0); \
  break;
  case 0xf5: OP_MOV_A_ADDRR(regs.x);  //mov a,$addr+x
  case 0xf6: OP_MOV_A_ADDRR(regs.y);  //mov a,$addr+y

  case 0x8f:  //mov $dp,#$ss
    rd = op_readpc();
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, rd);
    break;

  case 0xaf:  //mov (x)+,a
    op_io();
    op_io();
    op_writedp(regs.x++, regs.a);
    break;

  case 0xba:  //movw ya,$dp
    sp = op_readpc();
    regs.a = op_readdp(sp + 0);
    op_io();
    regs.y = op_readdp(sp + 1);
    regs.p.n = (regs.ya & 0x8000);
    regs.p.z = (regs.ya == 0);
    break;

  case 0xbd:  //mov sp,x
    op_io();
    regs.sp = regs.x;
    break;

  case 0xbf:  //mov a,(x)+
    op_io();
    regs.a = op_readdp(regs.x++);
    op_io();
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xc6:  //mov (x),a
    op_io();
    op_readdp(regs.x);
    op_writedp(regs.x, regs.a);
    break;

  case 0xc7:  //mov ($dp+x),a
    sp  = op_readpc();
    op_io();
    sp += regs.x;
    dp  = op_readdp(sp + 0) << 0;
    dp |= op_readdp(sp + 1) << 8;
    op_read(dp);
    op_write(dp, regs.a);
    break;

  case 0xd7:  //mov ($dp)+y,a
    sp  = op_readpc();
    dp  = op_readdp(sp + 0) << 0;
    dp |= op_readdp(sp + 1) << 8;
    op_io();
    dp += regs.y;
    op_read(dp);
    op_write(dp, regs.a);
    break;

  case 0xda:  //movw $dp,ya
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp + 0, regs.a);
    op_writedp(dp + 1, regs.y);
    break;

  case 0xe7:  //mov a,($dp+x)
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    regs.a = op_read(sp);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xf7:  //mov a,($dp)+y
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    regs.a = op_read(sp + regs.y);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xfa:  //mov $dp,$dp
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    op_writedp(dp, rd);
    break;

//
// read/modify/write
//

#define OP_ADJUST_DP(func) \
  dp = op_readpc(); \
  rd = op_readdp(dp); \
  rd = func(rd); \
  op_writedp(dp, rd); \
  break;
  case 0x0b: OP_ADJUST_DP(fn_asl);  //asl $dp
  case 0x2b: OP_ADJUST_DP(fn_rol);  //rol $dp
  case 0x4b: OP_ADJUST_DP(fn_lsr);  //lsr $dp
  case 0x6b: OP_ADJUST_DP(fn_ror);  //ror $dp
  case 0x8b: OP_ADJUST_DP(fn_dec);  //dec $dp
  case 0xab: OP_ADJUST_DP(fn_inc);  //inc $dp

#define OP_ADJUST_ADDR(func) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  rd  = op_read(dp); \
  rd  = func(rd); \
  op_write(dp, rd); \
  break;
  case 0x0c: OP_ADJUST_ADDR(fn_asl);  //asl $addr
  case 0x2c: OP_ADJUST_ADDR(fn_rol);  //rol $addr
  case 0x4c: OP_ADJUST_ADDR(fn_lsr);  //lsr $addr
  case 0x6c: OP_ADJUST_ADDR(fn_ror);  //ror $addr
  case 0x8c: OP_ADJUST_ADDR(fn_dec);  //dec $addr
  case 0xac: OP_ADJUST_ADDR(fn_inc);  //inc $addr

#define OP_ADJUSTW_DP(adjust) \
  dp  = op_readpc(); \
  rd  = op_readdp(dp) << 0; \
  rd += adjust; \
  op_writedp(dp++, rd); \
  rd += op_readdp(dp) << 8; \
  op_writedp(dp, rd >> 8); \
  regs.p.n = (rd & 0x8000); \
  regs.p.z = (rd == 0); \
  break;
  case 0x1a: OP_ADJUSTW_DP(-1);  //decw $dp
  case 0x3a: OP_ADJUSTW_DP(+1);  //incw $dp

#define OP_ADJUST_DPX(func) \
  dp = op_readpc(); \
  op_io(); \
  rd = op_readdp(dp + regs.x); \
  rd = func(rd); \
  op_writedp(dp + regs.x, rd); \
  break;
  case 0x1b: OP_ADJUST_DPX(fn_asl);  //asl $dp+x
  case 0x3b: OP_ADJUST_DPX(fn_rol);  //rol $dp+x
  case 0x5b: OP_ADJUST_DPX(fn_lsr);  //lsr $dp+x
  case 0x7b: OP_ADJUST_DPX(fn_ror);  //ror $dp+x
  case 0x9b: OP_ADJUST_DPX(fn_dec);  //dec $dp+x
  case 0xbb: OP_ADJUST_DPX(fn_inc);  //inc $dp+x

#define OP_ADJUST_REG(func, target) \
  op_io(); \
  target = func(target); \
  break;
  case 0x1c: OP_ADJUST_REG(fn_asl, regs.a);  //asl a
  case 0x3c: OP_ADJUST_REG(fn_rol, regs.a);  //rol a
  case 0x5c: OP_ADJUST_REG(fn_lsr, regs.a);  //lsr a
  case 0x7c: OP_ADJUST_REG(fn_ror, regs.a);  //ror a
  case 0x9c: OP_ADJUST_REG(fn_dec, regs.a);  //dec a
  case 0xbc: OP_ADJUST_REG(fn_inc, regs.a);  //inc a

  case 0x1d: OP_ADJUST_REG(fn_dec, regs.x);  //dec x
  case 0x3d: OP_ADJUST_REG(fn_inc, regs.x);  //inc x

  case 0xdc: OP_ADJUST_REG(fn_dec, regs.y);  //dec y
  case 0xfc: OP_ADJUST_REG(fn_inc, regs.y);  //inc y

//
// bit manipulation
//

#define OP_OR1_BIT(op) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  bit = dp >> 13; \
  dp &= 0x1fff; \
  rd  = op_read(dp); \
  op_io(); \
  regs.p.c = regs.p.c = ((bool)(rd & (1 << bit)) ^ op); \
  break;
  case 0x0a: OP_OR1_BIT(0);  //or1 c,$addr:bit
  case 0x2a: OP_OR1_BIT(1);  //or1 c,!$addr:bit

#define OP_AND1_BIT(op) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  bit = dp >> 13; \
  dp &= 0x1fff; \
  rd  = op_read(dp); \
  regs.p.c = regs.p.c & ((bool)(rd & (1 << bit)) ^ op); \
  break;
  case 0x4a: OP_AND1_BIT(0);  //and1 c,$addr:bit
  case 0x6a: OP_AND1_BIT(1);  //and1 c,!$addr:bit

  case 0x8a:  //eor1 c,$addr:bit
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_read(dp);
    op_io();
    regs.p.c = regs.p.c ^ (bool)(rd & (1 << bit));
    break;

  case 0xaa:  //mov1 c,$addr:bit
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    bit = sp >> 13;
    sp &= 0x1fff;
    rd  = op_read(sp);
    regs.p.c = (rd & (1 << bit));
    break;

  case 0xca:  //mov1 $addr:bit,c
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(dp);
    regs.p.c ? rd |= (1 << bit) : rd &=~ (1 << bit);
    op_io();
    op_write(dp, rd);
    break;

  case 0xea:  //not1 c,$addr:bit
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_read(dp);
    rd ^= 1 << bit;
    op_write(dp, rd);
    break;

//
// miscellaneous
//

#define OP_TADJUST_ADDR(mathop) \
  dp  = op_readpc() << 0; \
  dp |= op_readpc() << 8; \
  rd  = op_read(dp); \
  regs.p.n = ((regs.a - rd) & 0x80); \
  regs.p.z = ((regs.a - rd) == 0); \
  op_read(dp); \
  op_write(dp, rd mathop regs.a); \
  break;
  case 0x0e: OP_TADJUST_ADDR(| );  //tset $addr,a
  case 0x4e: OP_TADJUST_ADDR(&~);  //tclr $addr,a

  case 0x5a:  //cmpw ya,$dp
    dp  = op_readpc();
    rd  = op_readdp(dp + 0) << 0;
    rd |= op_readdp(dp + 1) << 8;
    fn_cmpw(regs.ya, rd);
    break;

  case 0x9f:  //xcn a
    op_io(); op_io(); op_io(); op_io();
    regs.a = (regs.a >> 4) | (regs.a << 4);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xbe:  //das
    op_io();
    op_io();
    if(!regs.p.c || (regs.a) > 0x99) {
      regs.a -= 0x60;
      regs.p.c = 0;
    }
    if(!regs.p.h || (regs.a & 15) > 0x09) {
      regs.a -= 0x06;
    }
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  case 0xdf:  //daa
    op_io();
    op_io();
    if(regs.p.c || (regs.a) > 0x99) {
      regs.a += 0x60;
      regs.p.c = 1;
    }
    if(regs.p.h || (regs.a & 15) > 0x09) {
      regs.a += 0x06;
    }
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    break;

  }

  timer0.tick(cycle_count_table[opcode]);
  timer1.tick(cycle_count_table[opcode]);
  timer2.tick(cycle_count_table[opcode]);

  clock += cycle_table_cpu[opcode];
  dsp.clock -= cycle_table_dsp[opcode];
  synchronize_dsp();
}

const unsigned SMP::cycle_count_table[256] = {
  #define c 12
//0 1 2 3   4 5 6 7   8 9 A B   C D E F
  2,8,4,7,  3,4,3,6,  2,6,5,4,  5,4,6,8,  //0
  4,8,4,7,  4,5,5,6,  5,5,6,5,  2,2,4,6,  //1
  2,8,4,7,  3,4,3,6,  2,6,5,4,  5,4,7,4,  //2
  4,8,4,7,  4,5,5,6,  5,5,6,5,  2,2,3,8,  //3

  2,8,4,7,  3,4,3,6,  2,6,4,4,  5,4,6,6,  //4
  4,8,4,7,  4,5,5,6,  5,5,4,5,  2,2,4,3,  //5
  2,8,4,7,  3,4,3,6,  2,6,4,4,  5,4,7,5,  //6
  4,8,4,7,  4,5,5,6,  5,5,5,5,  2,2,3,6,  //7

  2,8,4,7,  3,4,3,6,  2,6,5,4,  5,2,4,5,  //8
  4,8,4,7,  4,5,5,6,  5,5,5,5,  2,2,c,5,  //9
  3,8,4,7,  3,4,3,6,  2,6,4,4,  5,2,4,4,  //A
  4,8,4,7,  4,5,5,6,  5,5,5,5,  2,2,3,4,  //B

  3,8,4,7,  4,5,4,7,  2,5,6,4,  5,2,4,9,  //C
  4,8,4,7,  5,6,6,7,  4,5,5,5,  2,2,8,3,  //D
  2,8,4,7,  3,4,3,6,  2,4,5,3,  4,3,4,1,  //E
  4,8,4,7,  4,5,5,6,  3,4,5,4,  2,2,6,1,  //F

  #undef c
};
