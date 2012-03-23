void ARM::thumb_step() {
  if(pipeline.reload) {
    pipeline.reload = false;

    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus_read(r(15), Half);

    r(15).data += 2;
    pipeline.decode = pipeline.fetch;
    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus_read(r(15), Half);
    step(1);
  }

  r(15).data += 2;
  pipeline.execute = pipeline.decode;
  pipeline.decode = pipeline.fetch;
  pipeline.fetch.address = r(15);
  pipeline.fetch.instruction = bus_read(r(15), Half);
  step(1);

//print(disassemble_registers(), "\n");
//print(disassemble_thumb_opcode(pipeline.execute.address), "\n");

  if((instruction() & 0xfc00) == 0x1800) { thumb_op_adjust_register(); return; }
  if((instruction() & 0xfc00) == 0x1c00) { thumb_op_adjust_immediate(); return; }
  if((instruction() & 0xe000) == 0x0000) { thumb_op_shift_immediate(); return; }
  if((instruction() & 0xe000) == 0x2000) { thumb_op_immediate(); return; }
  if((instruction() & 0xfc00) == 0x4000) { thumb_op_alu(); return; }
  if((instruction() & 0xff80) == 0x4700) { thumb_op_branch_exchange(); return; }
  if((instruction() & 0xfc00) == 0x4400) { thumb_op_alu_hi(); return; }
  if((instruction() & 0xf800) == 0x4800) { thumb_op_load_literal(); return; }
  if((instruction() & 0xf000) == 0x5000) { thumb_op_move_register_offset(); return; }
  if((instruction() & 0xf000) == 0x6000) { thumb_op_move_word_immediate(); return; }
  if((instruction() & 0xf000) == 0x7000) { thumb_op_move_byte_immediate(); return; }
  if((instruction() & 0xf000) == 0x8000) { thumb_op_move_half_immediate(); return; }
  if((instruction() & 0xf000) == 0x9000) { thumb_op_move_stack(); return; }
  if((instruction() & 0xf000) == 0xa000) { thumb_op_add_register_hi(); return; }
  if((instruction() & 0xff00) == 0xb000) { thumb_op_adjust_stack(); return; }
  if((instruction() & 0xf600) == 0xb400) { thumb_op_stack_multiple(); return; }
  if((instruction() & 0xf000) == 0xc000) { thumb_op_move_multiple(); return; }
  if((instruction() & 0xff00) == 0xdf00) { thumb_op_software_interrupt(); return; }
  if((instruction() & 0xf000) == 0xd000) { thumb_op_branch_conditional(); return; }
  if((instruction() & 0xf800) == 0xe000) { thumb_op_branch_short(); return; }
  if((instruction() & 0xf800) == 0xf000) { thumb_op_branch_long_prefix(); return; }
  if((instruction() & 0xf800) == 0xf800) { thumb_op_branch_long_suffix(); return; }

  exception = true;
}

bool ARM::thumb_condition(uint4 condition) {
  switch(condition) {
  case  0: return cpsr().z == 1;                          //EQ (equal)
  case  1: return cpsr().z == 0;                          //NE (not equal)
  case  2: return cpsr().c == 1;                          //CS (carry set)
  case  3: return cpsr().c == 0;                          //CC (carry clear)
  case  4: return cpsr().n == 1;                          //MI (negative)
  case  5: return cpsr().n == 0;                          //PL (positive)
  case  6: return cpsr().v == 1;                          //VS (overflow)
  case  7: return cpsr().v == 0;                          //VC (no overflow)
  case  8: return cpsr().c == 1 && cpsr().z == 0;         //HI (unsigned higher)
  case  9: return cpsr().c == 0 || cpsr().z == 1;         //LS (unsigned lower or same)
  case 10: return cpsr().n == cpsr().v;                   //GE (signed greater than or equal)
  case 11: return cpsr().n != cpsr().v;                   //LT (signed less than)
  case 12: return cpsr().z == 0 && cpsr().n == cpsr().v;  //GT (signed greater than)
  case 13: return cpsr().z == 1 || cpsr().n != cpsr().v;  //LE (signed less than or equal)
  }
}

