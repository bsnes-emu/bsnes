#include "../base.h"
#include "../timing/timing.h"
#include "g65816.h"

extern emustate    emu_state;
extern debugstate  debugger;
extern ppustate    ppu;
extern snes_timer *snes_time;
g65816            *gx816;

#include "g65816_ops.cpp"

void g65816::LoadROM(void) {
FILE *fp;
ulong fsize;
word  header_offset = 0;
word  cksum, icksum;
byte  t;
int   i;
  dprintf("* Loading [%s]...", emu_state.rom_name);

  fp = fopen(emu_state.rom_name, "rb");
  if(!fp)return;

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  map = MEMMAP_LOROM;

  if((fsize & 0x000fff) == 0x000200)header_offset = 512;

  fseek(fp, 0x7fdc + header_offset, SEEK_SET);
  cksum  = fgetc(fp) | fgetc(fp) << 8;
  icksum = fgetc(fp) | fgetc(fp) << 8;
  if(cksum + icksum == 0xffff)map = MEMMAP_LOROM;

  fseek(fp, 0xffdc + header_offset, SEEK_SET);
  cksum  = fgetc(fp) | fgetc(fp) << 8;
  icksum = fgetc(fp) | fgetc(fp) << 8;
  if(cksum + icksum == 0xffff)map = MEMMAP_HIROM;

  fseek(fp, 0 + header_offset, SEEK_SET);
  fsize -= header_offset;

  dprintf("* ROM detected as: %s", (map == MEMMAP_LOROM)?"LoROM":"HiROM");

  InitializeROM(map);

  fread(rom, 1, fsize, fp);

  t = (mem_read(MEMMODE_NONE, MEMSIZE_BYTE, 0x00ffd8) & 7);
  switch(t) {
  case 0: sram_size =   0;       break;
  case 1: sram_size =   2 * 1024;break;
  case 2: sram_size =   4 * 1024;break;
  case 3: sram_size =   8 * 1024;break;
  case 4: sram_size =  16 * 1024;break;
  case 5: sram_size =  32 * 1024;break;
  case 6: sram_size =  64 * 1024;break;
  case 7: sram_size = 128 * 1024;break;
  default:sram_size =   0;       break;
  }
  dprintf("* SRAM Size: %dkb", sram_size / 1024);

//pbr is loaded with 00, and 16-bit pc is loaded with reset vector at 0xfffc
//upon power on and at first reset
  regs.pc = mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00fffc);

  fclose(fp);

  if(sram_size != 0) {
    fp = fopen(emu_state.sram_name, "rb");

//create sram file if it does not exist
    if(!fp) {
      fp = fopen(emu_state.sram_name, "wb");
      for(i=0;i<sram_size;i++) {
        fputc(0, fp);
      }
      fclose(fp);
      fp = fopen(emu_state.sram_name, "rb");
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

//fix sram size if it is smaller than actual size
    if(fsize < sram_size) {
      fclose(fp);
      fp = fopen(emu_state.sram_name, "rb+wb");
      fseek(fp, 0, SEEK_END);
      while(fsize < sram_size) {
        fputc(0, fp);
        fsize++;
      }
      fclose(fp);
      fp = fopen(emu_state.sram_name, "rb");
    }

//read file into sram
    fseek(fp, 0, SEEK_SET);
    fread(sram, 1, sram_size, fp);
    fclose(fp);
  }
}

void g65816::PowerOn(byte first_time) {
//default register states
  regs.a.w = regs.x = regs.y = 0x0000;
  regs.d   = 0x0000;
  regs.s   = 0x01ff;
  regs.db  = 0x00;
  regs.p   = 0x34;
  regs.e   = true;
  snes_time->master_cycles = 0;
  memory_speed             = MEMSPEED_SLOWROM;
  wai_interrupt_occurred   = false;

  InitializeWRAM(0x00);
  PPUInit(first_time);
  UpdateDisplay();
  snes_time->set_speed_map(MEMSPEED_SLOWROM);
  cpu_state = CPUSTATE_RUN;
}

