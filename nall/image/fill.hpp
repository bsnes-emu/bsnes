#ifndef NALL_IMAGE_FILL_HPP
#define NALL_IMAGE_FILL_HPP

namespace nall {

void image::fill(uint64_t color) {
  uint8_t* dp = data;
  for(unsigned y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    for(unsigned x = 0; x < width; x++) {
      write(dp, color);
      dp += stride;
    }
  }
}

void image::linearGradient(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  for(unsigned y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    double muY = (double)y / (double)height;
    for(unsigned x = 0; x < width; x++) {
      double muX = (double)x / (double)width;
      write(dp, interpolate4f(a, b, c, d, muX, muY));
      dp += stride;
    }
  }
}

void image::horizontalGradient(uint64_t a, uint64_t b) {
  linearGradient(a, b, a, b);
}

void image::verticalGradient(uint64_t a, uint64_t b) {
  linearGradient(a, a, b, b);
}

void image::radialGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  if(radiusX < 1) radiusX = width  >> 1;
  if(radiusY < 1) radiusY = height >> 1;
  for(signed y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    double py = max(-radiusY, min(+radiusY, y - centerY)) * 1.0 / radiusY;
    for(signed x = 0; x < width; x++) {
      double px = max(-radiusX, min(+radiusX, x - centerX)) * 1.0 / radiusX;
      double mu = min(1.0, sqrt(px * px + py * py));
      write(dp, interpolate4f(a, b, mu));
      dp += stride;
    }
  }
}

void image::radialGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY) {
  radialGradient(a, b, radiusX, radiusY, width >> 1, height >> 1);
}

void image::radialGradient(uint64_t a, uint64_t b) {
  radialGradient(a, b, width >> 1, height >> 1, width >> 1, height >> 1);
}

}

#endif