void ARM::thumb_opcode(uint4 opcode, uint4 d, uint4 m) {
  switch(opcode) {
  case  0: r(d) = thumb_tst(r(d) & r(m));          break;  //AND
  case  1: r(d) = thumb_tst(r(d) ^ r(m));          break;  //EOR
  case  2: r(d) = thumb_lsl(r(d), r(m) & 0xff);    break;  //LSL
  case  3: r(d) = thumb_lsr(r(d), r(m) & 0xff);    break;  //LSR
  case  4: r(d) = thumb_asr(r(d), r(m) & 0xff);    break;  //ASR
  case  5: r(d) = thumb_add(r(d), r(m), cpsr().c); break;  //ADC
  case  6: r(d) = thumb_sub(r(d), r(m), cpsr().c); break;  //SBC
  case  7: r(d) = thumb_ror(r(d), r(m) & 0xff);    break;  //ROR
  case  8:        thumb_tst(r(d) & r(m));          break;  //TST
  case  9: r(d) = thumb_sub(0, r(m));              break;  //NEG
  case 10:        thumb_sub(r(d), r(m));           break;  //CMP
  case 11:        thumb_add(r(d), r(m));           break;  //CMN
  case 12: r(d) = thumb_tst(r(d) | r(m));          break;  //ORR
  case 13: r(d) = thumb_mul(r(d), r(m));           break;  //MUL
  case 14: r(d) = thumb_tst(r(d) & ~r(m));         break;  //BIC
  case 15: r(d) = thumb_tst(~r(m));                break;  //MVN
  }
}

uint32 ARM::thumb_tst(uint32 modify) {
  cpsr().n = modify >> 31;
  cpsr().z = modify == 0;
  return modify;
}

void ARM::thumb_cmp(uint32 source, uint32 modify) {
  uint32 result = source - modify;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
}

uint32 ARM::thumb_add(uint32 source, uint32 modify, bool carry) {
  uint32 result = source + modify + carry;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
  return result;
}

uint32 ARM::thumb_sub(uint32 source, uint32 modify, bool carry) {
  uint32 result = source - modify + carry;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
  return result;
}

uint32 ARM::thumb_lsl(uint32 source, uint32 modify) {
  while(modify--) {
    cpsr().c = source >> 31;
    source <<= 1;
  }
  cpsr().n = source >> 31;
  cpsr().z = source == 0;
  return source;
}

uint32 ARM::thumb_lsr(uint32 source, uint32 modify) {
  while(modify--) {
    cpsr().c = source & 1;
    source >>= 1;
  }
  cpsr().n = source >> 31;
  cpsr().z = source == 0;
  return source;
}

uint32 ARM::thumb_asr(uint32 source, uint32 modify) {
  while(modify--) {
    cpsr().c = source & 1;
    source = (int32)source >> 1;
  }
  cpsr().n = source >> 31;
  cpsr().z = source == 0;
  return source;
}

uint32 ARM::thumb_ror(uint32 source, uint32 modify) {
  while(modify--) {
    cpsr().c = source & 1;
    source = (source << 31) | (source >> 1);
  }
  cpsr().n = source >> 31;
  cpsr().z = source == 0;
  return source;
}

uint32 ARM::thumb_mul(uint32 source, uint32 modify) {
  source = source * modify;
  cpsr().n = source >> 31;
  cpsr().z = source == 0;
  cpsr().c = 0;
  return source;
}

//(add,sub) rd,rn,rm
//0001 10om mmnn nddd
//o = opcode
//m = rm
//n = rn
//d = rd
void ARM::thumb_op_adjust_register() {
  uint1 opcode = instruction() >> 9;
  uint3 m = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = thumb_add(r(n), r(m)); break;
  case 1: r(d) = thumb_sub(r(n), r(m)); break;
  }
}

//(add,sub) rd,rn,#immediate
//0001 11oi iinn nddd
//o = opcode
//i = immediate
//n = rn
//d = rd
void ARM::thumb_op_adjust_immediate() {
  uint1 opcode = instruction() >> 9;
  uint3 immediate = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = thumb_add(r(n), immediate); break;
  case 1: r(d) = thumb_sub(r(n), immediate); break;
  }
}

//(lsl,lsr,asr) rd,rm,#immediate
//000o oiii iimm mddd
//o = opcode
//i = immediate
//m = rm
//d = rd
void ARM::thumb_op_shift_immediate() {
  uint2 opcode = instruction() >> 11;
  uint5 immediate = instruction() >> 6;
  uint3 m = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = thumb_lsl(r(m), immediate); break;
  case 1: r(d) = thumb_lsr(r(m), immediate); break;
  case 2: r(d) = thumb_asr(r(m), immediate); break;
  }
}

