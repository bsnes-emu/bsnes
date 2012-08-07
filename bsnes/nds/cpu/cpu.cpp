#include <nds/nds.hpp>
#include "bit.hpp"
#define imatch(bits)  ((i & force< Bit::mask(bits) >()) \
                         == force< Bit::binary(bits) >())

template<uint32 arg> static constexpr uint32 force() { return arg; }

namespace NintendoDS {

#include "core.cpp"
#include "disasm.cpp"

ARM7TDMI arm7;
ARM946ES arm9;


CPUCore::CPUCore() {
  thread = nullptr;
}

CPUCore::~CPUCore() {
  if(thread) co_delete(thread);
}

void CPUCore::power() {
  ARMCore::power();
  
  clock = 0;
  trace = false;
  
  powerState = running;
  
  config.xorSeeds[0]     = 0;
  config.xorSeeds[1]     = 0;
  config.slot2ramTiming  = 0;
  config.slot2romTiming0 = 0;
  config.slot2romTiming1 = 0;
  config.slot2phi        = 0;
  
  interrupt.gate   = false;
  interrupt.enable = false;
  interrupt.flags  = 0;
  
  status.inVBlank    = false;
  status.inHBlank    = false;
  status.inVMatch    = false;
  status.irqOnVBlank = false;
  status.irqOnHBlank = false;
  status.irqOnVMatch = false;
  status.vcompare    = 0;
  
  msg.enable      = false;
  msg.irqOnRecv   = false;
  msg.irqOnSent   = false;
  msg.error       = 0;
  msg.empty       = 1;
  msg.full        = 0;
  msg.writeOffset = 0;
  msg.readOffset  = 0;
  for(auto &e : msg.buffer)
    e = 0;
  
  sync.enable = false;
  sync.output = 0;
  
  event.irq    = false;
  event.anydma = false;
  event.queue.reset();
  
  for(unsigned n = 0; n < 4; n++) {
    dma[n].enable      = false;
    dma[n].irq         = false;
    dma[n].size        = 0;
    dma[n].repeat      = false;
    dma[n].trigger     = 0;
    dma[n].srcStep     = 0;
    dma[n].destStep    = 0;
    dma[n].source      = 0;
    dma[n].dest        = 0;
    dma[n].count       = 0;
    dma[n].fill        = 0;
    dma[n].init.source = 0;
    dma[n].init.dest   = 0;
    dma[n].init.count  = 0;
    
    timer[n].enable     = false;
    timer[n].irq        = false;
    timer[n].cascade    = false;
    timer[n].divider    = 0;
    timer[n].reload     = 0;
    timer[n].count      = 0;
    timer[n].lastUpdate = 0;
    timer[n].event.action = [&, n]() { updateTimer(n); };
  }
}

void CPUCore::hdraw() {
  status.inHBlank = false;
  status.inVMatch = false;
  
  if(192 == video.line) {
    status.inVBlank = true;
    if(status.irqOnVBlank)
      interrupt.flags |= irqVBlank;
    
    // Trigger vblank DMAs
    dmaTrigger(0xf, 1);
  }
  if(262 == video.line) {
    status.inVBlank = false;
  }
  
  // Trigger peripheral-to-framebuffer DMA
  if(config.arm9 && 2 <= video.line && video.line < 192+2)
    dmaTrigger(0xf, 3);
  
  if(status.vcompare == video.line) {
    status.inVMatch = true;
    if(status.irqOnVMatch) {
      interrupt.flags |= irqVMatch;
    }
  }
}

void CPUCore::hblank() {
  status.inHBlank = true;
  if(status.irqOnHBlank)
    interrupt.flags |= irqHBlank;
  
  // Trigger HDMAs
  if(config.arm9 && video.line < 192)
    dmaTrigger(0xf, 2);
}



string CPUCore::tracePsr(uint32 value) {
  return { value & 1<<31? "N":" ",
           value & 1<<30? "Z":" ",
           value & 1<<29? "C":" ",
           value & 1<<28? "V":" ",
           value & 1<<27? "Q":" ",
           value & 1<< 7? "I":" ",
           value & 1<< 6? "F":" ",
           value & 1<< 5? "T":" ",
           "/", hex<2>(value & 0x1f) };
}

void CPUCore::traceInsn() {
  for(unsigned n = 0; n < 8; n++) print(hex<1>(n),":",hex<8>(r[n])," ");
  
  print(tracePsr(readCpsr()), "\n");
  
  for(unsigned n = 8; n < 16; n++) print(hex<1>(n),":",hex<8>(r[n])," ");
  
  if(mode != SYS && mode != USR) print(tracePsr(spsr()), "\n");
  else                           print("--------:--", "\n");
  
  if(Tf) print(hex<8>(r[15] - 4), ": ", hex<4>(iexecute), "  ", disasm::thumb(this, iexecute), "\n");
  else   print(hex<8>(r[15] - 8), ": ", hex<8>(iexecute), "  ", disasm::arm(this, iexecute), "\n");
  
  fflush(stdout);
}



uint32 CPUCore::readReg(uint32 addr, uint32 size) {
  addr &= ~3;
  
  //static bool vbl = 0;
  switch(addr-0x04000000) {
  case 0x0004: // Display status
    return           video.line << 16 | status.irqOnVMatch<< 5 | status.inVMatch<< 2
      | (status.vcompare & 0x0ff)<< 8 | status.irqOnHBlank<< 4 | status.inHBlank<< 1
      | (status.vcompare & 0x100)>> 1 | status.irqOnVBlank<< 3 | status.inVBlank<< 0;
    
  case 0x1004:
    return 0;
  
  case 0x00b0: // DMA0 source
  case 0x00bc: // DMA1 source
  case 0x00c8: // DMA2 source
  case 0x00d4: // DMA3 source
    return regDmaSource((addr-0x40000b0)/12);
    
  case 0x00b4: // DMA0 dest
  case 0x00c0: // DMA1 dest
  case 0x00cc: // DMA2 dest
  case 0x00d8: // DMA3 dest
    return regDmaDest((addr-0x40000b0)/12);
    
  case 0x00b8: // DMA0 control
  case 0x00c4: // DMA1 control
  case 0x00d0: // DMA2 control
  case 0x00dc: // DMA3 control
    return regDmaControl((addr-0x40000b0)/12);
    
  case 0x00e0: // DMA0 fill
  case 0x00e4: // DMA1 fill
  case 0x00e8: // DMA2 fill
  case 0x00ec: // DMA3 fill
    return regDmaFill(addr>>2 & 3);
    
  case 0x0100: // Timer 0
  case 0x0104: // Timer 1
  case 0x0108: // Timer 2
  case 0x010c: // Timer 3
    return regTimer(addr>>2 & 3);
  
  case 0x130: { // Keypad input
    unsigned keys = 0;
    for(unsigned n = 0; n < 10; n++) {
      if(interface->inputPoll(ID::Port::Buttons, 0, n))
        keys += 1<<n;
    }
    return keys ^ 0x3ff;
    }
  case 0x132: // Keypad interrupt
    break;
    
  // SYNC, message control, receive port
  case 0x000180: return regSync();
  case 0x000184: return regMsgControl();
  case 0x100000: return regMsgRecv();
    
  case 0x0001a0: return regSlot1Control();
  case 0x0001a4: return regSlot1RomControl();
  case 0x0001a8: return regSlot1RomCommand(0);
  case 0x0001ac: return regSlot1RomCommand(1);
  case 0x100010: return regSlot1RomRecv();
  
  // IME, IE, IF
  case 0x0208: return interrupt.gate;
  case 0x0210: return interrupt.enable;
  case 0x0214: return interrupt.flags;
  case 0x0218: return 0;  // IE: DSi bits
  case 0x021c: return 0;  // IF: DSi bits
  
  // DSi-enhanced software checks these.
  case 0x4000:  return 0;
  case 0x4008:  return 0<<15;
  }
  //print("r ",hex<8>(addr),":",size,": unimplemented\n");
  return 0;
}



void CPUCore::writeReg(uint32 addr, uint32 size, uint32 data) {
  uint32 mask = 0xffffffff;
  if(size == Half) mask = 0xffff << 8*(addr & 2);
  if(size == Byte) mask = 0xff << 8*(addr & 3);
  
  addr &= ~3, data &= mask;
  
  switch(addr-0x04000000) {
  case 0x0004: // Display status
    // It might seem odd to have this here, but each CPU has one.
    if(mask & 0xff00) status.vcompare = (status.vcompare & 0x100) | (data>>8 & 0x0ff);
    if(mask & 0x0080) status.vcompare = (status.vcompare & 0x0ff) | (data<<1 & 0x100);
    if(mask & 0x003f) {
      status.irqOnVMatch = data >> 5;
      status.irqOnHBlank = data >> 4;
      status.irqOnVBlank = data >> 3;
    }
    return;
    
  case 0x1004:
    return;
    
  case 0x00b0: // DMA0 source
  case 0x00bc: // DMA1 source
  case 0x00c8: // DMA2 source
  case 0x00d4: // DMA3 source
    return regDmaSource((addr-0x40000b0)/12, data, mask);
    
  case 0x00b4: // DMA0 dest
  case 0x00c0: // DMA1 dest
  case 0x00cc: // DMA2 dest
  case 0x00d8: // DMA3 dest
    return regDmaDest((addr-0x40000b0)/12, data, mask);
    
  case 0x00b8: // DMA0 control
  case 0x00c4: // DMA1 control
  case 0x00d0: // DMA2 control
  case 0x00dc: // DMA3 control
    return regDmaControl((addr-0x40000b0)/12, data, mask);
    
  case 0x00e0: // DMA0 fill
  case 0x00e4: // DMA1 fill
  case 0x00e8: // DMA2 fill
  case 0x00ec: // DMA3 fill
    return regDmaFill(addr>>2 & 3, data, mask);
    
  case 0x0100: // Timer 0
  case 0x0104: // Timer 1
  case 0x0108: // Timer 2
  case 0x010c: // Timer 3
    return regTimer(addr>>2 & 3, data, mask);
    
  case 0x132: // Keypad interrupt
    break;
    
  // SYNC, message control, send port
  case 0x0180: return regSync(data, mask);
  case 0x0184: return regMsgControl(data, mask);
  case 0x0188: return regMsgSend(data);
  
  // Slot 1
  case 0x01a0: return regSlot1Control(data, mask);
  case 0x01a4: return regSlot1RomControl(data, mask);
  case 0x01a8: return regSlot1RomCommand(0, data, mask);
  case 0x01ac: return regSlot1RomCommand(1, data, mask);
  case 0x01b0: return regSlot1RomSeed(0, data, mask);
  case 0x01b4: return regSlot1RomSeed(1, data, mask);
  case 0x01b8: return regSlot1RomSeed(2, data, mask);
  
  // IME, IE, IF
  case 0x0208: if(mask & 1) interrupt.gate = data & 1; return;
  case 0x0210: interrupt.enable = interrupt.enable & ~mask | data; return;
  case 0x0214: interrupt.flags &= ~data; return;
  case 0x0218: return;  // IE: DSi bits
  case 0x021c: return;  // IF: DSi bits
  }
  //print("w ",hex<8>(addr),":",size," = ",hex<8>(data),": unimplemented\n");
}

#include "slot.cpp"
#include "message.cpp"
#include "dma.cpp"
#include "timer.cpp"
#include "arm7tdmi.cpp"
#include "arm946es.cpp"

#undef imatch

}
