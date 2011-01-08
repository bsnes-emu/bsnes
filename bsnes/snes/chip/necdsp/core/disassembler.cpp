#ifdef UPDCORE_CPP

string uPDcore::disassemble(uint11 ip) {
  string output = { hex<3>(ip), "  " };
  uint24 opcode = programROM[ip];
  uint2 type = opcode >> 22;

  if(type == 0 || type == 1) {  //OP,RT
    uint2 pselect = opcode >> 20;
    uint4 alu     = opcode >> 16;
    uint1 asl     = opcode >> 15;
    uint2 dpl     = opcode >> 13;
    uint4 dphm    = opcode >>  9;
    uint1 rpdcr   = opcode >>  8;
    uint4 src     = opcode >>  4;
    uint4 dst     = opcode >>  0;

    switch(alu) {
      case  0: output << "nop     "; break;
      case  1: output << "or      "; break;
      case  2: output << "and     "; break;
      case  3: output << "xor     "; break;
      case  4: output << "sub     "; break;
      case  5: output << "add     "; break;
      case  6: output << "sbb     "; break;
      case  7: output << "adc     "; break;
      case  8: output << "dec     "; break;
      case  9: output << "inc     "; break;
      case 10: output << "cmp     "; break;
      case 11: output << "shr1    "; break;
      case 12: output << "shl1    "; break;
      case 13: output << "shl2    "; break;
      case 14: output << "shl4    "; break;
      case 15: output << "xchg    "; break;
    }

    if(alu < 8) {
      switch(pselect) {
        case 0: output << "ram,"; break;
        case 1: output << "idb,"; break;
        case 2: output << "m,"; break;
        case 3: output << "n,"; break;
      }
    }

    switch(asl) {
      case 0: output << "a"; break;
      case 1: output << "b"; break;
    }

    if(dst) {
      output << "\n     mov     ";

      switch(src) {
        case  0: output << "trb,"; break;
        case  1: output << "a,"; break;
        case  2: output << "b,"; break;
        case  3: output << "tr,"; break;
        case  4: output << "dp,"; break;
        case  5: output << "rp,"; break;
        case  6: output << "ro,"; break;
        case  7: output << "sgn,"; break;
        case  8: output << "dr,"; break;
        case  9: output << "drnf,"; break;
        case 10: output << "sr,"; break;
        case 11: output << "sim,"; break;
        case 12: output << "sil,"; break;
        case 13: output << "k,"; break;
        case 14: output << "l,"; break;
        case 15: output << "mem,"; break;
      }

      switch(dst) {
        case  0: output << "non"; break;
        case  1: output << "a"; break;
        case  2: output << "b"; break;
        case  3: output << "tr"; break;
        case  4: output << "dp"; break;
        case  5: output << "rp"; break;
        case  6: output << "dr"; break;
        case  7: output << "sr"; break;
        case  8: output << "sol"; break;
        case  9: output << "som"; break;
        case 10: output << "k"; break;
        case 11: output << "klr"; break;
        case 12: output << "klm"; break;
        case 13: output << "l"; break;
        case 14: output << "trb"; break;
        case 15: output << "mem"; break;
      }
    }

    if(dpl) {
      switch(dpl) {
        case 0: output << "\n     dpnop"; break;
        case 1: output << "\n     dpinc"; break;
        case 2: output << "\n     dpdec"; break;
        case 3: output << "\n     dpclr"; break;
      }
    }

    if(dphm) {
      switch(dphm) {
        case  0: output << "\n     m0"; break;
        case  1: output << "\n     m1"; break;
        case  2: output << "\n     m2"; break;
        case  3: output << "\n     m3"; break;
        case  4: output << "\n     m4"; break;
        case  5: output << "\n     m5"; break;
        case  6: output << "\n     m6"; break;
        case  7: output << "\n     m7"; break;
        case  8: output << "\n     m8"; break;
        case  9: output << "\n     m9"; break;
        case 10: output << "\n     ma"; break;
        case 11: output << "\n     mb"; break;
        case 12: output << "\n     mc"; break;
        case 13: output << "\n     md"; break;
        case 14: output << "\n     me"; break;
        case 15: output << "\n     mf"; break;
      }
    }

    if(rpdcr == 1) {
      output << "\n     rpdec";
    }

    if(type == 1) {
      output << "\n     ret";
    }
  }

  if(type == 2) {  //JP
    uint9 brch = opcode >> 13;
    uint11 na  = opcode >>  2;

    switch(brch) {
      case 0x080: output << "jnca    "; break;
      case 0x082: output << "jca     "; break;
      case 0x084: output << "jncb    "; break;
      case 0x086: output << "jcb     "; break;
      case 0x088: output << "jnza    "; break;
      case 0x08a: output << "jza     "; break;
      case 0x08c: output << "jnzb    "; break;
      case 0x08e: output << "jzb     "; break;
      case 0x090: output << "jnova0  "; break;
      case 0x092: output << "jova0   "; break;
      case 0x094: output << "jnovb0  "; break;
      case 0x096: output << "jovb0   "; break;
      case 0x098: output << "jnova1  "; break;
      case 0x09a: output << "jova1   "; break;
      case 0x09c: output << "jnovb1  "; break;
      case 0x09e: output << "jovb1   "; break;
      case 0x0a0: output << "jnsa0   "; break;
      case 0x0a2: output << "jsa0    "; break;
      case 0x0a4: output << "jnsb0   "; break;
      case 0x0a6: output << "jsb0    "; break;
      case 0x0a8: output << "jnsa1   "; break;
      case 0x0aa: output << "jsa1    "; break;
      case 0x0ac: output << "jnsb1   "; break;
      case 0x0ae: output << "jsb1    "; break;
      case 0x0b0: output << "jdpl0   "; break;
      case 0x0b1: output << "jdpln0  "; break;
      case 0x0b2: output << "jdplf   "; break;
      case 0x0b3: output << "jdplnf  "; break;
      case 0x0b4: output << "jnsiak  "; break;
      case 0x0b6: output << "jsiak   "; break;
      case 0x0b8: output << "jnsoak  "; break;
      case 0x0ba: output << "jsoak   "; break;
      case 0x0bc: output << "jnrqm   "; break;
      case 0x0be: output << "jrqm    "; break;
      case 0x100: output << "jmp     "; break;
      case 0x140: output << "call    "; break;
      default:    output << "??????  "; break;
    }

    output << "$" << hex<3>(na);
  }

  if(type == 3) {  //LD
    output << "ld      ";
    uint16 id = opcode >> 6;
    uint4 dst = opcode >> 0;

    output << "$" << hex<4>(id) << ",";

    switch(dst) {
      case  0: output << "non"; break;
      case  1: output << "a"; break;
      case  2: output << "b"; break;
      case  3: output << "tr"; break;
      case  4: output << "dp"; break;
      case  5: output << "rp"; break;
      case  6: output << "dr"; break;
      case  7: output << "sr"; break;
      case  8: output << "sol"; break;
      case  9: output << "som"; break;
      case 10: output << "k"; break;
      case 11: output << "klr"; break;
      case 12: output << "klm"; break;
      case 13: output << "l"; break;
      case 14: output << "trb"; break;
      case 15: output << "mem"; break;
    }
  }

  return output;
}

#endif
