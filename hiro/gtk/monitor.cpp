#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  return gdk_display_get_n_monitors(gdk_display_get_default());
}

auto pMonitor::dpi(uint monitor) -> Position {
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  auto monitorGeometry = geometry(monitor);
  auto scaleFactor = gdk_monitor_get_scale_factor(gdkMonitor);
  return {
    round(monitorGeometry.width() * scaleFactor / (gdk_monitor_get_width_mm(gdkMonitor) / 25.4)),
    round(monitorGeometry.height() * scaleFactor / (gdk_monitor_get_height_mm(gdkMonitor) / 25.4))
  };
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  GdkRectangle rectangle = {};
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  gdk_monitor_get_geometry(gdkMonitor, &rectangle);
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

auto pMonitor::primary() -> uint {
  auto display = gdk_display_get_default();
  auto primary = gdk_display_get_primary_monitor(display);

  // gdk_display_get_primary_monitor() returns a &GdkMonitor, but most APIs
  // want an index number.
  for (auto i = 1; i < gdk_display_get_n_monitors(display); i++ )
    if (primary == gdk_display_get_monitor(display, i)) return i;
  return 0;
}

auto pMonitor::workspace(uint monitor) -> Geometry {
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  GdkRectangle rectangle = {};
  gdk_monitor_get_workarea(gdkMonitor, &rectangle);
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

}

#endif
