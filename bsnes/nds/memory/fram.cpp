
FRAM::~FRAM() { }

FRAM::FRAM(uint32 esize) {
  data = new uint8[size = esize];
  memset(data, 0xff, size);
}

FRAM::FRAM(const stream& memory, uint32 esize) {
  data = new uint8[size = esize];
  memset(data, 0xff, size);
  memory.read(data, min(memory.size(), size));
}

void FRAM::power() {
  command = 0;
  writeEnable = false;
}

void FRAM::select(bool state) {
  if(state) return;
  
  // Reset and wait for a new command
  if(command == 0x02) writeEnable = false;
  command = 0;
}

uint8 FRAM::transfer(uint8 data) {
  if(command == 0) {
    command = data;
    address = addrCount = 0;
    
    if(command == 0x02) addrCount = 2;
    if(command == 0x03) addrCount = 2;
    if(command == 0x04) writeEnable = false;
    if(command == 0x06) writeEnable = true;
    return 0xff;
  }
  
  if(addrCount) { // Input address
    addrCount--;
    address = address<<8 | data;
    return 0xff;
  }
  
  if(command == 0x01) {
    return 0xff; // Write status
  }
  
  if(command == 0x05) { // Read status
    return writeEnable<<1; // | writing<<0;
  }
  
  if(command == 0x03) { // Read data
    if(address == size) address = 0;
    return this->data[address++];
  }
  
  if(command == 0x02) { // Write data
    if(address == size) address = 0;
    return this->data[address++] = data;
  }
  return 0xff;
}
