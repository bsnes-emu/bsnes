bool EmuSettingsWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CHANGED: {
    if(info.control == &Port1) {
      config::snes.controller_port0 = Port1.GetSelection();
      snes->port_set_deviceid(0, config::snes.controller_port0);
    } else if(info.control == &Port2) {
      config::snes.controller_port1 = Port2.GetSelection();
      snes->port_set_deviceid(1, config::snes.controller_port1);
    }
  } break;

  case EVENT_CLICKED: {
    if(info.control == &HDMAEnable || info.control == &OffsetEnable ||
       info.control == &BG1P0 || info.control == &BG1P1 ||
       info.control == &BG2P0 || info.control == &BG2P1 ||
       info.control == &BG3P0 || info.control == &BG3P1 ||
       info.control == &BG4P0 || info.control == &BG4P1 ||
       info.control == &OAMP0 || info.control == &OAMP1 ||
       info.control == &OAMP2 || info.control == &OAMP3
    ) {
      config::cpu.hdma_enable     = HDMAEnable.Checked();
      config::ppu.opt_enable      = OffsetEnable.Checked();
      config::ppu.bg1_pri0_enable = BG1P0.Checked();
      config::ppu.bg1_pri1_enable = BG1P1.Checked();
      config::ppu.bg2_pri0_enable = BG2P0.Checked();
      config::ppu.bg2_pri1_enable = BG2P1.Checked();
      config::ppu.bg3_pri0_enable = BG3P0.Checked();
      config::ppu.bg3_pri1_enable = BG3P1.Checked();
      config::ppu.bg4_pri0_enable = BG4P0.Checked();
      config::ppu.bg4_pri1_enable = BG4P1.Checked();
      config::ppu.oam_pri0_enable = OAMP0.Checked();
      config::ppu.oam_pri1_enable = OAMP1.Checked();
      config::ppu.oam_pri2_enable = OAMP2.Checked();
      config::ppu.oam_pri3_enable = OAMP3.Checked();
    }
  } break;

  }

  return false;
}

void EmuSettingsWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Emulation Settings");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  PortSelectionLabel.Create(this, "visible", x, y, 460, 15, "Controller port selection:");
  y += 18;

  Port1Label.Create(this, "visible", x, y + 3, 90, 15, "Controller Port 1:");
  Port1.Create(this, "visible", x + 90, y, 135, 400, "None|Joypad 1|Joypad 2");
  Port2Label.Create(this, "visible", x + 230, y + 3, 90, 15, "Controller Port 2:");
  Port2.Create(this, "visible", x + 320, y, 135, 400, "None|Joypad 1|Joypad 2");
  y += 26;

  Separator1.Create(this, "visible|sunken", x, y, 460, 3);
  y += 8;

  HDMAEnable.Create(this, "visible|auto", x, y, 460, 15, "Enable HDMA");
  OffsetEnable.Create(this, "visible|auto", x, y + 15, 460, 15, "Enable offset-per-tile effects");
  y += 35;

  Separator2.Create(this, "visible|sunken", x, y, 460, 3);
  y += 8;

  PPULayerSettings.Create(this, "visible", x, y, 460, 15, "PPU layer enable settings:");
  BG1P0.Create(this, "visible|auto", x, y + 15, 100, 15, "BG1 Priority 0");
  BG1P1.Create(this, "visible|auto", x + 100, y + 15, 100, 15, "BG1 Priority 1");
  BG2P0.Create(this, "visible|auto", x, y + 30, 100, 15, "BG2 Priority 0");
  BG2P1.Create(this, "visible|auto", x + 100, y + 30, 100, 15, "BG2 Priority 1");
  BG3P0.Create(this, "visible|auto", x, y + 45, 100, 15, "BG3 Priority 0");
  BG3P1.Create(this, "visible|auto", x + 100, y + 45, 100, 15, "BG3 Priority 1");
  BG4P0.Create(this, "visible|auto", x, y + 60, 100, 15, "BG4 Priority 0");
  BG4P1.Create(this, "visible|auto", x + 100, y + 60, 100, 15, "BG4 Priority 1");
  OAMP0.Create(this, "visible|auto", x, y + 75, 100, 15, "OAM Priority 0");
  OAMP1.Create(this, "visible|auto", x + 100, y + 75, 100, 15, "OAM Priority 1");
  OAMP2.Create(this, "visible|auto", x + 200, y + 75, 100, 15, "OAM Priority 2");
  OAMP3.Create(this, "visible|auto", x + 300, y + 75, 100, 15, "OAM Priority 3");

  HDMAEnable.Check();
  OffsetEnable.Check();
  BG1P0.Check();
  BG1P1.Check();
  BG2P0.Check();
  BG2P1.Check();
  BG3P0.Check();
  BG3P1.Check();
  BG4P0.Check();
  BG4P1.Check();
  OAMP0.Check();
  OAMP1.Check();
  OAMP2.Check();
  OAMP3.Check();

  Port1.SetSelection(config::snes.controller_port0);
  Port2.SetSelection(config::snes.controller_port1);
}
