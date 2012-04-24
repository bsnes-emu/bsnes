Size pDesktop::size() {
  return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

Geometry pDesktop::workspace() {
  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}
