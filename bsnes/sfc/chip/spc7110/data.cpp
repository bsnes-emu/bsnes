#ifdef SPC7110_CPP

uint8 SPC7110::datarom_read(unsigned addr) {
  unsigned mask = (1 << (r4834 & 3)) - 1;  //8mbit, 16mbit, 32mbit, 64mbit
  unsigned range = 0x100000 * (1 + mask);
  unsigned offset = addr & 0x7fffff;

  //mirroring behavior is non-sensical. assuming a 32mbit data ROM:
  //banks 4-7 with mask 0-2 returns 0x00; banks 4-7 with mask 3 mirrors banks 0-3
  if(range <= drom_size && offset >= drom_size) return 0x00;

  return cartridge.rom.read(drom_base + Bus::mirror(offset, drom_size));
}

unsigned SPC7110::data_offset()    { return r4811 | r4812 << 8 | r4813 << 16; }
unsigned SPC7110::data_adjust()    { return r4814 | r4815 << 8; }
unsigned SPC7110::data_increment() { return r4816 | r4817 << 8; }
void SPC7110::set_data_offset(unsigned addr) { r4811 = addr; r4812 = addr >> 8; r4813 = addr >> 16; }
void SPC7110::set_data_adjust(unsigned addr) { r4814 = addr; r4815 = addr >> 8; }

void SPC7110::data_port_read_a() {
  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  if(r4818 & 2) offset += adjust;
  r4810 = datarom_read(offset);
}

void SPC7110::data_port_read_b() {
  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  r481a = datarom_read(offset + adjust);
}

void SPC7110::data_port_read() {
  data_port_read_a();
  data_port_read_b();
}

void SPC7110::data_port_increment_a() {
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  if(r4818 & 2) return set_data_adjust(adjust + 1);

  unsigned increment = r4818 & 1 ? data_increment() : 1u;
  if(r4818 & 4) increment = (int16)increment;

  if((r4818 & 16) == 0) set_data_offset(data_offset() + increment);
  if((r4818 & 16) != 0) set_data_adjust(adjust + increment);
}

void SPC7110::data_port_increment_b() {
  if(r4818 >> 5 != 3) return;

  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;

  if((r4818 & 16) == 0) set_data_offset(offset + adjust);
  if((r4818 & 16) != 0) set_data_adjust(adjust + adjust);
}

void SPC7110::data_port_increment() {
  if((r4818 & 2) == 0) return;
  if(r4818 & 16) return;

  if(r4818 >> 5 == 1) {
    unsigned increment = data_adjust() & 0xff;
    if(r4818 & 8) increment = (int8)increment;
    set_data_offset(data_offset() + increment);
  }

  if(r4818 >> 5 == 2) {
    unsigned increment = data_adjust();
    if(r4818 & 8) increment = (int16)increment;
    set_data_offset(data_offset() + increment);
  }
}

#endif
