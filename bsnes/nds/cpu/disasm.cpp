//#include "bit.hpp"
namespace disasm {

#define collect(i, bits)  (Bit::collect< Bit::field(bits) >((long)i))
#define match(i, bits)    ((i & force< Bit::mask(bits) >()) \
                             == force< Bit::binary(bits) >())

template<uint32 arg> static constexpr uint32 force() { return arg; }

static string conds[] = {
  "eq","ne","cs","cc","mi","pl","vs","vc","hi","ls","ge","lt","gt","le","",""
};
static string regs[] = {
  "r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","sp","lr","pc"
};
static string aluops[] = {
  "and","eor","sub","rsb","add","adc","sbc","rsc","tst","teq","cmp","cmn","orr","mov","bic","mvn"
};
static string shiftops[] = {
  "lsl","lsr","asr","ror"
};
static string blockops[] = {
  "da","ia","db","ib"
};

string armAddr(uint2 index, uint4 rn, string u, string rm) {
  if(index <  2) return {"[", regs[rn], "], ", u, rm};
  if(index == 2) return {"[", regs[rn], ", ", u, rm, "]"};
  if(index == 3) return {"[", regs[rn], ", ", u, rm, "]!"};
}

string armImmed(uint8 imm, uint4 rs) {
  if(!rs)        return {"#0x",hex<8>(imm)};
  if(imm & 0x03) return {"#0x",hex<8>(imm << 32-2*rs | imm >> 2*rs)};
  else           return {"#0x",hex<2>(imm), ", ", 2*rs};
}

string armOffset8(uint4 lo, uint4 hi) {
  return {"#0x",hex<2>(hi<<4 | lo)};
}

string armOffset12(uint12 offset) {
  return {"#0x",hex<3>(offset)};
}

string armRm(uint4 rm) {
  return regs[rm];
}

string armShiftImm(uint4 rm, uint2 sh, uint5 imm) {
  if(sh==0 && imm==0) return regs[rm];
  if(sh==3 && imm==0) return {regs[rm], ", rrx"};
  return {regs[rm], ", ", shiftops[sh], " #", imm==0? 32u : (unsigned)imm};
}

string armShiftReg(uint4 rm, uint2 sh, uint4 rs) {
  return {regs[rm], ", ", shiftops[sh], " ", regs[rs]};
}

string armUndefined(uint32 i) {
  return {"<undefined> #0x", hex<8>(i)};
}

string armBkpt(uint4 cc, uint12 hi, uint4 lo) {
  return {"bkpt", conds[cc], " #0x", hex<4>(hi<<4 | lo)};
}

string armSwi(uint4 cc, uint24 immed) {
  return {"swi", conds[cc], " #0x", hex<6>(immed)};
}

string armWritePsr(uint4 cc, uint1 psr, uint4 mask, string rm) {
  string f = mask&8? "f":"";
  string s = mask&4? "s":"";
  string x = mask&2? "x":"";
  string c = mask&1? "c":"";
  return {"msr", conds[cc], " ", psr? "spsr":"cpsr", "_", f,s,x,c, ", ", rm};
}

string armReadPsr(uint4 cc, uint1 psr, uint4 rd) {
  return {"mrs", conds[cc], " ", regs[rd], ", ", psr? "spsr":"cpsr"};
}

string armClz(uint4 cc, uint4 rd, uint4 rm) {
  return {"clz", conds[cc], " ", regs[rd], ", ", regs[rm]};
}

string armDspAdd(uint4 cc, uint2 op, uint4 rd, uint4 rn, uint4 rm) {
  string alu = op&1? "sub":"add";
  string d   = op&2? "d":"";
  return {"q",d,alu, conds[cc], " ", regs[rd], ", ", regs[rn], ", ", regs[rm]};
}

