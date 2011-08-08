#ifndef NALL_WINDOWS_COMPOSITOR_HPP
#define NALL_WINDOWS_COMPOSITOR_HPP
#ifdef _WIN32

namespace nall {

struct compositor {
  static bool enable(bool status);
  static void initialize();
private:
  static HMODULE module;
  static HRESULT (WINAPI *pDwmEnableComposition)(UINT);
};

HMODULE compositor::module = 0;
HRESULT (WINAPI *compositor::pDwmEnableComposition)(UINT) = 0;

bool compositor::enable(bool enable) {
  initialize();
  if(pDwmEnableComposition) return pDwmEnableComposition(enable) == S_OK;
  return false;
}

void compositor::initialize() {
  module = LoadLibraryA("dwmapi");
  if(module) {
    pDwmEnableComposition = (HRESULT (WINAPI*)(UINT))GetProcAddress(module, "DwmEnableComposition");
  }
}

}

#endif
#endif