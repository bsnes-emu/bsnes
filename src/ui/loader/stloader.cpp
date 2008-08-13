uintptr_t STLoaderWindow::close(event_t) {
  hide();
  return false;
}

uintptr_t STLoaderWindow::bbase_tick(event_t) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tbase.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cbase_tick(event_t) {
  tbase.set_text("");
  return true;
}

uintptr_t STLoaderWindow::bslotA_tick(event_t) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tslotA.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cslotA_tick(event_t) {
  tslotA.set_text("");
  return true;
}

uintptr_t STLoaderWindow::bslotB_tick(event_t) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tslotB.set_text(fn);
  return true;
}

uintptr_t STLoaderWindow::cslotB_tick(event_t) {
  tslotB.set_text("");
  return true;
}

uintptr_t STLoaderWindow::load_tick(event_t) {
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

uintptr_t STLoaderWindow::cancel_tick(event_t) {
  tbase.set_text("");
  tslotA.set_text("");
  tslotB.set_text("");
  hide();
  return true;
}

void STLoaderWindow::setup() {
  create(Window::AutoCenter, 565, 179, translate["Load Sufami Turbo Cartridge"]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  lbase.create(0, 555, 18, translate["Base cartridge:"]);
  tbase.create(0, 345, 25);
  bbase.create(0, 100, 25, translate["Browse ..."]);
  cbase.create(0, 100, 25, translate["Clear"]);

  lslotA.create(0, 555, 18, translate["Slot A cartridge:"]);
  tslotA.create(0, 345, 25);
  bslotA.create(0, 100, 25, translate["Browse ..."]);
  cslotA.create(0, 100, 25, translate["Clear"]);

  lslotB.create(0, 555, 18, translate["Slot B cartridge:"]);
  tslotB.create(0, 345, 25);
  bslotB.create(0, 100, 25, translate["Browse ..."]);
  cslotB.create(0, 100, 25, translate["Clear"]);

  load.create  (0, 275, 25, translate["Load"]);
  cancel.create(0, 275, 25, translate["Cancel"]);

  unsigned y = 5;
  attach(lbase,    5, y); y += 18;
  attach(tbase,    5, y);
  attach(bbase,  355, y);
  attach(cbase,  460, y); y += 25 + 5;
  attach(lslotA,   5, y); y += 18;
  attach(tslotA,   5, y);
  attach(bslotA, 355, y);
  attach(cslotA, 460, y); y += 25 + 5;
  attach(lslotB,   5, y); y += 18;
  attach(tslotB,   5, y);
  attach(bslotB, 355, y);
  attach(cslotB, 460, y); y += 25 + 5;
  attach(load,     5, y);
  attach(cancel, 285, y); y += 25 + 5;

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
