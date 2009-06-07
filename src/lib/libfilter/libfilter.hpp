#ifndef LIBFILTER_H
#define LIBFILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <nall/algorithm.hpp>
#include <nall/stdint.hpp>

namespace libfilter {
  #include "colortable.hpp"
  #include "filter.hpp"

  #include "direct.hpp"
  #include "scanline.hpp"
  #include "scale2x.hpp"
  #include "hq2x.hpp"
  #include "ntsc.hpp"
};

#endif
