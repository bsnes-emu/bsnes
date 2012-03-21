void ARM::thumb_step() {
  if(pipeline.reload) {
    pipeline.reload = false;

    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus.read(r(15), Half);

    r(15).data += 2;
    pipeline.decode = pipeline.fetch;
    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus.read(r(15), Half);
    step(1);
  }

  r(15).data += 2;
  pipeline.execute = pipeline.decode;
  pipeline.decode = pipeline.fetch;
  pipeline.fetch.address = r(15);
  pipeline.fetch.instruction = bus.read(r(15), Half);
  step(1);

  print(disassemble_thumb_opcode(pipeline.execute.address), "\n");

  if((instruction() & 0xe000) == 0x2000) { thumb_op_immediate(); return; }

  exception = true;
}

void ARM::thumb_mov(GPR &source, uint32 modify) {
  cpsr().n = modify >> 31;
  cpsr().z = modify == 0;
  source = modify;
}

void ARM::thumb_cmp(GPR &source, uint32 modify) {
  uint32 result = source - modify;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
}

void ARM::thumb_add(GPR &source, uint32 modify) {
  uint32 result = source + modify;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
  source = result;
}

void ARM::thumb_sub(GPR &source, uint32 modify) {
  uint32 result = source - modify;
  uint32 overflow = ~(source ^ modify) & (source ^ result);
  cpsr().n = result >> 31;
  cpsr().z = result == 0;
  cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
  cpsr().v = (1u << 31) & (overflow);
  source = result;
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
  case 0: thumb_mov(r(d), immediate); break;
  case 1: thumb_cmp(r(d), immediate); break;
  case 2: thumb_add(r(d), immediate); break;
  case 3: thumb_sub(r(d), immediate); break;
  }
}
