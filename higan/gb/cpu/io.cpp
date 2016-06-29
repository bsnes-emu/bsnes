auto CPU::wramAddress(uint16 addr) const -> uint {
  addr &= 0x1fff;
  if(addr < 0x1000) return addr;
  auto bank = status.wramBank + (status.wramBank == 0);
  return (bank * 0x1000) + (addr & 0x0fff);
}

auto CPU::joypPoll() -> void {
  uint button = 0, dpad = 0;

  button |= interface->inputPoll(0, 0, (uint)Input::Start) << 3;
  button |= interface->inputPoll(0, 0, (uint)Input::Select) << 2;
  button |= interface->inputPoll(0, 0, (uint)Input::B) << 1;
  button |= interface->inputPoll(0, 0, (uint)Input::A) << 0;

  dpad |= interface->inputPoll(0, 0, (uint)Input::Down) << 3;
  dpad |= interface->inputPoll(0, 0, (uint)Input::Up) << 2;
  dpad |= interface->inputPoll(0, 0, (uint)Input::Left) << 1;
  dpad |= interface->inputPoll(0, 0, (uint)Input::Right) << 0;

  if(system.revision() != System::Revision::SuperGameBoy) {
    //D-pad pivot makes it impossible to press opposing directions at the same time
    //however, Super Game Boy BIOS is able to set these bits together
    if(dpad & 4) dpad &= ~8;  //disallow up+down
    if(dpad & 2) dpad &= ~1;  //disallow left+right
  }

  status.joyp = 0x0f;
  if(status.p15 == 1 && status.p14 == 1) status.joyp -= status.mltReq;
  if(status.p15 == 0) status.joyp &= button ^ 0x0f;
  if(status.p14 == 0) status.joyp &= dpad ^ 0x0f;
  if(status.joyp != 0x0f) raise(Interrupt::Joypad);
}

auto CPU::readIO(uint16 addr) -> uint8 {
  if(addr >= 0xc000 && addr <= 0xfdff) return wram[wramAddress(addr)];
  if(addr >= 0xff80 && addr <= 0xfffe) return hram[addr & 0x7f];

  if(addr == 0xff00) {  //JOYP
    joypPoll();
    return 0xc0
         | (status.p15 << 5)
         | (status.p14 << 4)
         | (status.joyp << 0);
  }

  if(addr == 0xff01) {  //SB
    return 0x00;
  }

  if(addr == 0xff02) {  //SC
    return (status.serialTransfer << 7)
         | 0x7e
         | (status.serialClock << 0);
  }

  if(addr == 0xff04) {  //DIV
    return status.div >> 8;
  }

  if(addr == 0xff05) {  //TIMA
    return status.tima;
  }

  if(addr == 0xff06) {  //TMA
    return status.tma;
  }

  if(addr == 0xff07) {  //TAC
    return 0xf8
         | (status.timerEnable << 2)
         | (status.timerClock << 0);
  }

  if(addr == 0xff0f) {  //IF
    return 0xe0
         | (status.interruptRequestJoypad << 4)
         | (status.interruptRequestSerial << 3)
         | (status.interruptRequestTimer << 2)
         | (status.interruptRequestStat << 1)
         | (status.interruptRequestVblank << 0);
  }

  if(addr == 0xff4d) {  //KEY1
    return (status.speedDouble << 7);
  }

  if(addr == 0xff55) {  //HDMA5
    return (status.dmaCompleted << 7)
         | (((status.dmaLength / 16) - 1) & 0x7f);
  }

  if(addr == 0xff56) {  //RP
    return 0x02;
  }

  if(addr == 0xff6c) {  //???
    return 0xfe | status.ff6c;
  }

  if(addr == 0xff70) {  //SVBK
    return status.wramBank;
  }

  if(addr == 0xff72) {  //???
    return status.ff72;
  }

  if(addr == 0xff73) {  //???
    return status.ff73;
  }

  if(addr == 0xff74) {  //???
    return status.ff74;
  }

  if(addr == 0xff75) {  //???
    return 0x8f | status.ff75;
  }

  if(addr == 0xff76) {  //???
    return 0xff;
  }

  if(addr == 0xff77) {  //???
    return 0xff;
  }

  if(addr == 0xffff) {  //IE
    return 0xe0
         | (status.interruptEnableJoypad << 4)
         | (status.interruptEnableSerial << 3)
         | (status.interruptEnableTimer << 2)
         | (status.interruptEnableStat << 1)
         | (status.interruptEnableVblank << 0);
  }

  return 0xff;
}

