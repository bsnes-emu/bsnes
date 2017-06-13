//immediate, 2-cycle opcodes with idle cycle will become bus read
//when an IRQ is to be triggered immediately after opcode completion.
//this affects the following opcodes:
//  clc, cld, cli, clv, sec, sed, sei,
//  tax, tay, txa, txy, tya, tyx,
//  tcd, tcs, tdc, tsc, tsx, txs,
//  inc, inx, iny, dec, dex, dey,
//  asl, lsr, rol, ror, nop, xce.
auto WDC65816::idleIRQ() -> void {
  if(interruptPending()) {
    //modify I/O cycle to bus read cycle, do not increment PC
    read(PC);
  } else {
    idle();
  }
}

auto WDC65816::idle2() -> void {
  if(DL != 0x00) {
    idle();
  }
}

auto WDC65816::idle4(uint16 x, uint16 y) -> void {
  if(!XF || (x & 0xff00) != (y & 0xff00)) {
    idle();
  }
}

auto WDC65816::idle6(uint16 addr) -> void {
  if(EF && (PC & 0xff00) != (addr & 0xff00)) {
    idle();
  }
}

auto WDC65816::fetch() -> uint8 {
  return read(PCB << 16 | PCW++);
}

auto WDC65816::pull() -> uint8 {
  EF ? SL++ : S++;
  return read(S);
}

auto WDC65816::push(uint8 data) -> void {
  write(S, data);
  EF ? SL-- : S--;
}

auto WDC65816::pullN() -> uint8 {
  return read(++S);
}

auto WDC65816::pushN(uint8 data) -> void {
  write(S--, data);
}

auto WDC65816::readDirect(uint addr) -> uint8 {
  if(EF && !DL) return read(D | uint8(addr));
  return read(uint16(D + addr));
}

auto WDC65816::writeDirect(uint addr, uint8 data) -> void {
  if(EF && !DL) return write(D | uint8(addr), data);
  write(uint16(D + addr), data);
}

auto WDC65816::readDirectN(uint addr) -> uint8 {
  return read(uint16(D + addr));
}

auto WDC65816::readBank(uint addr) -> uint8 {
  return read((B << 16) + addr);
}

auto WDC65816::writeBank(uint addr, uint8 data) -> void {
  write((B << 16) + addr, data);
}

auto WDC65816::readStack(uint addr) -> uint8 {
  return read(uint16(S + addr));
}

auto WDC65816::writeStack(uint addr, uint8 data) -> void {
  write(uint16(S + addr), data);
}
