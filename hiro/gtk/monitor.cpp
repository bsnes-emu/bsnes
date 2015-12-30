#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  return gdk_screen_get_n_monitors(gdk_screen_get_default());
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
