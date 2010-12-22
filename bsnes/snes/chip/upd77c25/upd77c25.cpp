//NEC uPD77C25 emulation core
//author: byuu
//license: public domain

#include <snes.hpp>

#define UPD77C25_CPP
namespace SNES {

#include "serialization.cpp"
#include "disassembler.cpp"

UPD77C25 upd77c25;
UPD77C25SR upd77c25sr;
UPD77C25DR upd77c25dr;

void UPD77C25::Enter() { upd77c25.enter(); }

void UPD77C25::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    uint24 opcode = programROM[regs.pc++];
    switch(opcode >> 22) {
      case 0: exec_op(opcode); break;
      case 1: exec_rt(opcode); break;
      case 2: exec_jp(opcode); break;
      case 3: exec_ld(opcode); break;
    }

    int32 result = (int32)regs.k * regs.l;  //sign + 30-bit result
    regs.m = result >> 15;  //store sign + top 15-bits
    regs.n = result <<  1;  //store low 15-bits + zero

    step(1);
    synchronize_cpu();
  }
}

void UPD77C25::exec_op(uint24 opcode) {
  uint2 pselect = opcode >> 20;  //P select
  uint4 alu     = opcode >> 16;  //ALU operation mode
  uint1 asl     = opcode >> 15;  //accumulator select
  uint2 dpl     = opcode >> 13;  //DP low modify
  uint4 dphm    = opcode >>  9;  //DP high XOR modify
  uint1 rpdcr   = opcode >>  8;  //RP decrement
  uint4 src     = opcode >>  4;  //move source
  uint4 dst     = opcode >>  0;  //move destination

  switch(src) {
    case  0: regs.idb = regs.trb; break;
    case  1: regs.idb = regs.a; break;
    case  2: regs.idb = regs.b; break;
    case  3: regs.idb = regs.tr; break;
    case  4: regs.idb = regs.dp; break;
    case  5: regs.idb = regs.rp; break;
    case  6: regs.idb = dataROM[regs.rp]; break;
    case  7: regs.idb = 0x8000 - regs.flaga.s1; break;
    case  8: regs.idb = regs.dr; regs.sr.rqm = 1; break;
    case  9: regs.idb = regs.dr; break;
    case 10: regs.idb = regs.sr; break;
  //case 11: regs.idb = regs.sim; break;
  //case 12: regs.idb = regs.sil; break;
    case 13: regs.idb = regs.k; break;
    case 14: regs.idb = regs.l; break;
    case 15: regs.idb = dataRAM[regs.dp]; break;
  }

  if(alu) {
    uint16 p, q, r;
    Flag flag;
    bool c;

    switch(pselect) {
      case 0: p = dataRAM[regs.dp]; break;
      case 1: p = regs.idb; break;
      case 2: p = regs.m; break;
      case 3: p = regs.n; break;
    }

    switch(asl) {
      case 0: q = regs.a; flag = regs.flaga; c = regs.flagb.c; break;
      case 1: q = regs.b; flag = regs.flagb; c = regs.flaga.c; break;
    }

    switch(alu) {
      case  1: r = q | p; break;                    //OR
      case  2: r = q & p; break;                    //AND
      case  3: r = q ^ p; break;                    //XOR
      case  4: r = q - p; break;                    //SUB
      case  5: r = q + p; break;                    //ADD
      case  6: r = q - p - c; break;                //SBB
      case  7: r = q + p + c; break;                //ADC
      case  8: r = q - 1; p = 1; break;             //DEC
      case  9: r = q + 1; p = 1; break;             //INC
      case 10: r = ~q; break;                       //CMP
      case 11: r = (q >> 1) | (q & 0x8000); break;  //SHR1 (ASR)
      case 12: r = (q << 1) | c; break;             //SHL1 (ROL)
      case 13: r = (q << 2); break;                 //SHL2
      case 14: r = (q << 4); break;                 //SHL4
      case 15: r = (q << 8) | (q >> 8); break;      //XCHG
    }

    flag.s0 = (r & 0x8000);
    flag.z = (r == 0);

    switch(alu) {
      case  1: case  2: case  3: case 10: case 13: case 14: case 15: {
        flag.c = 0;
        flag.ov0 = 0;
        flag.ov1 = 0;
        break;
      }
      case  4: case  5: case  6: case  7: case  8: case  9: {
        if(alu & 1) {
          //addition
          flag.ov0 = (q ^ r) & ~(q ^ p) & 0x8000;
          flag.c = (r < q);
        } else {
          //subtraction
          flag.ov0 = (q ^ r) &  (q ^ p) & 0x8000;
          flag.c = (r > q);
        }
        if(flag.ov0) {
          flag.s1 = flag.ov1 ^ !(r & 0x8000);
          flag.ov1 = !flag.ov1;
        }
        break;
      }
      case 11: {
        flag.c = q & 1;
        flag.ov0 = 0;
        flag.ov1 = 0;
        break;
      }
      case 12: {
        flag.c = q >> 15;
        flag.ov0 = 0;
        flag.ov1 = 0;
        break;
      }
    }

    switch(asl) {
      case 0: regs.a = r; regs.flaga = flag; break;
      case 1: regs.b = r; regs.flagb = flag; break;
    }
  }

  exec_ld((regs.idb << 6) + dst);

  switch(dpl) {
    case 1: regs.dp = (regs.dp & 0xf0) + ((regs.dp + 1) & 0x0f); break;  //DPINC
    case 2: regs.dp = (regs.dp & 0xf0) + ((regs.dp - 1) & 0x0f); break;  //DPDEC
    case 3: regs.dp = (regs.dp & 0xf0); break;  //DPCLR
  }

  regs.dp ^= dphm << 4;

  if(rpdcr) regs.rp--;
}

