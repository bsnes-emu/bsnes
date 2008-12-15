#include "libfilter.hpp"
using nall::min;
using nall::max;

namespace libfilter {
  #include "colortable.cpp"
  #include "filter.cpp"

  #include "direct.cpp"
  #include "scanline.cpp"
  #include "scale2x.cpp"
  #include "hq2x.cpp"
  #include "ntsc.cpp"
}  //namespace libfilter
