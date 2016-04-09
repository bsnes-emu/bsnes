auto CPU::apuPortRead(uint24 addr, uint8 data) -> uint8 {
  synchronizeSMP();
  return smp.portRead(addr.bits(0,1));
}

auto CPU::cpuPortRead(uint24 addr, uint8 data) -> uint8 {
  addr &= 0xffff;

  //WMDATA
  if(addr == 0x2180) {
    return bus.read(0x7e0000 | status.wram_addr++, regs.mdr);
  }

  //JOYSER0
  //7-2 = MDR
  //1-0 = Joypad serial data
  if(addr == 0x4016) {
    uint8 r = regs.mdr & 0xfc;
    r |= device.controllerPort1->data();
    return r;
  }

  //JOYSER1
  if(addr == 0x4017) {
    //7-5 = MDR
    //4-2 = Always 1 (pins are connected to GND)
    //1-0 = Joypad serial data
    uint8 r = (regs.mdr & 0xe0) | 0x1c;
    r |= device.controllerPort2->data();
    return r;
  }

  //RDNMI
  if(addr == 0x4210) {
    //7   = NMI acknowledge
    //6-4 = MDR
    //3-0 = CPU (5a22) version
    uint8 r = (regs.mdr & 0x70);
    r |= (uint8)(rdnmi()) << 7;
    r |= (cpu_version & 0x0f);
    return r;
  }

  //TIMEUP
  if(addr == 0x4211) {
    //7   = IRQ acknowledge
    //6-0 = MDR
    uint8 r = (regs.mdr & 0x7f);
    r |= (uint8)(timeup()) << 7;
    return r;
  }

  //HVBJOY
  if(addr == 0x4212) {
    //7   = VBLANK acknowledge
    //6   = HBLANK acknowledge
    //5-1 = MDR
    //0   = JOYPAD acknowledge
    uint8 r = (regs.mdr & 0x3e);
    if(status.auto_joypad_active) r |= 0x01;
    if(hcounter() <= 2 || hcounter() >= 1096) r |= 0x40;  //hblank
    if(vcounter() >= ppu.vdisp()) r |= 0x80;  //vblank
    return r;
  }

  //RDIO
  if(addr == 0x4213) {
    return status.pio;
  }

  //RDDIVL
  if(addr == 0x4214) {
    return status.rddiv.byte(0);
  }

  //RDDIVH
  if(addr == 0x4215) {
    return status.rddiv.byte(1);
  }

  //RDMPYL
  if(addr == 0x4216) {
    return status.rdmpy.byte(0);
  }

  //RDMPYH
  if(addr == 0x4217) {
    return status.rdmpy.byte(1);
  }

  if(addr == 0x4218) return status.joy1.byte(0);  //JOY1L
  if(addr == 0x4219) return status.joy1.byte(1);  //JOY1H
  if(addr == 0x421a) return status.joy2.byte(0);  //JOY2L
  if(addr == 0x421b) return status.joy2.byte(1);  //JOY2H
  if(addr == 0x421c) return status.joy3.byte(0);  //JOY3L
  if(addr == 0x421d) return status.joy3.byte(1);  //JOY3H
  if(addr == 0x421e) return status.joy4.byte(0);  //JOY4L
  if(addr == 0x421f) return status.joy4.byte(1);  //JOY4H

  return data;
}

auto CPU::dmaPortRead(uint24 addr, uint8 data) -> uint8 {
  auto& channel = this->channel[addr.bits(4,6)];
  addr &= 0xff0f;

  //DMAPx
  if(addr == 0x4300) return (
    channel.direction << 7
  | channel.indirect << 6
  | channel.unused << 5
  | channel.reverse_transfer << 4
  | channel.fixed_transfer << 3
  | channel.transfer_mode << 0
  );

  //BBADx
  if(addr == 0x4301) return channel.dest_addr;

  //A1TxL
  if(addr == 0x4302) return channel.source_addr >> 0;

  //A1TxH
  if(addr == 0x4303) return channel.source_addr >> 8;

  //A1Bx
  if(addr == 0x4304) return channel.source_bank;

  //DASxL -- union { uint16 transfer_size; uint16 indirect_addr; };
  if(addr == 0x4305) return channel.transfer_size >> 0;

  //DASxH -- union { uint16 transfer_size; uint16 indirect_addr; };
  if(addr == 0x4306) return channel.transfer_size >> 8;

  //DASBx
  if(addr == 0x4307) return channel.indirect_bank;

  //A2AxL
  if(addr == 0x4308) return channel.hdma_addr >> 0;

  //A2AxH
  if(addr == 0x4309) return channel.hdma_addr >> 8;

  //NTRLx
  if(addr == 0x430a) return channel.line_counter;

  //???
  if(addr == 0x430b || addr == 0x430f) return channel.unknown;

  return data;
}

auto CPU::apuPortWrite(uint24 addr, uint8 data) -> void {
  synchronizeSMP();
  return portWrite(addr.bits(0,1), data);
}

