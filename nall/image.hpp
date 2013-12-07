#ifndef NALL_IMAGE_HPP
#define NALL_IMAGE_HPP

#include <nall/bmp.hpp>
#include <nall/filemap.hpp>
#include <nall/interpolation.hpp>
#include <nall/png.hpp>
#include <nall/stdint.hpp>
#include <algorithm>

namespace nall {

struct image {
  uint8_t* data   = nullptr;
  unsigned width  = 0;
  unsigned height = 0;
  unsigned pitch  = 0;
  unsigned size   = 0;

  bool endian     =  0;  //0 = lsb, 1 = msb
  unsigned depth  = 32;
  unsigned stride =  4;

  struct Channel {
    uint64_t mask;
    unsigned depth;
    unsigned shift;

    inline bool operator==(const Channel& source) {
      return mask == source.mask && depth == source.depth && shift == source.shift;
    }

    inline bool operator!=(const Channel& source) {
      return !operator==(source);
    }
  };

  enum class blend : unsigned {
    add,
    sourceAlpha,  //color = sourceColor * sourceAlpha + targetColor * (1 - sourceAlpha)
    sourceColor,  //color = sourceColor
    targetAlpha,  //color = targetColor * targetAlpha + sourceColor * (1 - targetAlpha)
    targetColor,  //color = targetColor
  };

  Channel alpha = {255u << 24, 8u, 24u};
  Channel red   = {255u << 16, 8u, 16u};
  Channel green = {255u <<  8, 8u,  8u};
  Channel blue  = {255u <<  0, 8u,  0u};

  typedef double (*interpolation)(double, double, double, double, double);
  static inline unsigned bitDepth(uint64_t color);
  static inline unsigned bitShift(uint64_t color);
  static inline uint64_t normalize(uint64_t color, unsigned sourceDepth, unsigned targetDepth);

  inline bool operator==(const image& source);
  inline bool operator!=(const image& source);

  inline image& operator=(const image& source);
  inline image& operator=(image&& source);
  inline image(const image& source);
  inline image(image&& source);
  inline image(bool endian, unsigned depth, uint64_t alphaMask, uint64_t redMask, uint64_t greenMask, uint64_t blueMask);
  inline image(const string& filename);
  inline image(const uint8_t* data, unsigned size);
  inline image();
  inline ~image();

  inline uint64_t read(const uint8_t* data) const;
  inline void write(uint8_t* data, uint64_t value) const;

