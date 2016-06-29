auto CPU::readAPU(uint24 addr, uint8 data) -> uint8 {
  synchronizeSMP();
  return smp.readPort(addr.bits(0,1));
}

auto CPU::readCPU(uint24 addr, uint8 data) -> uint8 {
  switch((uint16)addr) {

  //WMDATA
  case 0x2180: {
    return bus.read(0x7e0000 | status.wramAddress++, r.mdr);
  }

  //JOYSER0
  //7-2 = MDR
  //1-0 = Joypad serial data
  case 0x4016: {
    uint8 v = r.mdr & 0xfc;
    v |= SuperFamicom::peripherals.controllerPort1->data();
    return v;
  }

  //JOYSER1
  case 0x4017: {
    //7-5 = MDR
    //4-2 = Always 1 (pins are connected to GND)
    //1-0 = Joypad serial data
    uint8 v = (r.mdr & 0xe0) | 0x1c;
    v |= SuperFamicom::peripherals.controllerPort2->data();
    return v;
  }

  //RDNMI
  case 0x4210: {
    //7   = NMI acknowledge
    //6-4 = MDR
    //3-0 = CPU (5a22) version
    uint8 v = (r.mdr & 0x70);
    v |= (uint8)(rdnmi()) << 7;
    v |= (version & 0x0f);
    return v;
  }

  //TIMEUP
  case 0x4211: {
    //7   = IRQ acknowledge
    //6-0 = MDR
    uint8 v = (r.mdr & 0x7f);
    v |= (uint8)(timeup()) << 7;
    return v;
  }

  //HVBJOY
  case 0x4212: {
    //7   = VBLANK acknowledge
    //6   = HBLANK acknowledge
    //5-1 = MDR
    //0   = JOYPAD acknowledge
    uint8 v = (r.mdr & 0x3e);
    if(status.autoJoypadActive) v |= 0x01;
    if(hcounter() <= 2 || hcounter() >= 1096) v |= 0x40;  //hblank
    if(vcounter() >= ppu.vdisp()) v |= 0x80;  //vblank
    return v;
  }

  //RDIO
  case 0x4213: {
    return status.pio;
  }

  //RDDIVL
  case 0x4214: {
    return status.rddiv.byte(0);
  }

  //RDDIVH
  case 0x4215: {
    return status.rddiv.byte(1);
  }

  //RDMPYL
  case 0x4216: {
    return status.rdmpy.byte(0);
  }

  //RDMPYH
  case 0x4217: {
    return status.rdmpy.byte(1);
  }

  case 0x4218: return status.joy1.byte(0);  //JOY1L
  case 0x4219: return status.joy1.byte(1);  //JOY1H
  case 0x421a: return status.joy2.byte(0);  //JOY2L
  case 0x421b: return status.joy2.byte(1);  //JOY2H
  case 0x421c: return status.joy3.byte(0);  //JOY3L
  case 0x421d: return status.joy3.byte(1);  //JOY3H
  case 0x421e: return status.joy4.byte(0);  //JOY4L
  case 0x421f: return status.joy4.byte(1);  //JOY4H

  }

  return data;
}

auto CPU::readDMA(uint24 addr, uint8 data) -> uint8 {
  auto& channel = this->channel[addr.bits(4,6)];

  switch(addr & 0xff0f) {

  //DMAPx
  case 0x4300: return (
    channel.transferMode    << 0
  | channel.fixedTransfer   << 3
  | channel.reverseTransfer << 4
  | channel.unused          << 5
  | channel.indirect        << 6
  | channel.direction       << 7
  );

  //BBADx
  case 0x4301: return channel.targetAddress;

  //A1TxL
  case 0x4302: return channel.sourceAddress >> 0;

  //A1TxH
  case 0x4303: return channel.sourceAddress >> 8;

  //A1Bx
  case 0x4304: return channel.sourceBank;

  //DASxL -- union { uint16 transferSize; uint16 indirectAddress; };
  case 0x4305: return channel.transferSize.byte(0);

  //DASxH -- union { uint16 transferSize; uint16 indirectAddress; };
  case 0x4306: return channel.transferSize.byte(1);

  //DASBx
  case 0x4307: return channel.indirectBank;

  //A2AxL
  case 0x4308: return channel.hdmaAddress.byte(0);

  //A2AxH
  case 0x4309: return channel.hdmaAddress.byte(1);

  //NTRLx
  case 0x430a: return channel.lineCounter;

  //???
  case 0x430b:
  case 0x430f: return channel.unknown;

  }

  return data;
}

auto CPU::writeAPU(uint24 addr, uint8 data) -> void {
  synchronizeSMP();
  return portWrite(addr.bits(0,1), data);
}

