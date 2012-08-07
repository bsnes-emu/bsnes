#if !defined(_WIN32)
  #include <nds/nds.hpp>
  #include <sys/time.h>
#else
  // Suppress mingw's timeval which uses 32-bit longs
  #define _TIMEVAL_DEFINED
  #include <stdint.h>
  #include <sys/time.h>
  #include <nds/nds.hpp>

  struct timeval {
    int64_t tv_sec, tv_usec;
  };
  #define timegm(tm)          _mkgmtime64(tm)
  #define gmtime(tv)          _gmtime64(tv)
  #define gettimeofday(tv,tz) gettimeofday64(tv,tz)
  
  int gettimeofday64(struct timeval *tv, struct timezone *tz) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);  // UTC in 100ns units
    
    // UNIX epoch:     Jan 1 1970
    // Windows epoch:  Jan 1 1601
    //
    //   1970 - 1601   = 369 years = 11636784000 sec
    //   89 leap years => +89 days =    +7689600 sec
    int64_t diff = 11644473600LL * 10000000LL;
    int64_t ns = ft.dwLowDateTime + ft.dwHighDateTime * (1LL<<32) - diff;
    
    tv->tv_sec  = ns / 10000000;
    tv->tv_usec = ns % 10000000 / 10;
  }
#endif