  inline void free();
  inline bool empty() const;
  inline void allocate(unsigned width, unsigned height);
  inline bool crop(unsigned x, unsigned y, unsigned width, unsigned height);
  inline void impose(blend mode, unsigned targetX, unsigned targetY, image source, unsigned x, unsigned y, unsigned width, unsigned height);
  inline void fill(uint64_t color = 0);
  inline void gradient(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
  inline void horizontalGradient(uint64_t a, uint64_t b);
  inline void verticalGradient(uint64_t a, uint64_t b);
  inline bool load(const string& filename);
//inline bool loadBMP(const uint8_t* data, unsigned size);
  inline bool loadPNG(const uint8_t* data, unsigned size);
  inline void scale(unsigned width, unsigned height, bool linear = true);
  inline void transform(bool endian, unsigned depth, uint64_t alphaMask, uint64_t redMask, uint64_t greenMask, uint64_t blueMask);
  inline void alphaBlend(uint64_t alphaColor);

protected:
  inline uint8_t* allocate(unsigned width, unsigned height, unsigned stride);
  alwaysinline uint64_t interpolate1D(int64_t a, int64_t b, uint32_t x);
  alwaysinline uint64_t interpolate2D(int64_t a, int64_t b, int64_t c, int64_t d, uint32_t x, uint32_t y);
  inline void scaleLinearWidth(unsigned width);
  inline void scaleLinearHeight(unsigned height);
  inline void scaleLinear(unsigned width, unsigned height);
  inline void scaleNearest(unsigned width, unsigned height);
  inline bool loadBMP(const string& filename);
  inline bool loadPNG(const string& filename);
};

//static

unsigned image::bitDepth(uint64_t color) {
  unsigned depth = 0;
  if(color) while((color & 1) == 0) color >>= 1;
  while((color & 1) == 1) { color >>= 1; depth++; }
  return depth;
}

unsigned image::bitShift(uint64_t color) {
  unsigned shift = 0;
  if(color) while((color & 1) == 0) { color >>= 1; shift++; }
  return shift;
}

uint64_t image::normalize(uint64_t color, unsigned sourceDepth, unsigned targetDepth) {
  if(sourceDepth == 0 || targetDepth == 0) return 0;
  while(sourceDepth < targetDepth) {
    color = (color << sourceDepth) | color;
    sourceDepth += sourceDepth;
  }
  if(targetDepth < sourceDepth) color >>= (sourceDepth - targetDepth);
  return color;
}

//public

bool image::operator==(const image& source) {
  if(width != source.width) return false;
  if(height != source.height) return false;
  if(pitch != source.pitch) return false;

  if(endian != source.endian) return false;
  if(stride != source.stride) return false;

  if(alpha != source.alpha) return false;
  if(red != source.red) return false;
  if(green != source.green) return false;
  if(blue != source.blue) return false;

  return memcmp(data, source.data, width * height * stride) == 0;
}

bool image::operator!=(const image& source) {
  return !operator==(source);
}

image& image::operator=(const image& source) {
  free();

  width = source.width;
  height = source.height;
  pitch = source.pitch;
  size = source.size;

  endian = source.endian;
  stride = source.stride;

  alpha = source.alpha;
  red = source.red;
  green = source.green;
  blue = source.blue;

  data = allocate(width, height, stride);
  memcpy(data, source.data, source.size);
  return *this;
}

image& image::operator=(image&& source) {
  free();

  width = source.width;
  height = source.height;
  pitch = source.pitch;
  size = source.size;

  endian = source.endian;
  stride = source.stride;

  alpha = source.alpha;
  red = source.red;
  green = source.green;
  blue = source.blue;

  data = source.data;
  source.data = nullptr;
  return *this;
}

image::image(const image& source) {
  operator=(source);
}

image::image(image&& source) {
  operator=(std::forward<image>(source));
}

image::image(bool endian, unsigned depth, uint64_t alphaMask, uint64_t redMask, uint64_t greenMask, uint64_t blueMask) {
  this->endian = endian;
  this->depth  = depth;
  this->stride = (depth / 8) + ((depth & 7) > 0);

  alpha = {alphaMask, bitDepth(alphaMask), bitShift(alphaMask)};
  red   = {redMask,   bitDepth(redMask),   bitShift(redMask  )};
  green = {greenMask, bitDepth(greenMask), bitShift(greenMask)};
  blue  = {blueMask,  bitDepth(blueMask),  bitShift(blueMask )};
}

image::image(const string& filename) {
  load(filename);
}

image::image(const uint8_t* data, unsigned size) {
  loadPNG(data, size);
}

image::image() {
}

image::~image() {
  free();
}

uint64_t image::read(const uint8_t* data) const {
  uint64_t result = 0;
  if(endian == 0) {
    for(signed n = stride - 1; n >= 0; n--) result = (result << 8) | data[n];
  } else {
    for(signed n = 0; n < stride; n++) result = (result << 8) | data[n];
  }
  return result;
}

void image::write(uint8_t* data, uint64_t value) const {
  if(endian == 0) {
    for(signed n = 0; n < stride; n++) { data[n] = value; value >>= 8; }
  } else {
    for(signed n = stride - 1; n >= 0; n--) { data[n] = value; value >>= 8; }
  }
}

void image::free() {
  if(data) delete[] data;
  data = nullptr;
}

bool image::empty() const {
  if(data == nullptr) return true;
  if(width == 0 || height == 0) return true;
  return false;
}

void image::allocate(unsigned width, unsigned height) {
  if(data != nullptr && this->width == width && this->height == height) return;
  free();
  data = allocate(width, height, stride);
  pitch = width * stride;
  size = height * pitch;
  this->width = width;
  this->height = height;
}

void image::fill(uint64_t color) {
  uint8_t* dp = data;
  for(unsigned n = 0; n < width * height; n++) {
    write(dp, color);
    dp += stride;
  }
}

void image::gradient(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  //create gradient by scaling 2x2 image using linear interpolation
  //replace data with gradient data to prevent extra copy
  delete[] data;
  nall::image gradient;
  gradient.endian = endian, gradient.depth = depth, gradient.stride = stride;
  gradient.alpha = alpha, gradient.red = red, gradient.green = green, gradient.blue = blue;
  gradient.allocate(2, 2);
  uint8_t* dp = gradient.data;
  gradient.write(dp, a); dp += stride;
  gradient.write(dp, b); dp += stride;
  gradient.write(dp, c); dp += stride;
  gradient.write(dp, d); dp += stride;
  gradient.scale(width, height);
  data = gradient.data;
  gradient.data = nullptr;
}

void image::horizontalGradient(uint64_t a, uint64_t b) {
  gradient(a, b, a, b);
}

void image::verticalGradient(uint64_t a, uint64_t b) {
  gradient(a, a, b, b);
}

bool image::load(const string& filename) {
  if(loadBMP(filename) == true) return true;
  if(loadPNG(filename) == true) return true;
  return false;
}

bool image::crop(unsigned outputX, unsigned outputY, unsigned outputWidth, unsigned outputHeight) {
  if(outputX + outputWidth > width) return false;
  if(outputY + outputHeight > height) return false;

  uint8_t* outputData = allocate(outputWidth, outputHeight, stride);
  unsigned outputPitch = outputWidth * stride;

  #pragma omp parallel for
  for(unsigned y = 0; y < outputHeight; y++) {
    const uint8_t* sp = data + pitch * (outputY + y) + stride * outputX;
    uint8_t* dp = outputData + outputPitch * y;
    for(unsigned x = 0; x < outputWidth; x++) {
      write(dp, read(sp));
      sp += stride;
      dp += stride;
    }
  }

  delete[] data;
  data = outputData;
  width = outputWidth;
  height = outputHeight;
  pitch = outputPitch;
  size = width * pitch;
  return true;
}

void image::impose(blend mode, unsigned targetX, unsigned targetY, image source, unsigned sourceX, unsigned sourceY, unsigned sourceWidth, unsigned sourceHeight) {
  source.transform(endian, depth, alpha.mask, red.mask, green.mask, blue.mask);

  for(unsigned y = 0; y < sourceHeight; y++) {
    const uint8_t* sp = source.data + source.pitch * (sourceY + y) + source.stride * sourceX;
    uint8_t* dp = data + pitch * (targetY + y) + stride * targetX;
    for(unsigned x = 0; x < sourceWidth; x++) {
      uint64_t sourceColor = source.read(sp);
      uint64_t targetColor = read(dp);

      int64_t sa = (sourceColor & alpha.mask) >> alpha.shift;
      int64_t sr = (sourceColor & red.mask  ) >> red.shift;
      int64_t sg = (sourceColor & green.mask) >> green.shift;
      int64_t sb = (sourceColor & blue.mask ) >> blue.shift;

      int64_t da = (targetColor & alpha.mask) >> alpha.shift;
      int64_t dr = (targetColor & red.mask  ) >> red.shift;
      int64_t dg = (targetColor & green.mask) >> green.shift;
      int64_t db = (targetColor & blue.mask ) >> blue.shift;

      uint64_t a, r, g, b;

      switch(mode) {
      case blend::add:
        a = max(sa, da);
        r = min(red.mask   >> red.shift,   ((sr * sa) >> alpha.depth) + ((dr * da) >> alpha.depth));
        g = min(green.mask >> green.shift, ((sg * sa) >> alpha.depth) + ((dg * da) >> alpha.depth));
        b = min(blue.mask  >> blue.shift,  ((sb * sa) >> alpha.depth) + ((db * da) >> alpha.depth));
        break;

      case blend::sourceAlpha:
        a = max(sa, da);
        r = dr + (((sr - dr) * sa) >> alpha.depth);
        g = dg + (((sg - dg) * sa) >> alpha.depth);
        b = db + (((sb - db) * sa) >> alpha.depth);
        break;

      case blend::sourceColor:
        a = sa;
        r = sr;
        g = sg;
        b = sb;
        break;

      case blend::targetAlpha:
        a = max(sa, da);
        r = sr + (((dr - sr) * da) >> alpha.depth);
        g = sg + (((dg - sg) * da) >> alpha.depth);
        b = sb + (((db - sb) * da) >> alpha.depth);
        break;

      case blend::targetColor:
        a = da;
        r = dr;
        g = dg;
        b = db;
        break;
      }

      write(dp, (a << alpha.shift) | (r << red.shift) | (g << green.shift) | (b << blue.shift));
      sp += source.stride;
      dp += stride;
    }
  }
}

void image::scale(unsigned outputWidth, unsigned outputHeight, bool linear) {
  if(width == outputWidth && height == outputHeight) return;  //no scaling necessary
  if(linear == false) return scaleNearest(outputWidth, outputHeight);

  if(width  == outputWidth ) return scaleLinearHeight(outputHeight);
  if(height == outputHeight) return scaleLinearWidth(outputWidth);

  //find fastest scaling method, based on number of interpolation operations required
  //magnification usually benefits from two-pass linear interpolation
  //minification usually benefits from one-pass bilinear interpolation
  unsigned d1wh = ((width  * outputWidth ) + (outputWidth * outputHeight)) * 1;
  unsigned d1hw = ((height * outputHeight) + (outputWidth * outputHeight)) * 1;
  unsigned d2wh = (outputWidth * outputHeight) * 3;

  if(d1wh <= d1hw && d1wh <= d2wh) return scaleLinearWidth(outputWidth), scaleLinearHeight(outputHeight);
  if(d1hw <= d2wh) return scaleLinearHeight(outputHeight), scaleLinearWidth(outputWidth);
  return scaleLinear(outputWidth, outputHeight);
}

void image::transform(bool outputEndian, unsigned outputDepth, uint64_t outputAlphaMask, uint64_t outputRedMask, uint64_t outputGreenMask, uint64_t outputBlueMask) {
  if(endian == outputEndian && depth == outputDepth && alpha.mask == outputAlphaMask && red.mask == outputRedMask && green.mask == outputGreenMask && blue.mask == outputBlueMask) return;

  image output(outputEndian, outputDepth, outputAlphaMask, outputRedMask, outputGreenMask, outputBlueMask);
  output.allocate(width, height);

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    const uint8_t* sp = data + pitch * y;
    uint8_t* dp = output.data + output.pitch * y;
    for(unsigned x = 0; x < width; x++) {
      uint64_t color = read(sp);
      sp += stride;

      uint64_t a = (color & alpha.mask) >> alpha.shift;
      uint64_t r = (color & red.mask) >> red.shift;
      uint64_t g = (color & green.mask) >> green.shift;
      uint64_t b = (color & blue.mask) >> blue.shift;

      a = normalize(a, alpha.depth, output.alpha.depth);
      r = normalize(r, red.depth, output.red.depth);
      g = normalize(g, green.depth, output.green.depth);
      b = normalize(b, blue.depth, output.blue.depth);

      output.write(dp, (a << output.alpha.shift) | (r << output.red.shift) | (g << output.green.shift) | (b << output.blue.shift));
      dp += output.stride;
    }
  }

