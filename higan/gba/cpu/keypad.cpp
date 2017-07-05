auto CPU::Keypad::run() -> void {
  //lookup table to convert button indexes to Emulator::Interface indexes
  static const uint lookup[] = {5, 4, 8, 9, 3, 2, 0, 1, 7, 6};

  if(!enable) return;

  bool test = condition;  //0 = OR, 1 = AND
  for(auto n : range(10)) {
    if(!flag[n]) continue;
    bool input = platform->inputPoll(0, 0, lookup[n]);
    if(condition == 0) test |= input;
    if(condition == 1) test &= input;
  }
  if(test) cpu.irq.flag |= CPU::Interrupt::Keypad;
}
