uintptr_t BSXLoaderWindow::close(event_t) {
  hide();
  return false;
}

uintptr_t BSXLoaderWindow::bbase_tick(event_t) {
  char fn[PATH_MAX];
  if(event::load_cart(fn) == true) tbase.set_text(fn);
  return true;
}

uintptr_t BSXLoaderWindow::cbase_tick(event_t) {
  tbase.set_text("");
  return true;
}

uintptr_t BSXLoaderWindow::bslot_tick(event_t) {
  char fn[PATH_MAX];
  if(event::load_cart(fn) == true) tslot.set_text(fn);
  return true;
}

uintptr_t BSXLoaderWindow::cslot_tick(event_t) {
  tslot.set_text("");
  return true;
}

uintptr_t BSXLoaderWindow::load_tick(event_t) {
  char base[PATH_MAX], slot[PATH_MAX];
  tbase.get_text(base, PATH_MAX);
  tslot.get_text(slot, PATH_MAX);

  if(mode == ModeBSX) {
    snes.config.path.bsx = base;
    event::load_cart_bsx(base, slot);
  } else if(mode == ModeBSC) {
    event::load_cart_bsc(base, slot);
  }

  tbase.set_text("");
  tslot.set_text("");
  hide();
  return true;
}

uintptr_t BSXLoaderWindow::cancel_tick(event_t) {
  tbase.set_text("");
  tslot.set_text("");
  hide();
  return true;
}

void BSXLoaderWindow::setup() {
  create(Window::AutoCenter, 565, 131, translate["Load BS-X Cartridge"]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  lbase.create(0, 555, 18, translate["Base cartridge:"]);
  tbase.create(0, 345, 25);
  bbase.create(0, 100, 25, translate["Browse ..."]);
  cbase.create(0, 100, 25, translate["Clear"]);

  lslot.create(0, 555, 18, translate["Slot cartridge:"]);
  tslot.create(0, 345, 25);
  bslot.create(0, 100, 25, translate["Browse ..."]);
  cslot.create(0, 100, 25, translate["Clear"]);

  load.create  (0, 275, 25, translate["Load"]);
  cancel.create(0, 275, 25, translate["Cancel"]);

  unsigned y = 5;
  attach(lbase,    5, y); y += 18;
  attach(tbase,    5, y);
  attach(bbase,  355, y);
  attach(cbase,  460, y); y += 25 + 5;
  attach(lslot,    5, y); y += 18;
  attach(tslot,    5, y);
  attach(bslot,  355, y);
  attach(cslot,  460, y); y += 25 + 5;
  attach(load,     5, y);
  attach(cancel, 285, y); y += 25 + 5;

  on_close       = bind(&BSXLoaderWindow::close, this);
  bbase.on_tick  = bind(&BSXLoaderWindow::bbase_tick, this);
  cbase.on_tick  = bind(&BSXLoaderWindow::cbase_tick, this);
  bslot.on_tick  = bind(&BSXLoaderWindow::bslot_tick, this);
  cslot.on_tick  = bind(&BSXLoaderWindow::cslot_tick, this);
  load.on_tick   = bind(&BSXLoaderWindow::load_tick, this);
  cancel.on_tick = bind(&BSXLoaderWindow::cancel_tick, this);
}

BSXLoaderWindow::BSXLoaderWindow() {
  mode = ModeBSX;
}
