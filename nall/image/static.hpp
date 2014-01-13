#ifndef NALL_IMAGE_STATIC_HPP
#define NALL_IMAGE_STATIC_HPP

namespace nall {

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

}

#endif
