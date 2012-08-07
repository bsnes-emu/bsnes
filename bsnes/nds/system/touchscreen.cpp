
Touchscreen::~Touchscreen() { }

void Touchscreen::power() {
  input = 0;
  bitDepth = 0;
  refMode = 0;
  powerMode = 0;
  adc = 0;
  last = 0;
}

void Touchscreen::select(bool state) {
  if(state) return;
  
  adc = 0;
}

bool Touchscreen::penDown() {
  if(powerMode == 3) return true;  // differs between NDS Lite and original?
  
  signed x = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::X);
  signed y = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::Y);
  signed p = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::Pressure);
  signed d = 0;//interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::PressureD);
  
  if(y < 0) y = -0x8000;
    
  if(x == -0x8000 || y == -0x8000) {
    return 0;  // no touch input
  }
  if(p == -0x8000)       // no analog pressure
    p = d? 0 : -0x7fff;  //   use digital
  
  return p > -0x7ff0;
}

uint8 Touchscreen::transfer(uint8 data) {
  if(data & 0x80) {
    // Start a new measurement
    input     = data>>4;
    bitDepth  = data>>3;
    refMode   = data>>2;
    powerMode = data>>0;
    
    signed NONE = -0x8000;
    signed x = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::X);
    signed y = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::Y);
    signed p = interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::Pressure);
    signed d = 0;//interface->inputPoll(ID::Port::Touchpad, 0, ID::Touchpad::PressureD);
    
    if(p==NONE)            p = !d? -0x7fff : -0x3fff; // no analog? use digital pen
    if(x==NONE || y==NONE) p = NONE;                  // check if pen offscreen
    if(y < 0)              p = NONE;                  // restrict to bottom screen
    
    x += 0x7fff; y *= 2;
    p += 0x7fff;
    
    if(p > 0) {
      signed z1 = 0x000, z2 = 0xfff, r = 0xffff - p;
      if(x > 0) {   // bleh, div-by-zero, just use medium pressure for now
        z1 = 0xc00; //0xffffff / (0x1000 + 0x1000/x + 0x1000*(r - y)/x);
        z2 = 0x400; //z1*(0x1000 + 0x1000*r/x) / 0x1000;
      }
      x /= 16;
      y /= 16;
      
      // Without any pressure, there's no connection, and the input floats.
      // The measurements seem to return previous values to some extent.
      if(input == xpos)      last = x;      // y+ (along x-/x+ axis)
      if(input == ypos)      last = y;      // x+ (along y-/y+ axis)
      if(input == pressure0) last = z1;     // x+ (along x-/y+ diagonal?)
      if(input == pressure1) last = z2;     // y- (along x-/y+ diagonal?)
    }
    
    signed vref = 0x34cc;                     // reference = 3.3V
    signed room = (273 + 25)*0x1000;          // 25*C in kelvin
    signed k = room;
    signed t1 = 0x1000*600 - (k-room)*21/10;  // t1 = 600mV @ room - 2.1mV/*K + error
    signed t2 = t1 + k * 0x1000/0x292b;       // t2 = t1 + k/2.573
    
    if(input == temp0) last = t1 * 0xfff/vref;  // temperature
    if(input == temp1) last = t2 * 0xfff/vref;  // temp (w/bias)
    if(input == vbatt) last = 0x000;            // grounded
    if(input == aux) {
      // This was split into two inputs - reference is always "single" mode
      if(refMode==single) last = 0x800;  // microphone
      if(refMode==diff)   last = 0xfff;  // Vcc (NDS Lite)
    }
    
    adc |= last;
    
    // 8-bit isn't any faster since you have to read two bytes over the serial
    // interface either way. Bumping the clock to 4MHz may not provide enough
    // time for the ADC to settle, producing less accurate results.
    if(bitDepth == read8)
      adc &= ~0xf;
  }
  // Response is 1 bit delay + 8 or 12 bits data.
  // This causes some misalignment:
  //   --------  0hhhhhhhlllll  - after command
  //   0hhhhhhh  lllll000-----  - after first read
  //   lllll000 [0hhhhhhhlllll] - after second read [+ command]
  //  [0hhhhhhh  lllll000-----] - ...
  //  [lllll000  0hhhhhhhlllll] -
  //   response   next data
  //
  // It's perfectly legit (and common) to send a new command while reading the
  // low byte. By doing this, you receive a stream of readings with no gaps or
  // command overhead in between.
  return adc <<= 8, adc >> 21;
}
