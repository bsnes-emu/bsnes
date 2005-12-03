long PPUCfgWindow::wndproc(UINT msg, WPARAM wparam, LPARAM lparam) {

#define toggle(id) \
  config::##id##_enable.toggle(); \
  check(LOWORD(wparam), config::##id##_enable)

  switch(msg) {
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case PPUCFG_BG1_PRI0:    toggle(ppu.bg1_pri0); break;
    case PPUCFG_BG1_PRI1:    toggle(ppu.bg1_pri1); break;
    case PPUCFG_BG2_PRI0:    toggle(ppu.bg2_pri0); break;
    case PPUCFG_BG2_PRI1:    toggle(ppu.bg2_pri1); break;
    case PPUCFG_BG3_PRI0:    toggle(ppu.bg3_pri0); break;
    case PPUCFG_BG3_PRI1:    toggle(ppu.bg3_pri1); break;
    case PPUCFG_BG4_PRI0:    toggle(ppu.bg4_pri0); break;
    case PPUCFG_BG4_PRI1:    toggle(ppu.bg4_pri1); break;
    case PPUCFG_OAM_PRI0:    toggle(ppu.oam_pri0); break;
    case PPUCFG_OAM_PRI1:    toggle(ppu.oam_pri1); break;
    case PPUCFG_OAM_PRI2:    toggle(ppu.oam_pri2); break;
    case PPUCFG_OAM_PRI3:    toggle(ppu.oam_pri3); break;
    case PPUCFG_HDMA_ENABLE: toggle(cpu.hdma);     break;
    case PPUCFG_OPT_ENABLE:  toggle(ppu.opt);      break;
    } break;
  case WM_CLOSE:
    hide();
    return 0;
  }

#undef toggle

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void PPUCfgWindow::setup() {
  control("STATIC", "BG / OAM display toggles:",    WS_VISIBLE,   5,   5, 200,  15, PPUCFG_STATIC0);

  control("BUTTON", "BG1 Priority 0", BS_CHECKBOX | WS_VISIBLE,   5,  20,  95,  15, PPUCFG_BG1_PRI0);
  control("BUTTON", "BG1 Priority 1", BS_CHECKBOX | WS_VISIBLE, 105,  20,  95,  15, PPUCFG_BG1_PRI1);
  control("BUTTON", "BG2 Priority 0", BS_CHECKBOX | WS_VISIBLE,   5,  35,  95,  15, PPUCFG_BG2_PRI0);
  control("BUTTON", "BG2 Priority 1", BS_CHECKBOX | WS_VISIBLE, 105,  35,  95,  15, PPUCFG_BG2_PRI1);
  control("BUTTON", "BG3 Priority 0", BS_CHECKBOX | WS_VISIBLE,   5,  50,  95,  15, PPUCFG_BG3_PRI0);
  control("BUTTON", "BG3 Priority 1", BS_CHECKBOX | WS_VISIBLE, 105,  50,  95,  15, PPUCFG_BG3_PRI1);
  control("BUTTON", "BG4 Priority 0", BS_CHECKBOX | WS_VISIBLE,   5,  65,  95,  15, PPUCFG_BG4_PRI0);
  control("BUTTON", "BG4 Priority 1", BS_CHECKBOX | WS_VISIBLE, 105,  65,  95,  15, PPUCFG_BG4_PRI1);
  control("BUTTON", "OAM Priority 0", BS_CHECKBOX | WS_VISIBLE,   5,  80,  95,  15, PPUCFG_OAM_PRI0);
  control("BUTTON", "OAM Priority 1", BS_CHECKBOX | WS_VISIBLE, 105,  80,  95,  15, PPUCFG_OAM_PRI1);
  control("BUTTON", "OAM Priority 2", BS_CHECKBOX | WS_VISIBLE,   5,  95,  95,  15, PPUCFG_OAM_PRI2);
  control("BUTTON", "OAM Priority 3", BS_CHECKBOX | WS_VISIBLE, 105,  95,  95,  15, PPUCFG_OAM_PRI3);

  control("STATIC", "Misc. video options:",         WS_VISIBLE,   5, 115, 200,  15, PPUCFG_STATIC1);
  control("BUTTON", "Enable HDMA (affects CPU)",
                                      BS_CHECKBOX | WS_VISIBLE,   5, 130, 200,  15, PPUCFG_HDMA_ENABLE);
  control("BUTTON", "Enable offset-per-tile mode effects",
                                      BS_CHECKBOX | WS_VISIBLE,   5, 145, 200,  15, PPUCFG_OPT_ENABLE);

  check(PPUCFG_BG1_PRI0, config::ppu.bg1_pri0_enable);
  check(PPUCFG_BG1_PRI1, config::ppu.bg1_pri1_enable);
  check(PPUCFG_BG2_PRI0, config::ppu.bg2_pri0_enable);
  check(PPUCFG_BG2_PRI1, config::ppu.bg2_pri1_enable);
  check(PPUCFG_BG3_PRI0, config::ppu.bg3_pri0_enable);
  check(PPUCFG_BG3_PRI1, config::ppu.bg3_pri1_enable);
  check(PPUCFG_BG4_PRI0, config::ppu.bg4_pri0_enable);
  check(PPUCFG_BG4_PRI1, config::ppu.bg4_pri1_enable);
  check(PPUCFG_OAM_PRI0, config::ppu.oam_pri0_enable);
  check(PPUCFG_OAM_PRI1, config::ppu.oam_pri1_enable);
  check(PPUCFG_OAM_PRI2, config::ppu.oam_pri2_enable);
  check(PPUCFG_OAM_PRI3, config::ppu.oam_pri3_enable);

  check(PPUCFG_HDMA_ENABLE, config::cpu.hdma_enable);
  check(PPUCFG_OPT_ENABLE,  config::ppu.opt_enable);
}
