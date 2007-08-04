#include "audio.h"

void Audio::update_frequency() {
uint freq  = config::audio.frequency;
uint speed = config::system.speed_normal;
  switch(uint(config::system.speed)) {
  case 1: speed = config::system.speed_slowest; break;
  case 2: speed = config::system.speed_slow;    break;
  case 3: speed = config::system.speed_normal;  break;
  case 4: speed = config::system.speed_fast;    break;
  case 5: speed = config::system.speed_fastest; break;
  }
//convert speed setting from integer to fp-percentage and adjust default frequency by this value
  frequency = uint( double(speed) * 0.01 * double(freq) );
//convert latency from time in ms to time in samples, divide by 3 for ring buffers
  latency = uint( (double(frequency) / 1000.0) * (double(config::audio.latency) / 3.0) );
}

Audio::Audio() {
  update_frequency();
}
