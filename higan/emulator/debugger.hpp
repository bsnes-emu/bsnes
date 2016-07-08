#pragma once

namespace Emulator {

#if defined(DEBUGGER)
  #define debug(id, ...) if(debugger.id) debugger.id(__VA_ARGS__)
  #define privileged public
#else
  #define debug(id, ...)
  #define privileged private
#endif

}
