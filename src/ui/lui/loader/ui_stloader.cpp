bool STLoaderWindow::message(uint id, uintptr_t param) {
ui::Control *control = (ui::Control*)param;
  if(id == ui::Message::Close) {
    hide();
    return false;
  }

  if(id == ui::Message::Clicked) {
    if(control == &bbase) {
    char fn[PATH_MAX];
      if(event::load_rom(fn) == true) tbase.set_text(fn);
    }

    if(control == &bslotA) {
    char fn[PATH_MAX];
      if(event::load_rom(fn) == true) tslotA.set_text(fn);
    }

    if(control == &bslotB) {
    char fn[PATH_MAX];
      if(event::load_rom(fn) == true) tslotB.set_text(fn);
    }

    if(control == &cbase)  tbase.set_text("");
    if(control == &cslotA) tslotA.set_text("");
    if(control == &cslotB) tslotB.set_text("");

    if(control == &load) {
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
    }

    if(control == &cancel) {
      tbase.set_text("");
      tslotA.set_text("");
      tslotB.set_text("");
      hide();
    }
  }

  return true;
}

void STLoaderWindow::setup() {
  create(ui::Window::Center, 640, 200, "Load Sufami Turbo Cartridge");

uint bh = ui::Button::ideal_height;
uint eh = ui::Editbox::ideal_height;
uint lh = ui::Label::ideal_height;
uint mh = max(bh, eh);

uint y = 5;
  lbase.create(*this, 0,   5, y, 630, lh, "Base cartridge:");
  y += lh;
  tbase.create(*this, 0,   5, y, 420, mh);
  bbase.create(*this, 0, 430, y, 100, mh, "Browse ...");
  cbase.create(*this, 0, 535, y, 100, mh, "Clear");
  y += mh + 5;

  lslotA.create(*this, 0,   5, y, 630, lh, "Slot A cartridge:");
  y += lh;
  tslotA.create(*this, 0,   5, y, 420, mh);
  bslotA.create(*this, 0, 430, y, 100, mh, "Browse ...");
  cslotA.create(*this, 0, 535, y, 100, mh, "Clear");
  y += mh + 5;

  lslotB.create(*this, 0,   5, y, 630, lh, "Slot B cartridge:");
  y += lh;
  tslotB.create(*this, 0,   5, y, 420, mh);
  bslotB.create(*this, 0, 430, y, 100, mh, "Browse ...");
  cslotB.create(*this, 0, 535, y, 100, mh, "Clear");
  y += mh + lh;

  load.create(*this, 0, 5, y, 312, mh, "Load");
  cancel.create(*this, 0, 323, y, 312, mh, "Cancel");
  y += mh + 5;

  resize(640, y);
}
