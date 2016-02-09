//Synchronous serial communications cable emulation

//Hardware:
//Teensy++ 2.0 USB
//AT90USB1286

//Connection Diagram:
//SNES GND <> Teensy GND
//SNES IOBit <> Teensy B0
//SNES Latch <> Teensy D2
//SNES Data1 <> Teensy D3
//SNES Clock <> 1Kohm Resistor <> Teensy D5
//Teensy D5 <> Teensy D7

USART::USART(bool port) : Controller(port) {
  string filename{interface->path(ID::SuperFamicom), "usart.so"};
  if(openAbsolute(filename)) {
    usart_init = sym("usart_init");
    usart_main = sym("usart_main");
    if(usart_init && usart_main) create(Controller::Enter, 10'000'000);
  }
}

USART::~USART() {
  if(open()) close();
}

auto USART::main() -> void {
  if(usart_init && usart_main) {
    usart_init(
      {&USART::quit, this},
      {&USART::usleep, this},
      {&USART::readable, this},
      {&USART::read, this},
      {&USART::writable, this},
      {&USART::write, this}
    );
    usart_main({});
  }
  while(true) step(10'000'000);
}

auto USART::quit() -> bool {
  step(1);
  return false;
}

auto USART::usleep(uint microseconds) -> void {
  step(10 * microseconds);
}

auto USART::readable() -> bool {
  step(1);
  return txbuffer.size();
}

//SNES -> USART
auto USART::read() -> uint8 {
  step(1);
  while(txbuffer.size() == 0) step(1);
  uint8 data = txbuffer[0];
  txbuffer.remove(0);
  return data;
}

auto USART::writable() -> bool {
  step(1);
  return true;
}

//USART -> SNES
auto USART::write(uint8 data) -> void {
  step(1);
  rxbuffer.append(data ^ 0xff);
}

//clock
auto USART::data() -> uint2 {
  //Joypad
  if(iobit()) {
    if(counter >= 16) return 1;
    uint2 result = 0;
    if(counter < 12) result = interface->inputPoll(port, (uint)Device::ID::Gamepad, counter);
    if(latched == 0) counter++;
    return result;
  }

  //SNES -> USART
  if(txlength == 0) {
    if(latched == 0) txlength++;
  } else if(txlength <= 8) {
    txdata = (latched << 7) | (txdata >> 1);
    txlength++;
  } else {
    if(latched == 1) txbuffer.append(txdata);
    txlength = 0;
  }

  //USART -> SNES
  if(rxlength == 0 && rxbuffer.size()) {
    data1 = 1;
    rxdata = rxbuffer[0];
    rxbuffer.remove(0);
    rxlength++;
  } else if(rxlength <= 8) {
    data1 = rxdata & 1;
    rxdata >>= 1;
    rxlength++;
  } else {
    data1 = 0;
    rxlength = 0;
  }

  return (data2 << 1) | (data1 << 0);
}

//latch
auto USART::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;
}
