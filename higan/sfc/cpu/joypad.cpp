//called every 256 clocks; see CPU::addClocks()
auto CPU::stepAutoJoypadPoll() -> void {
  if(vcounter() >= (!ppu.overscan() ? 225 : 240)) {
    //cache enable state at first iteration
    if(status.auto_joypad_counter == 0) status.auto_joypad_latch = status.auto_joypad_poll;
    status.auto_joypad_active = status.auto_joypad_counter <= 15;

    if(status.auto_joypad_active && status.auto_joypad_latch) {
      if(status.auto_joypad_counter == 0) {
        device.controllerPort1->latch(1);
        device.controllerPort2->latch(1);
        device.controllerPort1->latch(0);
        device.controllerPort2->latch(0);
      }

      uint2 port0 = device.controllerPort1->data();
      uint2 port1 = device.controllerPort2->data();

      status.joy1 = status.joy1 << 1 | port0.bit(0);
      status.joy2 = status.joy2 << 1 | port1.bit(0);
      status.joy3 = status.joy3 << 1 | port0.bit(1);
      status.joy4 = status.joy4 << 1 | port1.bit(1);
    }

    status.auto_joypad_counter++;
  }
}
