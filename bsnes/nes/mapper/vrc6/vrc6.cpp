VRC6 vrc6;

//

void VRC6::Pulse::clock() {
  if(--divider == 0) {
    divider = frequency;
    cycle++;
    output = (mode == 1 || cycle < duty) ? volume : (uint4)0;
  }

  if(enable == false) output = 0;
}

void VRC6::Pulse::serialize(serializer &s) {
  s.integer(mode);
  s.integer(duty);
  s.integer(volume);
  s.integer(enable);
  s.integer(frequency);

  s.integer(divider);
  s.integer(cycle);
  s.integer(output);
}

VRC6::Pulse::Pulse() {
  for(unsigned n = 0; n < 16; n++) {
    dac[n] = -15360 + (n * 2048);
  }
}

//

void VRC6::Sawtooth::clock() {
  if(--divider == 0) {
    divider = frequency;
    if(phase == 0) {
      phase = 0;
      accumulator = 0;
    } else if((phase & 1) == 0) {
      accumulator += rate;
    }
  }

  output = accumulator >> 3;
  if(enable == false) output = 0;
}

void VRC6::Sawtooth::serialize(serializer &s) {
  s.integer(rate);
  s.integer(enable);
  s.integer(frequency);

  s.integer(divider);
  s.integer(phase);
  s.integer(accumulator);
  s.integer(output);
}

VRC6::Sawtooth::Sawtooth() {
  for(unsigned n = 0; n < 32; n++) {
    dac[n] = -15872 + (n * 1024);
  }
}

//

void VRC6::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(irq_enable) {
      if(irq_mode == 0) {
        irq_scalar -= 3;
        if(irq_scalar <= 0) {
          irq_scalar += 341;
          if(irq_counter == 0xff) {
            irq_counter = irq_latch;
            irq_line = 1;
          } else {
            irq_counter++;
          }
        }
      }

      if(irq_mode == 1) {
        if(irq_counter == 0xff) {
          irq_counter = irq_latch;
          irq_line = 1;
        } else {
          irq_counter++;
        }
      }
    }

    pulse1.clock();
    pulse2.clock();
    sawtooth.clock();

    signed output = 0;
    output +=   pulse1.dac[  pulse1.output];
    output +=   pulse2.dac[  pulse2.output];
    output += sawtooth.dac[sawtooth.output];
    output /= 6;  //div by 3 for channel count; div that by 2 to match NES volume intensity
    output = sclamp<16>(output);

    apu.set_sample(output);
    cpu.set_irq_line(irq_line);
    tick();
  }
}

uint8 VRC6::prg_read(uint16 addr) {
  if((addr & 0xe000) == 0x6000) {
    return prg_ram[addr & 0x1fff];
  }

  if((addr & 0xc000) == 0x8000) {
    return prg_data((prg_bank[0] << 14) | (addr & 0x3fff));
  }

  if((addr & 0xe000) == 0xc000) {
    return prg_data((prg_bank[1] << 13) | (addr & 0x1fff));
  }

  if((addr & 0xe000) == 0xe000) {
    return prg_data((0xff << 13) | (addr & 0x1fff));
  }

  return cpu.mdr();
}

