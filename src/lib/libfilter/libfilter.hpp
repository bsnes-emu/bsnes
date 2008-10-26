#ifndef LIBFILTER_H
#define LIBFILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <nall/algorithm.hpp>
#include <nall/stdint.hpp>

namespace libfilter {
  #include "colortable.h"
  #include "filter.h"

  #include "direct.h"
  #include "scanline.h"
  #include "scale2x.h"
  #include "hq2x.h"
  #include "ntsc.h"
} //namespace libfilter

#endif //ifndef LIBFILTER_H
