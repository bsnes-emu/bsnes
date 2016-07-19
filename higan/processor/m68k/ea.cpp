template<uint Size> auto M68K::fetch(EA& ea) -> uint32 {
  ea.valid = true;

  switch(ea.mode) {

  case 0: {  //data register direct
    return read(ea.reg);
  }

  case 1: {  //data register indirect
    return read(ea.reg);
  }

  case 2: {  //address register indirect
    return read(ea.reg);
  }

  case 3: {  //address register indirect with post-increment
    return read(ea.reg);
  }

  case 4: {  //address register indirect with pre-decrement
    return read(ea.reg);
  }

  case 5: {  //address register indirect with displacement
    return read(ea.reg) + (int16)readPC();
  }

  case 6: {  //address register indirect with index
    auto extension = readPC();
    auto index = read(Register{extension >> 12});
    if(extension & 0x800) index = (int16)index;
    return read(ea.reg) + index + (int8)extension;
  }

  case 7: {  //absolute short indirect
    return (int16)readPC();
  }

  case 8: {  //absolute long indirect
    return readPC<Long>();
  }

  case 9: {  //program counter indirect with displacement
    auto base = r.pc;
    return base + (int16)readPC();
  }

  case 10: {  //program counter indirect with index
    auto base = r.pc;
    auto extension = readPC();
    auto index = read(Register{extension >> 12});
    if(extension & 0x800) index = (int16)index;
    return base + index + (int8)extension;
  }

  case 11: {  //immediate
    return readPC<Size>();
  }

  }

  return 0;
}

template<uint Size, bool Update> auto M68K::read(EA& ea) -> uint32 {
  if(!ea.valid) ea.address = fetch<Size>(ea);

  switch(ea.mode) {

  case 0: {  //data register direct
    return clip<Size>(ea.address);
  }

  case 1: {  //address register direct
    return clip<Size>(ea.address);
  }

  case 2: {  //address register indirect
    return read<Size>(ea.address);
  }

  case 3: {  //address register indirect with post-increment
    auto data = read<Size>(ea.address);
    if(Update) write(ea.reg, ea.address += (Size == Long ? 4 : 2));
    return data;
  }

  case 4: {  //address register indirect with pre-decrement
    auto data = read<Size>((uint32)(ea.address - (Size == Long ? 4 : 2)));
    if(Update) write(ea.reg, ea.address -= (Size == Long ? 4 : 2));
    return data;
  }

  case 5: {  //address register indirect with displacement
    return read<Size>(ea.address);
  }

  case 6: {  //address register indirect with index
    return read<Size>(ea.address);
  }

  case 7: {  //absolute short indirect
    return read<Size>(ea.address);
  }

  case 8: {  //absolute long indirect
    return read<Size>(ea.address);
  }

  case 9: {  //program counter indirect with displacement
    return read<Size>(ea.address);
  }

  case 10: {  //program counter indirect with index
    return read<Size>(ea.address);
  }

  case 11: {  //immediate
    return clip<Size>(ea.address);
  }

  }

  return 0;
}

template<uint Size, bool Update> auto M68K::write(EA& ea, uint32 data) -> void {
  if(!ea.valid) ea.address = fetch<Size>(ea);

  switch(ea.mode) {

  case 0: {  //data register direct
    return write<Size>(ea.reg, data);
  }

  case 1: {  //address register direct
    return write<Size>(ea.reg, data);
  }

  case 2: {  //address register indirect
    return write<Size>(ea.address, data);
  }

  case 3: {  //address register indirect with post-increment
    write<Size>(ea.address, data);
    if(Update) write(ea.reg, ea.address += (Size == Long ? 4 : 2));
    return;
  }

  case 4: {  //address register indirect with pre-decrement
    write<Size>((uint32)(ea.address - (Size == Long ? 4 : 2)), data);
    if(Update) write(ea.reg, ea.address -= (Size == Long ? 4 : 2));
    return;
  }

  case 5: {  //address register indirect with displacement
    return write<Size>(ea.address, data);
  }

  case 6: {  //address register indirect with index
    return write<Size>(ea.address, data);
  }

  case 7: {  //absolute short indirect
    return write<Size>(ea.address, data);
  }

  case 8: {  //absolute long indirect
    return write<Size>(ea.address, data);
  }

  case 9: {  //program counter indirect with displacement
    return write<Size>(ea.address, data);
  }

  case 10: {  //program counter indirect with index
    return write<Size>(ea.address, data);
  }

  case 11: {  //immediate
    return;
  }

  }
}

template<uint Size> auto M68K::flush(EA& ea, uint32 data) -> void {
  switch(ea.mode) {

  case 3: {  //address register indirect with post-increment
    write<Size>(ea.reg, data);
    return;
  }

  case 4: {  //address register indirect with pre-decrement
    write<Size>(ea.reg, data);
    return;
  }

  }
}
