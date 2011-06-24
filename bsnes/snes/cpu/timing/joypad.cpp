#ifdef CPU_CPP

//called every 256 clocks; see CPU::add_clocks()
void CPU::step_auto_joypad_poll() {
  if(vcounter() >= (ppu.overscan() == false ? 225 : 240)) {
    status.auto_joypad_active = status.auto_joypad_counter <= 15;

    if(status.auto_joypad_active && status.auto_joypad_poll) {
      if(status.auto_joypad_counter == 0) {
      //input.poll();
        controllers.port1->latch(1);
        controllers.port2->latch(1);
        controllers.port1->latch(0);
        controllers.port2->latch(0);
      }

    //uint8 port0 = input.port_read(0);
    //uint8 port1 = input.port_read(1);
      uint2 port0 = controllers.port1->data();
      uint2 port1 = controllers.port2->data();

      status.joy1 = (status.joy1 << 1) | (bool)(port0 & 1);
      status.joy2 = (status.joy2 << 1) | (bool)(port1 & 1);
      status.joy3 = (status.joy3 << 1) | (bool)(port0 & 2);
      status.joy4 = (status.joy4 << 1) | (bool)(port1 & 2);
    }

    status.auto_joypad_counter++;
  }
}

#endif
