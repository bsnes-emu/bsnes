#include <nes/nes.hpp>

namespace NES {

APU apu;

const uint8 APU::length_counter_table[32] = {
  0x0a, 0xfe, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xa0, 0x08, 0x3c, 0x0a, 0x0e, 0x0c, 0x1a, 0x0e,
  0x0c, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xc0, 0x18, 0x48, 0x1a, 0x10, 0x1c, 0x20, 0x1e,
};

const uint16 APU::ntsc_noise_period_table[16] = {
  4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068,
};

const uint16 APU::pal_noise_period_table[16] = {
  4, 7, 14, 30, 60, 88, 118, 148, 188, 236, 354, 472, 708,  944, 1890, 3778,
};

const uint16 APU::ntsc_dmc_period_table[16] = {
  428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54,
};

const uint16 APU::pal_dmc_period_table[16] = {
  398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98, 78, 66, 50,
};

void APU::Main() {
  apu.main();
}

void APU::main() {
  while(true) {
    unsigned rectangle_output, triangle_output, noise_output, dmc_output;

    rectangle_output  = rectangle[0].clock();
    rectangle_output += rectangle[1].clock();
    triangle_output = triangle.clock();
    noise_output = noise.clock();
    dmc_output = dmc.clock();

    clock_frame_counter_divider();

    signed output = rectangle_dac[rectangle_output] + dmc_triangle_noise_dac[dmc_output][triangle_output][noise_output];
    system.interface->audio_sample(output);

    tick();
  }
}

void APU::tick() {
  clock += 12;
  if(clock >= 0) co_switch(cpu.thread);
}

void APU::power() {
  reset();
}

void APU::reset() {
  Processor::create(APU::Main, 21477272);

  for(unsigned n = 0; n < 2; n++) {
    rectangle[n].length_counter = 0;

    rectangle[n].envelope.speed = 0;
    rectangle[n].envelope.use_speed_as_volume = 0;
    rectangle[n].envelope.loop_mode = 0;
    rectangle[n].envelope.reload_decay = 0;
    rectangle[n].envelope.decay_counter = 0;
    rectangle[n].envelope.decay_volume = 0;

    rectangle[n].sweep.shift = 0;
    rectangle[n].sweep.decrement = 0;
    rectangle[n].sweep.period = 0;
    rectangle[n].sweep.counter = 0;
    rectangle[n].sweep.enable = 0;
    rectangle[n].sweep.reload = 0;
    rectangle[n].sweep.rectangle_period = 0;

    rectangle[n].duty = 0;
    rectangle[n].duty_counter = 0;
    rectangle[n].period = 0;
    rectangle[n].period_counter = 1;
  }

  triangle.length_counter = 0;

  triangle.linear_length = 0;
  triangle.halt_length_counter = 0;
  triangle.period = 0;
  triangle.period_counter = 1;
  triangle.step_counter = 0;
  triangle.linear_length_counter = 0;
  triangle.reload_linear = 0;

  noise.length_counter = 0;

  noise.envelope.speed = 0;
  noise.envelope.use_speed_as_volume = 0;
  noise.envelope.loop_mode = 0;
  noise.envelope.reload_decay = 0;
  noise.envelope.decay_counter = 0;
  noise.envelope.decay_volume = 0;

  noise.period = 0;
  noise.period_counter = 1;
  noise.short_mode = 0;
  noise.lfsr = 1;

  dmc.irq = 0;
  dmc.loop = 0;
  dmc.period = 0;
  dmc.counter = 0;
  dmc.addr = 0xc000;
  dmc.length = 1;

  frame.mode = 0;
  frame.counter = 0;
  frame.divider = 1;

  enabled_channels = 0;
}

uint8 APU::read(uint16 addr) {
  if(addr == 0x4015) {
    uint8 result = 0x00;
    result |= rectangle[0].length_counter ? 1 : 0;
    result |= rectangle[1].length_counter ? 2 : 0;
    result |=     triangle.length_counter ? 4 : 0;
    result |=        noise.length_counter ? 8 : 0;
    return result;
  }

  return cpu.mdr();
}

void APU::write(uint16 addr, uint8 data) {
  const unsigned r = (addr >> 2) & 1;  //rectangle#

  switch(addr) {
  case 0x4000: case 0x4004:
    rectangle[r].duty = data >> 6;
    rectangle[r].envelope.loop_mode = data & 0x20;
    rectangle[r].envelope.use_speed_as_volume = data & 0x10;
    rectangle[r].envelope.speed = data & 0x0f;
    break;

  case 0x4001: case 0x4005:
    rectangle[r].sweep.enable = data & 0x80;
    rectangle[r].sweep.period = (data >> 4) & 7;
    rectangle[r].sweep.decrement = data & 0x08;
    rectangle[r].sweep.shift = data & 0x07;
    rectangle[r].sweep.reload = true;
    break;

  case 0x4002: case 0x4006:
    rectangle[r].period &= 0x700;
    rectangle[r].period |= data;
    rectangle[r].sweep.rectangle_period &= 0x700;
    rectangle[r].sweep.rectangle_period |= data;
    break;

  case 0x4003: case 0x4007:
    rectangle[r].period &= 0xff;
    rectangle[r].period |= (data & 7) << 8;
    rectangle[r].sweep.rectangle_period &= 0xff;
    rectangle[r].sweep.rectangle_period |= (data & 7) << 8;

    rectangle[r].duty_counter = 7;
    rectangle[r].envelope.reload_decay = true;

    if(enabled_channels & (1 << r)) {
      rectangle[r].length_counter = length_counter_table[(data >> 3) & 0x1f];
    }
    break;

  case 0x4008:
    triangle.halt_length_counter = data & 0x80;
    triangle.linear_length = data & 0x7f;
    break;

  case 0x400a:
    triangle.period &= 0x700;
    triangle.period |= data;
    break;

  case 0x400b:
    triangle.period &= 0xff;
    triangle.period |= (data & 7) << 8;

    triangle.reload_linear = true;

    if(enabled_channels & (1 << 2)) {
      triangle.length_counter = length_counter_table[(data >> 3) & 0x1f];
    }
    break;

  case 0x400c:
    noise.envelope.loop_mode = data & 0x20;
    noise.envelope.use_speed_as_volume = data & 0x10;
    noise.envelope.speed = data & 0x0f;
    break;

  case 0x400e:
    noise.short_mode = data & 0x80;
    noise.period = data & 0x0f;
    break;

  case 0x400f:
    noise.envelope.reload_decay = true;

    if(enabled_channels & (1 << 3)) {
      noise.length_counter = length_counter_table[(data >> 3) & 0x1f];
    }
    break;

  case 0x4010:
    dmc.irq = data & 0x80;
    dmc.loop = data & 0x40;
    dmc.period = ntsc_dmc_period_table[data & 0x0f];

  case 0x4011:
    dmc.counter = data & 0x7f;
    break;

  case 0x4012:
    dmc.addr = 0xc000 | (data << 6);
    break;

  case 0x4013:
    dmc.length = (data << 4) | 1;
    break;

  case 0x4015:
    if((data & 0x01) == 0) rectangle[0].length_counter = 0;
    if((data & 0x02) == 0) rectangle[1].length_counter = 0;
    if((data & 0x04) == 0)     triangle.length_counter = 0;
    if((data & 0x08) == 0)        noise.length_counter = 0;
    enabled_channels = data & 0x1f;
    break;

  case 0x4017:
    frame.mode = data >> 6;

    frame.counter = 0;
    if(frame.mode & 2) clock_frame_counter();
    frame.divider = 14915;
    break;
  }
}

void APU::Sweep::clock() {
  if(--counter == 0) {
    counter = period + 1;
    if(enable & shift && rectangle_period > 8) {
      signed delta = rectangle_period >> shift;

      if(decrement) {
        rectangle_period -= delta;
      //decrement first square wave by one extra
      } else if((rectangle_period + delta) < 0x800) {
        rectangle_period += delta;
      }
    }
  }

  if(reload) {
    reload = false;
    counter = period + 1;
  }
}

unsigned APU::Envelope::volume() const {
  return use_speed_as_volume ? speed :decay_volume;
}

void APU::Envelope::clock() {
  if(reload_decay) {
    reload_decay = false;
    decay_volume = 0x0f;
    decay_counter = speed + 1;
    return;
  }

  if(--decay_counter == 0) {
    decay_counter = speed + 1;
    if(decay_volume || loop_mode) decay_volume--;
  }
}

void APU::Rectangle::clock_length() {
  if(envelope.loop_mode == 0) {
    if(length_counter) length_counter--;
  }
}

bool APU::Rectangle::check_period() {
  const unsigned raw_period = sweep.rectangle_period;

  if(raw_period < 0x008 || raw_period > 0x7ff) return false;

  if(sweep.decrement == 0) {
    if((raw_period + (raw_period >> sweep.shift)) & 0x800) return false;
  }

  return true;
}

uint8 APU::Rectangle::clock() {
  if(check_period() == false) return 0;
  if(length_counter == 0) return 0;

  static const unsigned duty_table[] = { 1, 2, 4, 6 };
  uint8 result = (duty_counter < duty_table[duty]) ? envelope.volume() : 0;

  if(--period_counter == 0) {
    period_counter = (sweep.rectangle_period + 1) * 2;
    duty_counter++;
  }

  return result;
}

void APU::Triangle::clock_length() {
  if(halt_length_counter == 0) {
    if(length_counter > 0) length_counter--;
  }
}

void APU::Triangle::clock_length_counter() {
  if(reload_linear) {
    linear_length_counter = linear_length;
  } else if(linear_length_counter) {
    linear_length_counter--;
  }

  if(halt_length_counter == 0) reload_linear = false;
}

uint8 APU::Triangle::clock() {
  uint8 result = (step_counter & 0x0f) ^ ((step_counter & 0x10) ? 0x0f : 0x00);
  if(length_counter == 0 || linear_length_counter == 0) return result;

  if(--period_counter == 0) {
    step_counter = (step_counter + 1) & 0x1f;
    period_counter = period + 1;
  }

  return result;
}

void APU::Noise::clock_length() {
  if(envelope.loop_mode == 0) {
    if(length_counter > 0) length_counter--;
  }
}

uint8 APU::Noise::clock() {
  if(length_counter == 0) return 0;

  uint8 result = (lfsr & 1) ? envelope.volume() : 0;

  if(--period_counter == 0) {
    unsigned feedback;

    if(short_mode) {
      feedback = ((lfsr >> 0) & 1) ^ ((lfsr >> 6) & 1);
    } else {
      feedback = ((lfsr >> 0) & 1) ^ ((lfsr >> 1) & 1);
    }

    lfsr = (lfsr >> 1) | (feedback << 14);
    period_counter = apu.ntsc_noise_period_table[period];
  }

  return result;
}

uint8 APU::DMC::clock() {
  uint8 result = counter;

  return result;
}

void APU::clock_frame_counter() {
  frame.counter++;

  if(frame.counter & 1) {
    rectangle[0].clock_length();
    rectangle[0].sweep.clock();
    rectangle[1].clock_length();
    rectangle[1].sweep.clock();
    triangle.clock_length();
    noise.clock_length();
  }

  rectangle[0].envelope.clock();
  rectangle[1].envelope.clock();
  triangle.clock_length_counter();
  noise.envelope.clock();

  if(frame.counter == 0) {
    if(frame.mode & 2) frame.divider += 14195;
  }
}

void APU::clock_frame_counter_divider() {
  frame.divider -= 2;
  if(frame.divider <= 0) {
    clock_frame_counter();
    frame.divider += 14195;
  }
}

APU::APU() {
  for(unsigned amp = 0; amp < 32; amp++) {
    if(amp == 0) {
      rectangle_dac[amp] = 0;
    } else {
      rectangle_dac[amp] = 16384.0 * 95.88 / (8128.0 / amp + 100.0);
    }
  }

  for(unsigned dmc_amp = 0; dmc_amp < 128; dmc_amp++) {
    for(unsigned triangle_amp = 0; triangle_amp < 16; triangle_amp++) {
      for(unsigned noise_amp = 0; noise_amp < 16; noise_amp++) {
        if(dmc_amp == 0 && triangle_amp == 0 && noise_amp == 0) {
          dmc_triangle_noise_dac[dmc_amp][triangle_amp][noise_amp] = 0;
        } else {
          dmc_triangle_noise_dac[dmc_amp][triangle_amp][noise_amp]
          = 16384.0 * 159.79 / (100.0 + 1.0 / (triangle_amp / 8227.0 + noise_amp / 12241.0 + dmc_amp / 22638.0));
        }
      }
    }
  }
}

}