void VRC6::prg_write(uint16 addr, uint8 data) {
  if((addr & 0xe000) == 0x6000) {
    prg_ram[addr & 0x1fff] = data;
    return;
  }

  addr = (addr & 0xf003);
  if(abus_swap) addr = (addr & ~3) | ((addr & 2) >> 1) | ((addr & 1) << 1);

  switch(addr) {
  case 0x8000: case 0x8001: case 0x8002: case 0x8003:
    prg_bank[0] = data;
    break;

  case 0x9000:
    pulse1.mode = data & 0x80;
    pulse1.duty = (data & 0x70) >> 4;
    pulse1.volume = data & 0x0f;
    break;

  case 0x9001:
    pulse1.frequency = (pulse1.frequency & 0x0f00) | ((data & 0xff) << 0);
    break;

  case 0x9002:
    pulse1.frequency = (pulse1.frequency & 0x00ff) | ((data & 0x0f) << 8);
    pulse1.enable = data & 0x80;
    break;

  case 0xa000:
    pulse2.mode = data & 0x80;
    pulse2.duty = (data & 0x70) >> 4;
    pulse2.volume = data & 0x0f;
    break;

  case 0xa001:
    pulse2.frequency = (pulse2.frequency & 0x0f00) | ((data & 0xff) << 0);
    break;

  case 0xa002:
    pulse2.frequency = (pulse2.frequency & 0x00ff) | ((data & 0x0f) << 8);
    pulse2.enable = data & 0x80;
    break;

  case 0xb000:
    sawtooth.rate = data & 0x3f;
    break;

  case 0xb001:
    sawtooth.frequency = (sawtooth.frequency & 0x0f00) | ((data & 0xff) << 0);
    break;

  case 0xb002:
    sawtooth.frequency = (sawtooth.frequency & 0x00ff) | ((data & 0x0f) << 8);
    sawtooth.enable = data & 0x80;
    break;

  case 0xb003:
    mirror_select = (data >> 2) & 3;
    break;

  case 0xc000: case 0xc001: case 0xc002: case 0xc003:
    prg_bank[1] = data;
    break;

  case 0xd000: case 0xd001: case 0xd002: case 0xd003:
    chr_bank[0 + (addr & 3)] = data;
    break;

  case 0xe000: case 0xe001: case 0xe002: case 0xe003:
    chr_bank[4 + (addr & 3)] = data;
    break;

  case 0xf000:
    irq_latch = data;
    break;

  case 0xf001:
    irq_mode = data & 0x04;
    irq_enable = data & 0x02;
    irq_acknowledge = data & 0x01;
    if(irq_enable) {
      irq_counter = irq_latch;
      irq_scalar = 341;
    }
    irq_line = 0;
    break;

  case 0xf002:
    irq_enable = irq_acknowledge;
    irq_line = 0;
    break;
  }
}

uint8 VRC6::chr_read(uint16 addr) {
  unsigned bank = chr_bank[(addr >> 10) & 7];
  return chr_data((bank << 10) | (addr & 0x03ff));
}

void VRC6::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  unsigned bank = chr_bank[(addr >> 10) & 7];
  chr_data((bank << 10) | (addr & 0x03ff)) = data;
}

unsigned VRC6::ciram_addr(unsigned addr) const {
  switch(mirror_select) {
  case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
  case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
  case 2: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
  case 3: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
  }
}

uint8 VRC6::ciram_read(uint13 addr) {
  return ppu.ciram_read(ciram_addr(addr));
}

void VRC6::ciram_write(uint13 addr, uint8 data) {
  return ppu.ciram_write(ciram_addr(addr), data);
}

unsigned VRC6::ram_size() {
  return 8192u;
}

uint8* VRC6::ram_data() {
  return prg_ram;
}

void VRC6::power() {
  reset();
}

void VRC6::reset() {
  foreach(n, prg_ram) n = 0xff;

  prg_bank[0] = 0;
  prg_bank[1] = 0;
  chr_bank[0] = 0;
  chr_bank[1] = 0;
  chr_bank[2] = 0;
  chr_bank[3] = 0;
  chr_bank[4] = 0;
  chr_bank[5] = 0;
  chr_bank[6] = 0;
  chr_bank[7] = 0;
  mirror_select = 0;
  irq_latch = 0;
  irq_mode = 0;
  irq_enable = 0;
  irq_acknowledge = 0;

  irq_counter = 0;
  irq_scalar = 0;
  irq_line = 0;

  pulse1.mode = 0;
  pulse1.duty = 0;
  pulse1.volume = 0;
  pulse1.enable = 0;
  pulse1.frequency = 0;

  pulse1.divider = 0;
  pulse1.cycle = 0;
  pulse1.output = 0;

  pulse2.mode = 0;
  pulse2.duty = 0;
  pulse2.volume = 0;
  pulse2.enable = 0;
  pulse2.frequency = 0;

  pulse2.divider = 0;
  pulse2.cycle = 0;
  pulse2.output = 0;

  sawtooth.rate = 0;
  sawtooth.enable = 0;
  sawtooth.frequency = 0;

  sawtooth.divider = 0;
  sawtooth.phase = 0;
  sawtooth.accumulator = 0;
  sawtooth.output = 0;
}

void VRC6::serialize(serializer &s) {
  s.array(prg_ram);

  s.array(prg_bank);
  s.array(chr_bank);
  s.integer(mirror_select);
  s.integer(irq_latch);
  s.integer(irq_mode);
  s.integer(irq_enable);
  s.integer(irq_acknowledge);

  s.integer(irq_counter);
  s.integer(irq_scalar);
  s.integer(irq_line);

  pulse1.serialize(s);
  pulse2.serialize(s);
  sawtooth.serialize(s);
}
