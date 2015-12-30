auto CPU::pio() -> uint8 {
  return status.pio;
}

auto CPU::joylatch() -> bool {
  return status.joypad_strobe_latch;
}

auto CPU::interrupt_pending() -> bool {
  return false;
}

auto CPU::port_read(uint8 port) -> uint8 {
  return port_data[port & 3];
}

auto CPU::port_write(uint8 port, uint8 data) -> void {
  port_data[port & 3] = data;
}

auto CPU::op_io() -> void {
  add_clocks(6);
}

auto CPU::op_read(uint addr) -> uint8 {
  regs.mdr = bus.read(addr, regs.mdr);
  add_clocks(speed(addr));
  return regs.mdr;
}

auto CPU::op_write(uint addr, uint8 data) -> void {
  add_clocks(speed(addr));
  bus.write(addr, regs.mdr = data);
}

auto CPU::speed(uint addr) const -> uint {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.rom_speed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}
