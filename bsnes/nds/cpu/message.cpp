
void CPUCore::popMsg() {
  msg.full = false;
  if(++msg.readOffset == msg.writeOffset)
    msg.empty = true;
}

void CPUCore::pushMsg(uint32 data) {
  msg.buffer[msg.writeOffset] = data;
  msg.empty = false;
  if(++msg.writeOffset == msg.readOffset)
      msg.full = true;
}

void CPUCore::clearMsg() {
  msg.readOffset = 0;
  msg.writeOffset = 0;
  msg.empty = true;
  msg.full = false;
  for(unsigned n = 0; n < 16; n++)
    msg.buffer[n] = 0;
}

uint32 CPUCore::regMsgControl() {
  return msg.enable<<15 | msg.error<<14
       | msg.irqOnRecv<<10 | other->msg.full<<9 | other->msg.empty<<8
       | msg.irqOnSent<<2  | this->msg.full<<1  | this->msg.empty<<0;
}

void CPUCore::regMsgControl(uint32 data, uint32 mask) {
  if(mask & 0x00ff) {
    bool previously = msg.irqOnSent;
    
    msg.irqOnSent = data>>2;
    if(data & 1<<3) clearMsg();
    
    // Edge triggered
    if(msg.empty && msg.irqOnSent && previously == false) {
      interrupt.flags |= irqMsgSent;
    }
  }
  if(mask & 0xff00) {
    bool previously = msg.irqOnRecv;
    
    msg.irqOnRecv = data>>10;
    msg.enable    = data>>15;
    if(data & 1<<14) msg.error = false;
    
    // Edge triggered
    if(!other->msg.empty && msg.irqOnRecv && previously == false)
      interrupt.flags |= irqMsgRecv;
  }
}

void CPUCore::regMsgSend(uint32 data) {
  if(msg.enable) {
    if(msg.full) {
      msg.error = true;
      return;
    }
    bool wasEmpty = msg.empty;
    pushMsg(data);
    
    if(wasEmpty && other->msg.irqOnRecv)
      other->interrupt.flags |= irqMsgRecv;
  }
}

uint32 CPUCore::regMsgRecv() {
  uint32 data = other->msg.buffer[other->msg.readOffset];
  
  if(other->msg.enable) {
    if(other->msg.empty) {
      msg.error = true;
      return data;
    }
    other->popMsg();
    
    if(other->msg.empty && other->msg.irqOnSent)
      other->interrupt.flags |= irqMsgSent;
  }
  return data;
}

uint32 CPUCore::regSync() {
  return sync.enable<<14 | this->sync.output<<8 | other->sync.output<<0;
}

void CPUCore::regSync(uint32 data, uint32 mask) {
  if(mask & 0xff00) {
    sync.enable = data>>14;
    sync.output = data>>8;
    if(data & 1<<13) {
      if(other->sync.enable)
        other->interrupt.flags |= irqSync;
    }
  }
}
