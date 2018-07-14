#if defined(Hiro_Monitor)

//per-monitor API is only on Windows 10+

namespace hiro {

struct MonitorInfo {
  uint monitor = 0;
  uint primary = 0;
  Geometry geometry;
  uint index = 0;
};

static auto CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
  MonitorInfo& info = *(MonitorInfo*)dwData;
  MONITORINFOEX mi{};
  mi.cbSize = sizeof(MONITORINFOEX);
  GetMonitorInfo(hMonitor, &mi);
  string displayName = (const char*)utf8_t(mi.szDevice);
  if(displayName.beginsWith(R"(\\.\DISPLAYV)")) return true;  //ignore pseudo-monitors
  if(mi.dwFlags & MONITORINFOF_PRIMARY) info.primary = info.index;
  if(info.monitor == info.index) {
    info.geometry = {lprcMonitor->left, lprcMonitor->top, lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top};
  }
  info.index++;
  return true;
}

auto pMonitor::count() -> uint {
  return GetSystemMetrics(SM_CMONITORS);
}

auto pMonitor::dpi(uint monitor) -> Position {
  HDC hdc = GetDC(0);
  auto dpiX = (float)GetDeviceCaps(hdc, LOGPIXELSX);
  auto dpiY = (float)GetDeviceCaps(hdc, LOGPIXELSY);
  ReleaseDC(0, hdc);
  return {dpiX, dpiY};
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  MonitorInfo info;
  info.monitor = monitor;
  EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&info);
  return info.geometry;
}

auto pMonitor::primary() -> uint {
  MonitorInfo info;
  EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&info);
  return info.primary;
}

auto pMonitor::workspace(uint monitor) -> Geometry {
  return pDesktop::workspace();
}

}

#endif
