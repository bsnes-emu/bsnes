#include <nds/nds.hpp>

namespace NintendoDS {

APU apu;


void APU::power() {
  for(unsigned n = 0; n < 16; n++) {
    auto &v = voices[n];
    
    v.enable = false;
    v.hold = false;
    v.running = false;
    v.format = Voice::PCM8;
    v.duty = 0;
    v.limit = 0;
    v.panning = 0x40;
    v.volumeBase = 0x7f;
    v.volumeExp = 0;
    v.amplitude = 0x7f0;
    
    v.source = 0;
    v.length = 0;
    v.counter = 0;
    v.sample = 0;
    
    v.init.source = 0;
    v.init.counter = 0;
    v.init.length1 = 0;
    v.init.length2 = 0;
    
    v.event.action = [&, n]() { stepVoice(n); };
  }
  // Audio runs at 16.8MHz. I've somewhat arbitrarily set
  // it up so that things run in the following order:
  //   - channel read + update (+0)
  //   - mixer (+1)
  //   - eventually, capture (+2)
  //
  // Mixer runs at 33Khz = 66 MHz / 2048
  mixEvent.action = [&]() { stepMixer(); };
  arm7.event.queue.add(2048+1, mixEvent);
  
  powered = true;
}

void APU::stepMixer() {
  arm7.event.queue.add(2048, mixEvent);
  
  int64 l = 0, r = 0;
  
  for(unsigned n = 0; n < 16; n++) {
    auto &v = voices[n];
    int64 s = v.sample * v.amplitude;
    
    l += s * (128 - v.panning);
    r += s * (0   + v.panning);
  }
  l = sclamp<16>(l / 0x80000);
  r = sclamp<16>(r / 0x80000);
  
  interface->audioSample(l, r);
}

void APU::stepVoice(unsigned no) {
  auto &v = voices[no];
  
  uint32 t = arm7.event.queue.time;
  
       if(v.format == Voice::PCM8)            stepPCM8(no);
  else if(v.format == Voice::PCM16)           stepPCM16(no);
  else if(v.format == Voice::ADPCM4)          stepADPCM4(no);
  else if(v.format == Voice::PSG && no >= 14) stepNoise(no);
  else if(v.format == Voice::PSG && no >=  8) stepPulse(no);
  
  v.counter = v.init.counter;
  
  uint32 fetchTime  = arm7.event.queue.time - t;
  uint32 nextSample = 4*(0x10000 - v.counter);
  
  nextSample -= fetchTime;
  if(unsigned k = t & 3)
    nextSample += 4 - k;
  
  // When this happens, the buffering can't keep up. It isn't clear
  // what the real system will do, but it can't be anything good...
  if(nextSample >= 0x80000000)
    nextSample = 0;
  
  // This requires further thought and investigation.
  // Do buffering issues delay the playback timer? (hopefully not)
  // Do audio DMAs interleave in any way? (doubtful; it'd be 10x slower)
  if(v.running)
    arm7.event.queue.add(nextSample, v.event);
}

void APU::stepPulse(unsigned no) {
  auto &v = voices[no];
  uint3 step = v.state--;
  
  v.sample = step > v.duty? -0x7fff : +0x7fff;
}

void APU::stepNoise(unsigned no) {
  auto &v = voices[no];
  bool out = v.state & 1;
  
  v.state >>= 1;
  v.state ^= 0x6000*out;
  v.sample = out? -0x7fff : +0x7fff;
}

void APU::stepPCM8(unsigned no) {
  auto &v = voices[no];
  
  checkLength(no);
  fillBuffer(no);
  v.sample = 0x100*int8(v.buffer[v.index/8] >> 4*(v.index & 6));
  v.index += 2;
  v.length -= 1;
}

void APU::stepPCM16(unsigned no) {
  auto &v = voices[no];

  checkLength(no);
  fillBuffer(no);
  v.sample = int16(v.buffer[v.index/8] >> 4*(v.index & 4));
  v.index += 4;
  v.length -= 2;
}

void APU::stepADPCM4(unsigned no) {
  auto &v = voices[no];
  
  static const int16 table[] = {
    0x0007,0x0008,0x0009,0x000a,0x000b,0x000c,0x000d,0x000e,
    0x0010,0x0011,0x0013,0x0015,0x0017,0x0019,0x001c,0x001f,
    0x0022,0x0025,0x0029,0x002d,0x0032,0x0037,0x003c,0x0042,
    0x0049,0x0050,0x0058,0x0061,0x006b,0x0076,0x0082,0x008f,
    0x009d,0x00ad,0x00be,0x00d1,0x00e6,0x00fd,0x0117,0x0133,
    0x0151,0x0173,0x0198,0x01c1,0x01ee,0x0220,0x0256,0x0292,
    0x02d4,0x031c,0x036c,0x03c3,0x0424,0x048e,0x0502,0x0583,
    0x0610,0x06ab,0x0756,0x0812,0x08e0,0x09c3,0x0abd,0x0bd0,
    0x0cff,0x0e4c,0x0fba,0x114c,0x1307,0x14ee,0x1706,0x1954,
    0x1bdc,0x1ea5,0x21b6,0x2515,0x28ca,0x2cdf,0x315b,0x364b,
    0x3bb9,0x41b2,0x4844,0x4f7e,0x5771,0x602f,0x69ce,0x7462,
    0x7fff
  };

  checkLength(no);
  fillBuffer(no);
  
  if(v.state == 0x7fff) {
    uint32 header = v.buffer[0];
    
    // Minimum length is 4 words anyway, but check this later..
    v.sample = header>>0;
    v.state  = min(88, header>>16 & 0x7f);
    v.index  += 8;
    v.length -= 4;
  }
  
  int4  s = v.buffer[v.index/8] >> 4*(v.index & 7);
  int16 x = table[v.state--];
  
  int16     delta  = x/8;
  if(s & 1) delta += x/4;
  if(s & 2) delta += x/2;
  if(s & 4) delta += x/1,  v.state += 2*(s & 3) + 3;
  
  if(s < 0) v.sample = max(-0x7fff, v.sample - delta);
  else      v.sample = min(+0x7fff, v.sample + delta);
  
  v.state = max(0, min(88, v.state));
  
  if(++v.index & 1) return;
  v.length -= 1;
}



void APU::stopVoice(unsigned no) {
  auto &v = voices[no];
  
  arm7.event.queue.remove(v.event);
  v.running = false;
  if(v.hold == false)
    v.sample = 0;
}

void APU::fillBuffer(unsigned no) {
  auto &v = voices[no];
  if(v.index) return;
  
  for(unsigned i = 0; i < 4; i++, v.source += 4)
    v.buffer[i] = arm7.read(v.source, Word, true);
}

void APU::checkLength(unsigned no) {
  auto &v = voices[no];
  if(v.length) return;
  
  if(v.loop.source == 0xffffffff) {
    // We're passing the loop point for the first time. In particular
    // the hardware will store the ADPCM state. For this reason it's not
    // possible to stream ADPCM audio without software assist. :(
    v.length      = v.loop.length;
    v.loop.sample = v.sample;
    v.loop.state  = v.state;
    v.loop.source = v.source;
    if(v.index) // Account for buffer
      v.loop.source += 4*(v.index/8 - 4);
    
    if(v.limit && v.length < 4)
      return stopVoice(no);
  }
  else {
    if(v.limit & Voice::once)
      return stopVoice(no);
    
    // There are still issues with loops here.. or possibly
    // readback of the voice status.
    if(v.limit & Voice::looped) {
      v.source = v.loop.source;
      v.length = v.loop.length;
      v.sample = v.loop.sample;
      v.state  = v.loop.state;
      v.index  = 0;
    }
  }
}

void APU::checkEnable(unsigned no) {
  auto &v = voices[no];
  
  // maxmod's interpolated mode expects this undocumented behavior. Instead
  // of software mixing, it resamples each voice's audio into a small buffer.
  // Master enable is used to trigger every voice at the same time.
  if(v.running == (enable && v.enable)) return;
  
  stopVoice(no);
  v.running = enable && v.enable;
  if(!v.running) return;
  
  uint32 next = 4*(0x10000 - v.init.counter);
  
  if(unsigned k = arm7.event.queue.time & 3)
    next += 4 - k;  // Align to 16MHz audio clock
  
  arm7.event.queue.add(next, v.event);
  
  // After 1 sample..
  v.length = 1;
  v.event.action = [&, no]() {
    uint32 period = 4*(0x10000 - v.init.counter);
    
    // Start the channel. From testing, it seems impossible to adjust
    // length1/2 afterwards, so the settings must be latched somewhere.
    // The loop flags can be changed though.
    v.source      = v.init.source;
    v.counter     = v.init.counter;
    v.length      = 4*v.init.length1;
    v.loop.length = 4*v.init.length2;
    v.loop.source = 0xffffffff;
    v.sample      = 0;
    v.index       = 0;
    v.state       = 0x7fff;
    
    // PCM has a couple more samples of startup latency.
    // ADPCM takes a further 8 samples to process the header..
    switch(v.format) {
    case Voice::PCM8:   // add further startup latency
    case Voice::PCM16:  period =  3*period; break;
    case Voice::ADPCM4: period = 11*period; break;
    case Voice::PSG:    period =  1*period; break;
    }
    
    unsigned k = arm7.event.queue.time & 3;
    if(k) period += 4 - k;
    
    arm7.event.queue.add(period, v.event);
    v.event.action = [&, no]() { stepVoice(no); };
  };
}


uint32 APU::regControl() {
  return volume<<0 | output[0]<<8 | output[1]<<10 | muteDsp[0]<<12 | muteDsp[1]<<13 | enable<<15;
}

uint32 APU::regBias() {
  return bias;
}

uint32 APU::regCaptureControl() {
  return capture[0]<<7 | capture[1]<<15;
}

uint32 APU::regCaptureDest(unsigned no) {
  return captureDest[no];
}



void APU::regControl(uint32 data, uint32 mask) {
  if(mask & 0x00ff) {
    volume     = data >> 0;
  }
  if(mask & 0xff00) {
    output[0]  = data >> 8;
    output[1]  = data >> 10;
    muteDsp[0] = data >> 12;
    muteDsp[1] = data >> 13;
    enable     = data >> 15;
    
    if(enable) {
      // Start any pending voices - maxmod uses this for interpolated mode.
      // What it does is:
      // - clear the master enable
      // - enable all 16 channels (won't do anything yet)
      // - set the master enable (this will trigger all 16 at once)
      // - wait ~16K clocks @ 33Mhz (16 samples), then set the mixing timer.
      for(unsigned no = 0; no < 16; no++)
        checkEnable(no);
    }
  }
}

void APU::regBias(uint32 data, uint32 mask) {
  bias = data;
}

void APU::regCaptureControl(uint32 data, uint32 mask) {
  capture[0] = data>>7;
  capture[1] = data>>15;
}

void APU::regCaptureDest(unsigned no, uint32 data, uint32 mask) {
  captureDest[no] = data;
}

void APU::regCaptureLength(unsigned no, uint32 data, uint32 mask) {
  captureLength[no] = data;
}



uint32 APU::regVoiceControl(unsigned no) {
  auto &v = voices[no];
  
  return v.volumeBase<<0 | v.volumeExp<<8 | v.hold<<15 | v.panning<<16
       | v.duty<<24 | v.limit<<27 | v.format<<29 | (v.running)<<31;
}

void APU::regVoiceControl(unsigned no, uint32 data, uint32 mask) {
  auto &v = voices[no];
  
  int exponent[] = { 4, 3, 2, 0 };
  
  if(mask & 0x0000007f) {
    v.volumeBase = data >> 0;
    v.amplitude  = v.volumeBase << exponent[v.volumeExp];
  }
  if(mask & 0x00008300) {
    v.volumeExp = data >> 8;
    v.hold      = data >> 15;
    v.amplitude = v.volumeBase << exponent[v.volumeExp];
  }
  if(mask & 0x007f0000) {
    v.panning = data >> 16;
  }
  if(mask & 0xff000000) {
    v.duty   = data >> 24;
    v.limit  = data >> 27;
    v.format = data >> 29;
    v.enable = data >> 31;
    
    checkEnable(no);
  }
}

void APU::regVoiceSource(unsigned no, uint32 data, uint32 mask) {
  auto &v = voices[no];
  
  v.init.source = data & 0x07fffffc;
}

void APU::regVoicePeriod(unsigned no, uint32 data, uint32 mask) {
  auto &v = voices[no];
  
  if(mask & 0x0000ffff) v.init.counter ^= (v.init.counter ^ data>>0)  & mask>>0;
  if(mask & 0xffff0000) v.init.length1 ^= (v.init.length1 ^ data>>16) & mask>>16;
}

void APU::regVoiceLength(unsigned no, uint32 data, uint32 mask) {
  auto &v = voices[no];
  
  v.init.length2 ^= (v.init.length2 ^ data) & mask & 0x3fffff;
}

}