void UPD77C25::exec_rt(uint24 opcode) {
  exec_op(opcode);
  stack_pull();
}

void UPD77C25::exec_jp(uint24 opcode) {
  uint9 brch = opcode >> 13;  //branch
  uint11 na  = opcode >>  2;  //next address

  bool r = false;

  switch(brch) {
    case 0x080: r = (regs.flaga.c == 0);      break;  //JNCA
    case 0x082: r = (regs.flaga.c == 1);      break;  //JCA
    case 0x084: r = (regs.flagb.c == 0);      break;  //JNCB
    case 0x086: r = (regs.flagb.c == 1);      break;  //JCB
    case 0x088: r = (regs.flaga.z == 0);      break;  //JNZA
    case 0x08a: r = (regs.flaga.z == 1);      break;  //JZA
    case 0x08c: r = (regs.flagb.z == 0);      break;  //JNZB
    case 0x08e: r = (regs.flagb.z == 1);      break;  //JZB
    case 0x090: r = (regs.flaga.ov0 == 0);    break;  //JNOVA0
    case 0x092: r = (regs.flaga.ov0 == 1);    break;  //JOVA0
    case 0x094: r = (regs.flagb.ov0 == 0);    break;  //JNOVB0
    case 0x096: r = (regs.flagb.ov0 == 1);    break;  //JOVB0
    case 0x098: r = (regs.flaga.ov1 == 0);    break;  //JNOVA1
    case 0x09a: r = (regs.flaga.ov1 == 1);    break;  //JOVA1
    case 0x09c: r = (regs.flagb.ov1 == 0);    break;  //JNOVB1
    case 0x09e: r = (regs.flagb.ov1 == 1);    break;  //JOVB1
    case 0x0a0: r = (regs.flaga.s0 == 0);     break;  //JNSA0
    case 0x0a2: r = (regs.flaga.s0 == 1);     break;  //JSA0
    case 0x0a4: r = (regs.flagb.s0 == 0);     break;  //JNSB0
    case 0x0a6: r = (regs.flagb.s0 == 1);     break;  //JSB0
    case 0x0a8: r = (regs.flaga.s1 == 0);     break;  //JNSA1
    case 0x0aa: r = (regs.flaga.s1 == 1);     break;  //JSA1
    case 0x0ac: r = (regs.flagb.s1 == 0);     break;  //JNSB1
    case 0x0ae: r = (regs.flagb.s1 == 1);     break;  //JSB1

    case 0x0b0: r = (regs.dp & 0x0f) == 0x00; break;  //JDPL0
    case 0x0b1: r = (regs.dp & 0x0f) != 0x00; break;  //JDPLN0
    case 0x0b2: r = (regs.dp & 0x0f) == 0x0f; break;  //JDPLF
    case 0x0b3: r = (regs.dp & 0x0f) != 0x0f; break;  //JDPLNF

    case 0x0b4: r = (regs.siack == 0);        break;  //JNSIAK
    case 0x0b6: r = (regs.siack == 1);        break;  //JSIAK

    case 0x0b8: r = (regs.soack == 0);        break;  //JNSOAK
    case 0x0ba: r = (regs.soack == 1);        break;  //JSOAK

    case 0x0bc: r = (regs.sr.rqm == 0);       break;  //JNRQM
    case 0x0be: r = (regs.sr.rqm == 1);       break;  //JRQM

    case 0x100: r = true;                     break;  //JMP
    case 0x140: r = true; stack_push();       break;  //CALL
  }

  if(r) regs.pc = na;
}