  operator=(std::move(output));
}

void image::alphaBlend(uint64_t alphaColor) {
  uint64_t alphaR = (alphaColor & red.mask  ) >> red.shift;
  uint64_t alphaG = (alphaColor & green.mask) >> green.shift;
  uint64_t alphaB = (alphaColor & blue.mask ) >> blue.shift;

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    for(unsigned x = 0; x < width; x++) {
      uint64_t color = read(dp);

      uint64_t colorA = (color & alpha.mask) >> alpha.shift;
      uint64_t colorR = (color & red.mask  ) >> red.shift;
      uint64_t colorG = (color & green.mask) >> green.shift;
      uint64_t colorB = (color & blue.mask ) >> blue.shift;
      double alphaScale = (double)colorA / (double)((1 << alpha.depth) - 1);

      colorA = (1 << alpha.depth) - 1;
      colorR = (colorR * alphaScale) + (alphaR * (1.0 - alphaScale));
      colorG = (colorG * alphaScale) + (alphaG * (1.0 - alphaScale));
      colorB = (colorB * alphaScale) + (alphaB * (1.0 - alphaScale));

      write(dp, (colorA << alpha.shift) | (colorR << red.shift) | (colorG << green.shift) | (colorB << blue.shift));
      dp += stride;
    }
  }
}

