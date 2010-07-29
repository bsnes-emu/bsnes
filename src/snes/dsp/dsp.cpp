#include <snes/snes.hpp>

#define DSP_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "dsp-debugger.cpp"
#endif

void DSP::power() {
  create();
}

void DSP::reset() {
  create();
}

void DSP::serialize(serializer &s) {
  Processor::serialize(s);
}

}
