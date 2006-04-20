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

  void   notify(uint32 message, uint32 param1 = 0, uint32 param2 = 0);
enum { DRAM, SPCRAM, VRAM, OAM, CGRAM };
  uint8  read(uint8 mode, uint32 addr);
  void   write(uint8 mode, uint32 addr, uint8 data);

  Debugger();
} debugger;

#include "ui_debugger.h"
#include "ui_memory.h"
