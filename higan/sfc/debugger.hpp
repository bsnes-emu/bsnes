#pragma once

namespace SuperFamicom {

struct Debugger {
  struct CPU {
    function<void (uint24, uint8)> read;
    function<void (uint24, uint8)> write;
    function<void (uint24)> execute;
    function<void ()> nmi;
    function<void ()> irq;
  } cpu;

  struct SMP {
    function<void (uint16, uint8)> read;
    function<void (uint16, uint8)> write;
    function<void (uint16)> execute;
  } smp;

  struct PPU {
    struct VRAM {
      function<void (uint17, uint8)> read;
      function<void (uint17, uint8)> write;
    } vram;

    struct OAM {
      function<void (uint10, uint8)> read;
      function<void (uint10, uint8)> write;
    } oam;

    struct CGRAM {
      function<void (uint9, uint8)> read;
      function<void (uint9, uint8)> write;
    } cgram;
  } ppu;
};

extern Debugger debugger;

}
