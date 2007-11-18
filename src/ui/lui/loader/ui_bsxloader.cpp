bool BSXLoaderWindow::message(uint id, uintptr_t param) {
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

    if(control == &bslot) {
    char fn[PATH_MAX];
      if(event::load_rom(fn) == true) tslot.set_text(fn);
    }

    if(control == &cbase) tbase.set_text("");
    if(control == &cslot) tslot.set_text("");

    if(control == &load) {
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
    }

    if(control == &cancel) {
      tbase.set_text("");
      tslot.set_text("");
      hide();
    }
  }

  return true;
}

void BSXLoaderWindow::setup() {
  create(ui::Window::Center, 640, 200, "Load BS-X Cartridge");

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

  lslot.create(*this, 0,   5, y, 630, lh, "Slot cartridge:");
  y += lh;
  tslot.create(*this, 0,   5, y, 420, mh);
  bslot.create(*this, 0, 430, y, 100, mh, "Browse ...");
  cslot.create(*this, 0, 535, y, 100, mh, "Clear");
  y += mh + lh;

  load.create(*this, 0, 5, y, 312, mh, "Load");
  cancel.create(*this, 0, 323, y, 312, mh, "Cancel");
  y += mh + 5;

  resize(640, y);
}

BSXLoaderWindow::BSXLoaderWindow() {
  mode = ModeBSX;
}