auto CPU::cpuPortWrite(uint24 addr, uint8 data) -> void {
  addr &= 0xffff;

  //WMDATA
  if(addr == 0x2180) {
    bus.write(0x7e0000 | status.wram_addr++, data);
  }

  //WMADDL
  if(addr == 0x2181) {
    status.wram_addr.bits(0,7) = data;
  }

  //WMADDM
  if(addr == 0x2182) {
    status.wram_addr.bits(8,15) = data;
  }

  //WMADDH
  if(addr == 0x2183) {
    status.wram_addr.bit(16) = data.bit(0);
  }

  //JOYSER0
  if(addr == 0x4016) {
    //bit 0 is shared between JOYSER0 and JOYSER1, therefore
    //strobing $4016.d0 affects both controller port latches.
    //$4017 bit 0 writes are ignored.
    device.controllerPort1->latch(data.bit(0));
    device.controllerPort2->latch(data.bit(0));
  }

  //NMITIMEN
  if(addr == 0x4200) {
    status.auto_joypad_poll = data.bit(0);
    nmitimenUpdate(data);
  }

  //WRIO
  if(addr == 0x4201) {
    if(status.pio.bit(7) && !data.bit(7)) ppu.latchCounters();
    status.pio = data;
  }

  //WRMPYA
  if(addr == 0x4202) {
    status.wrmpya = data;
  }

  //WRMPYB
  if(addr == 0x4203) {
    status.rdmpy = 0;
    if(alu.mpyctr || alu.divctr) return;

    status.wrmpyb = data;
    status.rddiv = (status.wrmpyb << 8) | status.wrmpya;

    alu.mpyctr = 8;  //perform multiplication over the next eight cycles
    alu.shift = status.wrmpyb;
  }

  //WRDIVL
  if(addr == 0x4204) {
    status.wrdiva.byte(0) = data;
  }

  //WRDIVH
  if(addr == 0x4205) {
    status.wrdiva.byte(1) = data;
  }

  //WRDIVB
  if(addr == 0x4206) {
    status.rdmpy = status.wrdiva;
    if(alu.mpyctr || alu.divctr) return;

    status.wrdivb = data;

    alu.divctr = 16;  //perform division over the next sixteen cycles
    alu.shift = status.wrdivb << 16;
  }

  //HTIMEL
  if(addr == 0x4207) {
    status.hirq_pos.bits(0,7) = data;
  }

  //HTIMEH
  if(addr == 0x4208) {
    status.hirq_pos.bit(8) = data.bit(0);
  }

  //VTIMEL
  if(addr == 0x4209) {
    status.virq_pos.bits(0,7) = data;
  }

  //VTIMEH
  if(addr == 0x420a) {
    status.virq_pos.bit(8) = data;
  }

  //DMAEN
  if(addr == 0x420b) {
    for(auto n : range(8)) channel[n].dma_enabled = data.bit(n);
    if(data) status.dma_pending = true;
  }

  //HDMAEN
  if(addr == 0x420c) {
    for(auto n : range(8)) channel[n].hdma_enabled = data.bit(n);
  }

  //MEMSEL
  if(addr == 0x420d) {
    status.rom_speed = data.bit(0) ? 6 : 8;
  }
}

auto CPU::dmaPortWrite(uint24 addr, uint8 data) -> void {
  auto& channel = this->channel[addr.bits(4,6)];
  addr &= 0xff0f;

  //DMAPx
  if(addr == 0x4300) {
    channel.direction = data & 0x80;
    channel.indirect = data & 0x40;
    channel.unused = data & 0x20;
    channel.reverse_transfer = data & 0x10;
    channel.fixed_transfer = data & 0x08;
    channel.transfer_mode = data & 0x07;
  }

  //DDBADx
  if(addr == 0x4301) {
    channel.dest_addr = data;
  }

  //A1TxL
  if(addr == 0x4302) {
    channel.source_addr = (channel.source_addr & 0xff00) | (data << 0);
  }

  //A1TxH
  if(addr == 0x4303) {
    channel.source_addr = (channel.source_addr & 0x00ff) | (data << 8);
  }

  //A1Bx
  if(addr == 0x4304) {
    channel.source_bank = data;
  }

  //DASxL -- union { uint16 transfer_size; uint16 indirect_addr; };
  if(addr == 0x4305) {
    channel.transfer_size = (channel.transfer_size & 0xff00) | (data << 0);
  }

  //DASxH -- union { uint16 transfer_size; uint16 indirect_addr; };
  if(addr == 0x4306) {
    channel.transfer_size = (channel.transfer_size & 0x00ff) | (data << 8);
  }

  //DASBx
  if(addr == 0x4307) {
    channel.indirect_bank = data;
  }

  //A2AxL
  if(addr == 0x4308) {
    channel.hdma_addr = (channel.hdma_addr & 0xff00) | (data << 0);
  }

  //A2AxH
  if(addr == 0x4309) {
    channel.hdma_addr = (channel.hdma_addr & 0x00ff) | (data << 8);
  }

  //NTRLx
  if(addr == 0x430a) {
    channel.line_counter = data;
  }

  //???
  if(addr == 0x430b || addr == 0x430f) {
    channel.unknown = data;
  }
}
