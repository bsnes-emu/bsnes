#include <snes/snes.hpp>

#define DSP_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "dsp-debugger.cpp"
#endif

void DSP::Enter() { dsp.enter(); }

void DSP::power() {
  create(DSP::Enter, system.apu_frequency());
}

void DSP::reset() {
  create(DSP::Enter, system.apu_frequency());
}

void DSP::serialize(serializer &s) {
  Processor::serialize(s);
}

}
