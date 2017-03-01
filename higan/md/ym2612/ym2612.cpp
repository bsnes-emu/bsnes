#include <md/md.hpp>

namespace MegaDrive {

YM2612 ym2612;
#include "io.cpp"
#include "timer.cpp"
#include "channel.cpp"
#include "constants.cpp"

auto YM2612::Enter() -> void {
  while(true) scheduler.synchronize(), ym2612.main();
}

auto YM2612::main() -> void {
  sample();

  timerA.run();
  timerB.run();

  if(lfo.enable && ++lfo.divider == lfoDividers[lfo.rate]) {
    lfo.divider = 0;
    lfo.clock++;
    for(auto& channel : channels) {
      for(auto index : range(4)) {
        channel.updatePhase(index);  //due to vibrato
        channel.updateLevel(index);  //due to tremolo
      }
    }
  }

  if(++envelope.divider == 3) {
    envelope.divider = 0;
    envelope.clock++;
    for(auto& channel : channels) {
      for(auto index : range(4)) {
        channel.runPhase(index);
        channel.runEnvelope(index);
      }
    }
  } else {
    for(auto& channel : channels) {
      for(auto index : range(4)) {
        channel.runPhase(index);
      }
    }
  }

  step(1);
}

auto YM2612::sample() -> void {
  int left = 0;
  int right = 0;

  for(auto& channel : channels) {
    auto& op = channel.operators;

    const int modMask = -(1 << 1);
    const int sumMask = -(1 << 5);
    const int outMask = -(1 << 5);

    auto old = [&](uint n) -> int { return modMask & op[n].prior;  };
    auto mod = [&](uint n) -> int { return modMask & op[n].output; };
    auto out = [&](uint n) -> int { return sumMask & op[n].output; };

    auto wave = [&](uint n, int modulation) -> int {
      int x = modulation / 2 + op[n].phase.value / 0x400;
      int y = sine[x & 0x3ff] + op[n].outputLevel;
      return y < 0x2000 ? pow2[y & 0x1ff] << 2 >> y / 0x200 : 0;
    };

    int feedback = modMask & op[0].output + op[0].prior >> 9 - channel.feedback;
    int accumulator = 0;

    for(auto n : range(4)) op[n].prior = op[n].output;

    op[0].output = wave(0, feedback * (channel.feedback > 0));

    if(channel.algorithm == 0) {
      //0 -> 1 -> 2 -> 3
      op[1].output = wave(1, mod(0));
      op[2].output = wave(2, old(1));
      op[3].output = wave(3, mod(2));
      accumulator += out(3);
    }

    if(channel.algorithm == 1) {
      //(0 + 1) -> 2 -> 3
      op[1].output = wave(1, 0);
      op[2].output = wave(2, mod(0) + old(1));
      op[3].output = wave(3, mod(2));
      accumulator += out(3);
    }

    if(channel.algorithm == 2) {
      //0 + (1 -> 2) -> 3
      op[1].output = wave(1, 0);
      op[2].output = wave(2, old(1));
      op[3].output = wave(3, mod(0) + mod(2));
      accumulator += out(3);
    }

    if(channel.algorithm == 3) {
      //(0 -> 1) + 2 -> 3
      op[1].output = wave(1, mod(0));
      op[2].output = wave(2, 0);
      op[3].output = wave(3, mod(1) + mod(2));
      accumulator += out(3);
    }

    if(channel.algorithm == 4) {
      //(0 -> 1) + (2 -> 3)
      op[1].output = wave(1, mod(0));
      op[2].output = wave(2, 0);
      op[3].output = wave(3, mod(2));
      accumulator += out(1) + out(3);
    }

    if(channel.algorithm == 5) {
      //0 -> (1 + 2 + 3)
      op[1].output = wave(1, mod(0));
      op[2].output = wave(2, old(0));
      op[3].output = wave(3, mod(0));
      accumulator += out(1) + out(2) + out(3);
    }

    if(channel.algorithm == 6) {
      //(0 -> 1) + 2 + 3
      op[1].output = wave(1, mod(0));
      op[2].output = wave(2, 0);
      op[3].output = wave(3, 0);
      accumulator += out(1) + out(2) + out(3);
    }

    if(channel.algorithm == 7) {
      //0 + 1 + 2 + 3
      op[1].output = wave(1, 0);
      op[2].output = wave(2, 0);
      op[3].output = wave(3, 0);
      accumulator += out(0) + out(1) + out(2) + out(3);
    }

    int voiceData = outMask & min(max(accumulator, -0x1ffff), +0x1ffff);
    if(dac.enable && (&channel == &channels[5])) voiceData = dac.sample - 0x80 << 5;

    if(channel.leftEnable ) left  += voiceData;
    if(channel.rightEnable) right += voiceData;
  }

  int cutoff = 20;

  lpfLeft = (left - lpfLeft) * cutoff / 256;
  lpfRight = (right - lpfRight) * cutoff / 256;

  left = left * 2 / 6 + lpfLeft * 3 / 4;
  right = right * 2 / 6 + lpfRight * 3 / 4;

  stream->sample(left / 32768.0, right / 32768.0);
}

auto YM2612::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto YM2612::power() -> void {
  create(YM2612::Enter, system.colorburst() * 15.0 / 7.0 / 144.0);
  stream = Emulator::audio.createStream(2, frequency());

  memory::fill(&io, sizeof(IO));
  memory::fill(&lfo, sizeof(LFO));
  memory::fill(&dac, sizeof(DAC));
  memory::fill(&envelope, sizeof(Envelope));

  timerA.power();
  timerB.power();
  for(auto& channel : channels) channel.power();

  const int pos = 0;
  const int neg = 1;

  for(int x = 0; x <= 0xff; x++) {
    int y = int(-256 * log(sin((2 * x + 1) * Math::Pi / 1024)) / log(2) + 0.5);
    sine[0x000 + x] = pos + (y << 1);
    sine[0x1ff - x] = pos + (y << 1);
    sine[0x200 + x] = neg + (y << 1);
    sine[0x3ff - x] = neg + (y << 1);
  }

  for(int y = 0; y <= 0xff; y++) {
    int z = int(1024 * pow(2, (0xff - y) / 256.0) + 0.5);
    pow2[pos + (y << 1)] = +z;
    pow2[neg + (y << 1)] = ~z;  //not -z
  }
}

}
