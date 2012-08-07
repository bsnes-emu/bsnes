
uint32 CPUCore::regTimer(unsigned no) {
  auto &timer = this->timer[no];
  
  if(timer.cascade == false)
    updateTimer(no);
  
  return timer.count<<0 | timer.divider<<16
       | timer.cascade<<18 | timer.irq<<22 | timer.enable<<23;
}

void CPUCore::regTimer(unsigned no, uint32 data, uint32 mask) {
  auto &timer = this->timer[no];
  
  if(timer.cascade == false)
    updateTimer(no);
  
  event.queue.remove(timer.event);
  
  if(mask & 0x00ffff) {
    timer.reload ^= (timer.reload ^ data) & mask;
  }
  if(mask & 0xff0000) {
    if(!timer.enable && (data & 1<<23)) {
      timer.count = timer.reload;
    }
    timer.divider = data>>16;
    timer.cascade = data>>18;
    timer.irq     = data>>22;
    timer.enable  = data>>23;
  }
  
  if(timer.cascade == false)
    updateTimer(no);
}

void CPUCore::updateTimer(unsigned no) {
  auto &timer = this->timer[no];
  
  if(timer.enable) {
    uint16 old = timer.count;
    uint32 ticks = 1;
    
    unsigned dbits[] = { 1, 7, 9, 11 };  // 33MHz, 512KHz, 128KHz, 32KHz
    unsigned s       = dbits[timer.divider];
    uint32   divider = 1 << s;
    
    if(timer.cascade) {
      timer.count++;
    } else {
      ticks        = (event.queue.time >> s) - (timer.lastUpdate >> s);
      timer.count += ticks;
    }
    
    if(ticks && timer.count <= old) {
      if(timer.irq) interrupt.flags |= irqTimer << no;
      
      timer.count = timer.reload;
      
      if(no < 3 && this->timer[no+1].cascade)
        updateTimer(no+1);
    }
    
    if(timer.cascade == false) {
      uint32 ticksLeft    = 0x10000 - timer.count;
      uint32 nextTick     = divider;// - (event.queue.time & divider-1);
      uint32 nextOverflow = nextTick + divider * (ticksLeft-1);
      
      // The above commented part breaks maxmod's interpolated audio
      // - perhaps the internal counters have expanded?
      if(event.queue.time & 1)
        nextTick -= 1;  // align to 33MHz clock however
      
      event.queue.add(nextOverflow, timer.event);
    }
  }
  timer.lastUpdate = event.queue.time;
}