auto CPU::writeIO(uint16 addr, uint8 data) -> void {
  if(addr >= 0xc000 && addr <= 0xfdff) { wram[wramAddress(addr)] = data; return; }
  if(addr >= 0xff80 && addr <= 0xfffe) { hram[addr & 0x7f] = data; return; }

  if(addr == 0xff00) {  //JOYP
    status.p15 = data & 0x20;
    status.p14 = data & 0x10;
    interface->joypWrite(status.p15, status.p14);
    return;
  }

  if(addr == 0xff01) {  //SB
    status.serialData = data;
    return;
  }

  if(addr == 0xff02) {  //SC
    status.serialTransfer = data & 0x80;
    status.serialClock = data & 0x01;
    if(status.serialTransfer) status.serialBits = 8;
    return;
  }

  if(addr == 0xff04) {  //DIV
    status.div = 0;
    return;
  }

  if(addr == 0xff05) {  //TIMA
    status.tima = data;
    return;
  }

  if(addr == 0xff06) {  //TMA
    status.tma = data;
    return;
  }

  if(addr == 0xff07) {  //TAC
    status.timerEnable = data & 0x04;
    status.timerClock = data & 0x03;
    return;
  }

  if(addr == 0xff0f) {  //IF
    status.interruptRequestJoypad = data & 0x10;
    status.interruptRequestSerial = data & 0x08;
    status.interruptRequestTimer = data & 0x04;
    status.interruptRequestStat = data & 0x02;
    status.interruptRequestVblank = data & 0x01;
    return;
  }

  if(addr == 0xff4d) {  //KEY1
    status.speedSwitch = data & 0x01;
    return;
  }

  if(addr == 0xff51) {  //HDMA1
    status.dmaSource = (status.dmaSource & 0x00ff) | (data << 8);
    return;
  }

  if(addr == 0xff52) {  //HDMA2
    status.dmaSource = (status.dmaSource & 0xff00) | (data & 0xf0);
    return;
  }

  if(addr == 0xff53) {  //HDMA3
    status.dmaTarget = (status.dmaTarget & 0x00ff) | (data << 8);
    return;
  }

  if(addr == 0xff54) {  //HDMA4
    status.dmaTarget = (status.dmaTarget & 0xff00) | (data & 0xf0);
    return;
  }

  if(addr == 0xff55) {  //HDMA5
    status.dmaMode = data & 0x80;
    status.dmaLength = ((data & 0x7f) + 1) * 16;
    status.dmaCompleted = !status.dmaMode;

    if(status.dmaMode == 0) {
      do {
        for(auto n : range(16)) {
          writeDMA(status.dmaTarget++, readDMA(status.dmaSource++));
        }
        step(8 << status.speedDouble);
        status.dmaLength -= 16;
      } while(status.dmaLength);
    }
    return;
  }

  if(addr == 0xff56) {  //RP
    return;
  }

  if(addr == 0xff6c) {  //???
    status.ff6c = data & 0x01;
    return;
  }

  if(addr == 0xff72) {  //???
    status.ff72 = data;
    return;
  }

  if(addr == 0xff73) {  //???
    status.ff73 = data;
    return;
  }

  if(addr == 0xff74) {  //???
    status.ff74 = data;
    return;
  }

  if(addr == 0xff75) {  //???
    status.ff75 = data & 0x70;
    return;
  }

  if(addr == 0xff70) {  //SVBK
    status.wramBank = data & 0x07;
    return;
  }

  if(addr == 0xffff) {  //IE
    status.interruptEnableJoypad = data & 0x10;
    status.interruptEnableSerial = data & 0x08;
    status.interruptEnableTimer = data & 0x04;
    status.interruptEnableStat = data & 0x02;
    status.interruptEnableVblank = data & 0x01;
    return;
  }
}