//(mov,cmp,add,sub) (rd,rn),#immediate
//001o orrr iiii iiii
//o = opcode
//r = (rd,rn)
//i = immediate
void ARM::thumb_op_immediate() {
  uint2 opcode = instruction() >> 11;
  uint3 d = instruction() >> 8;
  uint8 immediate = instruction();

  switch(opcode) {
  case 0: r(d) = thumb_tst(      immediate); break;
  case 1:        thumb_cmp(r(d), immediate); break;
  case 2: r(d) = thumb_add(r(d), immediate); break;
  case 3: r(d) = thumb_sub(r(d), immediate); break;
  }
}

//{opcode} rd,rm
//0100 00oo oomm mddd
//o = opcode
//m = rm
//d = rd
void ARM::thumb_op_alu() {
  uint4 opcode = instruction() >> 6;
  uint3 m = instruction() >> 3;
  uint3 d = instruction();

  thumb_opcode(opcode, d, m);
}

//bx rm
//0100 0111 0mmm m---
//m = rm
void ARM::thumb_op_branch_exchange() {
  uint4 m = instruction() >> 3;

  r(15) = r(m);
  cpsr().t = r(m) & 1;
}

//{opcode} rd,rm
//0100 01oo DMmm mddd
//o = opcode
//M:m = rm
//D:d = rd
void ARM::thumb_op_alu_hi() {
  uint2 opcode = instruction() >> 8;
  uint4 m = instruction() >> 3;
  uint3 dl = instruction();
  uint1 dh = instruction() >> 7;

  uint4 d = (dh << 3) + (dl << 0);
  switch(opcode) {
  case 0: r(d) = thumb_add(r(d), r(m)); break;  //ADD
  case 1:        thumb_sub(r(d), r(m)); break;  //SUB
  case 2: r(d) = thumb_tst(      r(m)); break;  //MOV
  }
}

//ldr rd,[pc,#+/-offset]
//0100 1ddd oooo oooo
//d = rd
//o = offset
void ARM::thumb_op_load_literal() {
  uint3 d = instruction() >> 8;
  uint8 displacement = instruction();

  unsigned rm = (r(15) & ~3) + displacement * 4;
  r(d) = bus_read(rm, Word);
}

//(ld(r,s),str){b,h} rd,[rn,rm]
//0101 ooom mmnn nddd
//o = opcode
//m = rm
//n = rn
//d = rd
void ARM::thumb_op_move_register_offset() {
  uint3 opcode = instruction() >> 9;
  uint3 m = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: bus_write(r(n) + r(m), Word, r(d));        break;  //STR
  case 1: bus_write(r(n) + r(m), Half, r(d));        break;  //STRH
  case 2: bus_write(r(n) + r(m), Byte, r(d));        break;  //STRB
  case 3: r(d) = (int8)bus_read(r(n) + r(m), Byte);  break;  //LDSB
  case 4: r(d) = bus_read(r(n) + r(m), Word);        break;  //LDR
  case 5: r(d) = bus_read(r(n) + r(m), Half);        break;  //LDRH
  case 6: r(d) = bus_read(r(n) + r(m), Byte);        break;  //LDRB
  case 7: r(d) = (int16)bus_read(r(n) + r(m), Half); break;  //LDSH
  }
}

//(ldr,str) rd,[rn,#offset]
//0110 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
void ARM::thumb_op_move_word_immediate() {
  uint1 load = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  if(load == 1) r(d) = bus_read(r(n) + offset * 4, Word);
  if(load == 0) bus_write(r(d) + offset * 4, Word, r(n));
}

//(ldr,str)b rd,[rn,#offset]
//0111 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
void ARM::thumb_op_move_byte_immediate() {
  uint1 load = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint4 d = instruction() >> 0;

  if(load == 1) r(d) = bus_read(r(n) + offset, Byte);
  if(load == 0) bus_write(r(d) + offset, Byte, r(n));
}

//(ldr,str)h rd,[rn,#offset]
//1000 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
void ARM::thumb_op_move_half_immediate() {
  uint1 load = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  if(load == 1) r(d) = bus_read(r(n) + offset * 2, Half);
  if(load == 0) bus_write(r(d) + offset * 2, Half, r(n));
}

