#if defined(Hiro_Monitor)

namespace hiro {

unsigned pMonitor::count() {
  return gdk_screen_get_n_monitors(gdk_screen_get_default());
}

Geometry pMonitor::geometry(unsigned monitor) {
  GdkRectangle rectangle = {0};
  gdk_screen_get_monitor_geometry(gdk_screen_get_default(), monitor, &rectangle);
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

unsigned pMonitor::primary() {
  return gdk_screen_get_primary_monitor(gdk_screen_get_default());
}

}

#endif
