#ifndef NALL_IMAGE_LOAD_HPP
#define NALL_IMAGE_LOAD_HPP

namespace nall {

bool image::loadBMP(const string& filename) {
  uint32_t* outputData;
  unsigned outputWidth, outputHeight;
  if(bmp::read(filename, outputData, outputWidth, outputHeight) == false) return false;

  allocate(outputWidth, outputHeight);
  const uint32_t* sp = outputData;
  uint8_t* dp = data;

  for(unsigned y = 0; y < outputHeight; y++) {
    for(unsigned x = 0; x < outputWidth; x++) {
      uint32_t color = *sp++;
      uint64_t a = normalize((uint8_t)(color >> 24), 8, alpha.depth);
      uint64_t r = normalize((uint8_t)(color >> 16), 8, red.depth);
      uint64_t g = normalize((uint8_t)(color >>  8), 8, green.depth);
      uint64_t b = normalize((uint8_t)(color >>  0), 8, blue.depth);
      write(dp, (a << alpha.shift) | (r << red.shift) | (g << green.shift) | (b << blue.shift));
      dp += stride;
    }
  }

  delete[] outputData;
  return true;
}

bool image::loadPNG(const string& filename) {
  if(!file::exists(filename)) return false;
  auto buffer = file::read(filename);
  return loadPNG(buffer.data(), buffer.size());
}

bool image::loadPNG(const uint8_t* pngData, unsigned pngSize) {
  png source;
  if(source.decode(pngData, pngSize) == false) return false;

  allocate(source.info.width, source.info.height);
  const uint8_t* sp = source.data;
  uint8_t* dp = data;

  auto decode = [&]() -> uint64_t {
    uint64_t p, r, g, b, a;

    switch(source.info.colorType) {
    case 0:  //L
      r = g = b = source.readbits(sp);
      a = (1 << source.info.bitDepth) - 1;
      break;
    case 2:  //R,G,B
      r = source.readbits(sp);
      g = source.readbits(sp);
      b = source.readbits(sp);
      a = (1 << source.info.bitDepth) - 1;
      break;
    case 3:  //P
      p = source.readbits(sp);
      r = source.info.palette[p][0];
      g = source.info.palette[p][1];
      b = source.info.palette[p][2];
      a = (1 << source.info.bitDepth) - 1;
      break;
    case 4:  //L,A
      r = g = b = source.readbits(sp);
      a = source.readbits(sp);
      break;
    case 6:  //R,G,B,A
      r = source.readbits(sp);
      g = source.readbits(sp);
      b = source.readbits(sp);
      a = source.readbits(sp);
      break;
    }

    a = normalize(a, source.info.bitDepth, alpha.depth);
    r = normalize(r, source.info.bitDepth, red.depth);
    g = normalize(g, source.info.bitDepth, green.depth);
    b = normalize(b, source.info.bitDepth, blue.depth);

    return (a << alpha.shift) | (r << red.shift) | (g << green.shift) | (b << blue.shift);
  };

  for(unsigned y = 0; y < height; y++) {
    for(unsigned x = 0; x < width; x++) {
      write(dp, decode());
      dp += stride;
    }
  }

  return true;
}

}

#endif
