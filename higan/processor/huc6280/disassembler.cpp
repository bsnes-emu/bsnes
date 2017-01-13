auto HuC6280::disassemble(uint16 pc) -> string {
  string s{hex(r.pc, 4L), "  "};
  uint8 opcode = disassembleRead(pc++);

  #define op(id, name, ...) case id: o = {name, string_vector{__VA_ARGS__}.merge(",")};
  string o;
  switch(opcode) {
  op(0xea, "nop")
  }
  if(!o) o = {"??? (", hex(opcode, 2L), ")"};
  s.append(pad(o, -16L, ' '));
  #undef op

  s.append(" A:", hex(r.a, 2L));
  s.append(" X:", hex(r.x, 2L));
  s.append(" Y:", hex(r.y, 2L));
  s.append(" S:", hex(r.s, 2L));
  s.append(" ");
  s.append(r.p.n ? "N" : "n");
  s.append(r.p.v ? "V" : "v");
  s.append(r.p.t ? "T" : "t");
  s.append(r.p.b ? "B" : "b");
  s.append(r.p.d ? "D" : "d");
  s.append(r.p.i ? "I" : "i");
  s.append(r.p.z ? "Z" : "z");
  s.append(r.p.c ? "C" : "c");

  return s;
}
