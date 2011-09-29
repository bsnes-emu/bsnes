#ifndef NALL_COMPOSITOR_HPP
#define NALL_COMPOSITOR_HPP

#include <nall/intrinsics.hpp>

namespace nall {

struct compositor {
  inline static bool enabled();
  inline static bool enable(bool status);
};

#if defined(PLATFORM_X)

bool compositor::enabled() {
  FILE *fp = popen("xfconf-query -c xfwm4 -p '/general/use_compositing'", "r");
  if(fp == 0) return false;

  char buffer[512];
  if(fgets(buffer, sizeof buffer, fp) == 0) return false;

  if(!memcmp(buffer, "true", 4)) return true;
  return false;
}

bool compositor::enable(bool status) {
  FILE *fp;
  if(status) {
    fp = popen("xfconf-query -c xfwm4 -p '/general/use_compositing' -t 'bool' -s 'true'", "r");
  } else {
    fp = popen("xfconf-query -c xfwm4 -p '/general/use_compositing' -t 'bool' -s 'false'", "r");
  }
  if(fp == 0) return false;
  pclose(fp);
  return true;
}

#elif defined(PLATFORM_WINDOWS)

bool compositor::enabled() {
  HMODULE module = GetModuleHandleW(L"dwmapi");
  if(module == 0) module = LoadLibraryW(L"dwmapi");
  if(module == 0) return false;

  auto pDwmIsCompositionEnabled = (HRESULT (WINAPI*)(BOOL*))GetProcAddress(module, "DwmIsCompositionEnabled");
  if(pDwmIsCompositionEnabled == 0) return false;

  BOOL result;
  if(pDwmIsCompositionEnabled(&result) != S_OK) return false;
  return result;
}

bool compositor::enable(bool status) {
  HMODULE module = GetModuleHandleW(L"dwmapi");
  if(module == 0) module = LoadLibraryW(L"dwmapi");
  if(module == 0) return false;

  auto pDwmEnableComposition = (HRESULT (WINAPI*)(UINT))GetProcAddress(module, "DwmEnableComposition");
  if(pDwmEnableComposition == 0) return false;

  if(pDwmEnableComposition(status) != S_OK) return false;
  return true;
}

#else

bool compositor::enabled() {
  return false;
}

bool compositor::enable(bool) {
  return false;
}

#endif

}

#endif
