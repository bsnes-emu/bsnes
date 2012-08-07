
Flash::~Flash() { }

Flash::Flash(uint32 esize, uint32 id) {
  data = new uint8[size = esize];
  memset(data, 0xff, size);
  this->id = id;
}

Flash::Flash(const stream& memory, uint32 esize, uint32 id) {
  data = new uint8[size = esize];
  memset(data, 0xff, size);
  memory.read(data, min(memory.size(), size));
  this->id = id;
}

void Flash::power() {
  powered = true;
  command = 0;
  writeEnable = false;
}

void Flash::select(bool state) {
  if(state) return;
  
  //print("flash: deselect - cmd=",hex<2>(command)," wen=",writeEnable,"\n");
  if(command && writeEnable) {
    // Finish some previously submitted commands
    if(command == 0x0a) {
      // Read any remaining page into the buffer,
      // so it's not lost during the erase cycle.
      while(count < 256) {
        buffer[count++] = data[address++];
        if(!(address & 0xff)) address -= 0x100;
      }
    }
    
    if(command == 0x0a || command == 0xdb || command == 0xd8) {
      // Modify page / erase page / erase 64Kbyte sector
      unsigned block = command == 0xd8? 0x10000 : 0x100;
      
      address = page & ~(block-1);
      
      // Erasing sets data bits to 1.
      for(unsigned n = 0; n < block; n++)
        data[address++] = 0xff;
      
      writeEnable = false;
    }
    
    if(command == 0x0a || command == 0x02) {
      // Modify page / write page
      address = page;
      //print("finishing write to ",hex<6>(page),"\n");
      
      for(unsigned n = 0; n < 256; n++) {
        // Writing can only clear bits.
        data[address++] &= buffer[n];
        if(!(address & 0xff)) address -= 0x100;
      }
      writeEnable = false;
    }
  }
  // Reset and wait for a new command
  command = 0;
}

uint8 Flash::transfer(uint8 data) {
  if(command == 0) {
    command = data;
    count = 0;
    address = 0;
    addrCount = 0;
    
    //print("flash: cmd ",hex<2>(command),"\n");
    
    if(command == 0xab) powered = true;  // Wake up
    
    if(powered) {
      if(command == 0x9f) count = 3;             // Read ID
      if(command == 0xb9) powered = false;       // Power down
      if(command == 0x06) writeEnable = true;    // Write enable
      if(command == 0x04) writeEnable = false;   // Write disable
      
      if(command == 0xdb || command == 0xd8) addrCount = 3;  // Erase page / sector
      if(command == 0x02 || command == 0x0a) addrCount = 3;  // Write data
      if(command == 0x03 || command == 0x0b) addrCount = 3;  // Read data
    }
    return 0;
  }
  if(!powered)
    return 0;
  
  if(addrCount) {
    // Input address
    addrCount--;
    address = address<<8 | data;
    page = address; 
    if(command == 0x0b) count = 1;  // Dummy byte between address and data
    return 0;
  }
  
  if(command == 0x9f) {
    // Read ID
    //print("flash: read id\n");
    if(count) count--;
    return id >> 8*count;
  }
  
  if(command == 0x05) {
    // Read status
    //print("flash: read status\n");
    return writeEnable<<1; // | writing<<0;
  }
  
  if(command == 0x03 || command == 0x0b) {
    // Read data
    //if(address == page) print("flash: read ",hex<6>(address),"\n");
    if(count) { count--; return 0; }
    if(address >= size) address = 0;
    return this->data[address++];
  }
  
  if(command == 0x02 || command == 0x0a) {
    // Write page / modify page
    //if(address == page) print("flash: write ",hex<6>(address),"\n");
    if(count < 0x100) {
      if(address >= size) address = 0;
      buffer[count++] = data;
      uint8 r = this->data[address++];
      if(!(address & 0xff)) address -= 0x100;
      return r;
    }
    return 0;
  }
  return 0;
}