//(ldr,str) rd,[sp,#relative]
//1001 oddd rrrr rrrr
//o = opcode
//d = rd
//r = relative
void ARM::thumb_op_move_stack() {
  uint1 opcode = instruction() >> 11;
  uint3 d = instruction() >> 8;
  int8 relative = instruction();

  if(opcode == 0) bus_write(r(13) + relative * 4, Word, r(d));
  if(opcode == 1) r(d) = bus_read(r(13) + relative * 4, Word);
}

//add rd,{pc,sp},#immediate
//1010 sddd iiii iiii
//s = sp (0 = pc)
//d = rd
//i = immediate
void ARM::thumb_op_add_register_hi() {
  uint1 sp = instruction() >> 11;
  uint3 d = instruction() >> 8;
  uint8 immediate = instruction();

  if(sp == 0) r(d) = (r(15) & ~2) + immediate;
  if(sp == 1) r(d) = r(13) + immediate;
}

//(add,sub) sp,#immediate
//1011 0000 oiii iiii
//o = opcode
//i = immediate
void ARM::thumb_op_adjust_stack() {
  uint1 opcode = instruction() >> 7;
  uint7 immediate = instruction();

  if(opcode == 0) r(13) += immediate * 4;
  if(opcode == 1) r(13) -= immediate * 4;
}

//push {r...{,lr}}
//pop {r...{,pc}}
//1011 o10r llll llll
//o = opcode (0 = push, 1 = pop)
//r = push lr -or- pop pc
//l = register list
void ARM::thumb_op_stack_multiple() {
  uint1 load = instruction() >> 11;
  uint1 branch = instruction() >> 8;
  uint8 list = instruction();

  if(load == 1) {
    for(unsigned l = 0; l < 8; l++) {
      if(list & (1 << l)) {
        r(l) = bus_read(r(13), Word);
        r(13) += 4;
      }
    }
    if(branch) {
      r(15) = bus_read(r(13), Word);
      r(13) += 4;
    }
  }

  if(load == 0) {
    for(unsigned l = 0; l < 8; l++) {
      if(list & (1 << l)) {
        r(13) -= 4;
        bus_write(r(13), Word, r(l));
      }
    }
    if(branch) {
      r(13) -= 4;
      bus_write(r(13), Word, r(14));
    }
  }
}

//(ldmia,stmdb) rn!,{r...}
//1100 lnnn llll llll
//l = load (0 = save)
//n = rn
//l = register list
void ARM::thumb_op_move_multiple() {
  uint1 load = instruction() >> 11;
  uint3 n = instruction() >> 8;
  uint8 list = instruction();

  if(load == 1) {
    for(unsigned l = 0; l < 8; l++) {
      if(list & (1 << l)) {
        r(l) = bus_read(r(n), Word);
        r(n) += 4;
      }
    }
  }

  if(load == 0) {
    for(unsigned l = 0; l < 8; l++) {
      if(list & (1 << l)) {
        r(n) -= 4;
        bus_write(r(n), Word, r(l));
      }
    }
  }
}

//swi #immediate
//1101 1111 iiii iiii
//i = immediate
void ARM::thumb_op_software_interrupt() {
  uint8 immediate = instruction();

  auto psr = cpsr();
  processor.setMode(Processor::Mode::SVC);
  r(14) = r(15) - 2;
  spsr() = psr;
  cpsr().t = 0;
  cpsr().i = 1;
  r(15) = 0x00000008;
}

//b{condition}
//1101 cccc dddd dddd
//c = condition
//d = displacement
void ARM::thumb_op_branch_conditional() {
  uint4 condition = instruction() >> 8;
  int8 displacement = instruction();

  if(thumb_condition(condition) == false) return;
  r(15) = r(15) + displacement * 2;
}

//b address
//1110 0ooo oooo oooo
//o = offset
void ARM::thumb_op_branch_short() {
  int11 displacement = instruction();

  r(15) += displacement * 2;
}

//bl address
//1111 0ooo oooo oooo
//o = offset
void ARM::thumb_op_branch_long_prefix() {
  uint11 offsethi = instruction();

  r(14) = offsethi;
}

//bl address
//1111 1ooo oooo oooo
//o = offset
void ARM::thumb_op_branch_long_suffix() {
  uint11 offsetlo = instruction();

  int22 displacement = ((uint11)r(14) << 11) | offsetlo;
  r(14) = r(15) | 1;
  r(15) += displacement * 2;
}
