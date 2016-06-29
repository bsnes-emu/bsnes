auto PPU::readIO(uint32 addr) -> uint8 {
  auto bgcnt = [&]() -> Registers::Background::Control& { return regs.bg[addr.bits(1,2)].control; };
  auto wf = [&]() -> Registers::WindowFlags& {
    static uint id[] = {In0, In1, Out, Obj};
    return regs.windowflags[id[addr.bits(0,1)]];
  };

  switch(addr) {

  //DISPCNT
  case 0x0400'0000: return (
    regs.control.bgmode     << 0
  | regs.control.cgbmode    << 3
  | regs.control.frame      << 4
  | regs.control.hblank     << 5
  | regs.control.objmapping << 6
  | regs.control.forceblank << 7
  );
  case 0x0400'0001: return (
    regs.control.enable[BG0]       << 0
  | regs.control.enable[BG1]       << 1
  | regs.control.enable[BG2]       << 2
  | regs.control.enable[BG3]       << 3
  | regs.control.enable[OBJ]       << 4
  | regs.control.enablewindow[In0] << 5
  | regs.control.enablewindow[In1] << 6
  | regs.control.enablewindow[Obj] << 7
  );

  //GRSWP
  case 0x0400'0002: return regs.greenswap;
  case 0x0400'0003: return 0;

  //DISPSTAT
  case 0x0400'0004: return (
    regs.status.vblank          << 0
  | regs.status.hblank          << 1
  | regs.status.vcoincidence    << 2
  | regs.status.irqvblank       << 3
  | regs.status.irqhblank       << 4
  | regs.status.irqvcoincidence << 5
  );
  case 0x0400'0005: return (
    regs.status.vcompare
  );

  //VCOUNT
  case 0x0400'0006: return regs.vcounter.byte(0);
  case 0x0400'0007: return regs.vcounter.byte(1);

  //BG0CNT, BG1CNT, BG2CNT, BG3CNT
  case 0x0400'0008: case 0x0400'000a: case 0x0400'000c: case 0x0400'000e: return (
    bgcnt().priority           << 0
  | bgcnt().characterbaseblock << 2
  | bgcnt().unused             << 4
  | bgcnt().mosaic             << 6
  | bgcnt().colormode          << 7
  );
  case 0x0400'0009: case 0x0400'000b: case 0x0400'000d: case 0x0400'000f: return (
    bgcnt().screenbaseblock << 0
  | bgcnt().affinewrap      << 5
  | bgcnt().screensize      << 6
  );

  //WININ, WINOUT
  case 0x0400'0048: case 0x0400'0049: case 0x0400'004a: case 0x0400'004b: return (
    wf().enable[BG0] << 0
  | wf().enable[BG1] << 1
  | wf().enable[BG2] << 2
  | wf().enable[BG3] << 3
  | wf().enable[OBJ] << 4
  | wf().enable[SFX] << 4
  );

  //BLTCNT
  case 0x0400'0050: return (
    regs.blend.control.above[BG0] << 0
  | regs.blend.control.above[BG1] << 1
  | regs.blend.control.above[BG2] << 2
  | regs.blend.control.above[BG3] << 3
  | regs.blend.control.above[OBJ] << 4
  | regs.blend.control.above[SFX] << 5
  | regs.blend.control.mode       << 6
  );
  case 0x0400'0051: return (
    regs.blend.control.below[BG0] << 0
  | regs.blend.control.below[BG1] << 1
  | regs.blend.control.below[BG2] << 2
  | regs.blend.control.below[BG3] << 3
  | regs.blend.control.below[OBJ] << 4
  | regs.blend.control.below[SFX] << 5
  );

  //BLDALPHA
  case 0x0400'0052: return regs.blend.eva;
  case 0x0400'0053: return regs.blend.evb;

  //BLDY is write-only

  }

  return 0;
}

