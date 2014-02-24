#include <sfc/sfc.hpp>

#define MSU1_CPP
namespace SuperFamicom {

MSU1 msu1;

#include "serialization.cpp"

void MSU1::Enter() { msu1.enter(); }

void MSU1::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    int16 left = 0, right = 0;

    if(mmio.audio_play) {
      if(audiofile.open()) {
        if(audiofile.end()) {
          if(!mmio.audio_repeat) {
            mmio.audio_play = false;
            audiofile.seek(mmio.audio_play_offset = 8);
          } else {
            audiofile.seek(mmio.audio_play_offset = mmio.audio_loop_offset);
          }
        } else {
          mmio.audio_play_offset += 4;
          left  = audiofile.readl(2);
          right = audiofile.readl(2);
        }
      } else {
        mmio.audio_play = false;
      }
    }

    signed lchannel = (double)left  * (double)mmio.audio_volume / 255.0;
    signed rchannel = (double)right * (double)mmio.audio_volume / 255.0;
    left  = sclamp<16>(lchannel);
    right = sclamp<16>(rchannel);
    if(dsp.mute()) left = 0, right = 0;

    audio.coprocessor_sample(left, right);
    step(1);
    synchronize_cpu();
  }
}

void MSU1::init() {
}

void MSU1::load() {
}

void MSU1::unload() {
  if(datafile.open()) datafile.close();
  if(audiofile.open()) audiofile.close();
}

void MSU1::power() {
  audio.coprocessor_enable(true);
  audio.coprocessor_frequency(44100.0);
}

void MSU1::reset() {
  create(MSU1::Enter, 44100);

  mmio.data_seek_offset = 0;
  mmio.data_read_offset = 0;

  mmio.audio_play_offset = 0;
  mmio.audio_loop_offset = 0;

  mmio.audio_track = 0;
  mmio.audio_volume = 0;

  mmio.data_busy = false;
  mmio.audio_busy = false;
  mmio.audio_repeat = false;
  mmio.audio_play = false;
  mmio.audio_error = false;

  data_open();
  audio_open();
}

void MSU1::data_open() {
  if(datafile.open()) datafile.close();
  auto document = Markup::Document(cartridge.information.markup.cartridge);
  string name = document["cartridge/msu1/rom/name"].data;
  if(name.empty()) name = "msu1.rom";
  if(datafile.open({interface->path(ID::SuperFamicom), name}, file::mode::read)) {
    datafile.seek(mmio.data_read_offset);
  }
}

void MSU1::audio_open() {
  if(audiofile.open()) audiofile.close();
  auto document = Markup::Document(cartridge.information.markup.cartridge);
  string name = {"track-", mmio.audio_track, ".pcm"};
  for(auto track : document.find("cartridge/msu1/track")) {
    if(numeral(track["number"].data) != mmio.audio_track) continue;
    name = track["name"].data;
    break;
  }
  if(audiofile.open({interface->path(ID::SuperFamicom), name}, file::mode::read)) {
    if(audiofile.size() >= 8) {
      uint32 header = audiofile.readm(4);
      if(header == 0x4d535531) {  //"MSU1"
        mmio.audio_loop_offset = 8 + audiofile.readl(4) * 4;
        if(mmio.audio_loop_offset > audiofile.size()) mmio.audio_loop_offset = 8;
        mmio.audio_error = false;
        audiofile.seek(mmio.audio_play_offset);
        return;
      }
    }
    audiofile.close();
  }
  mmio.audio_error = true;
}

uint8 MSU1::mmio_read(unsigned addr) {
  cpu.synchronize_coprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000:
    return (mmio.data_busy    << 7)
         | (mmio.audio_busy   << 6)
         | (mmio.audio_repeat << 5)
         | (mmio.audio_play   << 4)
         | (mmio.audio_error  << 3)
         | (Revision          << 0);
  case 0x2001:
    if(mmio.data_busy) return 0x00;
    if(datafile.end()) return 0x00;
    mmio.data_read_offset++;
    return datafile.read();
  case 0x2002: return 'S';
  case 0x2003: return '-';
  case 0x2004: return 'M';
  case 0x2005: return 'S';
  case 0x2006: return 'U';
  case 0x2007: return '1';
  }
}

void MSU1::mmio_write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000: mmio.data_seek_offset = (mmio.data_seek_offset & 0xffffff00) | (data <<  0); break;
  case 0x2001: mmio.data_seek_offset = (mmio.data_seek_offset & 0xffff00ff) | (data <<  8); break;
  case 0x2002: mmio.data_seek_offset = (mmio.data_seek_offset & 0xff00ffff) | (data << 16); break;
  case 0x2003: mmio.data_seek_offset = (mmio.data_seek_offset & 0x00ffffff) | (data << 24);
    mmio.data_read_offset = mmio.data_seek_offset;
    data_open();
    break;
  case 0x2004: mmio.audio_track = (mmio.audio_track & 0xff00) | (data << 0); break;
  case 0x2005: mmio.audio_track = (mmio.audio_track & 0x00ff) | (data << 8);
    mmio.audio_play_offset = 8;
    audio_open();
    break;
  case 0x2006: mmio.audio_volume = data; break;
  case 0x2007:
    if(mmio.audio_busy) break;
    if(mmio.audio_error) break;
    mmio.audio_repeat = data & 2;
    mmio.audio_play   = data & 1;
    break;
  }
}

}
