
EEPROM::~EEPROM() { }

EEPROM::EEPROM(uint32 esize, uint32 psize) {
  data = new uint8[size = esize];
  pageSize = psize;
  memset(data, 0xff, size);
}

EEPROM::EEPROM(const stream& memory, uint32 esize, uint32 psize) {
  data = new uint8[size = esize];
  pageSize = psize;
  memset(data, 0xff, size);
  memory.read(data, min(memory.size(), size));
}

void EEPROM::power() {
  if(!pageSize) {
         if(size <=  0x200) pageSize = 0x10;
    else if(size <= 0x2000) pageSize = 0x20;
    else                    pageSize = 0x80;
  }
  command = 0;
  writeEnable = false;
}

void EEPROM::select(bool state) {
  if(state) return;
  
  if(command && writeEnable) {
    // Finish some previously submitted commands
    if(command == 0x02 || command == 0x0a) {
      //print("finishing write to ",hex<6>(page),"\n");
      address = page;
      
      // Write page
      for(unsigned n = 0; n < count; n++) {
        if(!(address & pageSize-1)) address = page;
        this->data[address++] = buffer[n];
      }
      writeEnable = false;
    }
  }
  // Reset and wait for a new command
  command = 0;
}

uint8 EEPROM::transfer(uint8 data) {
  if(command == 0) {
    command = data;
    address = count = 0;
    addrCount = 0;
    
    //print("eeprom: cmd ",hex<2>(command),"\n");
    if(command == 0x02 || command == 0x0a) addrCount = 1 + (size > 512);
    if(command == 0x03 || command == 0x0b) addrCount = 1 + (size > 512);
    
    if(command == 0x0a && size == 512) address = 1;  // read/write 2nd page
    if(command == 0x0b && size == 512) address = 1;  //   (512-byte only)
    
    if(command == 0x04) writeEnable = false;
    if(command == 0x06) writeEnable = true;
    return 0xff;
  }
  
  if(addrCount) { // Input address
    addrCount--;
    address = address<<8 | data;
    page = address;
    return 0xff;
  }
  
  if(command == 0x01) {
    return 0xff; // Write status
  }
  
  if(command == 0x05) { // Read status
    return (size > 512? 0x00 : 0xf0) | writeEnable<<1; // | writing<<0;
  }
  
  if(command == 0x03 || command == 0x0b) { // Read data
    //if(address == page) print("eeprom: read ",hex<6>(address),"\n");
    if(address == size) address = 0;
    return this->data[address++];
  }
  
  if(command == 0x02 || command == 0x0a) { // Write data
    //if(address == page) print("eeprom: write ",hex<6>(address),"\n");
    if(count < pageSize) {
      if(!(address & pageSize-1)) address = page;
      buffer[count++] = data;
      return this->data[address++];
    }
  }
  return 0xff;
}
