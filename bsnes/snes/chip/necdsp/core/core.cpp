#include <snes/snes.hpp>

#define UPDCORE_CPP
namespace SNES {

#include "disassembler.cpp"
#include "serialization.cpp"

void uPDcore::exec() {
#if 0
static uint16 lastpc = 0xffff;
if(lastpc != regs.pc) {
  print(
    "A:", hex<4>((uint16)regs.a), " B:", hex<4>((uint16)regs.b),
    " DR:", hex<4>(regs.dr), " SR:", hex<4>(regs.sr),
    " FA:", hex<2>(regs.flaga), " FB:", hex<2>(regs.flagb),
    " RP:", hex<4>(regs.rp), " DP:", hex<4>(regs.dp), " SO:", hex<4>(regs.so), "\n",
    "K: ", hex<4>((uint16)regs.k), " L:", hex<4>((uint16)regs.l),
    " M:", hex<4>((uint16)regs.m), " N:", hex<4>((uint16)regs.n),
    " TR:", hex<4>(regs.tr), " TRB:", hex<4>(regs.trb), " [DP15]:", hex<4>(dataRAM[0x15]), "\n"
  );
  print(disassemble(lastpc = regs.pc), "\n");
}
#endif

  uint24 opcode = programROM[regs.pc];
  regs.pc = regs.pc + 1;
  switch(opcode >> 22) {
    case 0: exec_op(opcode); break;
    case 1: exec_rt(opcode); break;
    case 2: exec_jp(opcode); break;
    case 3: exec_ld(opcode); break;
  }

  int32 result = (int32)regs.k * regs.l;  //sign + 30-bit result
  regs.m = result >> 15;  //store sign + top 15-bits
  regs.n = result <<  1;  //store low 15-bits + zero
}

void uPDcore::exec_op(uint24 opcode) {
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
    case 11: regs.idb = regs.si; break;  //MSB
    case 12: regs.idb = regs.si; break;  //LSB
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
      case 13: r = (q << 2) |  3; break;            //SHL2
      case 14: r = (q << 4) | 15; break;            //SHL4
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

  regs.dp = regs.dp ^ (dphm << 4);

  if(rpdcr) regs.rp = regs.rp - 1;
}

void uPDcore::exec_rt(uint24 opcode) {
  exec_op(opcode);
  stack_pull();
}

void uPDcore::exec_jp(uint24 opcode) {
  uint9 brch = opcode >> 13;  //branch
  uint11 na  = opcode >>  2;  //next address
  uint2 bank = opcode >>  0;  //bank address

  uint16 jps = (regs.pc & 0x3800) | (na << 0);
  uint16 jpl = (regs.pc & 0x2000) | (bank << 11) | (na << 0);

  switch(brch) {
    case 0x080: if(regs.flaga.c == 0) regs.pc = jps; return;  //JNCA
    case 0x082: if(regs.flaga.c == 1) regs.pc = jps; return;  //JCA
    case 0x084: if(regs.flagb.c == 0) regs.pc = jps; return;  //JNCB
    case 0x086: if(regs.flagb.c == 1) regs.pc = jps; return;  //JCB

    case 0x088: if(regs.flaga.z == 0) regs.pc = jps; return;  //JNZA
    case 0x08a: if(regs.flaga.z == 1) regs.pc = jps; return;  //JZA
    case 0x08c: if(regs.flagb.z == 0) regs.pc = jps; return;  //JNZB
    case 0x08e: if(regs.flagb.z == 1) regs.pc = jps; return;  //JZB

    case 0x090: if(regs.flaga.ov0 == 0) regs.pc = jps; return;  //JNOVA0
    case 0x092: if(regs.flaga.ov0 == 1) regs.pc = jps; return;  //JOVA0
    case 0x094: if(regs.flagb.ov0 == 0) regs.pc = jps; return;  //JNOVB0
    case 0x096: if(regs.flagb.ov0 == 1) regs.pc = jps; return;  //JOVB0

    case 0x098: if(regs.flaga.ov1 == 0) regs.pc = jps; return;  //JNOVA1
    case 0x09a: if(regs.flaga.ov1 == 1) regs.pc = jps; return;  //JOVA1
    case 0x09c: if(regs.flagb.ov1 == 0) regs.pc = jps; return;  //JNOVB1
    case 0x09e: if(regs.flagb.ov1 == 1) regs.pc = jps; return;  //JOVB1

    case 0x0a0: if(regs.flaga.s0 == 0) regs.pc = jps; return;  //JNSA0
    case 0x0a2: if(regs.flaga.s0 == 1) regs.pc = jps; return;  //JSA0
    case 0x0a4: if(regs.flagb.s0 == 0) regs.pc = jps; return;  //JNSB0
    case 0x0a6: if(regs.flagb.s0 == 1) regs.pc = jps; return;  //JSB0

    case 0x0a8: if(regs.flaga.s1 == 0) regs.pc = jps; return;  //JNSA1
    case 0x0aa: if(regs.flaga.s1 == 1) regs.pc = jps; return;  //JSA1
    case 0x0ac: if(regs.flagb.s1 == 0) regs.pc = jps; return;  //JNSB1
    case 0x0ae: if(regs.flagb.s1 == 1) regs.pc = jps; return;  //JSB1

    case 0x0b0: if((regs.dp & 0x0f) == 0x00) regs.pc = jps; return;  //JDPL0
    case 0x0b1: if((regs.dp & 0x0f) != 0x00) regs.pc = jps; return;  //JDPLN0
    case 0x0b2: if((regs.dp & 0x0f) == 0x0f) regs.pc = jps; return;  //JDPLF
    case 0x0b3: if((regs.dp & 0x0f) != 0x0f) regs.pc = jps; return;  //JDPLNF

    case 0x0bc: if(regs.sr.rqm == 0) regs.pc = jps; return;  //JNRQM
    case 0x0be: if(regs.sr.rqm == 1) regs.pc = jps; return;  //JRQM

    case 0x100: regs.pc = jps; return;                //JMP
    case 0x140: stack_push(); regs.pc = jps; return;  //CALL
  }

  switch(brch) {
    case 0x000: regs.pc = regs.so; return;

    case 0x081: if(regs.flaga.c == 1) regs.pc = jpl; return;

    case 0x0bf: if(regs.sr.rqm == 1) regs.pc = jpl; return;
  }

  print(hex<4>(regs.pc - 1), ": unknown jump ", hex<3>(brch), "\n");

/*bool r = false;
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
  case 0x0bc: r = (regs.sr.rqm == 0);       break;  //JNRQM
  case 0x0be: r = (regs.sr.rqm == 1);       break;  //JRQM
  case 0x100: r = true;                     break;  //JMP
  case 0x140: r = true; stack_push();       break;  //CALL
}
if(r) regs.pc = na;*/
}

void uPDcore::exec_ld(uint24 opcode) {
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
    case  8: regs.so = id; break;  //LSB
    case  9: regs.so = id; break;  //MSB
    case 10: regs.k = id; break;
    case 11: regs.k = id; regs.l = dataROM[regs.rp]; break;
    case 12: regs.l = id; regs.k = dataRAM[regs.dp | 0x40]; break;
    case 13: regs.l = id; break;
    case 14: regs.trb = id; break;
    case 15: dataRAM[regs.dp] = id; break;
  }
}

void uPDcore::stack_push() {
  for(unsigned r = 63; r >= 1; r--) regs.stack[r] = regs.stack[r - 1];
  regs.stack[0] = regs.pc;
}

void uPDcore::stack_pull() {
  regs.pc = regs.stack[0];
  for(unsigned r = 0; r <= 62; r++) regs.stack[r] = regs.stack[r + 1];
  regs.stack[63] = 0x0000;
}

uPDcore::uPDcore(unsigned pcbits, unsigned rpbits, unsigned dpbits) {
  programROMSize = 1 << pcbits;
  dataROMSize = 1 << rpbits;
  dataRAMSize = 1 << dpbits;

  programROM = new uint24[programROMSize];
  dataROM = new uint16[dataROMSize];
  dataRAM = new uint16[dataRAMSize];

  regs.pc.mask = programROMSize - 1;
  regs.rp.mask = dataROMSize - 1;
  regs.dp.mask = dataRAMSize - 1;
}

uPDcore::~uPDcore() {
  delete[] programROM;
  delete[] dataROM;
  delete[] dataRAM;
}

}
