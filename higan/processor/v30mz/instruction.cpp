auto V30MZ::interrupt(uint8 vector) -> void {
  wait(32);

  state.halt = false;
  state.poll = true;
  state.prefix = false;

  //if an IRQ fires during a rep string instruction;
  //flush prefix queue and seek back to first prefix.
  //this allows the transfer to resume after the IRQ.
  if(prefixes) {
    r.ip -= prefixes.size();
    prefixes.reset();
  }

  auto ip = read(Word, 0x0000, vector * 4 + 0);
  auto cs = read(Word, 0x0000, vector * 4 + 2);

  push(r.f);
  push(r.cs);
  push(r.ip);

  r.f.m = true;
  r.f.i = false;
  r.f.b = false;

  r.ip = ip;
  r.cs = cs;
}

#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);

auto V30MZ::instruction() -> void {
  switch(opcode = fetch()) {
  op(0x00, AddMemReg, Byte)
  op(0x01, AddMemReg, Word)
  op(0x02, AddRegMem, Byte)
  op(0x03, AddRegMem, Word)
  op(0x04, AddAccImm, Byte)
  op(0x05, AddAccImm, Word)
  op(0x06, PushReg, r.es)
  op(0x07, PopReg, r.es)
  op(0x08, OrMemReg, Byte)
  op(0x09, OrMemReg, Word)
  op(0x0a, OrRegMem, Byte)
  op(0x0b, OrRegMem, Word)
  op(0x0c, OrAccImm, Byte)
  op(0x0d, OrAccImm, Word)
  op(0x0e, PushReg, r.cs)
  op(0x0f, PopReg, r.cs)
  op(0x10, AdcMemReg, Byte)
  op(0x11, AdcMemReg, Word)
  op(0x12, AdcRegMem, Byte)
  op(0x13, AdcRegMem, Word)
  op(0x14, AdcAccImm, Byte)
  op(0x15, AdcAccImm, Word)
  op(0x16, PushReg, r.ss)
  op(0x17, PopReg, r.ss)
  op(0x18, SbbMemReg, Byte)
  op(0x19, SbbMemReg, Word)
  op(0x1a, SbbRegMem, Byte)
  op(0x1b, SbbRegMem, Word)
  op(0x1c, SbbAccImm, Byte)
  op(0x1d, SbbAccImm, Word)
  op(0x1e, PushReg, r.ds)
  op(0x1f, PopReg, r.ds)
  op(0x20, AndMemReg, Byte)
  op(0x21, AndMemReg, Word)
  op(0x22, AndRegMem, Byte)
  op(0x23, AndRegMem, Word)
  op(0x24, AndAccImm, Byte)
  op(0x25, AndAccImm, Word)
  op(0x26, Segment, r.es)
  op(0x27, DecimalAdjust, 0)  //daa
  op(0x28, SubMemReg, Byte)
  op(0x29, SubMemReg, Word)
  op(0x2a, SubRegMem, Byte)
  op(0x2b, SubRegMem, Word)
  op(0x2c, SubAccImm, Byte)
  op(0x2d, SubAccImm, Word)
  op(0x2e, Segment, r.cs)
  op(0x2f, DecimalAdjust, 1)  //das
  op(0x30, XorMemReg, Byte)
  op(0x31, XorMemReg, Word)
  op(0x32, XorRegMem, Byte)
  op(0x33, XorRegMem, Word)
  op(0x34, XorAccImm, Byte)
  op(0x35, XorAccImm, Word)
  op(0x36, Segment, r.ss)
  op(0x37, AsciiAdjust, 0)  //aaa
  op(0x38, CmpMemReg, Byte)
  op(0x39, CmpMemReg, Word)
  op(0x3a, CmpRegMem, Byte)
  op(0x3b, CmpRegMem, Word)
  op(0x3c, CmpAccImm, Byte)
  op(0x3d, CmpAccImm, Word)
  op(0x3e, Segment, r.ds)
  op(0x3f, AsciiAdjust, 1)  //aas
  op(0x40, IncReg, r.ax)
  op(0x41, IncReg, r.cx)
  op(0x42, IncReg, r.dx)
  op(0x43, IncReg, r.bx)
  op(0x44, IncReg, r.sp)
  op(0x45, IncReg, r.bp)
  op(0x46, IncReg, r.si)
  op(0x47, IncReg, r.di)
  op(0x48, DecReg, r.ax)
  op(0x49, DecReg, r.cx)
  op(0x4a, DecReg, r.dx)
  op(0x4b, DecReg, r.bx)
  op(0x4c, DecReg, r.sp)
  op(0x4d, DecReg, r.bp)
  op(0x4e, DecReg, r.si)
  op(0x4f, DecReg, r.di)
  op(0x50, PushReg, r.ax)
  op(0x51, PushReg, r.cx)
  op(0x52, PushReg, r.dx)
  op(0x53, PushReg, r.bx)
  op(0x54, PushReg, r.sp)
  op(0x55, PushReg, r.bp)
  op(0x56, PushReg, r.si)
  op(0x57, PushReg, r.di)
  op(0x58, PopReg, r.ax)
  op(0x59, PopReg, r.cx)
  op(0x5a, PopReg, r.dx)
  op(0x5b, PopReg, r.bx)
  op(0x5c, PopReg, r.sp)
  op(0x5d, PopReg, r.bp)
  op(0x5e, PopReg, r.si)
  op(0x5f, PopReg, r.di)
  op(0x60, PushAll)
  op(0x61, PopAll)
  op(0x62, Bound)
//op(0x63, ...)
//op(0x64, ...)  repnc
//op(0x65, ...)  repc
//op(0x66, ...)  fpo2
//op(0x67, ...)  fpo2
  op(0x68, PushImm, Word)
  op(0x69, MultiplySignedRegMemImm, Word)
  op(0x6a, PushImm, Byte)
  op(0x6b, MultiplySignedRegMemImm, Byte)
  op(0x6c, InString, Byte)
  op(0x6d, InString, Word)
  op(0x6e, OutString, Byte)
  op(0x6f, OutString, Word)
  op(0x70, JumpIf, r.f.v == 1)
  op(0x71, JumpIf, r.f.v == 0)
  op(0x72, JumpIf, r.f.c == 1)
  op(0x73, JumpIf, r.f.c == 0)
  op(0x74, JumpIf, r.f.z == 1)
  op(0x75, JumpIf, r.f.z == 0)
  op(0x76, JumpIf, r.f.z == 1 || r.f.c == 1)
  op(0x77, JumpIf, r.f.z != 1 && r.f.c != 1)
  op(0x78, JumpIf, r.f.s == 1)
  op(0x79, JumpIf, r.f.s == 0)
  op(0x7a, JumpIf, r.f.p == 1)
  op(0x7b, JumpIf, r.f.p == 0)
  op(0x7c, JumpIf, r.f.s != r.f.v && r.f.z == 0)
  op(0x7d, JumpIf, r.f.s == r.f.v || r.f.z == 1)
  op(0x7e, JumpIf, r.f.s != r.f.v || r.f.z == 1)
  op(0x7f, JumpIf, r.f.s == r.f.v && r.f.z == 0)
  op(0x80, Group1MemImm, Byte, 0)
  op(0x81, Group1MemImm, Word, 0)
  op(0x82, Group1MemImm, Byte, 1)
  op(0x83, Group1MemImm, Word, 1)
  op(0x84, TestMemReg, Byte)
  op(0x85, TestMemReg, Word)
  op(0x86, ExchangeMemReg, Byte)
  op(0x87, ExchangeMemReg, Word)
  op(0x88, MoveMemReg, Byte)
  op(0x89, MoveMemReg, Word)
  op(0x8a, MoveRegMem, Byte)
  op(0x8b, MoveRegMem, Word)
  op(0x8c, MoveMemSeg)
  op(0x8d, LoadEffectiveAddressRegMem)
  op(0x8e, MoveSegMem)
  op(0x8f, PopMem)
  op(0x90, Nop)
  op(0x91, Exchange, r.ax, r.cx)
  op(0x92, Exchange, r.ax, r.dx)
  op(0x93, Exchange, r.ax, r.bx)
  op(0x94, Exchange, r.ax, r.sp)
  op(0x95, Exchange, r.ax, r.bp)
  op(0x96, Exchange, r.ax, r.si)
  op(0x97, Exchange, r.ax, r.di)
  op(0x98, SignExtendByte)
  op(0x99, SignExtendWord)
  op(0x9a, CallFar)
  op(0x9b, Wait)
  op(0x9c, PushFlags)
  op(0x9d, PopFlags)
  op(0x9e, StoreFlagsAcc)
  op(0x9f, LoadAccFlags)
  op(0xa0, MoveAccMem, Byte)
  op(0xa1, MoveAccMem, Word)
  op(0xa2, MoveMemAcc, Byte)
  op(0xa3, MoveMemAcc, Word)
  op(0xa4, MoveString, Byte)
  op(0xa5, MoveString, Word)
  op(0xa6, CompareString, Byte)
  op(0xa7, CompareString, Word)
  op(0xa8, TestAcc, Byte)
  op(0xa9, TestAcc, Word)
  op(0xaa, StoreString, Byte)
  op(0xab, StoreString, Word)
  op(0xac, LoadString, Byte)
  op(0xad, LoadString, Word)
  op(0xae, ScanString, Byte)
  op(0xaf, ScanString, Word)
  op(0xb0, MoveRegImm, r.al)
  op(0xb1, MoveRegImm, r.cl)
  op(0xb2, MoveRegImm, r.dl)
  op(0xb3, MoveRegImm, r.bl)
  op(0xb4, MoveRegImm, r.ah)
  op(0xb5, MoveRegImm, r.ch)
  op(0xb6, MoveRegImm, r.dh)
  op(0xb7, MoveRegImm, r.bh)
  op(0xb8, MoveRegImm, r.ax)
  op(0xb9, MoveRegImm, r.cx)
  op(0xba, MoveRegImm, r.dx)
  op(0xbb, MoveRegImm, r.bx)
  op(0xbc, MoveRegImm, r.sp)
  op(0xbd, MoveRegImm, r.bp)
  op(0xbe, MoveRegImm, r.si)
  op(0xbf, MoveRegImm, r.di)
  op(0xc0, Group2MemImm, Byte)
  op(0xc1, Group2MemImm, Word)
  op(0xc2, ReturnImm)
  op(0xc3, Return)
  op(0xc4, LoadSegmentMem, r.es)
  op(0xc5, LoadSegmentMem, r.ds)
  op(0xc6, MoveMemImm, Byte)
  op(0xc7, MoveMemImm, Word)
  op(0xc8, Enter)
  op(0xc9, Leave)
  op(0xca, ReturnFarImm)
  op(0xcb, ReturnFar)
  op(0xcc, Int3)
  op(0xcd, IntImm)
  op(0xce, Into)
  op(0xcf, ReturnInt)
  op(0xd0, Group2MemImm, Byte, (uint8)1)
  op(0xd1, Group2MemImm, Word, (uint8)1)
  op(0xd2, Group2MemImm, Byte, (uint8)r.cl)
  op(0xd3, Group2MemImm, Word, (uint8)r.cl)
  op(0xd4, AdjustAfterMultiply)
  op(0xd5, AdjustAfterDivide)
  op(0xd6, Translate)  //xlat (undocumented mirror)
  op(0xd7, Translate)  //xlat
//op(0xd8, ...)  //fpo1
//op(0xd9, ...)  //fpo1
//op(0xda, ...)  //fpo1
//op(0xdb, ...)  //fpo1
//op(0xdc, ...)  //fpo1
//op(0xdd, ...)  //fpo1
//op(0xde, ...)  //fpo1
//op(0xdf, ...)  //fpo1
  op(0xe0, LoopWhile, 0)  //loopnz
  op(0xe1, LoopWhile, 1)  //loopz
  op(0xe2, Loop)
  op(0xe3, JumpIf, r.cx == 0)
  op(0xe4, In, Byte)
  op(0xe5, In, Word)
  op(0xe6, Out, Byte)
  op(0xe7, Out, Word)
  op(0xe8, CallNear)
  op(0xe9, JumpNear)
  op(0xea, JumpFar)
  op(0xeb, JumpShort)
  op(0xec, InDX, Byte)
  op(0xed, InDX, Word)
  op(0xee, OutDX, Byte)
  op(0xef, OutDX, Word)
  op(0xf0, Lock)
//op(0xf1, ...)
  op(0xf2, Repeat)  //repnz
  op(0xf3, Repeat)  //repz
  op(0xf4, Halt)
  op(0xf5, ComplementCarry)
  op(0xf6, Group3MemImm, Byte)
  op(0xf7, Group3MemImm, Word)
  op(0xf8, ClearFlag, r.f.c.bit)
  op(0xf9, SetFlag, r.f.c.bit)
  op(0xfa, ClearFlag, r.f.i.bit)
  op(0xfb, SetFlag, r.f.i.bit)
  op(0xfc, ClearFlag, r.f.d.bit)
  op(0xfd, SetFlag, r.f.d.bit)
  op(0xfe, Group4MemImm, Byte)
  op(0xff, Group4MemImm, Word)
  }
}

#undef op
