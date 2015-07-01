#ifdef PROCESSOR_ARM_HPP

auto ARM::arm_opcode(uint32 rm) {
  uint4 opcode = instruction() >> 21;
  uint1 save = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;

  uint32 rn = r(n);

  switch(opcode) {
  case  0: r(d) = bit(rn & rm);           break;  //AND
  case  1: r(d) = bit(rn ^ rm);           break;  //EOR
  case  2: r(d) = sub(rn, rm, 1);         break;  //SUB
  case  3: r(d) = sub(rm, rn, 1);         break;  //RSB
  case  4: r(d) = add(rn, rm, 0);         break;  //ADD
  case  5: r(d) = add(rn, rm, cpsr().c);  break;  //ADC
  case  6: r(d) = sub(rn, rm, cpsr().c);  break;  //SBC
  case  7: r(d) = sub(rm, rn, cpsr().c);  break;  //RSC
  case  8:        bit(rn & rm);           break;  //TST
  case  9:        bit(rn ^ rm);           break;  //TEQ
  case 10:        sub(rn, rm, 1);         break;  //CMP
  case 11:        add(rn, rm, 0);         break;  //CMN
  case 12: r(d) = bit(rn | rm);           break;  //ORR
  case 13: r(d) = bit(rm);                break;  //MOV
  case 14: r(d) = bit(rn & ~rm);          break;  //BIC
  case 15: r(d) = bit(~rm);               break;  //MVN
  }

  if(exceptionMode() && d == 15 && save) {
    cpsr() = spsr();
    processor.setMode((Processor::Mode)cpsr().m);
  }
}

auto ARM::arm_move_to_status(uint32 rm) {
  uint1 source = instruction() >> 22;
  uint4 field = instruction() >> 16;

  if(source == 1) {
    if(mode() == Processor::Mode::USR) return;
    if(mode() == Processor::Mode::SYS) return;
  }

  PSR &psr = source ? spsr() : cpsr();

  if(field & 1) {
    if(source == 1 || privilegedMode()) {
      psr.i = rm & 0x00000080;
      psr.f = rm & 0x00000040;
      psr.t = rm & 0x00000020;
      psr.m = rm & 0x0000001f;
      if(source == 0) processor.setMode((Processor::Mode)psr.m);
    }
  }

  if(field & 8) {
    psr.n = rm & 0x80000000;
    psr.z = rm & 0x40000000;
    psr.c = rm & 0x20000000;
    psr.v = rm & 0x10000000;
  }
}

//mul{condition}{s} rd,rm,rs
//mla{condition}{s} rd,rm,rs,rn
//cccc 0000 00as dddd nnnn ssss 1001 mmmm
//c = condition
//a = accumulate
//s = save flags
//d = rd
//n = rn
//s = rs
//m = rm
auto ARM::arm_op_multiply() {
  uint1 accumulate = instruction() >> 21;
  uint1 save = instruction() >> 20;
  uint4 d = instruction() >> 16;
  uint4 n = instruction() >> 12;
  uint4 s = instruction() >> 8;
  uint4 m = instruction();

  r(d) = mul(accumulate ? r(n) : 0u, r(m), r(s));
}

//(u,s)mull{condition}{s} rdlo,rdhi,rm,rs
//(u,s)mlal{condition}{s} rdlo,rdhi,rm,rs
//cccc 0000 1sas hhhh llll ssss 1001 mmmm
//c = condition
//s = sign-extend
//a = accumulate
//s = save flags
//h = rdhi
//l = rdlo
//s = rs
//m = rm
auto ARM::arm_op_multiply_long() {
  uint1 signextend = instruction() >> 22;
  uint1 accumulate = instruction() >> 21;
  uint1 save = instruction() >> 20;
  uint4 dhi = instruction() >> 16;
  uint4 dlo = instruction() >> 12;
  uint4 s = instruction() >> 8;
  uint4 m = instruction();

  uint64 rm = r(m);
  uint64 rs = r(s);
  if(signextend) {
    rm = (int32)rm;
    rs = (int32)rs;
  }

  uint64 rd = rm * rs;
  if(accumulate) rd += ((uint64)r(dhi) << 32) + ((uint64)r(dlo) << 0);

  r(dhi) = rd >> 32;
  r(dlo) = rd >>  0;

  if(save) {
    cpsr().n = r(dhi) >> 31;
    cpsr().z = r(dhi) == 0 && r(dlo) == 0;
  //cpsr().c = 0;  //(undefined)
  //cpsr().v = 0;  //(undefined)
  }
}

