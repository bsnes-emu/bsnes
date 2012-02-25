#ifdef CONTROLLER_CPP

//Synchronous serial communications cable emulation

//Hardware:
//Teensy++ 2.0 USB
//AT90USB1286

//Connection Diagram:
//[SNES]  [Teensy]
// +5v     ---
// Clock   D5
// Latch   D2
// Data1   D3
// Data2   ---
// IOBit   ---
// GND     GND

static uint8 usart_read();
static void usart_write(uint8 data);

//USART -> SNES
uint2 USART::data() {
  if(rxlength == 0) {
    data1 = 0;
    rxdata = usart_read();
  } else if(rxlength <= 8) {
    data1 = rxdata & 1;
    rxdata >>= 1;
  } else {
    data1 = 1;
    rxlength = 0;
  }

  return (data2 << 1) | (data1 << 0);
}

//SNES -> USART
void USART::latch(bool data) {
  if(txlength == 0 && latched == 1 && data == 0) {
    txlength++;
  } else if(txlength <= 8) {
    txdata = (data << 7) | (txdata >> 1);
    txlength++;
  } else {
    if(data == 1) usart_write(txdata);
    txlength = 0;
  }

  latched = data;
}

USART::USART(bool port) : Controller(port) {
  latched = 0;
  data1 = 0;
  data2 = 0;

  rxlength = 0;
  rxdata = 0;

  txlength = 0;
  txdata = 0;
}

USART::~USART() {
}

static uint8 usart_read() {
  return 0xff;
}

static void usart_write(uint8 data) {
}

#endif
