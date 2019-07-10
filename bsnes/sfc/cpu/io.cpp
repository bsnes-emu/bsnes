auto CPU::readRAM(uint24 addr, uint8 data) -> uint8 {
  return wram[addr];
}

auto CPU::readAPU(uint24 addr, uint8 data) -> uint8 {
  synchronize(smp);
  return smp.portRead(addr.bits(0,1));
}

auto CPU::readCPU(uint24 addr, uint8 data) -> uint8 {
  switch((uint16)addr) {
  case 0x2180:  //WMDATA
    return bus.read(0x7e0000 | io.wramAddress++, data);

  case 0x4016:  //JOYSER0
    data &= 0xfc;
    data |= controllerPort1.device->data();
    return data;

  case 0x4017:  //JOYSER1
    data &= 0xe0;
    data |= 0x1c;  //pins are connected to GND
    data |= controllerPort2.device->data();
    return data;

  case 0x4210:  //RDNMI
    data &= 0x70;
    data |= rdnmi() << 7;
    data |= (uint4)version;
    return data;

  case 0x4211:  //TIMEUP
    data &= 0x7f;
    data |= timeup() << 7;
    return data;

  case 0x4212:  //HVBJOY
    data &= 0x3e;
    data |= (status.autoJoypadActive) << 0;
    data |= (hcounter() <= 2 || hcounter() >= 1096) << 6;  //hblank
    data |= (vcounter() >= ppu.vdisp()) << 7;              //vblank
    return data;

  case 0x4213: return io.pio;            //RDIO

  case 0x4214: return io.rddiv.byte(0);  //RDDIVL
  case 0x4215: return io.rddiv.byte(1);  //RDDIVH
  case 0x4216: return io.rdmpy.byte(0);  //RDMPYL
  case 0x4217: return io.rdmpy.byte(1);  //RDMPYH

  case 0x4218: return io.joy1.byte(0);   //JOY1L
  case 0x4219: return io.joy1.byte(1);   //JOY1H
  case 0x421a: return io.joy2.byte(0);   //JOY2L
  case 0x421b: return io.joy2.byte(1);   //JOY2H
  case 0x421c: return io.joy3.byte(0);   //JOY3L
  case 0x421d: return io.joy3.byte(1);   //JOY3H
  case 0x421e: return io.joy4.byte(0);   //JOY4L
  case 0x421f: return io.joy4.byte(1);   //JOY4H

  }

  return data;
}

auto CPU::readDMA(uint24 addr, uint8 data) -> uint8 {
  auto& channel = this->channels[addr.bits(4,6)];

  switch(addr & 0xff8f) {

  case 0x4300:  //DMAPx
    return (
      channel.transferMode    << 0
    | channel.fixedTransfer   << 3
    | channel.reverseTransfer << 4
    | channel.unused          << 5
    | channel.indirect        << 6
    | channel.direction       << 7
    );

  case 0x4301: return channel.targetAddress;          //BBADx
  case 0x4302: return channel.sourceAddress.byte(0);  //A1TxL
  case 0x4303: return channel.sourceAddress.byte(1);  //A1TxH
  case 0x4304: return channel.sourceBank;             //A1Bx
  case 0x4305: return channel.transferSize.byte(0);   //DASxL
  case 0x4306: return channel.transferSize.byte(1);   //DASxH
  case 0x4307: return channel.indirectBank;           //DASBx
  case 0x4308: return channel.hdmaAddress.byte(0);    //A2AxL
  case 0x4309: return channel.hdmaAddress.byte(1);    //A2AxH
  case 0x430a: return channel.lineCounter;            //NTRLx
  case 0x430b: return channel.unknown;                //???x
  case 0x430f: return channel.unknown;                //???x ($43xb mirror)

  }

  return data;
}

auto CPU::writeRAM(uint24 addr, uint8 data) -> void {
  wram[addr] = data;
}

auto CPU::writeAPU(uint24 addr, uint8 data) -> void {
  synchronize(smp);
  return smp.portWrite(addr.bits(0,1), data);
}

