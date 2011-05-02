void SMP::op_io() {
  timer0.tick();
  timer1.tick();
  timer2.tick();
}

uint8 SMP::op_read(uint16 addr) {
  timer0.tick();
  timer1.tick();
  timer2.tick();

  if((addr & 0xfff0) == 0x00f0) return mmio_read(addr);
  if((addr & 0xffc0) == 0xffc0 && status.iplrom_enable) return iplrom[addr & 0x3f];
  return apuram[addr];
}

void SMP::op_write(uint16 addr, uint8 data) {
  timer0.tick();
  timer1.tick();
  timer2.tick();

  if((addr & 0xfff0) == 0x00f0) mmio_write(addr, data);
  apuram[addr] = data;  //all writes go to RAM, even MMIO writes
}

#define op_readpc() op_read(regs.pc++)
#define op_readdp(addr) op_read((regs.p.p << 8) + addr)
#define op_writedp(addr, data) op_write((regs.p.p << 8) + addr, data)

#define OP_ADJUST_DP(func) \
  dp = op_readpc(); \
  rd = op_readdp(dp); \
  rd = func(rd); \
  op_writedp(dp, rd);

#define OP_ADJUST_REG(func, target) \
  op_io(); \
  target = func(target);

#define OP_BRANCH(condition) \
  rd = op_readpc(); \
  if(condition) { \
    op_io(); \
    op_io(); \
    regs.pc += (int8)rd; \
  }

#define OP_MOV_DP_REG(source) \
  dp = op_readpc(); \
  op_readdp(dp); \
  op_writedp(dp, source);

#define OP_MOV_REG_CONST(target) \
  target = op_readpc(); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0);

#define OP_MOV_REG_DP(target) \
  sp = op_readpc(); \
  target = op_readdp(sp); \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0);

#define OP_MOV_REG_REG(target, source) \
  op_io(); \
  target = source; \
  regs.p.n = (target & 0x80); \
  regs.p.z = (target == 0);

#define OP_READ_REG_DP(func, target) \
  dp = op_readpc(); \
  rd = op_readdp(dp); \
  target = func(target, rd);

void SMP::op_step() {
  unsigned opcode = op_readpc();
  op_io();

  static unsigned rd, wr, dp, sp;

  switch(opcode) {

  case 0x00:  //nop
    break;

  case 0x10:  //bpl $rr
    OP_BRANCH(regs.p.n == 0)
    break;

  case 0x1d:  //dec x
    OP_ADJUST_REG(fn_dec, regs.x)
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

  case 0x5d:  //mov x,a
    OP_MOV_REG_REG(regs.x, regs.a)
    break;

  case 0x78:  //cmp $dp,#$ss
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    fn_cmp(wr, rd);
    op_io();
    break;

  case 0x7d:  //mov a,x
    OP_MOV_REG_REG(regs.a, regs.x)
    break;

  case 0x7e:  //cmp y,$dp
    OP_READ_REG_DP(fn_cmp, regs.y)
    break;

  case 0x8f:  //mov $dp,#$ss
    rd = op_readpc();
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, rd);
    break;

  case 0xab:  //inc $dp
    OP_ADJUST_DP(fn_inc)
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

  case 0xc4:  //mov $dd,a
    OP_MOV_DP_REG(regs.a)
    break;

  case 0xc6:  //mov (x),a
    op_io();
    op_readdp(regs.x);
    op_writedp(regs.x, regs.a);
    break;

  case 0xcb:  //mov $dp,y
    OP_MOV_DP_REG(regs.y)
    break;

  case 0xcd:  //mov x,#$ss
    OP_MOV_REG_CONST(regs.x)
    break;

  case 0xd0:  //bne $rr
    OP_BRANCH(regs.p.z == 0)
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

  case 0xdd:  //mov a,y
    OP_MOV_REG_REG(regs.a, regs.y)
    break;

  case 0xe4:  //mov y,$dp
    OP_MOV_REG_DP(regs.a)
    break;

  case 0xe8:  //mov a,#$ss
    OP_MOV_REG_CONST(regs.a)
    break;

  case 0xeb:  //mov y,$dp
    OP_MOV_REG_DP(regs.y)
    break;

  case 0xfc:  //inc y
    OP_ADJUST_REG(fn_inc, regs.y)
    break;

  default:
    static bool stop = false;
    regs.pc--;
    if(stop == false) {
      stop = true;
      print("Unknown opcode @ ", hex<4>(regs.pc), ":", hex<2>(opcode), "\n");
      char buffer[4096];
      disassemble_opcode(buffer, regs.pc);
      print(buffer, "\n");
    }
    break;
  }

  step(cycle_table[opcode]);
}

const unsigned SMP::cycle_table[256] = {
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
