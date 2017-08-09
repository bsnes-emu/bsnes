auto ARM::thumb_opcode(uint4 opcode, uint4 d, uint4 m) {
  switch(opcode) {
  case  0: r(d) = bit(r(d) & r(m));          break;  //AND
  case  1: r(d) = bit(r(d) ^ r(m));          break;  //EOR
  case  2: r(d) = bit(lsl(r(d), r(m)));      break;  //LSL
  case  3: r(d) = bit(lsr(r(d), r(m)));      break;  //LSR
  case  4: r(d) = bit(asr(r(d), r(m)));      break;  //ASR
  case  5: r(d) = add(r(d), r(m), cpsr().c); break;  //ADC
  case  6: r(d) = sub(r(d), r(m), cpsr().c); break;  //SBC
  case  7: r(d) = bit(ror(r(d), r(m)));      break;  //ROR
  case  8:        bit(r(d) & r(m));          break;  //TST
  case  9: r(d) = sub(0, r(m), 1);           break;  //NEG
  case 10:        sub(r(d), r(m), 1);        break;  //CMP
  case 11:        add(r(d), r(m), 0);        break;  //CMN
  case 12: r(d) = bit(r(d) | r(m));          break;  //ORR
  case 13: r(d) = mul(0, r(m), r(d));        break;  //MUL
  case 14: r(d) = bit(r(d) & ~r(m));         break;  //BIC
  case 15: r(d) = bit(~r(m));                break;  //MVN
  }
}

//(add,sub) rd,rn,rm
//0001 10om mmnn nddd
//o = opcode
//m = rm
//n = rn
//d = rd
auto ARM::thumb_op_adjust_register() {
  uint1 opcode = instruction() >> 9;
  uint3 m = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = add(r(n), r(m), 0); break;
  case 1: r(d) = sub(r(n), r(m), 1); break;
  }
}

//(add,sub) rd,rn,#immediate
//0001 11oi iinn nddd
//o = opcode
//i = immediate
//n = rn
//d = rd
auto ARM::thumb_op_adjust_immediate() {
  uint1 opcode = instruction() >> 9;
  uint3 immediate = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = add(r(n), immediate, 0); break;
  case 1: r(d) = sub(r(n), immediate, 1); break;
  }
}

//(lsl,lsr,asr) rd,rm,#immediate
//000o oiii iimm mddd
//o = opcode
//i = immediate
//m = rm
//d = rd
auto ARM::thumb_op_shift_immediate() {
  uint2 opcode = instruction() >> 11;
  uint5 immediate = instruction() >> 6;
  uint3 m = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: r(d) = bit(lsl(r(m), immediate)); break;
  case 1: r(d) = bit(lsr(r(m), immediate == 0 ? 32u : (uint)immediate)); break;
  case 2: r(d) = bit(asr(r(m), immediate == 0 ? 32u : (uint)immediate)); break;
  }
}

//(mov,cmp,add,sub) (rd,rn),#immediate
//001o orrr iiii iiii
//o = opcode
//r = (rd,rn)
//i = immediate
auto ARM::thumb_op_immediate() {
  uint2 opcode = instruction() >> 11;
  uint3 d = instruction() >> 8;
  uint8 immediate = instruction();

  switch(opcode) {
  case 0: r(d) = bit(      immediate   ); break;
  case 1:        sub(r(d), immediate, 1); break;
  case 2: r(d) = add(r(d), immediate, 0); break;
  case 3: r(d) = sub(r(d), immediate, 1); break;
  }
}

//{opcode} rd,rm
//0100 00oo oomm mddd
//o = opcode
//m = rm
//d = rd
auto ARM::thumb_op_alu() {
  uint4 opcode = instruction() >> 6;
  uint3 m = instruction() >> 3;
  uint3 d = instruction();

  thumb_opcode(opcode, d, m);
}