namespace NintendoDS {

System system;


SPIDevice::~SPIDevice() { }


VRAMMapping::VRAMMapping() {
  data = nullptr; size = 0;
  dirtyBits = nullptr;
}
VRAMMapping::VRAMMapping(HalfMemory& target, uint8 *dirtyBits, unsigned pageno)
  : dirtyBits(dirtyBits + pageno*256/0x4000)
{
  data = target.data + 0x4000/2*pageno; size = 0;
}
VRAMMapping::~VRAMMapping() {
  data = nullptr; size = 0;
  dirtyBits = nullptr;
}

uint16& VRAMMapping::operator[](uint32 addr) {
  return data? data[addr>>1 & 0x1fff]
             : system.unmappedVram[addr>>1 & 0x1fff];
}

uint32 VRAMMapping::read(uint32 addr, uint32 size) {
  if(!data) return 0;
  return HalfMemory::read(addr & 0x3fff, size);
}
void VRAMMapping::write(uint32 addr, uint32 size, uint32 word) {
  if(!data) return;       HalfMemory::write(addr & 0x3fff, size, word);
  if(!dirtyBits) return;  dirtyBits[(addr & 0x3fff)/256] = 0xff;
}
bool VRAMMapping::dirty(uint32 addr) {
  return dirtyBits && dirtyBits[(addr & 0x3fff)/256];
}


System::System() {
  firmware.data  = new uint8 [(firmware.size  = 0x080000)/1]();
  
  ewram.data     = new uint16[(ewram.size    = 0x400000)/2];
  iwram.data     = new uint32[(iwram.size    = 0x010000)/4];
  swram[0].data  = new uint32[(swram[0].size = 0x004000)/4];
  swram[1].data  = new uint32[(swram[1].size = 0x004000)/4];
  
  vram[0].data   = new uint16[(vram[0].size  = 0x020000)/2];
  vram[1].data   = new uint16[(vram[1].size  = 0x020000)/2];
  vram[2].data   = new uint16[(vram[2].size  = 0x020000)/2];
  vram[3].data   = new uint16[(vram[3].size  = 0x020000)/2];
  vram[4].data   = new uint16[(vram[4].size  = 0x010000)/2];
  vram[5].data   = new uint16[(vram[5].size  = 0x004000)/2];
  vram[6].data   = new uint16[(vram[6].size  = 0x004000)/2];
  vram[7].data   = new uint16[(vram[7].size  = 0x008000)/2];
  vram[8].data   = new uint16[(vram[8].size  = 0x004000)/2];
  
  wxram.data     = new uint16[(wxram.size    = 0x002000)/2];
  
  memset(firmware.data,  0, firmware.size);
  
  callerThread = nullptr;
  activeThread = nullptr;
  running = false;
}

void System::mapVram(VRAMMapping* loc, uint8 *dirtyBits, unsigned npages, HalfMemory& bank) {
  for(unsigned offset = 0; offset < npages; offset++)
    loc[offset] = VRAMMapping{ bank, dirtyBits, offset % (bank.size/0x4000) };
}

void System::clearVmap() {
  for(unsigned n = 0; n < 8; n++) {
    for(auto &m : vmap.arm9[n]) m = VRAMMapping{};
  }
  for(auto &m : vmap.arm7)      m = VRAMMapping{};
  for(auto &m : vmap.tex)       m = VRAMMapping{};
  for(auto &m : vmap.texpal)    m = VRAMMapping{};
  for(auto &m : vmap.bgpal[0])  m = VRAMMapping{};
  for(auto &m : vmap.objpal[0]) m = VRAMMapping{};
  for(auto &m : vmap.bgpal[1])  m = VRAMMapping{};
  for(auto &m : vmap.objpal[1]) m = VRAMMapping{};
}

void System::updateVmap() {
  static const uint32 offsets[] = {
    0x00000, 0x20000, 0x40000, 0x60000, 0x80000, 0x90000, 0x94000, 0x98000, 0xa0000
  };
  static const uint32 altOffsets[] = {
    0x00000, 0x04000, 0x10000, 0x14000
  };
  
  clearVmap();
  gpu.texCache.reset();
  
  auto bank = vmap.regs;
  
  for(unsigned n = 0; n < 7; n++) {
    if(!bank[n].enable) continue;
    
    unsigned offset = offsets[n] >> 14;
    unsigned npages = vram[n].size >> 14;
    
    int sel = bank[n].bus;
    
    // Restrict MST to valid options (some are mirrored)
    if(n < 2 || n < 4 && sel > 4 || sel > 5) sel &= 3;
    
    if(sel == VRAMConfig::display)
      mapVram(&vmap.arm9[4][offset], &vmap.dirty[offsets[n]/256], npages, vram[n]);
  }
  
  // Banks 0-3, all are 128K, mapped at increments of the same size.
  for(unsigned n = 0; n < 4; n++) {
    if(!bank[n].enable) continue;
    
    unsigned offset = offsets[bank[n].offset] >> 14;
    unsigned npages = vram[n].size >> 14;
    
    uint8* dirty    = &vmap.dirty[offsets[n]/256];
    uint8* dirtyEnd = &vmap.dirty[offsets[n+1]/256];
    
    VRAMMapping *bus = nullptr;
    int sel = bank[n].bus;
    if(n < 2 || sel > 4) sel &= 3;
    
    if(          sel == VRAMConfig::bg0)  bus = &vmap.bg(0, offset);
    if(n <  2 && sel == VRAMConfig::obj0) bus = &vmap.obj(0, offset);
    if(n >= 2 && sel == VRAMConfig::arm7) bus = &vmap.arm7[offset];
    if(n == 2 && sel == VRAMConfig::ppu1) bus = &vmap.bg(1, 0);
    if(n == 3 && sel == VRAMConfig::ppu1) bus = &vmap.obj(1, 0);
    
    if(sel == VRAMConfig::tex) {
      bus = &vmap.tex[offset];
      
      // Discard dirty textures, then mark the locked pages clean. Lines
      // 191..213 are available for texture updates; games wishing to do so
      // will perform at least one {unlock, DMA, lock} cycle in that time.
      gpu.texCache.flushDirty(n);
      memset(dirty, 0, dirtyEnd-dirty);
    }
    if(bus) mapVram(bus, dirty, npages, vram[n]);
  }
  
  // Banks 4-6 are 32K + 16K + 16K and carry over from the GBA.
  // They always map into the first 128K of address space.
  for(unsigned n = 4; n < 7; n++) {
    if(!bank[n].enable) continue;
    
    uint8* dirty    = &vmap.dirty[offsets[n]/256];
    uint8* dirtyEnd = &vmap.dirty[offsets[n+1]/256];
    
    unsigned offset = n > 4? altOffsets[bank[n].offset] >> 14 : 0;
    unsigned npages = vram[n].size >> 14;
    
    VRAMMapping *bus = nullptr;
    int sel = bank[n].bus;
    if(sel > 5) sel &= 3;
    
    if(sel == VRAMConfig::bg0)     bus = &vmap.bg(0, offset);
    if(sel == VRAMConfig::obj0)    bus = &vmap.obj(0, offset);
    if(sel == VRAMConfig::bgpal0)  bus = &vmap.bgpal[0][offset];
    if(sel == VRAMConfig::objpal0) bus = &vmap.objpal[0][offset];
    
    if(sel == VRAMConfig::texpal) {
      bus = &vmap.texpal[offset];
      
      // The current implementation caches textures in ABGR format,
      // so we need to check for dirty palettes, too.
      gpu.texCache.flushDirty(n);
      memset(dirty, 0, dirtyEnd-dirty);
    }
    
    if(bus) {
      // Banks 5 and 6 (x) have          offset 0 [x x     ..............]
      // mirroring that allows them to          1 [ x x    ..128K banks..]
      // map adjacent to larger banks.          2 [    x x ..............]
      // Bank 4 (f) is fixed:                   3 [ffff x x..............]
                mapVram(bus + 0, dirty, npages, vram[n]);
      if(n > 4) mapVram(bus + 2, dirty, npages, vram[n]);
    }
  }
  
  // These two banks are only used with PPU1.
  for(unsigned n = 8; n < 10; n++) {
    if(!bank[n].enable) continue;
    
    uint8* dirty = &vmap.dirty[offsets[n-1]/256];
    int sel = bank[n].bus & 3;
    
    if(sel == VRAMConfig::bg1) {
      // These map in an odd sequence   [hh  hh  ]
      // where the Is mirror in pairs:  [  ii  ii]
      mapVram(&vmap.bg(1, n==9? 2 : 0), dirty, 2, vram[n-1]);
      mapVram(&vmap.bg(1, n==9? 6 : 4), dirty, 2, vram[n-1]);
    }
    if(n == 9 && sel == VRAMConfig::obj1)    mapVram(&vmap.obj(1, 0), dirty,    8, vram[n-1]);
    if(n == 8 && sel == VRAMConfig::bgpal1)  mapVram(&vmap.bgpal[1][0], dirty,  8, vram[n-1]);
    if(n == 9 && sel == VRAMConfig::objpal1) mapVram(&vmap.objpal[1][0], dirty, 8, vram[n-1]);
    if(n == 8 && sel == VRAMConfig::display) mapVram(&vmap.arm9[4][38], dirty,  2, vram[n-1]);
    if(n == 9 && sel == VRAMConfig::display) mapVram(&vmap.arm9[4][40], dirty,  1, vram[n-1]);
  }
  
  // Still unimplemented: bank[7] for SWRAM
  //   (currently both ARM7 and ARM9 have access)
  
  // Apply mirroring to address spaces smaller than 512K
  // Only needed in cases where it's possible to read out of bounds.
  for(unsigned n = 0; n < 8; n++) {
    // OBJ0 - 256K
    vmap.obj(0, n+16) = vmap.obj(0, n+0);
    vmap.obj(0, n+24) = vmap.obj(0, n+8);
    // BG1, OBJ1 - 128K
    vmap.bg(1, n+ 8)  = vmap.bg(1, n);
    vmap.bg(1, n+16)  = vmap.bg(1, n);
    vmap.bg(1, n+24)  = vmap.bg(1, n);
    vmap.obj(1, n+ 8) = vmap.obj(1, n);
    vmap.obj(1, n+16) = vmap.obj(1, n);
    vmap.obj(1, n+24) = vmap.obj(1, n);
  }
  
  for(unsigned i = 0; i < 4; i++)
    for(unsigned n = 0; n < 32; n++)
      vmap.arm9[i][n+32] = vmap.arm9[i][n+0];
}

uint32 System::regVmap(unsigned index) {
  if(index == 0) return vmap.regs[3]<<24 | vmap.regs[2]<<16 | vmap.regs[1]<<8 | vmap.regs[0]<<0;
  if(index == 1) return vmap.regs[7]<<24 | vmap.regs[6]<<16 | vmap.regs[5]<<8 | vmap.regs[4]<<0;
  if(index == 2) return                                       vmap.regs[9]<<8 | vmap.regs[8]<<0;
}

void System::regVmap(unsigned index, uint32 data, uint32 mask) {
  if(mask & 0x000000ff)              vmap.regs[4*index + 0] = data>>0;
  if(mask & 0x0000ff00)              vmap.regs[4*index + 1] = data>>8;
  if(mask & 0x00ff0000 && index < 2) vmap.regs[4*index + 2] = data>>16;
  if(mask & 0xff000000 && index < 2) vmap.regs[4*index + 3] = data>>24;
  
  updateVmap();
}



void System::loadArm7Bios(const stream& stream) {
  delete[] arm7.bios.data;
  
  arm7.bios.size = stream.size();
  arm7.bios.data = new uint32[(arm7.bios.size + 3)/4];
  
  for(unsigned n = 0; n < arm7.bios.size; n += 4)
    arm7.bios.write(n, Word, stream.readl(4));
}


void System::loadArm9Bios(const stream& stream) {
  delete[] arm9.bios.data;
  
  arm9.bios.size = stream.size();
  arm9.bios.data = new uint32[(arm9.bios.size + 3)/4];
  
  for(unsigned n = 0; n < arm9.bios.size; n += 4)
    arm9.bios.write(n, Word, stream.readl(4));
}


void System::loadFirmware(const stream& stream) {
  delete[] firmware.data;
  
  firmware.size = stream.size();
  firmware.data = new uint8[firmware.size];
  for(unsigned n = 0; n < firmware.size; n++)
    firmware.write(n, Byte, stream.read());
}

void System::saveFirmware(const stream& stream) {
  for(unsigned n = 0; n < firmware.size; n++)
    stream.write(firmware.read(n, Byte));
}


void System::loadRTC(const stream& stream) {
  struct timeval cur = {};
  gettimeofday(&cur, 0);
  
  clock.reset();
  clock.stoppedSec = cur.tv_sec;
  clock.stoppedUSec = cur.tv_usec;
  
  if(!stream.size()) return;
  
  uint8* xml = new uint8[stream.size() + 1];
  stream.read(xml, stream.size());
  xml[stream.size()] = 0;
  
  XML::Document document((const char*)xml);
  delete[] xml;
  
  if(document["rtc"].exists() == false)
    return;

  auto &rtc = document["rtc"];
  //string model = rtc["model"].data;
  
  if(rtc["settings"].exists()) {
    clock.status1  = hex(rtc["settings"]["status"].data);
    clock.status2  = hex(rtc["settings"]["mode"].data);
    clock.adjust   = hex(rtc["settings"]["adjust"].data);
    clock.userByte = hex(rtc["settings"]["scratch"].data);
  }
  if(rtc["clock"].exists()) {
    lstring date = rtc["clock"]["date"].data.split("/");
    lstring time = rtc["clock"]["time"].data.split(":");
    string  wday = rtc["clock"]["day"].data;
    
    if(date.size() >= 3 && time.size() >= 3) {
      string sec  = time[2];
      string frac = "0";
      
      if(sec.position(".")) {
        frac = sec.split(".")[1];
        sec  = sec.split(".")[0];
      }
      clock.weekday = hex(wday)    & 0x07;
      clock.year    = hex(date[0]) & 0xff;
      clock.month   = hex(date[1]) & 0x1f;
      clock.day     = hex(date[2]) & 0x3f;
      clock.hour    = hex(time[0]) & 0x3f;
      clock.minute  = hex(time[1]) & 0x7f;
      clock.second  = hex(sec)     & 0x7f;
      clock.counter = hex(frac);
    }
  }
  
  for(unsigned i = 0; i < 2; i++) {
    string alarm = {"alarm", i+1};
    if(rtc[alarm].exists() == false) continue;
    
    string  wday = rtc[alarm]["day"].data;
    lstring time = rtc[alarm]["time"].data.split(":");
    lstring mode = rtc[alarm]["mode"].data.split(",");
    
    clock.alarm[i].setting[0] = hex(wday) & 0x07;
    
    if(time.size() >= 2) {
      clock.alarm[i].setting[1] = hex(time[0]) & 0x7f;
      clock.alarm[i].setting[2] = hex(time[1]) & 0x7f;
    }
    for(auto &select : mode) {
      if(select.iequals("day"))    clock.alarm[i].weekday |= 0x80;
      if(select.iequals("hour"))   clock.alarm[i].hour    |= 0x80;
      if(select.iequals("minute")) clock.alarm[i].minute  |= 0x80;
    }
  }

  if(rtc["saved"].exists()) {
    lstring savedOn  = rtc["saved"]["on"].data.split("/");
    lstring savedAt  = rtc["saved"]["at"].data.split(":");
    string  savedDst = rtc["saved"]["dst"].data;
    
    if(savedOn.size() >= 3 && savedAt.size() >= 3) {
      struct tm last = {};
      string sec = savedAt[2];
      string usec = "0";
      
      if(sec.position(".")) {
        usec = sec.split(".")[1];
        sec  = sec.split(".")[0];
      }
      last.tm_isdst = savedDst != ""? integer(savedDst) : -1;
      last.tm_yday  = -1;
      last.tm_year  = decimal(savedOn[0]) - 1900;
      last.tm_mon   = decimal(savedOn[1]) - 1;
      last.tm_mday  = decimal(savedOn[2]);
      last.tm_wday  = -1;
      last.tm_hour  = decimal(savedAt[0]);
      last.tm_min   = decimal(savedAt[1]);
      last.tm_sec   = decimal(sec);
      
      struct timeval now = {};
      gettimeofday(&now, nullptr);
      
      clock.freeze(timegm(&last), decimal(usec));
      clock.thaw(now.tv_sec, now.tv_usec);
    }
  }
}


void System::saveRTC(const stream& stream) {
  struct timeval tv = {};
  gettimeofday(&tv, nullptr);
  clock.thaw(tv.tv_sec, tv.tv_usec);
  clock.freeze(tv.tv_sec, tv.tv_usec);
  
  struct tm now = *gmtime(&tv.tv_sec);
  
  string saveDate = { decimal(1900 + now.tm_year), "/", decimal<2,'0'>(now.tm_mon+1), "/", decimal<2,'0'>(now.tm_mday) };
  string saveTime = { decimal<2,'0'>(now.tm_hour), ":", decimal<2,'0'>(now.tm_min),   ":", decimal<2,'0'>(now.tm_sec), ".", decimal<6,'0'>(tv.tv_usec) };
  
  string dateStr  = { hex<4>(0x2000 + clock.year), "/", hex<2>(clock.month), "/", hex<2>(clock.day) };
  string timeStr  = {          hex<2>(clock.hour), ":", hex<2>(clock.minute), ":", hex<2>(clock.second), ".", hex<4>(clock.counter) };
  
  string alarmTime[2], alarmDay[2], alarmMode[2];
  
  for(unsigned i = 0; i < 2; i++) {
    lstring conditions;
    if(clock.alarm[i].weekday & 0x80) conditions[i].append("day");
    if(clock.alarm[i].hour    & 0x80) conditions[i].append("hour");
    if(clock.alarm[i].minute  & 0x80) conditions[i].append("minute");
    
    alarmMode[i] = conditions.concatenate(",");
    alarmDay[i]  =   hex<1>(clock.alarm[i].weekday & 7);
    alarmTime[i] = { hex<2>(clock.alarm[i].hour   & 0x3f),
                ":", hex<2>(clock.alarm[i].minute & 0x7f) };
  };
  
  string xml{
    "<?xml version='1.0' encoding='UTF-8'?>\n",
    "<rtc>\n",
    "  <saved on='", saveDate, "'",
            " at='", saveTime, "'",
            " dst='", now.tm_isdst, "'/>\n",
    "\n",
    "  <settings status='",  hex<2>(clock.status1), "'",
               " mode='",    hex<2>(clock.status2), "'",
               " scratch='", hex<2>(clock.userByte), "'",
               " adjust='",  hex<2>(clock.adjust), "'/>\n",
    "\n",
    "  <clock date='", dateStr, "'",
            " day='",  hex<1>(clock.weekday), "'",
            " time='", timeStr, "'/>\n",
    "\n",
    "  <alarm1 time='", alarmTime[0], "' day='", alarmDay[0], "' mode='", alarmMode[0], "'/>\n",
    "  <alarm2 time='", alarmTime[1], "' day='", alarmDay[1], "' mode='", alarmMode[1], "'/>\n",
    "</rtc>"
  };
  stream.write((uint8*)(const char*)xml, xml.length());
}



void System::power() {
  // Clear memory
  memset(ewram.data,         0, ewram.size);
  memset(iwram.data,         0, iwram.size);
  memset(swram[0].data,      0, swram[0].size);
  memset(swram[1].data,      0, swram[1].size);
  memset(unmappedVram,       0, sizeof unmappedVram);
  
  for(unsigned n = 0; n < 9; n++)
    memset(vram[n].data, 0, vram[n].size);
  
  // Reset VRAM mappings
  for(auto &r : vmap.regs) r = 0x00;
  clearVmap();
  memset(vmap.dirty, 0xff, sizeof vmap.dirty);
  
  arm9.config.arm9 = true;
  arm9.config.arm7 = false;
  arm7.config.arm9 = false;
  arm7.config.arm7 = true;
  arm9.other = &arm7;
  arm7.other = &arm9;
  
  ppu[0].which = 0;
  ppu[1].which = 1;
  
  arm7.power();
  arm9.power();
  clock.power();
  powerMgr.power();
  firmware.power();
  touchscreen.power();
  slot1.power();
  video.power();
  gpu.power();
  ppu[0].power();
  ppu[1].power();
  apu.power();
  wifi.power();
  
  callerThread = co_active();
  activeThread = arm7.thread;  // Required since right now ARM7 loads the binaries..
}

void System::run() {
  callerThread = co_active();
  co_switch(activeThread);
}

void System::frame() {
  interface->videoRefresh(video.output, 256*4, 256, 384);
  activeThread = co_active();
  co_switch(callerThread);
}

#include "clock.cpp"
#include "powermgr.cpp"
#include "touchscreen.cpp"

}
