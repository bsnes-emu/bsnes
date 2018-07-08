#if defined(Hiro_Monitor)

auto Monitor::count() -> uint {
  return pMonitor::count();
}

auto Monitor::dpi(maybe<uint> monitor) -> Position {
  return pMonitor::dpi(monitor ? monitor() : primary());
}

auto Monitor::geometry(maybe<uint> monitor) -> Geometry {
  return pMonitor::geometry(monitor ? monitor() : primary());
}

auto Monitor::primary() -> uint {
  return pMonitor::primary();
}

auto Monitor::workspace(maybe<uint> monitor) -> Geometry {
  return pMonitor::workspace(monitor ? monitor() : primary());
}

#endif
