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

void image::gradient(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
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

void image::gradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY, function<double (double, double)> callback) {
  for(signed y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    double py = max(-radiusY, min(+radiusY, y - centerY)) * 1.0 / radiusY;
    for(signed x = 0; x < width; x++) {
      double px = max(-radiusX, min(+radiusX, x - centerX)) * 1.0 / radiusX;
      double mu = max(0.0, min(1.0, callback(px, py)));
      if(mu != mu) mu = 1.0;  //NaN
      write(dp, interpolate4f(a, b, mu));
      dp += stride;
    }
  }
}

void image::crossGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    x = fabs(x), y = fabs(y);
    return min(x, y) * min(x, y);
  });
}

void image::diamondGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return fabs(x) + fabs(y);
  });
}

void image::horizontalGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return fabs(x);
  });
}

void image::radialGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return sqrt(x * x + y * y);
  });
}

void image::sphericalGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return x * x + y * y;
  });
}

void image::squareGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return max(fabs(x), fabs(y));
  });
}

void image::verticalGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY) {
  return gradient(a, b, radiusX, radiusY, centerX, centerY, [](double x, double y) -> double {
    return fabs(y);
  });
}

}

#endif