//swp{condition}{b} rd,rm,[rn]
//cccc 0001 0b00 nnnn dddd ---- 1001 mmmm
//c = condition
//b = byte (0 = word)
//n = rn
//d = rd
//m = rm
auto ARM::arm_op_memory_swap() {
  uint1 byte = instruction() >> 22;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint4 m = instruction();

  uint32 word = load((byte ? Byte : Word) | Nonsequential, r(n));
  store((byte ? Byte : Word) | Nonsequential, r(n), r(m));
  r(d) = word;
}

//(ldr,str){condition}h rd,[rn,rm]{!}
//(ldr,str){condition}h rd,[rn],rm
//cccc 000p u0wl nnnn dddd ---- 1011 mmmm
//c = condition
//p = pre (0 = post)
//u = up
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//m = rm
auto ARM::arm_op_move_half_register() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 writeback = instruction() >> 21;
  uint1 l = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint4 m = instruction();

  uint32 rn = r(n);
  uint32 rm = r(m);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(l == 1) r(d) = load(Half | Nonsequential, rn);
  if(l == 0) store(Half | Nonsequential, rn, r(d));
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//(ldr,str){condition}h rd,[rn{,+/-offset}]{!}
//(ldr,str){condition}h rd,[rn]{,+/-offset}
//cccc 000p u1wl nnnn dddd iiii 1011 iiii
//c = condition
//p = pre (0 = post)
//u = up
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//i = immediate hi
//i = immediate lo
auto ARM::arm_op_move_half_immediate() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 writeback = instruction() >> 21;
  uint1 l = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint4 ih = instruction() >> 8;
  uint4 il = instruction();

  uint32 rn = r(n);
  uint8 immediate = (ih << 4) + (il << 0);

  if(pre == 1) rn = up ? rn + immediate : rn - immediate;
  if(l == 1) r(d) = load(Half | Nonsequential, rn);
  if(l == 0) store(Half | Nonsequential, rn, r(d));
  if(pre == 0) rn = up ? rn + immediate : rn - immediate;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//ldr{condition}s(h,b) rd,[rn,rm]{!}
//ldr{condition}s(h,b) rd,[rn],rm
//cccc 000p u0w1 nnnn dddd ---- 11h1 mmmm
//c = condition
//p = pre (0 = post)
//u = up
//w = writeback
//n = rn
//d = rd
//h = half (0 = byte)
//m = rm
auto ARM::arm_op_load_register() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 writeback = instruction() >> 21;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint1 half = instruction() >> 5;
  uint4 m = instruction();

  uint32 rn = r(n);
  uint32 rm = r(m);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  uint32 word = load((half ? Half : Byte) | Nonsequential, rn);
  r(d) = half ? (int16)word : (int8)word;
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//ldr{condition}s(h,b) rd,[rn{,+/-offset}]{!}
//ldr{condition}s(h,b) rd,[rn]{,+/-offset}
//cccc 000p u1w1 nnnn dddd iiii 11h1 iiii
//c = condition
//p = pre (0 = post)
//u = up
//w = writeback
//n = rn
//d = rd
//i = immediate hi
//h = half (0 = byte)
//i = immediate lo
auto ARM::arm_op_load_immediate() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 writeback = instruction() >> 21;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint4 ih = instruction() >> 8;
  uint1 half = instruction() >> 5;
  uint4 il = instruction();

  uint32 rn = r(n);
  uint8 immediate = (ih << 4) + (il << 0);

  if(pre == 1) rn = up ? rn + immediate : rn - immediate;
  uint32 word = load((half ? Half : Byte) | Nonsequential, rn);
  r(d) = half ? (int16)word : (int8)word;
  if(pre == 0) rn = up ? rn + immediate : rn - immediate;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//mrs{condition} rd,(c,s)psr
