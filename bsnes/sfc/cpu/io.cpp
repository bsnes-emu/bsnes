auto CPU::readRAM(uint addr, uint8 data) -> uint8 {
  return wram[addr];
}

auto CPU::readAPU(uint addr, uint8 data) -> uint8 {
  synchronize(smp);
  return smp.portRead(addr & 3);
}

auto CPU::readCPU(uint addr, uint8 data) -> uint8 {
  switch(addr & 0xffff) {
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

  case 0x4214: return bit8(io.rddiv,0);  //RDDIVL
  case 0x4215: return bit8(io.rddiv,1);  //RDDIVH
  case 0x4216: return bit8(io.rdmpy,0);  //RDMPYL
  case 0x4217: return bit8(io.rdmpy,1);  //RDMPYH

  case 0x4218: return bit8(io.joy1,0);   //JOY1L
  case 0x4219: return bit8(io.joy1,1);   //JOY1H
  case 0x421a: return bit8(io.joy2,0);   //JOY2L
  case 0x421b: return bit8(io.joy2,1);   //JOY2H
  case 0x421c: return bit8(io.joy3,0);   //JOY3L
  case 0x421d: return bit8(io.joy3,1);   //JOY3H
  case 0x421e: return bit8(io.joy4,0);   //JOY4L
  case 0x421f: return bit8(io.joy4,1);   //JOY4H

  }

  return data;
}

auto CPU::readDMA(uint addr, uint8 data) -> uint8 {
  auto& channel = this->channels[addr >> 4 & 7];

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
  case 0x4302: return bit8(channel.sourceAddress,0);  //A1TxL
  case 0x4303: return bit8(channel.sourceAddress,1);  //A1TxH
  case 0x4304: return channel.sourceBank;             //A1Bx
  case 0x4305: return bit8(channel.transferSize,0);   //DASxL
  case 0x4306: return bit8(channel.transferSize,1);   //DASxH
  case 0x4307: return channel.indirectBank;           //DASBx
  case 0x4308: return bit8(channel.hdmaAddress,0);    //A2AxL
  case 0x4309: return bit8(channel.hdmaAddress,1);    //A2AxH
  case 0x430a: return channel.lineCounter;            //NTRLx
  case 0x430b: return channel.unknown;                //???x
  case 0x430f: return channel.unknown;                //???x ($43xb mirror)

  }

  return data;
}

auto CPU::writeRAM(uint addr, uint8 data) -> void {
  wram[addr] = data;
}

auto CPU::writeAPU(uint addr, uint8 data) -> void {
  synchronize(smp);
  return smp.portWrite(addr & 3, data);
}

auto CPU::writeCPU(uint addr, uint8 data) -> void {
  switch(addr & 0xffff) {

  case 0x2180:  //WMDATA
    return bus.write(0x7e0000 | io.wramAddress++, data);

  case 0x2181:  //WMADDL
    bits(io.wramAddress,0-7) = data;
    return;

  case 0x2182:  //WMADDM
    bits(io.wramAddress,8-15) = data;
    return;

  case 0x2183:  //WMADDH
    bit1(io.wramAddress,16) = bit1(data,0);
    return;

  case 0x4016:  //JOYSER0
    //bit 0 is shared between JOYSER0 and JOYSER1:
    //strobing $4016.d0 affects both controller port latches.
    //$4017 bit 0 writes are ignored.
    controllerPort1.device->latch(data & 1);
    controllerPort2.device->latch(data & 1);
    return;

  case 0x4200:  //NMITIMEN
    io.autoJoypadPoll = data & 1;
    nmitimenUpdate(data);
    return;

  case 0x4201:  //WRIO
    if(bit1(io.pio,7) && !bit1(data,7)) ppu.latchCounters();
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
    bit8(io.wrdiva,0) = data;
    return;

  case 0x4205:  //WRDIVH
    bit8(io.wrdiva,1) = data;
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
    bits(io.htime,0-7) = data;
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4208:  //HTIMEH
    io.htime = (io.htime >> 2) - 1;
    bit1(io.htime,8) = data & 1;
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4209:  //VTIMEL
    bits(io.vtime,0-7) = data;
    return;

  case 0x420a:  //VTIMEH
    bit1(io.vtime,8) = data & 1;
    return;

  case 0x420b:  //DMAEN
    for(auto n : range(8)) channels[n].dmaEnable = bit1(data,n);
    if(data) status.dmaPending = true;
    return;

  case 0x420c:  //HDMAEN
    for(auto n : range(8)) channels[n].hdmaEnable = bit1(data,n);
    return;

  case 0x420d:  //MEMSEL
    io.fastROM = data & 1;
    return;

  }
}

auto CPU::writeDMA(uint addr, uint8 data) -> void {
  auto& channel = this->channels[addr >> 4 & 7];

  switch(addr & 0xff8f) {

  case 0x4300:  //DMAPx
    channel.transferMode    = bits(data,0-2);
    channel.fixedTransfer   = bit1(data,3);
    channel.reverseTransfer = bit1(data,4);
    channel.unused          = bit1(data,5);
    channel.indirect        = bit1(data,6);
    channel.direction       = bit1(data,7);
    return;

  case 0x4301:  //BBADx
    channel.targetAddress = data;
    return;

  case 0x4302:  //A1TxL
    bit8(channel.sourceAddress,0) = data;
    return;

  case 0x4303:  //A1TxH
    bit8(channel.sourceAddress,1) = data;
    return;

  case 0x4304:  //A1Bx
    channel.sourceBank = data;
    return;

  case 0x4305:  //DASxL
    bit8(channel.transferSize,0) = data;
    return;

  case 0x4306:  //DASxH
    bit8(channel.transferSize,1) = data;
    return;

  case 0x4307:  //DASBx
    channel.indirectBank = data;
    return;

  case 0x4308:  //A2AxL
    bit8(channel.hdmaAddress,0) = data;
    return;

  case 0x4309:  //A2AxH
    bit8(channel.hdmaAddress,1) = data;
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