//bx rm
//0100 0111 0mmm m---
//m = rm
auto ARM::thumb_op_branch_exchange() {
  uint4 m = instruction() >> 3;

  cpsr().t = r(m) & 1;
  r(15) = r(m);
}

//{opcode} rd,rm
//0100 01oo DMmm mddd
//o = opcode
//M:m = rm
//D:d = rd
auto ARM::thumb_op_alu_hi() {
  uint2 opcode = instruction() >> 8;
  uint4 m = instruction() >> 3;
  uint3 dl = instruction();
  uint1 dh = instruction() >> 7;

  uint4 d = (dh << 3) + (dl << 0);
  switch(opcode) {
  case 0: r(d) = r(d) + r(m); break;  //ADD (does not modify flags)
  case 1: sub(r(d), r(m), 1); break;  //SUB
  case 2: r(d) = r(m);        break;  //MOV (does not modify flags)
  }
}

//ldr rd,[pc,#offset]
//0100 1ddd oooo oooo
//d = rd
//o = offset
auto ARM::thumb_op_load_literal() {
  uint3 d = instruction() >> 8;
  uint8 displacement = instruction();

  uint rm = (r(15) & ~3) + displacement * 4;
  r(d) = load(Word | Nonsequential, rm);
}

//(ld(r,s),str){b,h} rd,[rn,rm]
//0101 ooom mmnn nddd
//o = opcode
//m = rm
//n = rn
//d = rd
auto ARM::thumb_op_move_register_offset() {
  uint3 opcode = instruction() >> 9;
  uint3 m = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  switch(opcode) {
  case 0: store(Word | Nonsequential, r(n) + r(m), r(d));          break;  //STR
  case 1: store(Half | Nonsequential, r(n) + r(m), r(d));          break;  //STRH
  case 2: store(Byte | Nonsequential, r(n) + r(m), r(d));          break;  //STRB
  case 3: r(d) = load(Byte | Nonsequential | Signed, r(n) + r(m)); break;  //LDSB
  case 4: r(d) = load(Word | Nonsequential, r(n) + r(m));          break;  //LDR
  case 5: r(d) = load(Half | Nonsequential, r(n) + r(m));          break;  //LDRH
  case 6: r(d) = load(Byte | Nonsequential, r(n) + r(m));          break;  //LDRB
  case 7: r(d) = load(Half | Nonsequential | Signed, r(n) + r(m)); break;  //LDSH
  }
}

//(ldr,str) rd,[rn,#offset]
//0110 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
auto ARM::thumb_op_move_word_immediate() {
  uint1 l = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  if(l == 1) r(d) = load(Word | Nonsequential, r(n) + offset * 4);
  if(l == 0) store(Word | Nonsequential, r(n) + offset * 4, r(d));
}

//(ldr,str)b rd,[rn,#offset]
//0111 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
auto ARM::thumb_op_move_byte_immediate() {
  uint1 l = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  if(l == 1) r(d) = load(Byte | Nonsequential, r(n) + offset);
  if(l == 0) store(Byte | Nonsequential, r(n) + offset, r(d));
}

//(ldr,str)h rd,[rn,#offset]
//1000 looo oonn nddd
//l = load
//o = offset
//n = rn
//d = rd
auto ARM::thumb_op_move_half_immediate() {
  uint1 l = instruction() >> 11;
  uint5 offset = instruction() >> 6;
  uint3 n = instruction() >> 3;
  uint3 d = instruction() >> 0;

  if(l == 1) r(d) = load(Half | Nonsequential, r(n) + offset * 2);
  if(l == 0) store(Half | Nonsequential, r(n) + offset * 2, r(d));
}

//(ldr,str) rd,[sp,#immediate]
//1001 oddd iiii iiii
//l = load
//d = rd
//i = immediate
auto ARM::thumb_op_move_stack() {
  uint1 l = instruction() >> 11;
  uint3 d = instruction() >> 8;
  uint8 immediate = instruction();

  if(l == 1) r(d) = load(Word | Nonsequential, r(13) + immediate * 4);
  if(l == 0) store(Word | Nonsequential, r(13) + immediate * 4, r(d));
}

