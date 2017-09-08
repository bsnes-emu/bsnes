#if defined(Hiro_Monitor)

auto Monitor::count() -> uint {
  return pMonitor::count();
}

auto Monitor::dpi(uint monitor) -> Position {
  return pMonitor::dpi(monitor);
}

auto Monitor::geometry(uint monitor) -> Geometry {
  return pMonitor::geometry(monitor);
}

auto Monitor::primary() -> uint {
  return pMonitor::primary();
}

#endif
