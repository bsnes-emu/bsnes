#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);

auto Z80::instruction() -> void {
  instructionsExecuted++;

  r.r = (r.r & 0x80) | (r.r + 1 & 0x7f);

  auto opcode = read(r.pc++);
  switch(opcode) {
  op(0x00, NOP)
  op(0xf3, DI)
  }

  print("[Z80] unimplemented instruction: ", hex(opcode, 2L), "\n");
  print("[Z80] instructions executed: ", --instructionsExecuted, "\n");
  while(true) wait();
}

#undef op