string armDspMul(uint4 cc, uint2 op, uint2 xy, uint4 rd, uint4 rn, uint4 rm, uint4 rs) {
  string x = xy&1? "t":"b";
  string y = xy&2? "t":"b";
  
  if(op==3)           return {"smul",x,y, conds[cc], " ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op==0)           return {"smla",x,y, conds[cc], " ", regs[rd], ", ", regs[rm], ", ", regs[rs], ", ", regs[rn]};
  if(op==2)           return {"smlal",x,y,conds[cc], " ", regs[rn], ", ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  
  if(op==1 && (xy&1)) return {"smulw",y,  conds[cc], " ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op==1)           return {"smlaw",y,  conds[cc], " ", regs[rd], ", ", regs[rm], ", ", regs[rs], ", ", regs[rn]};
}

string armMultiply(uint4 cc, uint4 op, uint4 rd, uint4 rn, uint4 rm, uint4 rs) {
  string s = op & 1? "s":"";
  op &= ~1;
  
  if(op== 0) return {"mul",   conds[cc], s, " ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op== 2) return {"mla",   conds[cc], s, " ", regs[rd], ", ", regs[rm], ", ", regs[rs], ", ", regs[rn]};
  
  if(op== 8) return {"umull", conds[cc], s, " ", regs[rn], ", ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op==10) return {"umlal", conds[cc], s, " ", regs[rn], ", ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op==12) return {"smull", conds[cc], s, " ", regs[rn], ", ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
  if(op==14) return {"smlal", conds[cc], s, " ", regs[rn], ", ", regs[rd], ", ", regs[rm], ", ", regs[rs]};
}

string armData(uint4 cc, uint5 op, uint4 rd, uint4 rn, string rm) {
  string alu = aluops[collect(op, "aaaa.")];
  string s   =        collect(op, "....s")? "s" : "";
  
  if(alu == "cmp" || alu == "cmn" || alu == "tst" || alu == "teq")
    return {alu, conds[cc], s, " ", regs[rn], ", ", rm};
  
  if(alu == "mov" || alu == "mvn" || rd == rn)
    return {alu, conds[cc], s, " ", regs[rd], ", ", rm};
  
  return {alu, conds[cc], s, " ", regs[rd], ", ", regs[rn], ", ", rm};
}

string armMem(uint4 cc, uint5 op, uint4 rd, uint4 rn, string rm) {
  uint2 index = collect(op, "p..w.");
  string u    = collect(op, ".u...")? "" : "-";
  string b    = collect(op, "..b..")? "b" : "";
  uint1  l    = collect(op, "....l");
  
  return {cc==15? "pld" : l? "ldr":"str", conds[cc], b, index==1?"t":"",
    " ", regs[rd], ", ", armAddr(index, rn, u, rm)};
}

string armMem_v4(uint4 cc, uint5 op1, uint2 op2, uint4 rd, uint4 rn, string rm) {
  uint2 index = collect(op1, "p..w.");
  string u    = collect(op1, ".u...")? "" : "-";
  uint1  l    = collect(op1, "....l");
  string sh   = op2==2?"sb" : op2==3?"sh" : "h";
  
  return {l?"ldr":"str", conds[cc], sh, " ", regs[rd], ", ", armAddr(index, rn, u, rm)};
}

string armMem_v5(uint4 cc, uint5 op1, uint2 op2, uint4 rd, uint4 rn, string rm) {
  uint2 index = collect(op1, "p..w.");
  string u    = collect(op1, ".u...")? "" : "-";
  uint1  s    = op2 & 1;
  
  return {s?"str":"ldr", conds[cc], "d ", regs[rd], ", ", armAddr(index, rn, u, rm)};
}

string armMemSwap(uint4 cc, uint1 op, uint4 rd, uint4 rn, uint4 rm) {
  string b = op? "b":"";
  return {"swp", conds[cc], b, " ", regs[rm],", ", regs[rd], ", [", regs[rn], "]"};
}

string armMemCop(uint4 cc, uint4 cpno, uint5 op, uint4 rd, uint4 rn, uint8 offset) {
  uint2 index = collect(op, "p..w.");
  string u    = collect(op, ".u...")? "" : "-";
  string n    = collect(op, "..n..")? "l" : "";
  uint1  l    = collect(op, "....l");
  
  return {l? "ldc":"stc", conds[cc], cc==15?"2":"", n, " p",cpno, ", c",regs[rd], ", ",
    armAddr(index, rn, index?u:"#", {"0x",hex<2>(offset)})};
}

string armBlock(uint4 cc, uint5 op, uint4 rn, uint16 rlist) {
  uint2 index = collect(op, "pu...");
  string s    = collect(op, "..s..")? "^" : "";
  string w    = collect(op, "...w.")? "!" : "";
  uint1 l     = collect(op, "....l");
  
  string regnames = "";
  for(unsigned b = 0; b < 16; b++) {
    if(rlist & 1<<b) regnames = {regnames, ",", regs[b]};
  }
  regnames.ltrim(",");
  return {l?"ldm":"stm", conds[cc], blockops[index], " ", regs[rn], w, ", {", regnames, "}", s};
}

string armBranch(uint4 cc, uint1 link, uint1 exch, uint32 target) {
  return {exch?"blx" : link?"bl" : "b", conds[cc], " 0x", hex<8>(target) + 2*link*exch};
}

string armBranchEx(uint4 cc, uint2 sh, uint4 rm) {
  return {sh & 1? "blx":"bx", conds[cc], " ", regs[rm]};
}

string armCop(uint4 cc, uint4 cpno, uint4 op1, uint4 rd, uint4 rn, uint4 rm, uint4 op2) {
  bool cdp = ~op2 & 1;
  bool l   =  op1 & 1;
  if(!cdp) op1 >>= 1;
  op2 >>= 1;
  return {cdp?"cdp" : l? "mrc":"mcr", conds[cc], cc==15?"2":"",
    " p",cpno, ",", op1>>1, ", ", cdp?"cr":"r",rd, ", ", "cr",rn, ", ", "cr",rm, ", ", op2};
}

string armCop_v5(uint4 cc, uint4 cpno, uint4 op1, uint4 rd, uint4 rn, uint4 rm, uint4 op2) {
  bool l = op1 & 1;
  return {l? "mrrc":"mcrr", conds[cc], cc==15?"2":"",
    " p",cpno, ",", op2, ", ", regs[rd], ", ", regs[rn], ", ", "cr",rm};
}



static string thumbmemregops[] = {
  "str", "strh", "strb", "ldrsb", "ldr", "ldrh", "ldrb", "ldrsh"
};
static string thumbmemimmops[] = {
  "str", "ldr", "strb", "ldrb", "strh", "ldrh"
};

string thumbUndefined(uint16 i) {
  return {"<undefined> #0x", hex<4>(i)};
}

string thumbBkpt(uint8 imm) {
  return {"bkpt #0x", hex<2>(imm)};
}

string thumbSwi(uint8 imm) {
  return {"swi #0x", hex<2>(imm)};
}

string thumbAddSub(uint2 op, uint3 rd, uint3 rn, uint3 rm) {
  if(op == 2 && rm == 0) return {"mov ", regs[rd], ", ", regs[rn]};
  if(op & 2) return {op & 1?"sub":"add", " ", regs[rd], ", ", regs[rn], ", #", rm};
  else       return {op & 1?"sub":"add", " ", regs[rd], ", ", regs[rn], ", ", regs[rm]};
}

string thumbShiftImm(uint2 op, uint3 rd, uint3 rm, uint5 imm) {
  return {shiftops[op], " ", regs[rd], ", ", regs[rm], ", #", op==0 || imm? (unsigned)imm : 32};
}

string thumbDataImm(uint2 op, uint3 rd, uint8 imm) {
  if(op == 0) return {"mov ", regs[rd], ", #0x", hex<2>(imm)};
  if(op == 1) return {"cmp ", regs[rd], ", #0x", hex<2>(imm)};
  if(op == 2) return {"add ", regs[rd], ", #0x", hex<2>(imm)};
  if(op == 3) return {"sub ", regs[rd], ", #0x", hex<2>(imm)};
}

string thumbDataLo(uint4 op, uint3 rd, uint3 rm) {
  if(op == 2) return {"lsl ", regs[rd], ", ", regs[rm]};
  if(op == 3) return {"lsr ", regs[rd], ", ", regs[rm]};
  if(op == 4) return {"asr ", regs[rd], ", ", regs[rm]};
  if(op == 7) return {"ror ", regs[rd], ", ", regs[rm]};
  if(op == 9) return {"neg ", regs[rd], ", ", regs[rm]};
  if(op ==13) return {"mul ", regs[rd], ", ", regs[rm]};
  
  return {aluops[op], " ", regs[rd], ", ", regs[rm]};
}

string thumbDataHi(uint2 op, uint4 rd, uint4 rm) {
  if(op == 0) return {"add ", regs[rd], ", ", regs[rm]};
  if(op == 1) return {"cmp ", regs[rd], ", ", regs[rm]};
  if(op == 2) return {"mov ", regs[rd], ", ", regs[rm]};
  if(rd & 8)  return {"blx ", regs[rm]};
  else        return {"bx ", regs[rm]};
}

string thumbMemReg(uint3 op, uint3 rd, uint3 rn, uint3 rm) {
  return {thumbmemregops[op], " ", regs[rd], ", [", regs[rn], ", ", regs[rm], "]"};
}

string thumbMemImm(uint5 op, uint3 rd, uint3 rn, uint5 offset) {
  unsigned rm = offset;
  if(op <  14) rm *= 4;
  if(op >= 16) rm *= 2;
  
  return {thumbmemimmops[op-12], " ", regs[rd], ", [", regs[rn], ", #0x", hex<3>(rm), "]"};
}

string thumbAddSP(uint1 op, uint7 offset) {
  return {op?"sub":"add", " sp, #0x", hex<3>(4*offset)};
}

string thumbRelative(uint5 op, uint3 rd, uint8 offset) {
  if(op == 9) return {"ldr ", regs[rd], ", [pc, #0x", hex<3>(4*offset), "]"};
  if(op ==18) return {"str ", regs[rd], ", [sp, #0x", hex<3>(4*offset), "]"};
  if(op ==19) return {"ldr ", regs[rd], ", [sp, #0x", hex<3>(4*offset), "]"};
  if(op ==20) return {"add ", regs[rd], ", pc, ", "#0x", hex<3>(4*offset)};
  if(op ==21) return {"add ", regs[rd], ", sp, ", "#0x", hex<3>(4*offset)};
}

string thumbBlock(uint4 op, uint3 rn, uint8 rlist) {
  bool l = op & 1;
  string regnames = "";
  for(unsigned b = 0; b < 8; b++) {
    if(rlist & 1<<b) regnames = {regnames, ",", regs[b]};
  }
  if(op < 8) {
    if( l && (rn & 1)) regnames = {regnames, ",pc"};
    if(!l && (rn & 1)) regnames = {regnames, ",lr"};
    
    regnames.ltrim(",");
    return {l? "pop":"push", " {", regnames, "}"};
  }
  else {
    regnames.ltrim(",");
    return {l? "ldm":"stm", " ", regs[rn], "!, {", regnames, "}"};
  }
}

string thumbCond(uint4 cc, uint32 target) {
  return {"b", conds[cc], " 0x", hex<8>(target)};
}

string thumbBranch(uint32 target) {
  return {"b 0x", hex<8>(target)};
}

string thumbBh(int11 offset_hi) {
  return {"bh 0x", hex<3>(2*offset_hi)};
}

string thumbBl(uint1 tbit, uint32 target) {
  return {tbit? "bl":"blx", " 0x", hex<8>(target)};
}

string arm(ARMCore* arm, uint32 i) {
  uint8 op = i>>20;
  uint4 cc = i>>28;
  uint4 rd = i>>12, rn = i>>16;
  uint4 rm = i,     rs = i>>8;
  uint2 sh = i>>5;
  uint32 pc = arm->r[15] & ~3;
  
  if(match(i, "1111 0101u101/// ..../")) return armMem(15, op, rd, rn, armOffset12(i));
  if(match(i, "1111 0111u101/// ...0/")) return armMem(15, op, rd, rn, armShiftImm(rm, sh, i>>7));
  if(match(i, "1111 101l..../// ..../")) return armBranch(14, 0, 1, pc + 4*int24(i));
  if(match(i, "1111 1100000l/// ..../")) return armUndefined(i);
  if(match(i, "1111 1100010l/// ..../")) return armCop_v5(15, i>>8, op, rd, rn, rm, i>>4);
  if(match(i, "1111 110punwl/// ..../")) return armMemCop(15, i>>8, op, rd, rn, i);
  if(match(i, "1111 1110..../// ..../")) return armCop(15, i>>8, op, rd, rn, rm, i>>4);
  if(match(i, "1111 ......../// ..../")) return armUndefined(i);
  
  if(match(i, ".... 00110r10/// ..../")) return armWritePsr(cc, i>>22,         rn, armImmed(i, rs));
  if(match(i, ".... 00010r10/// 0000/")) return armWritePsr(cc, i>>22,         rn, armRm(rm));
  if(match(i, ".... 00010r00/// 0000/")) return armReadPsr (cc, i>>22,     rd);
  if(match(i, ".... 00010010/// 00l1/")) return armBranchEx(cc,       sh,         rm);
  if(match(i, ".... 00010010/// 0111/")) return armBkpt    (cc, i>>8, i);
  if(match(i, ".... 00010110/// 0001/")) return armClz     (cc,            rd,     rm);
  if(match(i, ".... 00010ds0/// 0101/")) return armDspAdd  (cc, i>>21,     rd, rn, rm);
  if(match(i, ".... 00010oo0/// 1yx0/")) return armDspMul  (cc, i>>21, sh, rn, rd, rm, rs);
  
  if(match(i, ".... 0000luas/// 1001/")) return armMultiply(cc, op,     rn, rd, rm, rs);
  if(match(i, ".... 00010b00/// 1001/")) return armMemSwap (cc, i>>22,  rd, rn, rm);
  if(match(i, ".... 0000ui1l/// 1sh1/")) return armUndefined(i);
  if(match(i, ".... 000pu0w0/// 11s1/")) return armMem_v5  (cc, op, sh, rd, rn, armRm(i));
  if(match(i, ".... 000pu0wl/// 1sh1/")) return armMem_v4  (cc, op, sh, rd, rn, armRm(i));
  if(match(i, ".... 000pu1w0/// 11s1/")) return armMem_v5  (cc, op, sh, rd, rn, armOffset8(i, i>>8));
  if(match(i, ".... 000pu1wl/// 1sh1/")) return armMem_v4  (cc, op, sh, rd, rn, armOffset8(i, i>>8));
  
  if(match(i, ".... 000aaaas/// 0sh1/")) return armData    (cc, op, rd, rn, armShiftReg(rm, sh, rs));
  if(match(i, ".... 000aaaas/// .sh0/")) return armData    (cc, op, rd, rn, armShiftImm(rm, sh, i>>7));
  if(match(i, ".... 001aaaas/// ..../")) return armData    (cc, op, rd, rn, armImmed(i, rs));
  if(match(i, ".... 010pubwl/// ..../")) return armMem     (cc, op, rd, rn, armOffset12(i));
  if(match(i, ".... 011pubwl/// ...0/")) return armMem     (cc, op, rd, rn, armShiftImm(rm, sh, i>>7));
  
  if(match(i, ".... 100puswl/// ..../")) return armBlock(cc, op, rn, i);
  if(match(i, ".... 101l..../// ..../")) return armBranch(cc, i>>24, 0, pc + 4*int24(i));
  if(match(i, ".... 1100000l/// ..../")) return armUndefined(i);
  if(match(i, ".... 1100010l/// ..../")) return armCop_v5(cc, i>>8, op, rd, rn, rm, i>>4);
  if(match(i, ".... 110punwl/// ..../")) return armMemCop(cc, i>>8, op, rd, rn, i);
  if(match(i, ".... 1110..../// ..../")) return armCop(cc, i>>8, op, rd, rn, rm, i>>4);
  if(match(i, ".... 1111..../// ..../")) return armSwi(cc, i);
  
  return armUndefined(i);
}

string thumb(ARMCore* arm, uint16 i) {
  uint32 pc = arm->r[15] & ~1;

  if(match(i, "00011ismmmnnnddd")) return thumbAddSub  (i>>9,  i, i>>3, i>>6);
  if(match(i, "000ssiiiiimmmddd")) return thumbShiftImm(i>>11, i, i>>3, i>>6);
  if(match(i, "001oodddiiiiiiii")) return thumbDataImm (i>>11, i>>8, i);
  if(match(i, "010000oooommmddd")) return thumbDataLo  (i>>6,  i, i>>3);
  if(match(i, "010001oodmmmmddd")) return thumbDataHi  (i>>8, (i&7)+(i>>4&8), i>>3);
  
  if(match(i, "0101ooommmnnnddd")) return thumbMemReg  (i>>9,  i, i>>3, i>>6);
  if(match(i, "011bliiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  if(match(i, "1000liiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  
  if(match(i, "10111110........")) return thumbBkpt(i);
  if(match(i, "10110000siiiiiii")) return thumbAddSP   (       i>>7, i);
  if(match(i, "01001dddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(match(i, "1001odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(match(i, "1010odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  
  if(match(i, "11011110rrrrrrrr")) return thumbUndefined(i);
  if(match(i, "11011111rrrrrrrr")) return thumbSwi(i);
  if(match(i, "1101ccccrrrrrrrr")) return thumbCond    (       i>>8, pc + 2*int8(i));
  if(match(i, "11100rrrrrrrrrrr")) return thumbBranch  (pc + 2*int11(i));
  if(match(i, "11110rrrrrrrrrrr")) return thumbBh      (i);
  if(match(i, "11101rrrrrrrrrr1")) return thumbUndefined(i);
  if(match(i, "111t1rrrrrrrrrrr")) return thumbBl      (i>>12, arm->r[14] + 2*uint11(i));
  if(match(i, "1o..lnnnrrrrrrrr")) return thumbBlock   (i>>11, i>>8, i);
  
  return thumbUndefined(i);
}

#undef collect
#undef match
}
