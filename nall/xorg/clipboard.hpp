#pragma once

#include <nall/xorg/xorg.hpp>

namespace nall { namespace Clipboard {

auto clear() -> void {
  XDisplay display;
  if(auto atom = XInternAtom(display, "CLIPBOARD", XlibTrue)) {
    XSetSelectionOwner(display, atom, XlibNone, XlibCurrentTime);
  }
}

}}
