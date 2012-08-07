#include <nds/nds.hpp>

namespace NintendoDS {

Slot1 slot1;


Slot1::Slot1() {
  card = nullptr;
}

void Slot1::power() {
  if(card) {
    card->power();
    if(card->spi)
      card->spi->power();
  }
  enable = true;
  
  decryptLatency = 0;
  responseLatency = 0;
  xorData = 0;
  xorCmds = 0;
  dataReady = 0;
  blockSize = 0;
  clock = 0;
  secureMode = 0;
  transferPending = 0;
  transferIrq = 0;
  transferLength = 0;
  
  command = 0;
  lfsr[0] = 0;
  lfsr[1] = 0;
  
  spi.data = 0;
  spi.baud = 0;
  spi.hold = 0;
  spi.busy = 0;
  spi.enable = 0; 
}

void Slot1::load(GameCard* card) {
  this->card = card;
  if(card->spi)
    card->spi->power();
}

GameCard* Slot1::unload() {
  //if(card && card->spi)
  //  card->spi->select(false);
  
  auto r = card;
  card = nullptr;
  return r;
}


void Slot1::startRomTransfer() {
  transferPending = true;
  transferLength = 0;
  
  if(blockSize == 7) transferLength = 4;
  else if(blockSize) transferLength = 512 << (blockSize-1);
  
  if(transferLength) dataReady = true;
  else               transferPending = false;
  
  if(card) card->command(command);
  
  // Hack for now, until we implement timing. Should test whether the ARM9
  // halts during this entire DMA - one complete word transfers every 40 or
  // 64 clocks at 66MHz.
  CPUCore *arm = arm9.slot1access? (CPUCore*)&arm9 : (CPUCore*)&arm7;
  //while(transferLength > 0 && arm->dmaTrigger(0xf, 5)) /**/;
  int ch = -1;
  for(unsigned n = 0; n < 4; n++)
    if(arm->dma[n].enable && arm->dma[n].trigger == 5)
      ch = n;
  
  while(ch >= 0 && transferLength > 0)
    arm->dmaTransfer(ch);
}

uint8 Slot1::readRom() {
  uint8 data = 0xff;
  
  if(transferLength) {
    // Empty slot simply returns 0xff
    if(card) data = card->read();
    
    if(!--transferLength) {
      dataReady = false;
      transferPending = false;
      
      if(transferIrq) {
        if(arm9.slot1access == 0) arm7.interrupt.flags |= CPUCore::irqCardDone;
        if(arm9.slot1access == 1) arm9.interrupt.flags |= CPUCore::irqCardDone;
      }
    }
  }
  return data;
}

uint8 Slot1::spiTransfer(uint8 data) {
  if(!card || !card->spi)
    return 0xff;
  
  auto device = card->spi;
  device->select(true);
  uint8 r = device->transfer(data);
  device->select(spi.hold);
  return r;
}


GameCard::~GameCard() {
  delete spi;
}

GameCard::GameCard(uint32 id) {
  rom.size = 0x10000;
  rom.data = new uint8[rom.size];
  memset(rom.data, 0xff, rom.size);
  
  size     = bit::round(rom.size);
  chipId   = id;
  spi      = nullptr;
}

GameCard::GameCard(const stream& memory, uint32 esize, uint32 id) {
  rom.size = esize;
  rom.data = new uint8[rom.size];
  memset(rom.data, 0xff, rom.size);
  memory.read(rom.data, min(memory.size(), rom.size));
  
  size   = bit::round(rom.size);
  sha256 = nall::sha256(rom.data, rom.size);
  chipId = id;
  spi    = nullptr;
}

void GameCard::power() {
  state = idle;
}

void GameCard::command(uint64 command) {
  if((command>>56) == 0xb7) {
    state  = readData;
    offset = command>>24 & 0x00000fff;
    block  = command>>24 & 0xfffff000;
    block &= size-1;
    
    // Once initialized, forbid reading the header and startup code.
    if(block < 0x8000) block += 0x8000;
  }
  if((command>>56) == 0xb8) {
    state  = readId;
    offset = 0;
  }
}

uint8 GameCard::read() {
  uint8 r = 0xff;
  
  if(state == readData) {
    // We remain in this state indefinitely, until the next command.
    // However, reading from most (?) game cards wraps at 4K intervals.
    uint32 addr = (block + offset++) & size-1;
    offset &= 0xfff;
    
    // Cards come in 2^n ROM sizes, however many images have the empty space
    // trimmed off. Homebrew images aren't even padded out! Rather than waste
    // memory, just check for this and return $ff.
    if(addr < rom.size)
      r = rom.data[addr];
  }
  
  if(state == readId) {
    // Need to ensure the matching ID is in RAM or games won't run
    //   (where does the firmware put it?)
    r = 0;/**/chipId >> 8*offset++;
    offset &= 3;
  }
  return r;
}



void IRPort::power() {
  bypass = false;
  command = 0;
  if(slave)
    slave->power();
}

void IRPort::select(bool state) {
  if(bypass) {
    slave->select(state);
  }
  if(state == false) {
    bypass = false;
    command = 0;
  }
}

uint8 IRPort::transfer(uint8 data) {
  if(bypass) {
    // Pass transfers through to flash memory
    if(slave) return slave->transfer(data);
    else      return 0xff;
  }
  
  if(command == 0x00) {
    command = data;
    
    // Engage passthrough mode - allows access to save data.
    if(command == 0x00) {
      bypass = true;
      return 0xff;
    }
    
    if(command == 0x01) {
      // Receive? HG/SS expect an 8-bit packet size,
      // then 0..185 bytes, all XORed with $aa.
      return 0x00;
    }
    
    if(command == 0x02) {
      // Write a packet, maybe to transmit buffer
      return 0xff;
    }
    
    if(command == 0x08) {
      // ID of some kind? Is it even a valid command?
      // Returns $aa after powerup - haven't tested the Pokewalker with it.
      // Pokemon HG/SS frequently check this, even when not communicating.
      return 0xaa;
    }
  }
  return 0xff;
}

}
