#ifndef NALL_WINDOWS_GUID_HPP
#define NALL_WINDOWS_GUID_HPP

#include <nall/random.hpp>
#include <nall/string.hpp>

namespace nall {

//generate unique GUID
inline string guid() {
  LinearFeedbackShiftRegisterGenerator lfsr;
  lfsr.seed(time(nullptr));
  for(unsigned n = 0; n < 256; n++) lfsr();

  string output;
  for(unsigned n = 0; n < 4; n++) output.append(hex(lfsr(), 2L));
  output.append("-");
  for(unsigned n = 0; n < 2; n++) output.append(hex(lfsr(), 2L));
  output.append("-");
  for(unsigned n = 0; n < 2; n++) output.append(hex(lfsr(), 2L));
  output.append("-");
  for(unsigned n = 0; n < 2; n++) output.append(hex(lfsr(), 2L));
  output.append("-");
  for(unsigned n = 0; n < 6; n++) output.append(hex(lfsr(), 2L));
  return {"{", output, "}"};
}

}

#endif
