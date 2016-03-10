//Sony CXD1800Q
//CD-ROM decoder

auto SuperDisc::sonyPollIRQ() -> uint8 {
  return 0x00;
}

auto SuperDisc::sonyReadData() -> uint8 {
  uint8 command = sony.command++;

  auto match = [&](const string& compare) -> bool {
    char hi = compare[0];
    if(hi == '?') hi = 0;
    if(hi >= '0' && hi <= '9') hi -= '0';
    if(hi >= 'a' && hi <= 'f') hi -= 'a' - 10;
    if(hi != '?' && hi != command.bits(4,7)) return false;

    char lo = compare[1];
    if(lo == '?') lo = 0;
    if(lo >= '0' && lo <= '9') lo -= '0';
    if(lo >= 'a' && lo <= 'f') lo -= 'a' - 10;
    if(lo != '?' && lo != command.bits(0,3)) return false;

    return true;
  };

  //DMA
  if(match("00")) return 0x00;

  //INST
  if(match("01")) return 0x10;

  //STS
  if(match("02")) return 0x00;

  //HFLG
  if(match("03")) return 0x00;

  //HMIN
  if(match("?4")) return 0x00;

  //HSEC
  if(match("?5")) return 0x00;

  //HBLK
  if(match("?6")) return 0x00;

  //HMOD
  if(match("?7")) return 0x00;

  //SFIL
  if(match("08")) return 0x00;

  //SCH
  if(match("09")) return 0x00;

  //SMOD
  if(match("0a")) return 0x00;

  //SCI
  if(match("0b")) return 0x00;

  //CMAD
  if(match("0c")) return 0x00;
  if(match("0d")) return 0x00;

  //MDFM
  if(match("?e")) return 0x00;

  //ADPC
  if(match("?f")) return 0x00;

  //DMXF
  if(match("18")) return 0x00;
  if(match("19")) return 0x00;

  //DMAD
  if(match("1a")) return 0x00;
  if(match("1b")) return 0x00;

  //DRAD
  if(match("1c")) return 0x00;
  if(match("1d")) return 0x00;

  return 0x00;
}

auto SuperDisc::sonyWriteCommand(uint8 data) -> void {
  sony.command = data;
}

auto SuperDisc::sonyWriteData(uint8 data) -> void {
  uint8 command = sony.command++;

  auto match = [&](const string& compare) -> bool {
    char hi = compare[0];
    if(hi == '?') hi = 0;
    if(hi >= '0' && hi <= '9') hi -= '0';
    if(hi >= 'a' && hi <= 'f') hi -= 'a' - 10;
    if(hi != '?' && hi != command.bits(4,7)) return false;

    char lo = compare[1];
    if(lo == '?') lo = 0;
    if(lo >= '0' && lo <= '9') lo -= '0';
    if(lo >= 'a' && lo <= 'f') lo -= 'a' - 10;
    if(lo != '?' && lo != command.bits(0,3)) return false;

    return true;
  };

  //DRIF
  if(match("?1")) return;

  //CHCT
  if(match("?2")) return;

  //DECT
  if(match("?3")) return;

  //INMS
  if(match("?4")) return;

  //INCL
  if(match("?5")) return;

  //CI
  if(match("?6")) return;

  //DMAD
  if(match("?7")) return;
  if(match("?8")) return;

  //DMXF
  if(match("?9")) return;
  if(match("?a")) return;

  //DRAD
  if(match("?b")) return;
  if(match("?c")) return;

  //PLBA
  if(match("0d")) return;
}
