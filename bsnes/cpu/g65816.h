/*
  regs.dc is not a real register. it is used to store the data counter
  for reading from / writing to memory locations. it is also needed
  to emulate slowrom / fastrom, as the speed varies depending on the
  location where the memory was accessed.
*/

class g65816 {
public:
//cpu declarations
  struct {
    ulong pc, dc;
    union {
      byte b;
      word w;
      struct { byte l, h; }p;
    }a;
    word  x, y, d, s;
    byte  db, p;
    bool  e;
  }regs;
  byte  memory_speed;
  byte  toggle_memory_speed;
  bool  index_bank_crossed;

  byte  nmi_pin;
  bool  nmi_enabled;

  bool  wai_interrupt_occurred;

//memory declarations
  byte *wram, *sram, *rom;
  ulong map;
  ulong sram_size;
//debug declarations
  struct {
    ulong offset;
    byte  flags;
    byte  value;
    ulong hit_count;
  }bp_list[16];

//cpu functions (cpu/g65816.cpp)
  void  Run(void);
  void  LoadROM(void);
  void  PowerOn(byte first_time);
  void  Reset(void);
  void  InvokeIRQ(word addr);

//memory functions (mem/memory.cpp)
  void  InitializeROM(byte memory_map);
  void  InitializeWRAM(byte value);
  ulong mirror_offset(ulong offset);
  ulong convert_offset(byte read_mode, ulong addr, bool mirror = true);
  ulong adjust_base_offset(byte read_mode, ulong addr);
  ulong read_indirect_address(byte read_mode, ulong addr);
  ulong get_dc(byte read_mode, ulong addr);
  byte  mem_getbyte_direct(ulong addr, byte access_mode = MEMACCESS_NORMAL);
  byte  mem_getbyte(ulong addr, byte access_mode = MEMACCESS_NORMAL);
  void  mem_putbyte_direct(ulong addr, byte value, byte access_mode = MEMACCESS_NORMAL);
  void  mem_putbyte(ulong addr, byte value, byte access_mode = MEMACCESS_NORMAL);
  ulong mem_read(byte read_mode, byte read_size, ulong addr, byte access_mode = MEMACCESS_NORMAL);
  void  mem_write(byte write_mode, byte write_size, ulong addr, ulong value, byte access_mode = MEMACCESS_NORMAL);
  ulong rom_read(ulong addr, byte read_size);
  void  rom_write(ulong addr, ulong v, byte write_size);

  g65816();
  ~g65816();
};
