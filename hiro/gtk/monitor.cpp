#if defined(Hiro_Monitor)

//GTK 3.22 adds new monitor functions
//using GTK 2.x functions as FreeBSD 10.1 uses GTK 3.8

namespace hiro {

auto pMonitor::count() -> uint {
  #if HIRO_GTK==2 || 1
  return gdk_screen_get_n_monitors(gdk_screen_get_default());
  #elif HIRO_GTK==3
  return gdk_display_get_n_monitors(gdk_display_get_default());
  #endif
}

auto pMonitor::dpi(uint monitor) -> Position {
  #if HIRO_GTK==2 || 1
  //GTK2 does not support either per-monitor or per-axis DPI reporting
  float dpi = round(gdk_screen_get_resolution(gdk_screen_get_default()));
  return {dpi, dpi};
  #elif HIRO_GTK==3
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  return {
    round(gdk_monitor_get_width(gdkMonitor) / (gdk_monitor_get_width_mm(gdkMonitor) / 25.4)),
    round(gdk_monitor_get_height(gdkMonitor) / (gdk_monitor_get_height_mm(gdkMonitor) / 25.4))
  };
  #endif
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  GdkRectangle rectangle = {};
  #if HIRO_GTK==2 || 1
  gdk_screen_get_monitor_geometry(gdk_screen_get_default(), monitor, &rectangle);
  #elif HIRO_GTK==3
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  gdk_monitor_get_geometry(monitor, &rectangle);
  #endif
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

auto pMonitor::primary() -> uint {
  #if HIRO_GTK==2 || 1
  return gdk_screen_get_primary_monitor(gdk_screen_get_default());
  #elif HIRO_GTK==3
  return gdk_display_get_primary_monitor(gdk_display_get_default());
  #endif
}

auto pMonitor::workspace(uint monitor) -> Geometry {
  #if HIRO_GTK==2 || 1
  if(Monitor::count() == 1) {
    return Desktop::workspace();
  } else {
    //it is currently unknown how to get per-monitor workspace areas, use geometry instead
    return Monitor::geometry(monitor);
  }
  #elif HIRO_GTK==3
  auto gdkMonitor = gdk_display_get_monitor(gdk_display_get_default(), monitor);
  GdkRectangle rectangle = {};
  gdk_monitor_get_workarea(monitor, &rectangle);
  return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
  #endif
}

}

#endif
