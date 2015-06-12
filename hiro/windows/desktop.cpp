#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  return {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
}

auto pDesktop::workspace() -> Geometry {
  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  return {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
}

}

#endif