//protected

uint8_t* image::allocate(unsigned width, unsigned height, unsigned stride) {
  //allocate 1x1 larger than requested; so that linear interpolation does not require bounds-checking
  unsigned size = width * height * stride;
  unsigned padding = width * stride + stride;
  uint8_t* data = new uint8_t[size + padding];
  memset(data + size, 0x00, padding);
  return data;
}

//fixed-point reduction of: a * (1 - x) + b * x
uint64_t image::interpolate1D(int64_t a, int64_t b, uint32_t x) {
  return a + (((b - a) * x) >> 32);  //a + (b - a) * x
}

//fixed-point reduction of: a * (1 - x) * (1 - y) + b * x * (1 - y) + c * (1 - x) * y + d * x * y
uint64_t image::interpolate2D(int64_t a, int64_t b, int64_t c, int64_t d, uint32_t x, uint32_t y) {
  a = a + (((b - a) * x) >> 32);     //a + (b - a) * x
  c = c + (((d - c) * x) >> 32);     //c + (d - c) * x
  return a + (((c - a) * y) >> 32);  //a + (c - a) * y
}

void image::scaleLinearWidth(unsigned outputWidth) {
  uint8_t* outputData = allocate(outputWidth, height, stride);
  unsigned outputPitch = outputWidth * stride;
  uint64_t xstride = ((uint64_t)(width - 1) << 32) / max(1u, outputWidth - 1);

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    uint64_t xfraction = 0;

    const uint8_t* sp = data + pitch * y;
    uint8_t* dp = outputData + outputPitch * y;

    uint64_t a = read(sp);
    uint64_t b = read(sp + stride);
    sp += stride;

    unsigned x = 0;
    while(true) {
      while(xfraction < 0x100000000 && x++ < outputWidth) {
        uint64_t A = interpolate1D((a & alpha.mask) >> alpha.shift, (b & alpha.mask) >> alpha.shift, xfraction);
        uint64_t R = interpolate1D((a & red.mask  ) >> red.shift  , (b & red.mask  ) >> red.shift,   xfraction);
        uint64_t G = interpolate1D((a & green.mask) >> green.shift, (b & green.mask) >> green.shift, xfraction);
        uint64_t B = interpolate1D((a & blue.mask ) >> blue.shift , (b & blue.mask ) >> blue.shift,  xfraction);

        write(dp, (A << alpha.shift) | (R << red.shift) | (G << green.shift) | (B << blue.shift));
        dp += stride;
        xfraction += xstride;
      }
      if(x >= outputWidth) break;

      sp += stride;
      a = b;
      b = read(sp);
      xfraction -= 0x100000000;
    }
  }

  free();
  data = outputData;
  width = outputWidth;
  pitch = outputPitch;
  size = height * pitch;
}

