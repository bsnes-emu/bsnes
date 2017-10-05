template<uint Size> auto M68K::fetch(EffectiveAddress& ea) -> uint32 {
  if(!ea.valid.raise()) return ea.address;

  switch(ea.mode) {

  case DataRegisterDirect: {
    return read(DataRegister{ea.reg});
  }

  case AddressRegisterDirect: {
    return read(AddressRegister{ea.reg});
  }

  case AddressRegisterIndirect: {
    return read(AddressRegister{ea.reg});
  }

  case AddressRegisterIndirectWithPostIncrement: {
    return read(AddressRegister{ea.reg});
  }

  case AddressRegisterIndirectWithPreDecrement: {
    return read(AddressRegister{ea.reg});
  }

  case AddressRegisterIndirectWithDisplacement: {
    return read(AddressRegister{ea.reg}) + (int16)readPC();
  }

  case AddressRegisterIndirectWithIndex: {
    auto extension = readPC();
    auto index = extension & 0x8000
    ? read(AddressRegister{extension >> 12})
    : read(DataRegister{extension >> 12});
    if(!(extension & 0x800)) index = (int16)index;
    return read(AddressRegister{ea.reg}) + index + (int8)extension;
  }

  case AbsoluteShortIndirect: {
    return (int16)readPC();
  }

  case AbsoluteLongIndirect: {
    return readPC<Long>();
  }

  case ProgramCounterIndirectWithDisplacement: {
    auto base = r.pc;
    return base + (int16)readPC();
  }

  case ProgramCounterIndirectWithIndex: {
    auto base = r.pc;
    auto extension = readPC();
    auto index = extension & 0x8000
    ? read(AddressRegister{extension >> 12})
    : read(DataRegister{extension >> 12});
    if(!(extension & 0x800)) index = (int16)index;
    return base + index + (int8)extension;
  }

  case Immediate: {
    return readPC<Size>();
  }

  }

  return 0;
}

template<uint Size, bool hold> auto M68K::read(EffectiveAddress& ea) -> uint32 {
  ea.address = fetch<Size>(ea);

  switch(ea.mode) {

  case DataRegisterDirect: {
    return clip<Size>(ea.address);
  }

  case AddressRegisterDirect: {
    return sign<Size>(ea.address);
  }

  case AddressRegisterIndirect: {
    return read<Size>(ea.address);
  }

  case AddressRegisterIndirectWithPostIncrement: {
    auto address = ea.address + (ea.reg == 7 && Size == Byte ? bytes<Word>() : bytes<Size>());
    auto data = read<Size>(ea.address);
    if(!hold) write(AddressRegister{ea.reg}, ea.address = address);
    return data;
  }

  case AddressRegisterIndirectWithPreDecrement: {
    auto address = ea.address - (ea.reg == 7 && Size == Byte ? bytes<Word>() : bytes<Size>());
    auto data = read<Size>(address);
    if(!hold) write(AddressRegister{ea.reg}, ea.address = address);
    return data;
  }

  case AddressRegisterIndirectWithDisplacement: {
    return read<Size>(ea.address);
  }

  case AddressRegisterIndirectWithIndex: {
    return read<Size>(ea.address);
  }

  case AbsoluteShortIndirect: {
    return read<Size>(ea.address);
  }

  case AbsoluteLongIndirect: {
    return read<Size>(ea.address);
  }

  case ProgramCounterIndirectWithDisplacement: {
    return read<Size>(ea.address);
  }

  case ProgramCounterIndirectWithIndex: {
    return read<Size>(ea.address);
  }

  case Immediate: {
    return clip<Size>(ea.address);
  }

  }

  return 0;
}

template<uint Size, bool hold> auto M68K::write(EffectiveAddress& ea, uint32 data) -> void {
  ea.address = fetch<Size>(ea);

  switch(ea.mode) {

  case DataRegisterDirect: {
    return write<Size>(DataRegister{ea.reg}, data);
  }

  case AddressRegisterDirect: {
    return write<Size>(AddressRegister{ea.reg}, data);
  }

  case AddressRegisterIndirect: {
    return write<Size>(ea.address, data);
  }

  case AddressRegisterIndirectWithPostIncrement: {
    auto address = ea.address + (ea.reg == 7 && Size == Byte ? bytes<Word>() : bytes<Size>());
    write<Size>(ea.address, data);
    if(!hold) write(AddressRegister{ea.reg}, ea.address = address);
    return;
  }

  case AddressRegisterIndirectWithPreDecrement: {
    auto address = ea.address - (ea.reg == 7 && Size == Byte ? bytes<Word>() : bytes<Size>());
    write<Size, Reverse>(address, data);
    if(!hold) write(AddressRegister{ea.reg}, ea.address = address);
    return;
  }

  case AddressRegisterIndirectWithDisplacement: {
    return write<Size>(ea.address, data);
  }

  case AddressRegisterIndirectWithIndex: {
    return write<Size>(ea.address, data);
  }

  case AbsoluteShortIndirect: {
    return write<Size>(ea.address, data);
  }

  case AbsoluteLongIndirect: {
    return write<Size>(ea.address, data);
  }

  case ProgramCounterIndirectWithDisplacement: {
    return write<Size>(ea.address, data);
  }

  case ProgramCounterIndirectWithIndex: {
    return write<Size>(ea.address, data);
  }

  case Immediate: {
    return;
  }

  }
}
