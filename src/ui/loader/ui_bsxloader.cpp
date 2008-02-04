uintptr_t BSXLoaderWindow::close(Event) {
  hide();
  return false;
}

uintptr_t BSXLoaderWindow::bbase_tick(Event) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tbase.set_text(fn);
  return true;
}

uintptr_t BSXLoaderWindow::cbase_tick(Event) {
  tbase.set_text("");
  return true;
}

uintptr_t BSXLoaderWindow::bslot_tick(Event) {
char fn[PATH_MAX];
  if(event::load_rom(fn) == true) tslot.set_text(fn);
  return true;
}

uintptr_t BSXLoaderWindow::cslot_tick(Event) {
  tslot.set_text("");
  return true;
}

uintptr_t BSXLoaderWindow::load_tick(Event) {
char base[PATH_MAX], slot[PATH_MAX];
  tbase.get_text(base, PATH_MAX);
  tslot.get_text(slot, PATH_MAX);

  if(mode == ModeBSX) {
    config::path.bsx = base;
    event::load_cart_bsx(base, slot);
  } else if(mode == ModeBSC) {
    event::load_cart_bsc(base, slot);
  }

  tbase.set_text("");
  tslot.set_text("");
  hide();
  return true;
}

uintptr_t BSXLoaderWindow::cancel_tick(Event) {
  tbase.set_text("");
  tslot.set_text("");
  hide();
  return true;
}

void BSXLoaderWindow::setup() {
  create(Window::AutoCenter, 640, 150, "Load BS-X Cartridge");

  lbase.create(0, 630, 20, "Base cartridge:");
  tbase.create(0, 420, 30);
  bbase.create(0, 100, 30, "Browse ...");
  cbase.create(0, 100, 30, "Clear");

  lslot.create(0, 630, 20, "Slot cartridge:");
  tslot.create(0, 420, 30);
  bslot.create(0, 100, 30, "Browse ...");
  cslot.create(0, 100, 30, "Clear");

  load.create  (0, 312, 30, "Load");
  cancel.create(0, 312, 30, "Cancel");

uint y = 5;
  attach(lbase,    5, y); y += 20;
  attach(tbase,    5, y);
  attach(bbase,  430, y);
  attach(cbase,  535, y); y += 30 + 5;
  attach(lslot,    5, y); y += 20;
  attach(tslot,    5, y);
  attach(bslot,  430, y);
  attach(cslot,  535, y); y += 30 + 5;
  attach(load,     5, y);
  attach(cancel, 323, y); y += 30 + 5;

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
