auto V30MZ::disassemble(uint16 cs, uint16 ip, bool registers) -> string {
  string s;
  uint20 ea = (cs << 4) + ip;

  auto readByte = [&](uint offset) {
    uint8 byte = read((cs << 4) + (uint16)(ip + offset));
    return hex(byte, 2L);
  };
  auto readWord = [&](uint offset) {
    uint16 word = read((cs << 4) + (uint16)(ip + offset++)) << 0;
           word | read((cs << 4) + (uint16)(ip + offset++)) << 8;
    return hex(word, 4L);
  };

  uint8 opcode = read(ea);
  switch(opcode) {
  case 0x90: s = {"nop"}; break;
  case 0xb0: s = {"mov al,", readByte(1)}; break;
  case 0xb1: s = {"mov cl,", readByte(1)}; break;
  case 0xb2: s = {"mov dl,", readByte(1)}; break;
  case 0xb3: s = {"mov bl,", readByte(1)}; break;
  case 0xb4: s = {"mov ah,", readByte(1)}; break;
  case 0xb5: s = {"mov ch,", readByte(1)}; break;
  case 0xb6: s = {"mov dh,", readByte(1)}; break;
  case 0xb7: s = {"mov bh,", readByte(1)}; break;
  case 0xb8: s = {"mov ax,", readWord(1)}; break;
  case 0xb9: s = {"mov cx,", readWord(1)}; break;
  case 0xba: s = {"mov dx,", readWord(1)}; break;
  case 0xbb: s = {"mov bx,", readWord(1)}; break;
  case 0xbc: s = {"mov sp,", readWord(1)}; break;
  case 0xbd: s = {"mov bp,", readWord(1)}; break;
  case 0xbe: s = {"mov si,", readWord(1)}; break;
  case 0xbf: s = {"mov di,", readWord(1)}; break;
  case 0xe4: s = {"in  al,", readByte(1)}; break;
  case 0xe5: s = {"in  ax,", readByte(1)}; break;
  case 0xe6: s = {"out ", readByte(1), ",al"}; break;
  case 0xe7: s = {"out ", readByte(1), ",ax"}; break;
  case 0xea: s = {"jmp ", readWord(3), ":", readWord(1)}; break;
  case 0xec: s = {"in  al,dx"}; break;
  case 0xed: s = {"in  ax,dx"}; break;
  case 0xee: s = {"out dx,al"}; break;
  case 0xef: s = {"out dx,ax"}; break;
  case 0xf8: s = {"clc"}; break;
  case 0xf9: s = {"stc"}; break;
  case 0xfa: s = {"cli"}; break;
  case 0xfb: s = {"sti"}; break;
  case 0xfc: s = {"cld"}; break;
  case 0xfd: s = {"std"}; break;

  default:
    s = {"??? [", hex(opcode, 2L), "]"};
  }
  while(s.size() < 20) s.append(" ");

  if(!registers) return {hex(ea, 5L), "  ", s};

  return {
    hex(ea, 5L), "  ", s,
    " ax:", hex(r.ax, 4L),
    " bx:", hex(r.bx, 4L),
    " cx:", hex(r.cx, 4L),
    " dx:", hex(r.dx, 4L),
    " si:", hex(r.si, 4L),
    " di:", hex(r.di, 4L),
    " bp:", hex(r.bp, 4L),
    " sp:", hex(r.sp, 4L),
    " cs:", hex(r.cs, 4L),
    " ip:", hex(ip, 4L),
    " ds:", hex(r.ds, 4L),
    " es:", hex(r.es, 4L),
    " ss:", hex(r.ss, 4L), " ",
    r.f.m ? "M" : "m",
    r.f.v ? "V" : "v",
    r.f.d ? "D" : "d",
    r.f.i ? "I" : "i",
    r.f.b ? "B" : "b",
    r.f.s ? "S" : "s",
    r.f.z ? "Z" : "z",
    r.f.h ? "H" : "h",
    r.f.p ? "P" : "p",
    r.f.c ? "C" : "c"
  };
}
