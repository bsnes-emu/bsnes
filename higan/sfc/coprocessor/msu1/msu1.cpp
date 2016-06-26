#include <sfc/sfc.hpp>

namespace SuperFamicom {

MSU1 msu1;

#include "serialization.cpp"

auto MSU1::Enter() -> void {
  while(true) scheduler.synchronize(), msu1.main();
}

auto MSU1::main() -> void {
  double left = 0.0;
  double right = 0.0;

  if(mmio.audioPlay) {
    if(audioFile) {
      if(audioFile->end()) {
        if(!mmio.audioRepeat) {
          mmio.audioPlay = false;
          audioFile->seek(mmio.audioPlayOffset = 8);
        } else {
          audioFile->seek(mmio.audioPlayOffset = mmio.audioLoopOffset);
        }
      } else {
        mmio.audioPlayOffset += 4;
        left  = (double)audioFile->readl(2) / 32768.0 * (double)mmio.audioVolume / 255.0;
        right = (double)audioFile->readl(2) / 32768.0 * (double)mmio.audioVolume / 255.0;
        if(dsp.mute()) left = 0, right = 0;
      }
    } else {
      mmio.audioPlay = false;
    }
  }

  stream->sample(left, right);
  step(1);
  synchronizeCPU();
}

auto MSU1::init() -> void {
}

auto MSU1::load() -> void {
}

auto MSU1::unload() -> void {
  dataFile.reset();
  audioFile.reset();
}

auto MSU1::power() -> void {
}

auto MSU1::reset() -> void {
  create(MSU1::Enter, 44100);
  stream = Emulator::audio.createStream(2, 44100.0);

  mmio.dataSeekOffset = 0;
  mmio.dataReadOffset = 0;

  mmio.audioPlayOffset = 0;
  mmio.audioLoopOffset = 0;

  mmio.audioTrack = 0;
  mmio.audioVolume = 0;

  mmio.audioResumeTrack = ~0;  //no resume
  mmio.audioResumeOffset = 0;

  mmio.audioError = false;
  mmio.audioPlay = false;
  mmio.audioRepeat = false;
  mmio.audioBusy = false;
  mmio.dataBusy = false;

  dataOpen();
  audioOpen();
}

auto MSU1::dataOpen() -> void {
  dataFile.reset();
  auto document = BML::unserialize(cartridge.information.manifest.cartridge);
  string name = document["board/msu1/rom/name"].text();
  if(!name) name = "msu1.rom";
  if(dataFile = interface->open(ID::SuperFamicom, name, File::Read)) {
    dataFile->seek(mmio.dataReadOffset);
  }
}

auto MSU1::audioOpen() -> void {
  audioFile.reset();
  auto document = BML::unserialize(cartridge.information.manifest.cartridge);
  string name = {"track-", mmio.audioTrack, ".pcm"};
  for(auto track : document.find("board/msu1/track")) {
    if(track["number"].natural() != mmio.audioTrack) continue;
    name = track["name"].text();
    break;
  }
  if(audioFile = interface->open(ID::SuperFamicom, name, File::Read)) {
    if(audioFile->size() >= 8) {
      uint32 header = audioFile->readm(4);
      if(header == 0x4d535531) {  //"MSU1"
        mmio.audioLoopOffset = 8 + audioFile->readl(4) * 4;
        if(mmio.audioLoopOffset > audioFile->size()) mmio.audioLoopOffset = 8;
        mmio.audioError = false;
        audioFile->seek(mmio.audioPlayOffset);
        return;
      }
    }
    audioFile.reset();
  }
  mmio.audioError = true;
}

auto MSU1::read(uint24 addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000:
    return (
      Revision         << 0
    | mmio.audioError  << 3
    | mmio.audioPlay   << 4
    | mmio.audioRepeat << 5
    | mmio.audioBusy   << 6
    | mmio.dataBusy    << 7
    );
  case 0x2001:
    if(mmio.dataBusy) return 0x00;
    if(!dataFile) return 0x00;
    if(dataFile->end()) return 0x00;
    mmio.dataReadOffset++;
    return dataFile->read();
  case 0x2002: return 'S';
  case 0x2003: return '-';
  case 0x2004: return 'M';
  case 0x2005: return 'S';
  case 0x2006: return 'U';
  case 0x2007: return '1';
  }
}

auto MSU1::write(uint24 addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  addr = 0x2000 | (addr & 7);

  switch(addr) {
  case 0x2000: mmio.dataSeekOffset.byte(0) = data; break;
  case 0x2001: mmio.dataSeekOffset.byte(1) = data; break;
  case 0x2002: mmio.dataSeekOffset.byte(2) = data; break;
  case 0x2003: mmio.dataSeekOffset.byte(3) = data;
    mmio.dataReadOffset = mmio.dataSeekOffset;
    if(dataFile) dataFile->seek(mmio.dataReadOffset);
    break;
  case 0x2004: mmio.audioTrack.byte(0) = data; break;
  case 0x2005: mmio.audioTrack.byte(1) = data;
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
    mmio.audioPlay   = data.bit(0);
    mmio.audioRepeat = data.bit(1);
    bool audioResume = data.bit(2);
    if(!mmio.audioPlay && audioResume) {
      mmio.audioResumeTrack = mmio.audioTrack;
      mmio.audioResumeOffset = mmio.audioPlayOffset;
    }
    break;
  }
}

}
