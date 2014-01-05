namespace phoenix {

unsigned pMonitor::count() {
  return 1;
}

Geometry pMonitor::geometry(unsigned monitor) {
  return {0, 0, 0, 0};
}

unsigned pMonitor::primary() {
  return 0;
}

}
