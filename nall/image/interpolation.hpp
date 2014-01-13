#ifndef NALL_IMAGE_INTERPOLATION_HPP
#define NALL_IMAGE_INTERPOLATION_HPP

namespace nall {

void image::isplit(uint64_t* c, uint64_t color) {
  c[0] = (color & alpha.mask) >> alpha.shift;
  c[1] = (color & red.mask  ) >> red.shift;
  c[2] = (color & green.mask) >> green.shift;
  c[3] = (color & blue.mask ) >> blue.shift;
}

uint64_t image::imerge(const uint64_t* c) {
  return c[0] << alpha.shift | c[1] << red.shift | c[2] << green.shift | c[3] << blue.shift;
}

uint64_t image::interpolate1f(uint64_t a, uint64_t b, double x) {
  return a * (1.0 - x) + b * x;
}

uint64_t image::interpolate1f(uint64_t a, uint64_t b, uint64_t c, uint64_t d, double x, double y) {
  return a * (1.0 - x) * (1.0 - y) + b * x * (1.0 - y) + c * (1.0 - x) * y + d * x * y;
}

uint64_t image::interpolate1i(int64_t a, int64_t b, uint32_t x) {
  return a + (((b - a) * x) >> 32);  //a + (b - a) * x
}

uint64_t image::interpolate1i(int64_t a, int64_t b, int64_t c, int64_t d, uint32_t x, uint32_t y) {
  a = a + (((b - a) * x) >> 32);     //a + (b - a) * x
  c = c + (((d - c) * x) >> 32);     //c + (d - c) * x
  return a + (((c - a) * y) >> 32);  //a + (c - a) * y
}

uint64_t image::interpolate4f(uint64_t a, uint64_t b, double x) {
  uint64_t o[4], pa[4], pb[4];
  isplit(pa, a), isplit(pb, b);
  for(unsigned n = 0; n < 4; n++) o[n] = interpolate1f(pa[n], pb[n], x);
  return imerge(o);
}

uint64_t image::interpolate4f(uint64_t a, uint64_t b, uint64_t c, uint64_t d, double x, double y) {
  uint64_t o[4], pa[4], pb[4], pc[4], pd[4];
  isplit(pa, a), isplit(pb, b), isplit(pc, c), isplit(pd, d);
  for(unsigned n = 0; n < 4; n++) o[n] = interpolate1f(pa[n], pb[n], pc[n], pd[n], x, y);
  return imerge(o);
}

uint64_t image::interpolate4i(uint64_t a, uint64_t b, uint32_t x) {
  uint64_t o[4], pa[4], pb[4];
  isplit(pa, a), isplit(pb, b);
  for(unsigned n = 0; n < 4; n++) o[n] = interpolate1i(pa[n], pb[n], x);
  return imerge(o);
}

uint64_t image::interpolate4i(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint32_t x, uint32_t y) {
  uint64_t o[4], pa[4], pb[4], pc[4], pd[4];
  isplit(pa, a), isplit(pb, b), isplit(pc, c), isplit(pd, d);
  for(unsigned n = 0; n < 4; n++) o[n] = interpolate1i(pa[n], pb[n], pc[n], pd[n], x, y);
  return imerge(o);
}

}

#endif