//cccc 0001 0r00 ++++ dddd ---- 0000 ----
//c = condition
//r = SPSR (0 = CPSR)
//d = rd
auto ARM::arm_op_move_to_register_from_status() {
  uint1 source = instruction() >> 22;
  uint4 d = instruction() >> 12;

  if(source) {
    if(mode() == Processor::Mode::USR) return;
    if(mode() == Processor::Mode::SYS) return;
  }

  r(d) = source ? spsr() : cpsr();
}

//msr{condition} (c,s)psr:{fields},rm
//cccc 0001 0r10 ffff ++++ ---- 0000 mmmm
//c = condition
//r = SPSR (0 = CPSR)
//f = field mask
//m = rm
auto ARM::arm_op_move_to_status_from_register() {
  uint4 m = instruction();

  arm_move_to_status(r(m));
}

//bx{condition} rm
//cccc 0001 0010 ++++ ++++ ++++ 0001 mmmm
//c = condition
//m = rm
auto ARM::arm_op_branch_exchange_register() {
  uint4 m = instruction();

  cpsr().t = r(m) & 1;
  r(15) = r(m);
}

//msr{condition} (c,s)psr:{fields},#immediate
//cccc 0011 0r10 ffff ++++ rrrr iiii iiii
//c = condition
//r = SPSR (0 = CPSR)
//f = field mask
//r = rotate
//i = immediate
auto ARM::arm_op_move_to_status_from_immediate() {
  uint4 rotate = instruction() >> 8;
  uint8 immediate = instruction();

  uint32 rm = immediate;
  if(rotate) rm = ror(rm, 2 * rotate);

  arm_move_to_status(rm);
}

//{opcode}{condition}{s} rd,rm {shift} #immediate
//{opcode}{condition} rn,rm {shift} #immediate
//{opcode}{condition}{s} rd,rn,rm {shift} #immediate
//cccc 000o ooos nnnn dddd llll lss0 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//l = shift immediate
//s = shift
//m = rm
auto ARM::arm_op_data_immediate_shift() {
  uint1 save = instruction() >> 20;
  uint5 shift = instruction() >> 7;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint32 rs = shift;
  uint32 rm = r(m);
  carryout() = cpsr().c;

  if(mode == 0) rm = lsl(rm, rs);
  if(mode == 1) rm = lsr(rm, rs ? rs : 32);
  if(mode == 2) rm = asr(rm, rs ? rs : 32);
  if(mode == 3) rm = rs ? ror(rm, rs) : rrx(rm);

  arm_opcode(rm);
}

//{opcode}{condition}{s} rd,rm {shift} rs
//{opcode}{condition} rn,rm {shift} rs
//{opcode}{condition}{s} rd,rn,rm {shift} rs
//cccc 000o ooos nnnn dddd ssss 0ss1 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//s = rs
//s = shift
//m = rm
auto ARM::arm_op_data_register_shift() {
  uint1 save = instruction() >> 20;
  uint4 s = instruction() >> 8;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint8 rs = r(s);
  uint32 rm = r(m);
  carryout() = cpsr().c;

  if(mode == 0      ) rm = lsl(rm, rs < 33 ? rs : 33);
  if(mode == 1      ) rm = lsr(rm, rs < 33 ? rs : 33);
  if(mode == 2      ) rm = asr(rm, rs < 32 ? rs : 32);
  if(mode == 3 && rs) rm = ror(rm, rs & 31 == 0 ? 32 : rs & 31);

  arm_opcode(rm);
}

//{opcode}{condition}{s} rd,#immediate
//{opcode}{condition} rn,#immediate
//{opcode}{condition}{s} rd,rn,#immediate
//cccc 001o ooos nnnn dddd ssss iiii iiii
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//s = shift immediate
//i = immediate
auto ARM::arm_op_data_immediate() {
  uint1 save = instruction() >> 20;
  uint4 shift = instruction() >> 8;
  uint8 immediate = instruction();

  uint32 rm = immediate;

  carryout() = cpsr().c;
  if(shift) rm = ror(immediate, 2 * shift);

  arm_opcode(rm);
}

