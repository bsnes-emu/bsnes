class SNES {
protected:
bool is_debugger_enabled;

uint8 snes_region;

//APU synchronization
struct {
int32 cpu_freq, apu_freq;
int32 cpu_multbl[1024], apu_multbl[1024];
int32 cycles;
}apusync;

void update_timing();

public:
enum { NTSC = 0, PAL = 1 };

//system functions
  void run();
  virtual void  render_frame() = 0;
  virtual void  init();
  virtual void  power();
  virtual void  reset();
  virtual uint8 region();
  virtual void  set_region(uint8 new_region);

//input functions
enum {
  DEV_JOYPAD1 = 0,
  DEV_JOYPAD2 = 1
};
enum {
  JOYPAD_B      =  0, JOYPAD_Y      =  1,
  JOYPAD_SELECT =  2, JOYPAD_START  =  3,
  JOYPAD_UP     =  4, JOYPAD_DOWN   =  5,
  JOYPAD_LEFT   =  6, JOYPAD_RIGHT  =  7,
  JOYPAD_A      =  8, JOYPAD_X      =  9,
  JOYPAD_L      = 10, JOYPAD_R      = 11
};
  virtual void poll_input() = 0;
  virtual bool get_input_status(uint8 device, uint8 button) = 0;

//debugging functions
enum {
  NO_ACTION = 0,
  RENDER_FRAME,
  CPU_EXEC_OPCODE_BEGIN, CPU_EXEC_OPCODE_END,
  APU_EXEC_OPCODE_BEGIN, APU_EXEC_OPCODE_END,
  MEM_READ,    MEM_WRITE,
  SPCRAM_READ, SPCRAM_WRITE,
  VRAM_READ,   VRAM_WRITE,
  OAM_READ,    OAM_WRITE,
  CGRAM_READ,  CGRAM_WRITE,
};
  virtual void notify(uint32 message, uint32 param1 = 0, uint32 param2 = 0);
  virtual void debugger_enable();
  virtual void debugger_disable();
  virtual bool debugger_enabled();

  SNES();
};
