#include <nes/nes.hpp>

namespace NES {

#include "serialization.cpp"
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
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    unsigned rectangle_output, triangle_output, noise_output, dmc_output;

    rectangle_output  = rectangle[0].clock();
    rectangle_output += rectangle[1].clock();
    triangle_output = triangle.clock();
    noise_output = noise.clock();
    dmc_output = dmc.clock();

    clock_frame_counter_divider();

    signed output = rectangle_dac[rectangle_output] + dmc_triangle_noise_dac[dmc_output][triangle_output][noise_output];

    output  = filter.run_hipass_strong(output);
    output += cartridge_sample;
    output  = filter.run_hipass_weak(output);
  //output  = filter.run_lopass(output);
    output  = sclamp<16>(output);

    interface->audioSample(output);

    tick();
  }
}

void APU::tick() {
  clock += 12;
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

void APU::set_irq_line() {
  cpu.set_irq_apu_line(frame.irq_pending || dmc.irq_pending);
}

void APU::set_sample(int16 sample) {
  cartridge_sample = sample;
}

void APU::power() {
  filter.hipass_strong = 0;
  filter.hipass_weak = 0;
  filter.lopass = 0;

  for(unsigned n = 0; n < 2; n++) {
    rectangle[n].sweep.shift = 0;
    rectangle[n].sweep.decrement = 0;
    rectangle[n].sweep.period = 0;
    rectangle[n].sweep.counter = 1;
    rectangle[n].sweep.enable = 0;
    rectangle[n].sweep.reload = 0;
    rectangle[n].sweep.rectangle_period = 0;
  }

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

  dmc.length_counter = 0;
  dmc.irq_pending = 0;

  dmc.period = 0;
  dmc.period_counter = ntsc_dmc_period_table[0];
  dmc.irq_enable = 0;
  dmc.loop_mode = 0;
  dmc.dac_latch = 0;
  dmc.addr_latch = 0;
  dmc.length_latch = 0;
  dmc.read_addr = 0;
  dmc.dma_delay_counter = 0;
  dmc.bit_counter = 0;
  dmc.have_dma_buffer = 0;
  dmc.dma_buffer = 0;
  dmc.have_sample = 0;
  dmc.sample = 0;

  frame.irq_pending = 0;

  frame.mode = 0;
  frame.counter = 0;
  frame.divider = 1;

  enabled_channels = 0;
  cartridge_sample = 0;

  set_irq_line();
}

uint8 APU::read(uint16 addr) {
  if(addr == 0x4015) {
    uint8 result = 0x00;
    result |= rectangle[0].length_counter ? 0x01 : 0;
    result |= rectangle[1].length_counter ? 0x02 : 0;
    result |=     triangle.length_counter ? 0x04 : 0;
    result |=        noise.length_counter ? 0x08 : 0;
    result |=          dmc.length_counter ? 0x10 : 0;
    result |=           frame.irq_pending ? 0x40 : 0;
    result |=             dmc.irq_pending ? 0x80 : 0;

    frame.irq_pending = false;
    set_irq_line();

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
    rectangle[r].sweep.period = (data & 0x70) >> 4;
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
    dmc.irq_enable = data & 0x80;
    dmc.loop_mode = data & 0x40;
    dmc.period = data & 0x0f;

    dmc.irq_pending = dmc.irq_pending && dmc.irq_enable && !dmc.loop_mode;
    set_irq_line();
    break;

  case 0x4011:
    dmc.dac_latch = data & 0x7f;
    break;

  case 0x4012:
    dmc.addr_latch = data;
    break;

  case 0x4013:
    dmc.length_latch = data;
    break;

  case 0x4015:
    if((data & 0x01) == 0) rectangle[0].length_counter = 0;
    if((data & 0x02) == 0) rectangle[1].length_counter = 0;
    if((data & 0x04) == 0)     triangle.length_counter = 0;
    if((data & 0x08) == 0)        noise.length_counter = 0;

    (data & 0x10) ? dmc.start() : dmc.stop();
    dmc.irq_pending = false;

    set_irq_line();
    enabled_channels = data & 0x1f;
    break;

  case 0x4017:
    frame.mode = data >> 6;

    frame.counter = 0;
    if(frame.mode & 2) clock_frame_counter();
    if(frame.mode & 1) {
      frame.irq_pending = false;
      set_irq_line();
    }
    frame.divider = FrameCounter::NtscPeriod;
    break;
  }
}

signed APU::Filter::run_hipass_strong(signed sample) {
  hipass_strong += ((((int64)sample << 16) - (hipass_strong >> 16)) * HiPassStrong) >> 16;
  return sample - (hipass_strong >> 32);
}

signed APU::Filter::run_hipass_weak(signed sample) {
  hipass_weak += ((((int64)sample << 16) - (hipass_weak >> 16)) * HiPassWeak) >> 16;
  return sample - (hipass_weak >> 32);
}

signed APU::Filter::run_lopass(signed sample) {
  lopass += ((((int64)sample << 16) - (lopass >> 16)) * LoPass) >> 16;
  return (lopass >> 32);
}

bool APU::Sweep::check_period() {
  if(rectangle_period > 0x7ff) return false;

  if(decrement == 0) {
    if((rectangle_period + (rectangle_period >> shift)) & 0x800) return false;
  }

  return true;
}

void APU::Sweep::clock(unsigned channel) {
  if(--counter == 0) {
    counter = period + 1;
    if(enable && shift && rectangle_period > 8) {
      signed delta = rectangle_period >> shift;

      if(decrement) {
        rectangle_period -= delta;
        if(channel == 0) rectangle_period--;
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
  return use_speed_as_volume ? speed : decay_volume;
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

uint8 APU::Rectangle::clock() {
  if(sweep.check_period() == false) return 0;
  if(length_counter == 0) return 0;

  static const unsigned duty_table[] = { 1, 2, 4, 6 };
  uint8 result = (duty_counter < duty_table[duty]) ? envelope.volume() : 0;
  if(sweep.rectangle_period < 0x008) result = 0;

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

void APU::Triangle::clock_linear_length() {
  if(reload_linear) {
    linear_length_counter = linear_length;
  } else if(linear_length_counter) {
    linear_length_counter--;
  }

  if(halt_length_counter == 0) reload_linear = false;
}

uint8 APU::Triangle::clock() {
  uint8 result = step_counter & 0x0f;
  if((step_counter & 0x10) == 0) result ^= 0x0f;
  if(length_counter == 0 || linear_length_counter == 0) return result;

  if(--period_counter == 0) {
    step_counter++;
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

void APU::DMC::start() {
  if(length_counter == 0) {
    read_addr = 0x4000 + (addr_latch << 6);
    length_counter = (length_latch << 4) + 1;
  }
}

void APU::DMC::stop() {
  length_counter = 0;
  dma_delay_counter = 0;
  cpu.set_rdy_line(1);
  cpu.set_rdy_addr({ false, 0u });
}

uint8 APU::DMC::clock() {
  uint8 result = dac_latch;

  if(dma_delay_counter > 0) {
    dma_delay_counter--;

    if(dma_delay_counter == 1) {
      cpu.set_rdy_addr({ true, uint16(0x8000 | read_addr) });
    } else if(dma_delay_counter == 0) {
      cpu.set_rdy_line(1);
      cpu.set_rdy_addr({ false, 0u });

      dma_buffer = cpu.mdr();
      have_dma_buffer = true;
      length_counter--;
      read_addr++;

      if(length_counter == 0) {
        if(loop_mode) {
          start();
        } else if(irq_enable) {
          irq_pending = true;
          apu.set_irq_line();
        }
      }
    }
  }

  if(--period_counter == 0) {
    if(have_sample) {
      signed delta = (((sample >> bit_counter) & 1) << 2) - 2;
      unsigned data = dac_latch + delta;
      if((data & 0x80) == 0) dac_latch = data;
    }

    if(++bit_counter == 0) {
      if(have_dma_buffer) {
        have_sample = true;
        sample = dma_buffer;
        have_dma_buffer = false;
      } else {
        have_sample = false;
      }
    }

    period_counter = ntsc_dmc_period_table[period];
  }

  if(length_counter > 0 && have_dma_buffer == false && dma_delay_counter == 0) {
    cpu.set_rdy_line(0);
    dma_delay_counter = 4;
  }

  return result;
}

void APU::clock_frame_counter() {
  frame.counter++;

  if(frame.counter & 1) {
    rectangle[0].clock_length();
    rectangle[0].sweep.clock(0);
    rectangle[1].clock_length();
    rectangle[1].sweep.clock(1);
    triangle.clock_length();
    noise.clock_length();
  }

  rectangle[0].envelope.clock();
  rectangle[1].envelope.clock();
  triangle.clock_linear_length();
  noise.envelope.clock();

  if(frame.counter == 0) {
    if(frame.mode & 2) frame.divider += FrameCounter::NtscPeriod;
    if(frame.mode == 0) {
      frame.irq_pending = true;
      set_irq_line();
    }
  }
}

void APU::clock_frame_counter_divider() {
  frame.divider -= 2;
  if(frame.divider <= 0) {
    clock_frame_counter();
    frame.divider += FrameCounter::NtscPeriod;
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
