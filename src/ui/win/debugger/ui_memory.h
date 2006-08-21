enum {
  MEMORYMODE_DRAM   = 0,
  MEMORYMODE_ROM    = 1,
  MEMORYMODE_SRAM   = 2,
  MEMORYMODE_SPCRAM = 3,
  MEMORYMODE_VRAM   = 4,
  MEMORYMODE_OAM    = 5,
  MEMORYMODE_CGRAM  = 6,
};

class MemoryEditor : public Window {
public:
Editbox  View;
Combobox Mode;
Label    GotoLabel;
Editbox  GotoAddr;
Label    OffsetLabel;
Editbox  Offset;
Editbox  Data;
Button   EditMem;
Button   ExportMem;
Checkbox AutoUpdate;
Button   Update;

struct {
  uint8  mode;
  uint32 addr, mask;
} status;

  bool Event(EventInfo &info);

  long wndproc_view(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  void Refresh();
  void ExportMemory();

  void Show();
  void Setup();

  MemoryEditor();
} wMemory;
