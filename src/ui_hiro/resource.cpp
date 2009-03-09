namespace resource {

static uint8_t *icon48;
static uint8_t *controller;

void init() {
  //note: resources were removed, as hiro port is deprecated
  icon48     = new(zeromemory) uint8_t[48 * 48 * 4];
  controller = new(zeromemory) uint8_t[372 * 178 * 4];
}

} //namespace resource
