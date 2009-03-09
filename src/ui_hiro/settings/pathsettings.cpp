//========
//ROM path
//========

uintptr_t PathSettingsWindow::selectpath_rom(event_t) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    snes.config.path.rom = t;
    update_paths();
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_rom(event_t) {
  snes.config.path.rom = "";
  update_paths();
  return true;
}

//=============
//save RAM path
//=============

uintptr_t PathSettingsWindow::selectpath_save(event_t) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    snes.config.path.save = t;
    update_paths();
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_save(event_t) {
  snes.config.path.save = "";
  update_paths();
  return true;
}

//==============
//UPS patch path
//==============

uintptr_t PathSettingsWindow::selectpath_patch(event_t) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    snes.config.path.patch = t;
    update_paths();
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_patch(event_t) {
  snes.config.path.patch = "";
  update_paths();
  return true;
}

//==========
//cheat path
//==========

uintptr_t PathSettingsWindow::selectpath_cheat(event_t) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    snes.config.path.cheat = t;
    update_paths();
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_cheat(event_t) {
  snes.config.path.cheat = "";
  update_paths();
  return true;
}

//===========
//export path
//===========

uintptr_t PathSettingsWindow::selectpath_export(event_t) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    snes.config.path.data = t;
    update_paths();
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_export(event_t) {
  snes.config.path.data = "";
  update_paths();
  return true;
}

//==============
//user interface
//==============

void PathSettingsWindow::update_paths() {
  if(snes.config.path.rom != "") rompath.set_text(snes.config.path.rom);
  else rompath.set_text(translate["<last loaded from>"]);

  if(snes.config.path.save != "") savepath.set_text(snes.config.path.save);
  else savepath.set_text(translate["<same as loaded game>"]);

  if(snes.config.path.patch != "") patchpath.set_text(snes.config.path.patch);
  else patchpath.set_text(translate["<same as loaded game>"]);

  if(snes.config.path.cheat != "") cheatpath.set_text(snes.config.path.cheat);
  else cheatpath.set_text(translate["<same as loaded game>"]);

  if(snes.config.path.data != "") exportpath.set_text(snes.config.path.data);
  else exportpath.set_text(translate["<startup path>"]);
}

void PathSettingsWindow::setup() {
  create(0, 451, 370);

  lrompath.create(0, 451, 18, translate["Games:"]);
  rompath.create(Editbox::Readonly, 241, 25);
  romselect.create(0, 100, 25, string() << translate["{{path}}Select"] << " ...");
  romdefault.create(0, 100, 25, translate["{{path}}Default"]);

  lsavepath.create(0, 451, 18, translate["Save RAM:"]);
  savepath.create(Editbox::Readonly, 241, 25);
  saveselect.create(0, 100, 25, string() << translate["{{path}}Select"] << " ...");
  savedefault.create(0, 100, 25, translate["{{path}}Default"]);

  lpatchpath.create(0, 451, 18, translate["UPS patches:"]);
  patchpath.create(Editbox::Readonly, 241, 25);
  patchselect.create(0, 100, 25, string() << translate["{{path}}Select"] << " ...");
  patchdefault.create(0, 100, 25, translate["{{path}}Default"]);

  lcheatpath.create(0, 451, 18, translate["Cheat codes:"]);
  cheatpath.create(Editbox::Readonly, 241, 25);
  cheatselect.create(0, 100, 25, string() << translate["{{path}}Select"] << " ...");
  cheatdefault.create(0, 100, 25, translate["{{path}}Default"]);

  lexportpath.create(0, 451, 18, translate["Export data:"]);
  exportpath.create(Editbox::Readonly, 241, 25);
  exportselect.create(0, 100, 25, string() << translate["{{path}}Select"] << " ...");
  exportdefault.create(0, 100, 25, translate["{{path}}Default"]);

  unsigned y = 0;
  attach(lrompath,        0, y); y += 18;
  attach(rompath,         0, y);
  attach(romselect,     246, y);
  attach(romdefault,    351, y); y += 25 + 5;

  attach(lsavepath,       0, y); y += 18;
  attach(savepath,        0, y);
  attach(saveselect,    246, y);
  attach(savedefault,   351, y); y += 25 + 5;

  attach(lpatchpath,      0, y); y += 18;
  attach(patchpath,       0, y);
  attach(patchselect,   246, y);
  attach(patchdefault,  351, y); y += 25 + 5;

  attach(lcheatpath,      0, y); y += 18;
  attach(cheatpath,       0, y);
  attach(cheatselect,   246, y);
  attach(cheatdefault,  351, y); y += 25 + 5;

  attach(lexportpath,     0, y); y += 18;
  attach(exportpath,      0, y);
  attach(exportselect,  246, y);
  attach(exportdefault, 351, y); y += 25 + 5;

  romselect.on_tick = bind(&PathSettingsWindow::selectpath_rom, this);
  romdefault.on_tick = bind(&PathSettingsWindow::defaultpath_rom, this);

  saveselect.on_tick = bind(&PathSettingsWindow::selectpath_save, this);
  savedefault.on_tick = bind(&PathSettingsWindow::defaultpath_save, this);

  patchselect.on_tick = bind(&PathSettingsWindow::selectpath_patch, this);
  patchdefault.on_tick = bind(&PathSettingsWindow::defaultpath_patch, this);

  cheatselect.on_tick = bind(&PathSettingsWindow::selectpath_cheat, this);
  cheatdefault.on_tick = bind(&PathSettingsWindow::defaultpath_cheat, this);

  exportselect.on_tick = bind(&PathSettingsWindow::selectpath_export, this);
  exportdefault.on_tick = bind(&PathSettingsWindow::defaultpath_export, this);

  update_paths();
}
