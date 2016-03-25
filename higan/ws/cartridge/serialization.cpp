auto Cartridge::serialize(serializer& s) -> void {
  if(ram.size) s.array(ram.data, ram.size);
  if(eeprom.size()) eeprom.serialize(s);
  if(rtc.size) s.array(rtc.data, rtc.size);

  if(rtc.size) {
    s.integer(rtc.command);
    s.integer(rtc.index);
    s.integer(rtc.alarm);
    s.integer(rtc.alarmHour);
    s.integer(rtc.alarmMinute);
  }

  s.integer(r.romBank0);
  s.integer(r.romBank1);
  s.integer(r.romBank2);
  s.integer(r.sramBank);
}
