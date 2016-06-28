auto Cartridge::serialize(serializer& s) -> void {
  if(information.battery) s.array(ram.data, ram.size);
  s.integer(bootromEnable);

  s.integer(mbc1.rom.select);
  s.integer(mbc1.ram.enable);
  s.integer(mbc1.ram.select);
  s.integer(mbc1.mode);

  s.integer(mbc1m.rom.lo);
  s.integer(mbc1m.rom.hi);
  s.integer(mbc1m.mode);

  s.integer(mbc2.rom.select);
  s.integer(mbc2.ram.enable);

  s.integer(mbc3.rom.select);
  s.integer(mbc3.ram.enable);
  s.integer(mbc3.ram.select);
  s.integer(mbc3.rtc.latch);
  s.integer(mbc3.rtc.halt);
  s.integer(mbc3.rtc.second);
  s.integer(mbc3.rtc.minute);
  s.integer(mbc3.rtc.hour);
  s.integer(mbc3.rtc.day);
  s.integer(mbc3.rtc.dayCarry);
  s.integer(mbc3.rtc.latchSecond);
  s.integer(mbc3.rtc.latchMinute);
  s.integer(mbc3.rtc.latchHour);
  s.integer(mbc3.rtc.latchDay);
  s.integer(mbc3.rtc.latchDayCarry);

  s.integer(mbc5.rom.select);
  s.integer(mbc5.ram.enable);
  s.integer(mbc5.ram.select);

  s.integer(mmm01.rom.base);
  s.integer(mmm01.rom.select);
  s.integer(mmm01.ram.enable);
  s.integer(mmm01.ram.select);
  s.integer(mmm01.mode);

  s.integer(huc1.rom.select);
  s.integer(huc1.ram.writable);
  s.integer(huc1.ram.select);
  s.integer(huc1.model);

  s.integer(huc3.rom.select);
  s.integer(huc3.ram.enable);
  s.integer(huc3.ram.select);
}