void UPD77C25::exec_ld(uint24 opcode) {
  uint16 id = opcode >> 6;  //immediate data
  uint4 dst = opcode >> 0;  //destination

  regs.idb = id;

  switch(dst) {
    case  0: break;
    case  1: regs.a = id; break;
    case  2: regs.b = id; break;
    case  3: regs.tr = id; break;
    case  4: regs.dp = id; break;
    case  5: regs.rp = id; break;
    case  6: regs.dr = id; regs.sr.rqm = 1; break;
    case  7: regs.sr = (regs.sr & 0x907c) | (id & ~0x907c); break;
  //case  8: regs.sol = id; break;
  //case  9: regs.som = id; break;
    case 10: regs.k = id; break;
    case 11: regs.k = id; regs.l = dataROM[regs.rp]; break;
    case 12: regs.l = id; regs.k = dataRAM[regs.dp | 0x40]; break;
    case 13: regs.l = id; break;
    case 14: regs.trb = id; break;
    case 15: dataRAM[regs.dp] = id; break;
  }
}

void UPD77C25::stack_push() {
  regs.stack[3] = regs.stack[2];
  regs.stack[2] = regs.stack[1];
  regs.stack[1] = regs.stack[0];
  regs.stack[0] = regs.pc;
}

void UPD77C25::stack_pull() {
  regs.pc = regs.stack[0];
  regs.stack[0] = regs.stack[1];
  regs.stack[1] = regs.stack[2];
  regs.stack[2] = regs.stack[3];
  regs.stack[3] = 0x000;
}

uint8 UPD77C25::read(bool mode) {
  cpu.synchronize_coprocessor();
  if(mode == 0) return regs.sr >> 8;

  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      return regs.dr >> 0;
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      return regs.dr >> 8;
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    return regs.dr >> 0;
  }
}

void UPD77C25::write(bool mode, uint8 data) {
  cpu.synchronize_coprocessor();
  if(mode == 0) return;

  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      regs.dr = (regs.dr & 0xff00) | (data << 0);
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      regs.dr = (data << 8) | (regs.dr & 0x00ff);
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    regs.dr = (regs.dr & 0xff00) | (data << 0);
  }
}

void UPD77C25::init() {
}

void UPD77C25::enable() {
}

void UPD77C25::power() {
  for(unsigned i = 0; i <  256; i++) dataRAM[i] = 0x0000;
  reset();
}

void UPD77C25::reset() {
  create(UPD77C25::Enter, 8 * 1024 * 1024);  //8.192MHz

  regs.pc = 0x000;
  regs.stack[0] = 0x000;
  regs.stack[1] = 0x000;
  regs.stack[2] = 0x000;
  regs.stack[3] = 0x000;
  regs.flaga = 0x00;
  regs.flagb = 0x00;
  regs.sr = 0x0000;
  regs.rp = 0x3ff;
  regs.siack = 0;
  regs.soack = 0;
}

uint8 UPD77C25SR::read(unsigned) { return upd77c25.read(0); }
void UPD77C25SR::write(unsigned, uint8 data) { upd77c25.write(0, data); }

uint8 UPD77C25DR::read(unsigned) { return upd77c25.read(1); }
void UPD77C25DR::write(unsigned, uint8 data) { upd77c25.write(1, data); }

}
