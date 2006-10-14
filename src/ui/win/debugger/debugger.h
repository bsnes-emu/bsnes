class Debugger {
public:
struct {
  bool active;
  bool debugger_access;
} status;

  bool   active();
  void   activate();
  void   deactivate();
  void   refresh();

enum { DRAM, SPCRAM, VRAM, OAM, CGRAM };
  uint8  read(uint8 mode, uint32 addr);
  void   write(uint8 mode, uint32 addr, uint8 data);

  Debugger();
} debugger;

#include "ui_debugger.h"
#include "ui_tracer.h"
#include "ui_memory.h"
