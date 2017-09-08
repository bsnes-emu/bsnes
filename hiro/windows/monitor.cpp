#if defined(Hiro_Monitor)

namespace hiro {

struct MonitorInfo {
  uint monitor = 0;
  uint primary = 0;
  float dpiX = 0;
  float dpiY = 0;
  Geometry geometry;
  uint index = 0;
};

static auto CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
  MonitorInfo& info = *(MonitorInfo*)dwData;
  MONITORINFOEX mi{sizeof(MONITORINFOEX)};
  GetMonitorInfo(hMonitor, &mi);
  string displayName = (const char*)utf8_t(mi.szDevice);
  if(displayName.beginsWith(R"(\\.\DISPLAYV)")) return true;  //ignore pseudo-monitors
  if(mi.dwFlags & MONITORINFOF_PRIMARY) info.primary = info.index;
  if(info.monitor == info.index) {
    UINT dpiX = 0, dpiY = 0;
    GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    info.dpiX = dpiX;
    info.dpiY = dpiY;
    info.geometry = {lprcMonitor->left, lprcMonitor->top, lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top};
  }
  info.index++;
  return true;
}

auto pMonitor::count() -> uint {
  return GetSystemMetrics(SM_CMONITORS);
}

auto pMonitor::dpi(uint monitor) -> Position {
  MonitorInfo info;
  info.monitor = monitor;
  EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&info);
  return {info.dpiX, info.dpiY};
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

}

#endif
