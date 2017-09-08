#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  return gdk_screen_get_n_monitors(gdk_screen_get_default());
}

auto pMonitor::dpi(uint monitor) -> Position {
  //GTK+ does not support either per-monitor or per-axis DPI reporting
  float dpi = round(gdk_screen_get_resolution(gdk_screen_get_default()));
  return {dpi, dpi};
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  GdkRectangle rectangle = {0};
  gdk_screen_get_monitor_geometry(gdk_screen_get_default(), monitor, &rectangle);
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

auto pMonitor::primary() -> uint {
  return gdk_screen_get_primary_monitor(gdk_screen_get_default());
}

}

#endif
