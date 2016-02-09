#include <sfc/sfc.hpp>

namespace SuperFamicom {

MSU1 msu1;

#include "serialization.cpp"

auto MSU1::Enter() -> void {
  while(true) scheduler.synchronize(), msu1.main();
}

auto MSU1::main() -> void {
  int16 left = 0, right = 0;

  if(mmio.audioPlay) {
    if(audioFile.open()) {
      if(audioFile.end()) {
        if(!mmio.audioRepeat) {
          mmio.audioPlay = false;
          audioFile.seek(mmio.audioPlayOffset = 8);
        } else {
          audioFile.seek(mmio.audioPlayOffset = mmio.audioLoopOffset);
        }
      } else {
        mmio.audioPlayOffset += 4;
        left  = audioFile.readl(2);
        right = audioFile.readl(2);
      }
    } else {
      mmio.audioPlay = false;
    }
  }

  int lchannel = (double)left  * (double)mmio.audioVolume / 255.0;
  int rchannel = (double)right * (double)mmio.audioVolume / 255.0;
  left  = sclamp<16>(lchannel);
  right = sclamp<16>(rchannel);
  if(dsp.mute()) left = 0, right = 0;

  audio.coprocessorSample(left, right);
  step(1);
  synchronizeCPU();
}

auto MSU1::init() -> void {
}

auto MSU1::load() -> void {
}

auto MSU1::unload() -> void {
  if(dataFile.open()) dataFile.close();
  if(audioFile.open()) audioFile.close();
}

auto MSU1::power() -> void {
  audio.coprocessorEnable(true);
  audio.coprocessorFrequency(44100.0);
}

auto MSU1::reset() -> void {
  create(MSU1::Enter, 44100);

  mmio.dataSeekOffset = 0;
  mmio.dataReadOffset = 0;

  mmio.audioPlayOffset = 0;
  mmio.audioLoopOffset = 0;

  mmio.audioTrack = 0;
  mmio.audioVolume = 0;

  mmio.audioResumeTrack = ~0;  //no resume
  mmio.audioResumeOffset = 0;

  mmio.dataBusy = false;
  mmio.audioBusy = false;
  mmio.audioRepeat = false;
  mmio.audioPlay = false;
  mmio.audioError = false;

  dataOpen();
  audioOpen();
}

auto MSU1::dataOpen() -> void {
  if(dataFile.open()) dataFile.close();
  auto document = BML::unserialize(cartridge.information.markup.cartridge);
  string name = document["board/msu1/rom/name"].text();
  if(!name) name = "msu1.rom";
  if(dataFile.open({interface->path(ID::SuperFamicom), name}, file::mode::read)) {
    dataFile.seek(mmio.dataReadOffset);
  }
}

auto MSU1::audioOpen() -> void {
  if(audioFile.open()) audioFile.close();
  auto document = BML::unserialize(cartridge.information.markup.cartridge);
  string name = {"track-", mmio.audioTrack, ".pcm"};
  for(auto track : document.find("board/msu1/track")) {
    if(track["number"].natural() != mmio.audioTrack) continue;
    name = track["name"].text();
    break;
  }
  if(audioFile.open({interface->path(ID::SuperFamicom), name}, file::mode::read)) {
    if(audioFile.size() >= 8) {
      uint32 header = audioFile.readm(4);
      if(header == 0x4d535531) {  //"MSU1"
        mmio.audioLoopOffset = 8 + audioFile.readl(4) * 4;
        if(mmio.audioLoopOffset > audioFile.size()) mmio.audioLoopOffset = 8;
        mmio.audioError = false;
        audioFile.seek(mmio.audioPlayOffset);
        return;
      }
    }
    audioFile.close();
  }
  mmio.audioError = true;
}

auto MSU1::mmioRead(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000:
    return (mmio.dataBusy    << 7)
         | (mmio.audioBusy   << 6)
         | (mmio.audioRepeat << 5)
         | (mmio.audioPlay   << 4)
         | (mmio.audioError  << 3)
         | (Revision         << 0);
  case 0x2001:
    if(mmio.dataBusy) return 0x00;
    if(dataFile.end()) return 0x00;
    mmio.dataReadOffset++;
    return dataFile.read();
  case 0x2002: return 'S';
  case 0x2003: return '-';
  case 0x2004: return 'M';
  case 0x2005: return 'S';
  case 0x2006: return 'U';
  case 0x2007: return '1';
  }
}

auto MSU1::mmioWrite(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000: mmio.dataSeekOffset = (mmio.dataSeekOffset & 0xffffff00) | (data <<  0); break;
  case 0x2001: mmio.dataSeekOffset = (mmio.dataSeekOffset & 0xffff00ff) | (data <<  8); break;
  case 0x2002: mmio.dataSeekOffset = (mmio.dataSeekOffset & 0xff00ffff) | (data << 16); break;
  case 0x2003: mmio.dataSeekOffset = (mmio.dataSeekOffset & 0x00ffffff) | (data << 24);
    mmio.dataReadOffset = mmio.dataSeekOffset;
    dataOpen();
    break;
  case 0x2004: mmio.audioTrack = (mmio.audioTrack & 0xff00) | (data << 0); break;
  case 0x2005: mmio.audioTrack = (mmio.audioTrack & 0x00ff) | (data << 8);
    mmio.audioPlayOffset = 8;
    if(mmio.audioTrack == mmio.audioResumeTrack) {
      mmio.audioPlayOffset = mmio.audioResumeOffset;
      mmio.audioResumeTrack = ~0;  //erase resume track
      mmio.audioResumeOffset = 0;
    }
    audioOpen();
    break;
  case 0x2006:
    mmio.audioVolume = data;
    break;
  case 0x2007:
    if(mmio.audioBusy) break;
    if(mmio.audioError) break;
    bool audioResume = data & 4;
    mmio.audioRepeat = data & 2;
    mmio.audioPlay   = data & 1;
    if(!mmio.audioPlay && audioResume) {
      mmio.audioResumeTrack = mmio.audioTrack;
      mmio.audioResumeOffset = mmio.audioPlayOffset;
    }
    break;
  }
}

}