void image::scaleLinearHeight(unsigned outputHeight) {
  uint8_t* outputData = allocate(width, outputHeight, stride);
  uint64_t ystride = ((uint64_t)(height - 1) << 32) / max(1u, outputHeight - 1);

  #pragma omp parallel for
  for(unsigned x = 0; x < width; x++) {
    uint64_t yfraction = 0;

    const uint8_t* sp = data + stride * x;
    uint8_t* dp = outputData + stride * x;

    uint64_t a = read(sp);
    uint64_t b = read(sp + pitch);
    sp += pitch;

    unsigned y = 0;
    while(true) {
      while(yfraction < 0x100000000 && y++ < outputHeight) {
        uint64_t A = interpolate1D((a & alpha.mask) >> alpha.shift, (b & alpha.mask) >> alpha.shift, yfraction);
        uint64_t R = interpolate1D((a & red.mask  ) >> red.shift,   (b & red.mask  ) >> red.shift,   yfraction);
        uint64_t G = interpolate1D((a & green.mask) >> green.shift, (b & green.mask) >> green.shift, yfraction);
        uint64_t B = interpolate1D((a & blue.mask ) >> blue.shift,  (b & blue.mask ) >> blue.shift,  yfraction);

        write(dp, (A << alpha.shift) | (R << red.shift) | (G << green.shift) | (B << blue.shift));
        dp += pitch;
        yfraction += ystride;
      }
      if(y >= outputHeight) break;

      sp += pitch;
      a = b;
      b = read(sp);
      yfraction -= 0x100000000;
    }
  }

  free();
  data = outputData;
  height = outputHeight;
  size = height * pitch;
}