auto CPU::writeCPU(uint24 addr, uint8 data) -> void {
  switch((uint16)addr) {

  //WMDATA
  case 0x2180: {
    return bus.write(0x7e0000 | status.wramAddress++, data);
  }

  case 0x2181: status.wramAddress.bits( 0, 7) = data;        return;  //WMADDL
  case 0x2182: status.wramAddress.bits( 8,15) = data;        return;  //WMADDM
  case 0x2183: status.wramAddress.bit (16   ) = data.bit(0); return;  //WMADDH

  //JOYSER0
  case 0x4016: {
    //bit 0 is shared between JOYSER0 and JOYSER1, therefore
    //strobing $4016.d0 affects both controller port latches.
    //$4017 bit 0 writes are ignored.
    SuperFamicom::peripherals.controllerPort1->latch(data.bit(0));
    SuperFamicom::peripherals.controllerPort2->latch(data.bit(0));
    return;
  }

  //NMITIMEN
  case 0x4200: {
    status.autoJoypadPoll = data.bit(0);
    nmitimenUpdate(data);
    return;
  }

  //WRIO
  case 0x4201: {
    if(status.pio.bit(7) && !data.bit(7)) ppu.latchCounters();
    status.pio = data;
    return;
  }

  //WRMPYA
  case 0x4202: status.wrmpya = data; return;

  //WRMPYB
  case 0x4203: {
    status.rdmpy = 0;
    if(alu.mpyctr || alu.divctr) return;

    status.wrmpyb = data;
    status.rddiv = (status.wrmpyb << 8) | status.wrmpya;

    alu.mpyctr = 8;  //perform multiplication over the next eight cycles
    alu.shift = status.wrmpyb;
    return;
  }

  case 0x4204: { status.wrdiva.byte(0) = data; return; }  //WRDIVL
  case 0x4205: { status.wrdiva.byte(1) = data; return; }  //WRDIVH

  //WRDIVB
  case 0x4206: {
    status.rdmpy = status.wrdiva;
    if(alu.mpyctr || alu.divctr) return;

    status.wrdivb = data;

    alu.divctr = 16;  //perform division over the next sixteen cycles
    alu.shift = status.wrdivb << 16;
    return;
  }

  case 0x4207: status.hirqPos.bits(0,7) = data;        return;  //HTIMEL
  case 0x4208: status.hirqPos.bit (8  ) = data.bit(0); return;  //HTIMEH

  case 0x4209: status.virqPos.bits(0,7) = data;        return;  //VTIMEL
  case 0x420a: status.virqPos.bit (8  ) = data.bit(0); return;  //VTIMEH

  //DMAEN
  case 0x420b: {
    for(auto n : range(8)) channel[n].dmaEnabled = data.bit(n);
    if(data) status.dmaPending = true;
    return;
  }

  //HDMAEN
  case 0x420c: {
    for(auto n : range(8)) channel[n].hdmaEnabled = data.bit(n);
    return;
  }

  //MEMSEL
  case 0x420d: {
    status.romSpeed = data.bit(0) ? 6 : 8;
    return;
  }

  }
}

auto CPU::writeDMA(uint24 addr, uint8 data) -> void {
  auto& channel = this->channel[addr.bits(4,6)];

  switch(addr & 0xff0f) {

  //DMAPx
  case 0x4300: {
    channel.transferMode    = data.bits(0,2);
    channel.fixedTransfer   = data.bit (3);
    channel.reverseTransfer = data.bit (4);
    channel.unused          = data.bit (5);
    channel.indirect        = data.bit (6);
    channel.direction       = data.bit (7);
    return;
  }

  //DDBADx
  case 0x4301: channel.targetAddress = data; return;

  //A1TxL
  case 0x4302: channel.sourceAddress.byte(0) = data; return;

  //A1TxH
  case 0x4303: channel.sourceAddress.byte(1) = data; return;

  //A1Bx
  case 0x4304: channel.sourceBank = data; return;

  //DASxL -- union { uint16 transferSize; uint16 indirectAddress; };
  case 0x4305: channel.transferSize.byte(0) = data; return;

  //DASxH -- union { uint16 transferSize; uint16 indirectAddress; };
  case 0x4306: channel.transferSize.byte(1) = data; return;

  //DASBx
  case 0x4307: channel.indirectBank = data; return;

  //A2AxL
  case 0x4308: channel.hdmaAddress.byte(0) = data; return;

  //A2AxH
  case 0x4309: channel.hdmaAddress.byte(1) = data; return;

  //NTRLx
  case 0x430a: channel.lineCounter = data; return;

  //???
  case 0x430b:
  case 0x430f: channel.unknown = data; return;

  }
}
