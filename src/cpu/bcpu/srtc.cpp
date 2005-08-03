/*
  S-RTC chip emulation
  Used by Hudson Soft in Dai Kaijuu Monogatari II and Far East of Eden Zero.
  Currently, only the former is supported by bsnes.

  Original S-RTC emulation code via John Weidman/SNES9x
  Rewritten for compatibility with bsnes via byuu

  The S-RTC is a real-time clock chip that was added to the above two carts
  to allow the games to maintain the current time, even when the game was not
  powered on. Thus allowing special events at certain times, and on certain
  dates. Hudson Soft called this the PLG (Player's Life Gameplay System).

  This chip is a special case to the term 'emulation' itself.
  There are a few different ways to go about emulating this chip, and each
  result in a different style of emulation.

  The first is to simply return the current PC system time when the S-RTC is
  read from. This emulates the original S-RTC in the sense that it always
  returns the true current time, ignoring the speed that the SNES itself is
  running at. The downside to this method is that you lose the ability to set
  the time to whatever you choose inside the game itself. It will always return
  the true time, regardless. This can be overcome by changing the PC system time,
  which actually adds a greater degree of control over event timing, very useful
  for emulation. It also has a timeshifting flaw discussed below.

  The second is to run the S-RTC relative to the SNES speed. This means that
  if the emulator is sped up (via fast forward key, frameskipping, etc), or
  slowed down (via slowdown key, system bottlenecking, etc); the time increments
  slower, thus ~60 frames on the SNES equal one second. Without this, timeshifting
  will occur between the S-RTC and the real SNES.

  The third and final method is to save a copy of the local system time when the
  S-RTC is initially set, and compare the current system time against this value
  when setting the S-RTC time. This overcomes the first methods' shortcoming of
  not allowing the player to set the time in-game, however a new problem arises.
  You now have to save the time when the RTC was initially set to both savestates
  and to save-game data. This would require an extra file, or the breaking of
  perhaps the only standard format (.srm savegame backups) in the entire SNES
  emulation scene. You also give up the control of being able to override the
  RTC clock at will via the PC system time outside of emulation.
  The first method has another advantage over the third: Dai Kaijuu Monogatari II
  only allows dates in the range of the years 1996-2199. The first method gets
  around this limitation. But who knows, maybe it will break something in the
  game if the date exceeds 2199... I guess we'll worry about that in two hundred
  years from now.

  For my implementation, I chose to go with the first method. Both for simplicity
  and because I did not wish to create a new method for saving the system time
  whenever the RTC is set.
*/

void bCPU::srtc_set_time() {
time_t rawtime;
tm *t;
  ::time(&rawtime);
  t = localtime(&rawtime);

//see srtc.h for format of srtc.data[]
  srtc.data[0]  = t->tm_sec % 10;
  srtc.data[1]  = t->tm_sec / 10;
  srtc.data[2]  = t->tm_min % 10;
  srtc.data[3]  = t->tm_min / 10;
  srtc.data[4]  = t->tm_hour % 10;
  srtc.data[5]  = t->tm_hour / 10;
  srtc.data[6]  = t->tm_mday % 10;
  srtc.data[7]  = t->tm_mday / 10;
  srtc.data[8]  = t->tm_mon + 1;
  srtc.data[9]  = t->tm_year % 10;
  srtc.data[10] = (t->tm_year / 10) % 10;
  srtc.data[11] = 9 + (t->tm_year / 100);
  srtc.data[12] = t->tm_wday;
}

void bCPU::srtc_power() {
  memset(&srtc, 0, sizeof(srtc));
  reset();
}

void bCPU::srtc_reset() {
  srtc.index = -1;
  srtc.mode = SRTC_READ;
}

//Please see notes above about the implementation of the S-RTC
//Writes are stored the srtc.data[] array, but they are ignored
//as reads will refresh the data array with the current system
//time. The write method is only here for the sake of faux
//emulation of the real hardware.
void bCPU::srtc_write(uint8 data) {
  data &= 0x0f; //only the low four bits are used

  if(data >= 0x0d) {
    switch(data) {
    case 0x0d:
      srtc.mode = SRTC_READ;
      srtc.index = -1;
      break;
    case 0x0e:
      srtc.mode = SRTC_COMMAND;
      break;
    case 0x0f:
    //unknown behaviour
      break;
    }
    return;
  }

  if(srtc.mode == SRTC_WRITE) {
    if(srtc.index >= 0 && srtc.index < MAX_SRTC_INDEX) {
      srtc.data[srtc.index++] = data;

      if(srtc.index == MAX_SRTC_INDEX) {
      //all S-RTC data has been loaded by program
        srtc.data[srtc.index++] = 0x00; //day_of_week
      }
    }
  } else if(srtc.mode == SRTC_COMMAND) {
    switch(data) {
    case SRTC_COMMAND_CLEAR:
      memset(srtc.data, 0, MAX_SRTC_INDEX + 1);
      srtc.index = -1;
      srtc.mode = SRTC_READY;
      break;
    case SRTC_COMMAND_WRITE:
      srtc.index = 0;
      srtc.mode = SRTC_WRITE;
      break;
    default:
    //unknown behaviour
      srtc.mode = SRTC_READY;
      break;
    }
  } else {
    if(srtc.mode == SRTC_READ) {
    //ignore writes while in read mode
    } else if(srtc.mode == SRTC_READY) {
    //unknown behaviour
    }
  }
}

uint8 bCPU::srtc_read() {
  if(srtc.mode == SRTC_READ) {
    if(srtc.index < 0) {
      srtc_set_time();
      srtc.index++;
      return 0x0f; //send start message
    } else if(srtc.index > MAX_SRTC_INDEX) {
      srtc.index = -1;
      return 0x0f; //send finished message
    } else {
      return srtc.data[srtc.index++];
    }
  } else {
    return 0x00;
  }
}