void g65816::Reset(void) {
  regs.x &= 0xff;
  regs.y &= 0xff;
  regs.s  = 0x0100 | (regs.s & 0xff);
  regs.d  = 0x0000;
  regs.db = 0x00;
  regs.p  = 0x34;
  regs.e  = true;
  regs.pc = mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00fffc);
  snes_time->master_cycles = 0;
  memory_speed             = MEMSPEED_SLOWROM;
  wai_interrupt_occurred   = false;

  PPUInit(0); //0 blocks reallocating memory for vram, cgram, etc.
  UpdateDisplay();
  snes_time->set_speed_map(MEMSPEED_SLOWROM);
  cpu_state = CPUSTATE_RUN;
}

/***********
 *** IRQ ***
 ***********
cycles:
  [1] pbr,pc ; io
  [2] pbr,pc ; io
  [3] 0,s    ; pbr
  [4] 0,s-1  ; pch
  [5] 0,s-2  ; pcl
  [6] 0,s-3  ; p
  [7] 0,va   ; aavl
  [8] 0,va+1 ; aavh
*/

void g65816::InvokeIRQ(word addr) {
  snes_time->add_cpu_icycles(2);                           //1,2 [i/o]
  gx816->stack_write(gx816->regs.pc >> 16);                //3 [write pbr]
  gx816->stack_write(gx816->regs.pc >> 8);                 //4 [write pch]
  gx816->stack_write(gx816->regs.pc);                      //5 [write pcl]
  gx816->stack_write(gx816->regs.p);                       //6 [write p]
  gx816->op.r.p.l = gx816->op_read(OPMODE_ADDR, addr);     //7 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_ADDR, addr + 1); //8 [read high]

  gx816->regs.pc = gx816->op.r.w;
  gx816->regs.p |= PF_I;
  wai_interrupt_occurred = true;
  snes_time->update_timer();
}

void g65816::exec_op(void) {
FILE *fp;
byte  op;
int   i;
static ulong sram_save_tick = 0;
  if(cpu_state == CPUSTATE_RUN || cpu_state == CPUSTATE_STP) {
    op = mem_read(MEMMODE_NONE, MEMSIZE_BYTE, gx816->regs.pc);
    if(regs.e == true)g65816_optbl_e[op]();
    else switch((regs.p & 0x30)) {
    case 0x30:g65816_optbl_MX[op]();break;
    case 0x20:g65816_optbl_Mx[op]();break;
    case 0x10:g65816_optbl_mX[op]();break;
    case 0x00:g65816_optbl_mx[op]();break;
    }
    debugger.cpu_op_executed = true;
    debugger.disas_cpu_op    = true;
    snes_time->update_timer();
  } else if(cpu_state == CPUSTATE_DMA) {
    ppu_update_dma();
    snes_time->update_timer();
  }

  snes_time->update_timer_events();
  ppu_update_scanline();

//see if we need to backup sram to a file yet...
  if(sram_size != 0) {
    sram_save_tick++;
    if(sram_save_tick >= emu_state.sram_save_tick_count) {
      sram_save_tick = 0;
      fp = fopen(emu_state.sram_name, "wb");
      fwrite(sram, 1, sram_size, fp);
      fclose(fp);
    }
  }

  debug_test_bp(BPSRC_MEM, BP_EXEC, gx816->regs.pc, 0);
}

void g65816::Run(void) {
  if(snes_time->bridge.apu_cycles >= snes_time->bridge.cpu_cycles) {
    exec_op();
  }
  if(snes_time->bridge.cpu_cycles >= 65536 && snes_time->bridge.apu_cycles >= 65536) {
    snes_time->bridge.cpu_cycles &= 65535;
    snes_time->bridge.apu_cycles &= 65535;
  }
}

g65816::g65816() {
int   i;
  rom  = (byte*)malloc(0x600000);
  wram = (byte*)malloc(0x020000);
  sram = (byte*)malloc(0x0e0000);
  nmi_enabled = false;
  nmi_pin     = 0;

  op.dp = op.sp = 0x00;
  op.r.l  = 0x000000;
  op.aa.l = 0x000000;
}

g65816::~g65816() {
  if(rom)free(rom);
  if(wram)free(wram);
  if(sram)free(sram);
}
