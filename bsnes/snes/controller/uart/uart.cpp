#ifdef CONTROLLER_CPP

//Asynchronous serial communications cable emulation:
//The SNES controller ports can be used for bi-directional serial communication
//when wired to a specialized controller. This class implements said controller,
//for the primary purpose of testing code outside of real hardware.

//The basic idea is to wire the SNES controller pins to a UART, such as
//the MAX232N; or a serial->USB cable, such as the FTDI TTL-232R-5V.

//Connection Diagram:
//[SNES]    [UART]    [Purpose]
// Latch     RXD       Data transfer
// Data1     TXD       Data transfer
// Data2     RTS       Flow control (optional)
// IOBit     CTS       Flow control (optional)
// GND       GND       Circuit completion

//The SNES software program will have to use specially timed code to send and
//receive data at a specific baud-rate; whereas the PC handles timing via the
//UART.

//The emulator implementation is designed so that the same PC-side program can
//be used both under emulation and on real hardware. It does this by linking to
//a dynamic library for timing, read and write operations. This library is
//responsible for setting both the baud-rate and flow control setting. The
//SNES-side software program must know about and respect the library setting.

static void snesserial_tick(unsigned clocks);
static uint8 snesserial_read();
static void snesserial_write(uint8 data);

void UART::enter() {
  if(enable == false) {
    //fallback, in case library was not found
    interface->message("UART library not found");
    while(true) step(1);
  }
  step(256 * 8);  //simulate warm-up delay
  if(flowcontrol()) data2 = 1;
  main(snesserial_tick, snesserial_read, snesserial_write);  //stubs for Serial::step, Serial::read, Serial::write
  while(true) step(1);  //fallback, in case snesserial_main() returns (it should never do so)
}

uint8 UART::read() {
  while(latched == 0) step(1);
  while(latched == 1) step(1);
  step(4);

  uint8 data = 0;
  for(unsigned i = 0; i < 8; i++) {
    step(8);
    data = (latched << 7) | (data >> 1);
  }

  return data;
}

void UART::write(uint8 data) {
  if(flowcontrol()) while(iobit()) step(1);
  step(8);

  data1 = 1;
  step(8);

  for(unsigned i = 0; i < 8; i++) {
    data1 = (data & 1) ^ 1;
    data >>= 1;
    step(8);
  }

  data1 = 0;
  step(8);
}

uint2 UART::data() {
  return (data2 << 1) | (data1 << 0);
}

void UART::latch(bool data) {
  latched = data;
}

UART::UART(bool port) : Controller(port) {
  enable = false;
  string filename = interface->path(Cartridge::Slot::Base, "uart.so");
  if(open_absolute(filename)) {
    baudrate = sym("snesserial_baudrate");
    flowcontrol = sym("snesserial_flowcontrol");
    main = sym("snesserial_main");
    if(baudrate && flowcontrol && main) enable = true;
  }
  create(Controller::Enter, enable ? baudrate() * 8 : 1);

  latched = false;
  data1 = 0;
  data2 = 0;
}

UART::~UART() {
  if(opened()) close();
}

//stubs needed to call into class objects from global function pointers

static void snesserial_tick(unsigned clocks) {
  if(co_active() == input.port1->thread) {
    if(dynamic_cast<UART*>(input.port1)) {
      return ((UART*)input.port1)->step(clocks);
    }
  }

  if(co_active() == input.port2->thread) {
    if(dynamic_cast<UART*>(input.port2)) {
      return ((UART*)input.port2)->step(clocks);
    }
  }
}

static uint8 snesserial_read() {
  if(co_active() == input.port1->thread) {
    if(dynamic_cast<UART*>(input.port1)) {
      return ((UART*)input.port1)->read();
    }
  }

  if(co_active() == input.port2->thread) {
    if(dynamic_cast<UART*>(input.port2)) {
      return ((UART*)input.port2)->read();
    }
  }
}

static void snesserial_write(uint8 data) {
  if(co_active() == input.port1->thread) {
    if(dynamic_cast<UART*>(input.port1)) {
      return ((UART*)input.port1)->write(data);
    }
  }

  if(co_active() == input.port2->thread) {
    if(dynamic_cast<UART*>(input.port2)) {
      return ((UART*)input.port2)->write(data);
    }
  }
}

#endif