void image::scaleLinear(unsigned outputWidth, unsigned outputHeight) {
  uint8_t* outputData = allocate(outputWidth, outputHeight, stride);
  unsigned outputPitch = outputWidth * stride;

  uint64_t xstride = ((uint64_t)(width  - 1) << 32) / max(1u, outputWidth  - 1);
  uint64_t ystride = ((uint64_t)(height - 1) << 32) / max(1u, outputHeight - 1);

  #pragma omp parallel for
  for(unsigned y = 0; y < outputHeight; y++) {
    uint64_t yfraction = ystride * y;
    uint64_t xfraction = 0;

    const uint8_t* sp = data + pitch * (yfraction >> 32);
    uint8_t* dp = outputData + outputPitch * y;

    uint64_t a = read(sp);
    uint64_t b = read(sp + stride);
    uint64_t c = read(sp + pitch);
    uint64_t d = read(sp + pitch + stride);
    sp += stride;

    unsigned x = 0;
    while(true) {
      while(xfraction < 0x100000000 && x++ < outputWidth) {
        uint64_t A = interpolate2D((a & alpha.mask) >> alpha.shift, (b & alpha.mask) >> alpha.shift, (c & alpha.mask) >> alpha.shift, (d & alpha.mask) >> alpha.shift, xfraction, yfraction);
        uint64_t R = interpolate2D((a & red.mask  ) >> red.shift,   (b & red.mask  ) >> red.shift,   (c & red.mask  ) >> red.shift,   (d & red.mask  ) >> red.shift,   xfraction, yfraction);
        uint64_t G = interpolate2D((a & green.mask) >> green.shift, (b & green.mask) >> green.shift, (c & green.mask) >> green.shift, (d & green.mask) >> green.shift, xfraction, yfraction);
        uint64_t B = interpolate2D((a & blue.mask ) >> blue.shift,  (b & blue.mask ) >> blue.shift,  (c & blue.mask ) >> blue.shift,  (d & blue.mask ) >> blue.shift,  xfraction, yfraction);

        write(dp, (A << alpha.shift) | (R << red.shift) | (G << green.shift) | (B << blue.shift));
        dp += stride;
        xfraction += xstride;
      }
      if(x >= outputWidth) break;

      sp += stride;
      a = b;
      c = d;
      b = read(sp);
      d = read(sp + pitch);
      xfraction -= 0x100000000;
    }
  }

  free();
  data = outputData;
  width = outputWidth;
  height = outputHeight;
  pitch = outputPitch;
  size = height * pitch;
}

void image::scaleNearest(unsigned outputWidth, unsigned outputHeight) {
  uint8_t* outputData = allocate(outputWidth, outputHeight, stride);
  unsigned outputPitch = outputWidth * stride;

  uint64_t xstride = ((uint64_t)width  << 32) / outputWidth;
  uint64_t ystride = ((uint64_t)height << 32) / outputHeight;

  #pragma omp parallel for
  for(unsigned y = 0; y < outputHeight; y++) {
    uint64_t yfraction = ystride * y;
    uint64_t xfraction = 0;

    const uint8_t* sp = data + pitch * (yfraction >> 32);
    uint8_t* dp = outputData + outputPitch * y;

    uint64_t a = read(sp);

    unsigned x = 0;
    while(true) {
      while(xfraction < 0x100000000 && x++ < outputWidth) {
        write(dp, a);
        dp += stride;
        xfraction += xstride;
      }
      if(x >= outputWidth) break;

      sp += stride;
      a = read(sp);
      xfraction -= 0x100000000;
    }
  }

  free();
  data = outputData;
  width = outputWidth;
  height = outputHeight;
  pitch = outputPitch;
  size = height * pitch;
}

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

bool image::loadPNG(const string& filename) {
  if(!file::exists(filename)) return false;
  auto buffer = file::read(filename);
  return loadPNG(buffer.data(), buffer.size());
}

}

#endif
