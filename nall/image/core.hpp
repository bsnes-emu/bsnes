#ifndef NALL_IMAGE_CORE_HPP
#define NALL_IMAGE_CORE_HPP

namespace nall {

image::operator bool() const {
  return !empty();
}

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

image::image(const vector<uint8_t>& buffer) {
  loadPNG(buffer.data(), buffer.size());
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
    for(signed n = 0; n < stride; n++) {
      data[n] = value;
      value >>= 8;
    }
  } else {
    for(signed n = stride - 1; n >= 0; n--) {
      data[n] = value;
      value >>= 8;
    }
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

bool image::load(const string& filename) {
  if(loadBMP(filename) == true) return true;
  if(loadPNG(filename) == true) return true;
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

uint8_t* image::allocate(unsigned width, unsigned height, unsigned stride) {
  //allocate 1x1 larger than requested; so that linear interpolation does not require bounds-checking
  unsigned size = width * height * stride;
  unsigned padding = width * stride + stride;
  uint8_t* data = new uint8_t[size + padding];
  memset(data + size, 0x00, padding);
  return data;
}

}

#endif
