#if defined(Hiro_Monitor)

namespace hiro {

struct MonitorInfo {
  unsigned monitor = 0;
  unsigned primary = 0;
  unsigned index = 0;
  Geometry geometry;
};

static auto CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
  MonitorInfo& info = *(MonitorInfo*)dwData;
  MONITORINFOEX mi;
  memset(&mi, 0, sizeof(MONITORINFOEX));
  mi.cbSize = sizeof(MONITORINFOEX);
  GetMonitorInfo(hMonitor, &mi);
  string displayName = (const char*)utf8_t(mi.szDevice);
  if(displayName.beginsWith(R"(\\.\DISPLAYV)")) return TRUE;  //ignore pseudo-monitors
  if(mi.dwFlags & MONITORINFOF_PRIMARY) info.primary = info.index;
  if(info.monitor == info.index) {
    info.geometry = {lprcMonitor->left, lprcMonitor->top, lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top};
  }
  info.index++;
  return TRUE;
}

auto pMonitor::count() -> unsigned {
  return GetSystemMetrics(SM_CMONITORS);
}

auto pMonitor::geometry(unsigned monitor) -> Geometry {
  MonitorInfo info;
  info.monitor = monitor;
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&info);
  return info.geometry;
}

auto pMonitor::primary() -> unsigned {
  MonitorInfo info;
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&info);
  return info.primary;
}

}

#endif