//(ldr,str){condition}{b} rd,[rn{,+/-offset}]{!}
//(ldr,str){condition}{b} rd,[rn]{,+/-offset}
//cccc 010p ubwl nnnn dddd iiii iiii iiii
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//b = byte (1 = word)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//i = immediate
auto ARM::arm_op_move_immediate_offset() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 byte = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 l = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint12 rm = instruction();

  uint32 rn = r(n);
  auto& rd = r(d);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(l == 1) rd = load((byte ? Byte : Word) | Nonsequential, rn);
  if(l == 0) store((byte ? Byte : Word) | Nonsequential, rn, rd);
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//(ldr,str){condition}{b} rd,[rn,rm {mode} #immediate]{!}
//(ldr,str){condition}{b} rd,[rn],rm {mode} #immediate
//cccc 011p ubwl nnnn dddd llll lss0 mmmm
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up
//b = byte (1 = word)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//l = shift immediate
//s = shift mode
//m = rm
auto ARM::arm_op_move_register_offset() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 byte = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 l = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint5 immediate = instruction() >> 7;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint32 rn = r(n);
  auto& rd = r(d);
  uint32 rs = immediate;
  uint32 rm = r(m);
  bool c = cpsr().c;

  if(mode == 0) rm = lsl(rm, rs);
  if(mode == 1) rm = lsr(rm, rs ? rs : 32);
  if(mode == 2) rm = asr(rm, rs ? rs : 32);
  if(mode == 3) rm = rs ? ror(rm, rs) : rrx(rm);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(l == 1) rd = load((byte ? Byte : Word) | Nonsequential, rn);
  if(l == 0) store((byte ? Byte : Word) | Nonsequential, rn, rd);
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//(ldm,stm){condition}{mode} rn{!},{r...}
//cccc 100p uswl nnnn llll llll llll llll
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//s = spsr copy -or- usr register copy
//w = writeback
//l = load (0 = save)
//n = rn
//l = register list
auto ARM::arm_op_move_multiple() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 s = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 l = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint16 list = instruction();

  uint32 rn = r(n);
  if(pre == 0 && up == 1) rn = rn + 0;  //IA
  if(pre == 1 && up == 1) rn = rn + 4;  //IB
  if(pre == 1 && up == 0) rn = rn - bit::count(list) * 4 + 0;  //DB
  if(pre == 0 && up == 0) rn = rn - bit::count(list) * 4 + 4;  //DA

  Processor::Mode pmode = mode();
  bool usr = false;
  if(s && l == 1 && (list & 0x8000) == 0) usr = true;
  if(s && l == 0) usr = true;

  if(usr) processor.setMode(Processor::Mode::USR);

  unsigned sequential = Nonsequential;
  for(unsigned m = 0; m < 16; m++) {
    if(list & 1 << m) {
      if(l == 1) r(m) = read(Word | sequential, rn);
      if(l == 0) write(Word | sequential, rn, r(m));
      rn += 4;
      sequential = Sequential;
    }
  }

  if(usr) processor.setMode(pmode);

  if(l == 1) {
    idle();
    if(s && (list & 0x8000)) {
      if(mode() != Processor::Mode::USR && mode() != Processor::Mode::SYS) {
        cpsr() = spsr();
        processor.setMode((Processor::Mode)cpsr().m);
      }
    }
  } else {
    pipeline.nonsequential = true;
  }

  if(writeback) {
    if(up == 1) r(n) = r(n) + bit::count(list) * 4;  //IA, IB
    if(up == 0) r(n) = r(n) - bit::count(list) * 4;  //DA, DB
  }
}

//b{l}{condition} address
//cccc 101l dddd dddd dddd dddd dddd dddd
//c = condition
//l = link
//d = displacement (24-bit signed)
auto ARM::arm_op_branch() {
  uint1 link = instruction() >> 24;
  int24 displacement = instruction();

  if(link) r(14) = r(15) - 4;
  r(15) += displacement * 4;
}

//swi #immediate
//cccc 1111 iiii iiii iiii iiii iiii iiii
//c = condition
//i = immediate
auto ARM::arm_op_software_interrupt() {
  uint24 immediate = instruction();

  vector(0x00000008, Processor::Mode::SVC);
}

#endif
