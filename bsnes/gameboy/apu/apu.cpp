#include <gameboy/gameboy.hpp>

#define APU_CPP
namespace GameBoy {

#include "mmio/mmio.cpp"
#include "serialization.cpp"
APU apu;

void APU::power() {
  for(unsigned n = 0xff10; n <= 0xff3f; n++) bus.mmio[n] = this;

  channel1.sweep_time = 0;
  channel1.sweep_direction = 0;
  channel1.sweep_shift = 0;

  channel1.wave_pattern_duty = 0;
  channel1.sound_length = 0;

  channel1.initial_envelope_volume = 0;
  channel1.envelope_direction = 0;
  channel1.envelope_sweep = 0;

  channel1.frequency = 0;
  channel1.initialize = 0;
  channel1.consecutive_selection = 0;

  channel2.wave_pattern_duty = 0;
  channel2.sound_length = 0;

  channel2.initial_envelope_volume = 0;
  channel2.envelope_direction = 0;
  channel2.envelope_sweep = 0;

  channel2.frequency = 0;
  channel2.initialize = 0;
  channel2.consecutive_selection = 0;

  channel3.off = 0;

  channel3.sound_length = 0;

  channel3.output_level = 0;

  channel3.frequency = 0;
  channel3.initialize = 0;
  channel3.consecutive_selection = 0;

  for(unsigned n = 0; n < 16; n++) channel3.pattern[n] = 0;

  channel4.sound_length = 0;

  channel4.initial_envelope_volume = 0;
  channel4.envelope_direction = 0;
  channel4.envelope_sweep = 0;

  channel4.shift_clock_frequency = 0;
  channel4.counter_step_width = 0;
  channel4.dividing_ratio = 0;

  channel4.initialize = 0;
  channel4.consecutive_selection = 0;

  control.output_vin_to_so2 = 0;
  control.so2_output_level = 0;
  control.output_vin_to_so1 = 0;
  control.so1_output_level = 0;

  control.output_channel4_to_so2 = 0;
  control.output_channel3_to_so2 = 0;
  control.output_channel2_to_so2 = 0;
  control.output_channel1_to_so2 = 0;
  control.output_channel4_to_so1 = 0;
  control.output_channel3_to_so1 = 0;
  control.output_channel2_to_so1 = 0;
  control.output_channel1_to_so1 = 0;

  control.sound_on = 0;
  control.channel4_on = 0;
  control.channel3_on = 0;
  control.channel2_on = 0;
  control.channel1_on = 0;
}

}
