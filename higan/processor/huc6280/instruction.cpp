#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);

auto HuC6280::instruction() -> void {
  switch(opcode()) {
  op(0xea, NOP)
  }
}

#undef op
