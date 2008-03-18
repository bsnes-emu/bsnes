uintptr_t STLoaderWindow::close(Event) {
  hide();
  return false;
}

uintptr_t STLoaderWindow::bbase_tick(Event) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tbase.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cbase_tick(Event) {
  tbase.set_text("");
  return true;
}

uintptr_t STLoaderWindow::bslotA_tick(Event) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tslotA.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cslotA_tick(Event) {
  tslotA.set_text("");
  return true;
}

uintptr_t STLoaderWindow::bslotB_tick(Event) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tslotB.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cslotB_tick(Event) {
  tslotB.set_text("");
  return true;
}

uintptr_t STLoaderWindow::load_tick(Event) {
char base[PATH_MAX], slotA[PATH_MAX], slotB[PATH_MAX];
  tbase.get_text(base, PATH_MAX);
  tslotA.get_text(slotA, PATH_MAX);
  tslotB.get_text(slotB, PATH_MAX);

  config::path.st = base;
  event::load_cart_st(base, slotA, slotB);

  tbase.set_text("");
  tslotA.set_text("");
  tslotB.set_text("");
  hide();
  return true;
}

uintptr_t STLoaderWindow::cancel_tick(Event) {
  tbase.set_text("");
  tslotA.set_text("");
  tslotB.set_text("");
  hide();
  return true;
}

void STLoaderWindow::setup() {
  create(Window::AutoCenter, 640, 205, "Load Sufami Turbo Cartridge");
  set_icon(48, 48, (uint32_t*)resource::icon48);

  lbase.create(0, 630, 20, "Base cartridge:");
  tbase.create(0, 420, 30);
  bbase.create(0, 100, 30, "Browse ...");
  cbase.create(0, 100, 30, "Clear");

  lslotA.create(0, 630, 20, "Slot A cartridge:");
  tslotA.create(0, 420, 30);
  bslotA.create(0, 100, 30, "Browse ...");
  cslotA.create(0, 100, 30, "Clear");

  lslotB.create(0, 630, 20, "Slot B cartridge:");
  tslotB.create(0, 420, 30);
  bslotB.create(0, 100, 30, "Browse ...");
  cslotB.create(0, 100, 30, "Clear");

  load.create  (0, 312, 30, "Load");
  cancel.create(0, 312, 30, "Cancel");

uint y = 5;
  attach(lbase,    5, y); y += 20;
  attach(tbase,    5, y);
  attach(bbase,  430, y);
  attach(cbase,  535, y); y += 30 + 5;
  attach(lslotA,   5, y); y += 20;
  attach(tslotA,   5, y);
  attach(bslotA, 430, y);
  attach(cslotA, 535, y); y += 30 + 5;
  attach(lslotB,   5, y); y += 20;
  attach(tslotB,   5, y);
  attach(bslotB, 430, y);
  attach(cslotB, 535, y); y += 30 + 5;
  attach(load,     5, y);
  attach(cancel, 323, y); y += 30 + 5;

  on_close       = bind(&STLoaderWindow::close, this);
  bbase.on_tick  = bind(&STLoaderWindow::bbase_tick, this);
  cbase.on_tick  = bind(&STLoaderWindow::cbase_tick, this);
  bslotA.on_tick = bind(&STLoaderWindow::bslotA_tick, this);
  cslotA.on_tick = bind(&STLoaderWindow::cslotA_tick, this);
  bslotB.on_tick = bind(&STLoaderWindow::bslotB_tick, this);
  cslotB.on_tick = bind(&STLoaderWindow::cslotB_tick, this);
  load.on_tick   = bind(&STLoaderWindow::load_tick, this);
  cancel.on_tick = bind(&STLoaderWindow::cancel_tick, this);
}
