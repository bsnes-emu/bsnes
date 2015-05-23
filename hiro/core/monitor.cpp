#if defined(Hiro_Monitor)

auto Monitor::count() -> unsigned {
  return pMonitor::count();
}

auto Monitor::geometry(unsigned monitor) -> Geometry {
  return pMonitor::geometry(monitor);
}

auto Monitor::primary() -> unsigned {
  return pMonitor::primary();
}

#endif
