auto SPC7110::datarom_read(uint addr) -> uint8 {
  uint size = 1 << (r4834 & 3);  //size in MB
  uint mask = 0x100000 * size - 1;
  uint offset = addr & mask;
  if((r4834 & 3) != 3 && (addr & 0x400000)) return 0x00;
  return drom.read(Bus::mirror(offset, drom.size()));
}

auto SPC7110::data_offset() -> uint { return r4811 | r4812 << 8 | r4813 << 16; }
auto SPC7110::data_adjust() -> uint { return r4814 | r4815 << 8; }
auto SPC7110::data_stride() -> uint { return r4816 | r4817 << 8; }
auto SPC7110::set_data_offset(uint addr) -> void { r4811 = addr; r4812 = addr >> 8; r4813 = addr >> 16; }
auto SPC7110::set_data_adjust(uint addr) -> void { r4814 = addr; r4815 = addr >> 8; }

auto SPC7110::data_port_read() -> void {
  uint offset = data_offset();
  uint adjust = r4818 & 2 ? data_adjust() : 0;
  if(r4818 & 8) adjust = (int16)adjust;
  r4810 = datarom_read(offset + adjust);
}

auto SPC7110::data_port_increment_4810() -> void {
  uint offset = data_offset();
  uint stride = r4818 & 1 ? data_stride() : 1;
  uint adjust = data_adjust();
  if(r4818 & 4) stride = (int16)stride;
  if(r4818 & 8) adjust = (int16)adjust;
  if((r4818 & 16) == 0) set_data_offset(offset + stride);
  if((r4818 & 16) != 0) set_data_adjust(adjust + stride);
  data_port_read();
}

auto SPC7110::data_port_increment_4814() -> void {
  if(r4818 >> 5 != 1) return;
  uint offset = data_offset();
  uint adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
  data_port_read();
}

auto SPC7110::data_port_increment_4815() -> void {
  if(r4818 >> 5 != 2) return;
  uint offset = data_offset();
  uint adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
  data_port_read();
}

auto SPC7110::data_port_increment_481a() -> void {
  if(r4818 >> 5 != 3) return;
  uint offset = data_offset();
  uint adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
  data_port_read();
}