//add rd,{pc,sp},#immediate
//1010 sddd iiii iiii
//s = sp (0 = pc)
//d = rd
//i = immediate
auto ARM::thumb_op_add_register_hi() {
  uint1 sp = instruction() >> 11;
  uint3 d = instruction() >> 8;
  uint8 immediate = instruction();

  if(sp == 0) r(d) = (r(15) & ~2) + immediate * 4;
  if(sp == 1) r(d) = r(13) + immediate * 4;
}

//(add,sub) sp,#immediate
//1011 0000 oiii iiii
//o = opcode
//i = immediate
auto ARM::thumb_op_adjust_stack() {
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
auto ARM::thumb_op_stack_multiple() {
  uint1 l = instruction() >> 11;
  uint1 branch = instruction() >> 8;
  uint8 list = instruction();

  uint32 sp = 0;
  if(l == 1) sp = r(13);
  if(l == 0) sp = r(13) - (bit::count(list) + branch) * 4;

  uint sequential = Nonsequential;
  for(uint m : range(8)) {
    if(list & 1 << m) {
      if(l == 1) r(m) = read(Word | sequential, sp);  //POP
      if(l == 0) write(Word | sequential, sp, r(m));  //PUSH
      sp += 4;
      sequential = Sequential;
    }
  }

  if(branch) {
    //note: ARMv5+ POP sets cpsr().t
    if(l == 1) r(15) = read(Word | Nonsequential, sp);  //POP
    if(l == 0) write(Word | Nonsequential, sp, r(14));  //PUSH
    sp += 4;
  }

  if(l == 1) {
    idle();
    r(13) += (bit::count(list) + branch) * 4;
  } else {
    pipeline.nonsequential = true;
    r(13) -= (bit::count(list) + branch) * 4;
  }
}

//(ldmia,stmia) rn!,{r...}
//1100 lnnn llll llll
//l = load (0 = save)
//n = rn
//l = register list
auto ARM::thumb_op_move_multiple() {
  uint1 l = instruction() >> 11;
  uint3 n = instruction() >> 8;
  uint8 list = instruction();
  uint32 rn = r(n);  //rn may be in register list; so we must cache it

  for(uint m : range(8)) {
    if(list & 1 << m) {
      if(l == 1) r(m) = read(Word | Nonsequential, rn);  //LDMIA
      if(l == 0) write(Word | Nonsequential, rn, r(m));  //STMIA
      rn += 4;
    }
  }

  if(l == 0 || (list & 1 << n) == 0) r(n) = rn;  //update rn on save or when not in register list
  if(l == 1) idle();
}

//swi #immediate
//1101 1111 iiii iiii
//i = immediate
auto ARM::thumb_op_software_interrupt() {
  uint8 immediate = instruction();

  interrupt(Processor::Mode::SVC, 0x08);
}

//b{condition}
//1101 cccc dddd dddd
//c = condition
//d = displacement
auto ARM::thumb_op_branch_conditional() {
  uint4 flagcondition = instruction() >> 8;
  int8 displacement = instruction();

  if(condition(flagcondition) == false) return;
  r(15) = r(15) + displacement * 2;
}

//b address
//1110 0ooo oooo oooo
//o = offset
auto ARM::thumb_op_branch_short() {
  int11 displacement = instruction();

  r(15) += displacement * 2;
}

//bl address
//1111 0ooo oooo oooo
//o = offset
auto ARM::thumb_op_branch_long_prefix() {
  int11 offsethi = instruction();

  r(14) = r(15) + ((offsethi * 2) << 11);
}

//bl address
//1111 1ooo oooo oooo
//o = offset
auto ARM::thumb_op_branch_long_suffix() {
  uint11 offsetlo = instruction();

  r(15) = r(14) + (offsetlo * 2);
  r(14) = pipeline.decode.address | 1;
}