auto CPU::writeCPU(uint24 addr, uint8 data) -> void {
  switch((uint16)addr) {

  case 0x2180:  //WMDATA
    return bus.write(0x7e0000 | io.wramAddress++, data);

  case 0x2181:  //WMADDL
    io.wramAddress.bits(0,7) = data;
    return;

  case 0x2182:  //WMADDM
    io.wramAddress.bits(8,15) = data;
    return;

  case 0x2183:  //WMADDH
    io.wramAddress.bit(16) = data.bit(0);
    return;

  case 0x4016:  //JOYSER0
    //bit 0 is shared between JOYSER0 and JOYSER1:
    //strobing $4016.d0 affects both controller port latches.
    //$4017 bit 0 writes are ignored.
    controllerPort1.device->latch(data.bit(0));
    controllerPort2.device->latch(data.bit(0));
    return;

  case 0x4200:  //NMITIMEN
    io.autoJoypadPoll = data.bit(0);
    nmitimenUpdate(data);
    return;

  case 0x4201:  //WRIO
    if(io.pio.bit(7) && !data.bit(7)) ppu.latchCounters();
    io.pio = data;
    return;

  case 0x4202:  //WRMPYA
    io.wrmpya = data;
    return;

  case 0x4203:  //WRMPYB
    io.rdmpy = 0;
    if(alu.mpyctr || alu.divctr) return;

    io.wrmpyb = data;
    io.rddiv = (io.wrmpyb << 8) | io.wrmpya;

    alu.mpyctr = 8;  //perform multiplication over the next eight cycles
    alu.shift = io.wrmpyb;
    return;

  case 0x4204:  //WRDIVL
    io.wrdiva.byte(0) = data;
    return;

  case 0x4205:  //WRDIVH
    io.wrdiva.byte(1) = data;
    return;

  case 0x4206:  //WRDIVB
    io.rdmpy = io.wrdiva;
    if(alu.mpyctr || alu.divctr) return;

    io.wrdivb = data;

    alu.divctr = 16;  //perform division over the next sixteen cycles
    alu.shift = io.wrdivb << 16;
    return;

  case 0x4207:  //HTIMEL
    io.htime = (io.htime >> 2) - 1;
    io.htime.bits(0,7) = data;
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4208:  //HTIMEH
    io.htime = (io.htime >> 2) - 1;
    io.htime.bit(8) = data.bit(0);
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4209:  //VTIMEL
    io.vtime.bits(0,7) = data;
    return;

  case 0x420a:  //VTIMEH
    io.vtime.bit(8) = data.bit(0);
    return;

  case 0x420b:  //DMAEN
    for(auto n : range(8)) channels[n].dmaEnable = data.bit(n);
    if(data) status.dmaPending = true;
    return;

  case 0x420c:  //HDMAEN
    for(auto n : range(8)) channels[n].hdmaEnable = data.bit(n);
    return;

  case 0x420d:  //MEMSEL
    io.fastROM = data.bit(0);
    return;

  }
}

auto CPU::writeDMA(uint24 addr, uint8 data) -> void {
  auto& channel = this->channels[addr.bits(4,6)];

  switch(addr & 0xff8f) {

  case 0x4300:  //DMAPx
    channel.transferMode    = data.bits(0,2);
    channel.fixedTransfer   = data.bit (3);
    channel.reverseTransfer = data.bit (4);
    channel.unused          = data.bit (5);
    channel.indirect        = data.bit (6);
    channel.direction       = data.bit (7);
    return;

  case 0x4301:  //BBADx
    channel.targetAddress = data;
    return;

  case 0x4302:  //A1TxL
    channel.sourceAddress.byte(0) = data;
    return;

  case 0x4303:  //A1TxH
    channel.sourceAddress.byte(1) = data;
    return;

  case 0x4304:  //A1Bx
    channel.sourceBank = data;
    return;

  case 0x4305:  //DASxL
    channel.transferSize.byte(0) = data;
    return;

  case 0x4306:  //DASxH
    channel.transferSize.byte(1) = data;
    return;

  case 0x4307:  //DASBx
    channel.indirectBank = data;
    return;

  case 0x4308:  //A2AxL
    channel.hdmaAddress.byte(0) = data;
    return;

  case 0x4309:  //A2AxH
    channel.hdmaAddress.byte(1) = data;
    return;

  case 0x430a:  //NTRLx
    channel.lineCounter = data;
    return;

  case 0x430b:  //???x
    channel.unknown = data;
    return;

  case 0x430f:  //???x ($43xb mirror)
    channel.unknown = data;
    return;

  }
}