auto PPU::writeIO(uint32 addr, uint8 data) -> void {
  auto bgcnt = [&]() -> Registers::Background::Control& { return regs.bg[addr.bits(1,2)].control; };
  auto bgofs = [&]() -> Registers::Background& { return regs.bg[addr.bits(2,3)]; };
  auto bg = [&]() -> Registers::Background& { return regs.bg[addr.bits(4,5)]; };
  auto wf = [&]() -> Registers::WindowFlags& {
    static uint id[] = {In0, In1, Out, Obj};
    return regs.windowflags[id[addr.bits(0,1)]];
  };

  switch(addr) {

  //DISPCNT
  case 0x0400'0000:
    regs.control.bgmode     = data.bits(0,2);
    regs.control.cgbmode    = data.bit (3);
    regs.control.frame      = data.bit (4);
    regs.control.hblank     = data.bit (5);
    regs.control.objmapping = data.bit (6);
    regs.control.forceblank = data.bit (7);
    return;
  case 0x0400'0001:
    regs.control.enable[BG0]       = data.bit(0);
    regs.control.enable[BG1]       = data.bit(1);
    regs.control.enable[BG2]       = data.bit(2);
    regs.control.enable[BG3]       = data.bit(3);
    regs.control.enable[OBJ]       = data.bit(4);
    regs.control.enablewindow[In0] = data.bit(5);
    regs.control.enablewindow[In1] = data.bit(6);
    regs.control.enablewindow[Obj] = data.bit(7);
    return;

  //GRSWP
  case 0x0400'0002:
    regs.greenswap = data.bit(0);
    return;
  case 0x0400'0003: return;

  //DISPSTAT
  case 0x0400'0004:
    regs.status.irqvblank       = data.bit(3);
    regs.status.irqhblank       = data.bit(4);
    regs.status.irqvcoincidence = data.bit(5);
    return;
  case 0x0400'0005:
    regs.status.vcompare = data;
    return;

  //BG0CNT, BG1CNT, BG2CNT, BG3CNT
  case 0x0400'0008: case 0x0400'000a: case 0x0400'000c: case 0x0400'000e:
    bgcnt().priority           = data.bits(0,1);
    bgcnt().characterbaseblock = data.bits(2,3);
    bgcnt().unused             = data.bits(4,5);
    bgcnt().mosaic             = data.bit (6);
    bgcnt().colormode          = data.bit (7);
    return;
  case 0x0400'0009: case 0x0400'000b: case 0x0400'000d: case 0x0400'000f:
    if(addr.bits(1,2) <= 1) data.bit(5) = 0;  //clear affine wrap for BG0, BG1
    bgcnt().screenbaseblock = data.bits(0,4);
    bgcnt().affinewrap      = data.bit (5);
    bgcnt().screensize      = data.bits(6,7);
    return;

  //BG0HOFS, BG1HOFS, BG2BOFS, BG3HOFS
  case 0x0400'0010: case 0x0400'0014: case 0x0400'0018: case 0x0400'001c:
    bgofs().hoffset.bits(0,7) = data;
    return;
  case 0x0400'0011: case 0x0400'0015: case 0x0400'0019: case 0x0400'001d:
    bgofs().hoffset.bit(8) = data.bit(0);
    return;

  //BG0VOFS, BG1VOFS, BG2VOFS, BG3VOFS
  case 0x0400'0012: case 0x0400'0016: case 0x0400'001a: case 0x0400'001e:
    bgofs().voffset.bits(0,7) = data;
    return;
  case 0x0400'0013: case 0x0400'0017: case 0x0400'001b: case 0x0400'001f:
    bgofs().voffset.bit(8) = data.bit(0);
    return;

  //BG2PA, BG3PA
  case 0x0400'0020: case 0x0400'0030: bg().pa.byte(0) = data; return;
  case 0x0400'0021: case 0x0400'0031: bg().pa.byte(1) = data; return;

  //BG2PB, BG3PB
  case 0x0400'0022: case 0x0400'0032: bg().pb.byte(0) = data; return;
  case 0x0400'0023: case 0x0400'0033: bg().pb.byte(1) = data; return;

  //BG2PC, BG3PC
  case 0x0400'0024: case 0x0400'0034: bg().pc.byte(0) = data; return;
  case 0x0400'0025: case 0x0400'0035: bg().pc.byte(1) = data; return;

  //BG2PD, BG3PD
  case 0x0400'0026: case 0x0400'0036: bg().pd.byte(0) = data; return;
  case 0x0400'0027: case 0x0400'0037: bg().pd.byte(1) = data; return;

  //BG2X_L, BG2X_H, BG3X_L, BG3X_H
  case 0x0400'0028: case 0x0400'0038: bg().x.bits( 0, 7) = data.bits(0,7); bg().lx = bg().x; return;
  case 0x0400'0029: case 0x0400'0039: bg().x.bits( 8,15) = data.bits(0,7); bg().lx = bg().x; return;
  case 0x0400'002a: case 0x0400'003a: bg().x.bits(16,23) = data.bits(0,7); bg().lx = bg().x; return;
  case 0x0400'002b: case 0x0400'003b: bg().x.bits(24,27) = data.bits(0,3); bg().lx = bg().x; return;

  //BG2Y_L, BG2Y_H, BG3Y_L, BG3Y_H
  case 0x0400'002c: case 0x0400'003c: bg().y.bits( 0, 7) = data.bits(0,7); bg().ly = bg().y; return;
  case 0x0400'002d: case 0x0400'003d: bg().y.bits( 8,15) = data.bits(0,7); bg().ly = bg().y; return;
  case 0x0400'002e: case 0x0400'003e: bg().y.bits(16,23) = data.bits(0,7); bg().ly = bg().y; return;
  case 0x0400'002f: case 0x0400'003f: bg().y.bits(24,27) = data.bits(0,3); bg().ly = bg().y; return;

  //WIN0H
  case 0x0400'0040: regs.window[0].x2 = data; return;
  case 0x0400'0041: regs.window[0].x1 = data; return;

  //WIN1H
  case 0x0400'0042: regs.window[1].x2 = data; return;
  case 0x0400'0043: regs.window[1].x1 = data; return;

  //WIN0V
  case 0x0400'0044: regs.window[0].y2 = data; return;
  case 0x0400'0045: regs.window[0].y1 = data; return;

  //WIN1V
  case 0x0400'0046: regs.window[1].y2 = data; return;
  case 0x0400'0047: regs.window[1].y1 = data; return;

  //WININ, WINOUT
  case 0x0400'0048: case 0x0400'0049: case 0x0400'004a: case 0x0400'004b:
    wf().enable[BG0] = data.bit(0);
    wf().enable[BG1] = data.bit(1);
    wf().enable[BG2] = data.bit(2);
    wf().enable[BG3] = data.bit(3);
    wf().enable[OBJ] = data.bit(4);
    wf().enable[SFX] = data.bit(5);
    return;

  //MOSAIC
  case 0x0400'004c:
    regs.mosaic.bghsize = data.bits(0,3);
    regs.mosaic.bgvsize = data.bits(4,7);
    return;
  case 0x0400'004d:
    regs.mosaic.objhsize = data.bits(0,3);
    regs.mosaic.objvsize = data.bits(4,7);
    return;

  //BLDCNT
  case 0x0400'0050:
    regs.blend.control.above[BG0] = data.bit (0);
    regs.blend.control.above[BG1] = data.bit (1);
    regs.blend.control.above[BG2] = data.bit (2);
    regs.blend.control.above[BG3] = data.bit (3);
    regs.blend.control.above[OBJ] = data.bit (4);
    regs.blend.control.above[SFX] = data.bit (5);
    regs.blend.control.mode       = data.bits(6,7);
    return;
  case 0x0400'0051:
    regs.blend.control.below[BG0] = data.bit(0);
    regs.blend.control.below[BG1] = data.bit(1);
    regs.blend.control.below[BG2] = data.bit(2);
    regs.blend.control.below[BG3] = data.bit(3);
    regs.blend.control.below[OBJ] = data.bit(4);
    regs.blend.control.below[SFX] = data.bit(5);
    return;

  //BLDALPHA
  case 0x0400'0052: regs.blend.eva = data.bits(0,4); return;
  case 0x0400'0053: regs.blend.evb = data.bits(0,4); return;

  //BLDY
  case 0x0400'0054: regs.blend.evy = data.bits(0,4); return;
  case 0x0400'0055: return;

  }
}
