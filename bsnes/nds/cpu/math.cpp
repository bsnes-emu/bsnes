
void ARM946ES::regDivideControl(uint32 data, uint32 mask) {
  if(mask & 3) divMode = data;
  startDivide();
}

void ARM946ES::regNumerator(unsigned index, uint32 data, uint32 mask) {
  int64 update = data & mask;  update <<= 32*index;
  int64 umask  = mask;         umask  <<= 32*index;
  
  numerator ^= (numerator ^ update) & umask;
  startDivide();
}

void ARM946ES::regDenominator(unsigned index, uint32 data, uint32 mask) {
  int64 update = data & mask;  update <<= 32*index;
  int64 umask  = mask;         umask  <<= 32*index;
  
  denominator ^= (denominator ^ update) & umask;
  startDivide();
}



void ARM946ES::regSquareRootControl(uint32 data, uint32 mask) {
  if(mask & 1) rootMode = data;
  startSquareRoot();
}

void ARM946ES::regSquare(unsigned index, uint32 data, uint32 mask) {
  int64 update = data & mask;  update <<= 32*index;
  int64 umask  = mask;         umask  <<= 32*index;
  
  square ^= (square ^ update) & umask;
  startSquareRoot();
}



uint32 ARM946ES::regDivideControl() {
  return divMode<<0 | divByZero<<13 | divBusy<<15;
}

uint32 ARM946ES::regNumerator(unsigned index) {
  return numerator >> 32*index;
}

uint32 ARM946ES::regDenominator(unsigned index) {
  return denominator >> 32*index;
}

uint32 ARM946ES::regQuotient(unsigned index) {
  return quotient >> 32*index;
}

uint32 ARM946ES::regRemainder(unsigned index) {
  return remainder >> 32*index;
}



uint32 ARM946ES::regSquareRootControl() {
  return rootMode<<0 | rootBusy<<15;
}

uint32 ARM946ES::regSquare(unsigned index) {
  return square >> 32*index;
}

uint32 ARM946ES::regSquareRoot() {
  return squareRoot;
}



void ARM946ES::startDivide() {
  divByZero = false;
  
  int64 n = numerator, d = denominator;
  
  if(divMode == div32) { n &= 0xffffffffll; n -= 2*(n & 1ll<<31); }
  if(divMode != div64) { d &= 0xffffffffll; d -= 2*(d & 1ll<<31); }
  
  // Ugh, GBATEK does not say at all how remainder works with signed values.
  // Will have to fix this later..
  if(d == 0) {
    divByZero = true;
    quotient  = n >= 0? -1 : +1;
    remainder = n;
  }
  else if(n == (-1ll<<63) && d == -1ll) {
    quotient  = -1ll<<63;
  }
  else {
    quotient  = n / d;
    remainder = n % d;
  }
}



void ARM946ES::startSquareRoot() {
  uint64 x = square, r = 0;
  if(rootMode == sqrt32)
    x = uint32(x);
  
  for(uint64 bit = 1ull << 62; bit != 0; bit >>= 2) {
    r >>= 1;
    if(x >= 2*r + bit) {
      x -= 2*r + bit;
      r += bit;
    }
  }
  squareRoot = r;
}
