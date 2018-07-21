#pragma once

namespace nall { namespace DSP {

#if defined(NALL_DSP_PRECISION)
using real = NALL_DSP_PRECISION;
#else
using real = double;
#endif

}}
