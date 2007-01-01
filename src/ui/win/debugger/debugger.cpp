#include "ui_debugger.cpp"
#include "ui_tracer.cpp"
#include "ui_memory.cpp"

void init_debugger() {
  wDebug.SetIcon(100);
  wDebug.Create(0, "bsnes_debug", "title|minimize", 0, 0, 735, 350, "bsnes Debugger");
  wDebug.MoveToBottom();
  wDebug.MoveToLeft();

  wTracer.SetIcon(100);
  wTracer.Create(0, "bsnes_tracer", "title|minimize", 0, 0, 335, 160, "bsnes Tracer");
  wTracer.Center();

  wMemory.SetIcon(100);
  wMemory.Create(0, "bsnes_memory", "title|minimize", 0, 0, 500, 245, "bsnes Memory Editor");
  wMemory.Center();
}

void setup_debugger() {
  wDebug.Setup();
  wTracer.Setup();
  wMemory.Setup();
}

bool Debugger::active() { return status.active; }

void Debugger::activate() {
  if(status.active == true)return;
  status.active = true;

  bsnes.set_state(bSNES::STOP);
  wDebug.Show();
  wMain.CheckMenuItem(MENU_SETTINGS_DEBUGGER, true);
}

void Debugger::deactivate() {
  if(status.active == false)return;
  status.active = false;

  wDebug.Hide();
  wTracer.Hide();
  wMemory.Hide();
  wMain.CheckMenuItem(MENU_SETTINGS_DEBUGGER, false);
  bsnes.set_state(bSNES::RUN);
}

void Debugger::refresh() {
  wMemory.Refresh();
}

uint8 Debugger::read(uint8 mode, uint32 addr) {
uint8 r = 0x00;
  status.debugger_access = true;
  switch(mode) {
  case DRAM: {
    addr &= 0xffffff;
    if(!(addr & 0x400000) && (addr & 0xffff) >= 0x2000 && (addr & 0xffff) <= 0x5fff)break;
    r = r_mem->read(addr);
  } break;
  case SPCRAM:
    r = r_smp->spcram[addr & 0xffff];
    break;
  case VRAM:
    r = r_ppu->vram_read(addr & 0xffff);
    break;
  case OAM:
    r = r_ppu->oam_read(addr & 0x03ff);
    break;
  case CGRAM:
    r = r_ppu->cgram_read(addr & 0x01ff);
    break;
  }
  status.debugger_access = false;
  return r;
}

void Debugger::write(uint8 mode, uint32 addr, uint8 data) {
  status.debugger_access = true;
  switch(mode) {
  case DRAM:
    r_mem->cart_write_protect(false);
    r_mem->write(addr & 0xffffff, data);
    r_mem->cart_write_protect(true);
    break;
  case SPCRAM:
    r_smp->spcram[addr & 0xffff] = data;
    break;
  case VRAM:
    r_ppu->vram_write(addr & 0xffff, data);
    break;
  case OAM:
    r_ppu->oam_write(addr & 0x03ff, data);
    break;
  case CGRAM:
    r_ppu->cgram_write(addr & 0x01ff, data);
    break;
  }
  status.debugger_access = false;
}

Debugger::Debugger() {
  status.active = false;
  status.debugger_access = false;
}
