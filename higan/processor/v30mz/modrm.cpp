//ModRM functions
//d7-d6 => mod
//d5-d3 => reg
//d2-d0 => mem

auto V30MZ::modRM() -> void {
  auto byte = fetch();
  modrm.mod = byte >> 6;
  modrm.reg = byte >> 3;
  modrm.mem = byte >> 0;

  if(modrm.mod == 0 && modrm.mem == 6) {
    modrm.segment = segment(r.ds);
    modrm.address = fetch(Word);
  } else {
    switch(modrm.reg) {
    case 0: modrm.segment = segment(r.ds); modrm.address = r.bx + r.si; break;
    case 1: modrm.segment = segment(r.ds); modrm.address = r.bx + r.di; break;
    case 2: modrm.segment = segment(r.ss); modrm.address = r.bp + r.si; break;
    case 3: modrm.segment = segment(r.ss); modrm.address = r.bp + r.di; break;
    case 4: modrm.segment = segment(r.ds); modrm.address = r.si; break;
    case 5: modrm.segment = segment(r.ds); modrm.address = r.di; break;
    case 6: modrm.segment = segment(r.ds); modrm.address = r.bp; break;
    case 7: modrm.segment = segment(r.ds); modrm.address = r.bx; break;
    }
    if(modrm.mod == 1) modrm.address += (int8)fetch(Byte);
    if(modrm.mod == 2) modrm.address += (int16)fetch(Word);
  }
}

//

auto V30MZ::getMem(Size size, uint offset) -> uint16 {
  if(modrm.mod != 3) return read(size, modrm.segment, modrm.address + offset);
  if(size == Byte) return *r.b[modrm.mem];
  if(size == Word) return *r.w[modrm.mem];
  unreachable;
}

auto V30MZ::setMem(Size size, uint16 data) -> void {
  if(modrm.mod != 3) return write(size, modrm.segment, modrm.address, data);
  if(size == Byte) *r.b[modrm.mem] = data;
  if(size == Word) *r.w[modrm.mem] = data;
}

//

auto V30MZ::getReg(Size size) -> uint16 {
  if(size == Byte) return *r.b[modrm.reg];
  if(size == Word) return *r.w[modrm.reg];
  unreachable;
}

auto V30MZ::setReg(Size size, uint16 data) -> void {
  if(size == Byte) *r.b[modrm.reg] = data;
  if(size == Word) *r.w[modrm.reg] = data;
}

//

auto V30MZ::getSeg() -> uint16 {
  return *r.s[modrm.reg];
}

auto V30MZ::setSeg(uint16 data) -> void {
  *r.s[modrm.reg] = data;
}
