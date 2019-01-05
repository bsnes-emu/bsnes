auto TLCS900H::instruction() -> void {
  auto data = fetch();
  static const Register registers[] = {W, A, B, C, D, E, H, L};
  Register register;
  Memory memory;

  switch(data) {
  case 0x00: return instructionNoOperation();
  case 0x04: return;  //undefined
  case 0x12: return instructionComplementCarry();
  case 0x1f: return;  //undefined

  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Byte>(memory);

  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Byte>(memory);

  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Word>(memory);

  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Word>(memory);

  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Long>(memory);

  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Long>(memory);

  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    memory = {read<Long>(register)};
    return instructionTargetMemory(memory);

  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionTargetMemory(memory);

  case 0xc0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc3:
    data = fetch();
    memory = {read<Long>(Register{data & ~3})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch() & ~1});
    return instructionSourceMemory<Byte>(memory);
  case 0xc4:
    data = fetch();
    register = {data & ~3};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    memory = {read<Long>(register)};
    return instructionSourceMemory<Byte>(memory);
  case 0xc5:
    data = fetch();
    register = {data & ~3};
    memory = {read<Long>(register)};
    instructionSourceMemory<Byte>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    return;
  case 0xc6:
    //undefined
    return;
  case 0xc7:
    register = {fetch()};
    return instructionRegister<Byte>(register);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    register = registers[data & 7];
    return instructionRegister<Byte>(register);

  case 0xd0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd3:
    data = fetch();
    memory = {read<Long>(Register{data & ~3})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch() & ~1});
    return instructionSourceMemory<Word>(memory);
  case 0xd4:
    data = fetch();
    register = {data & ~3};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    memory = {read<Long>(register)};
    return instructionSourceMemory<Word>(memory);
  case 0xd5:
    data = fetch();
    register = {data & ~3};
    memory = {read<Long>(register)};
    instructionSourceMemory<Word>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    return;
  case 0xd6:
    //undefined
    return;
  case 0xd7:
    register = {fetch() & ~1};
    return instructionRegister<Word>(register);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    register = registers[data & 7];
    return instructionRegister<Word>(register);

  case 0xe0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe3:
    data = fetch();
    memory = {read<Long>(Register{data & ~3})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch() & ~1});
    return instructionSourceMemory<Long>(memory);
  case 0xe4:
    data = fetch();
    register = {data & ~3};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    memory = {read<Long>(register)};
    return instructionSourceMemory<Long>(memory);
  case 0xe5:
    data = fetch();
    register = {data & ~3};
    memory = {read<Long>(register)};
    instructionSourceMemory<Long>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    return;
  case 0xe6:
    //undefined
    return;
  case 0xe7:
    register = {fetch() & ~3};
    return instructionRegister<Long>(register);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    register = registers[data & 7];
    return instructionRegister<Long>(register);

  case 0xf0:
    memory = {fetch< uint8>()};
    return instructionTargetMemory(memory);
  case 0xf1:
    memory = {fetch<uint16>()};
    return instructionTargetMemory(memory);
  case 0xf2:
    memory = {fetch<uint24>()};
    return instructionTargetMemory(memory);
  case 0xf3:
    data = fetch();
    memory = {read<Long>(Register{data & ~3})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch() & ~1});
    return instructionTargetMemory(memory);
  case 0xf4:
    data = fetch();
    register = {data & ~3};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    memory = {read<Long>(register)};
    return instructionTargetMemory(memory);
  case 0xf5:
    data = fetch();
    register = {data & ~3};
    memory = {read<Long>(register)};
    instructionTargetMemory(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    return;
  case 0xf6: case 0xf7:
    //undefined
    return;

  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionSoftwareInterrupt(Immediate{data.bits(0,2)});
  }
}

template<typename Size>
auto TLCS900H::instructionRegister(Register input) -> void {
  auto data = fetch();
  static const Register registers[] = {W, A, B, C, D, E, H, L};
  Register register = registers[data.bits(0,2)];

  switch(data) {
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd<Size>(register, input);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry<Size>(register, input);
  }
}

template<typename Size>
auto TLCS900H::instructionSourceMemory(Memory input) -> void {
  auto data = fetch();
  static const Register registers[] = {W, A, B, C, D, E, H, L};
  Register register = registers[data.bits(0,2)];

  switch(data) {
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd<Size>(register, input);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry<Size>(register, input);
  }
}

auto TLCS900H::instructionTargetMemory(Memory input) -> void {
  auto data = fetch();
  static const Register registers[] = {W, A, B, C, D, E, H, L};
  Register register = registers[data.bits(0,2)];

  switch(data) {
  }
}
