/*
 * interpolation routines are located in: /src/lib/libinterp.h
 */

/*

inline uint Audio::bind_range(uint min, uint max, uint index) {
  return index < min ? min : index > max ? max : index;
}

void Audio::resample_point(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
double scalar = double(input_samples--) / double(output_samples--); //convert lengths to upper bounds
double sindex = 0.0;
  for(uint x = 0; x <= output_samples; x++) {
  uint32 y0 = input[bind_range(0, input_samples, uint32(sindex) + 0)];
  uint32 y1 = input[bind_range(0, input_samples, uint32(sindex) + 1)];
  double mu = sindex - uint(sindex); //calculate fractional portion of step
  uint16 yl = interpolate_point(mu, int16(y0 >>  0), int16(y1 >>  0));
  uint16 yr = interpolate_point(mu, int16(y0 >> 16), int16(y1 >> 16));
    output[x] = (yl << 0) + (yr << 16);
    sindex += scalar;
  }
}

void Audio::resample_linear(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
double scalar = double(input_samples--) / double(output_samples--); //convert lengths to upper bounds
double sindex = 0.0;
  for(uint x = 0; x <= output_samples; x++) {
  uint32 y0 = input[bind_range(0, input_samples, uint32(sindex) + 0)];
  uint32 y1 = input[bind_range(0, input_samples, uint32(sindex) + 1)];
  double mu = sindex - uint(sindex); //calculate fractional portion of step
  uint16 yl = interpolate_linear(mu, int16(y0 >>  0), int16(y1 >>  0));
  uint16 yr = interpolate_linear(mu, int16(y0 >> 16), int16(y1 >> 16));
    output[x] = (yl << 0) + (yr << 16);
    sindex += scalar;
  }
}

void Audio::resample_cosine(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
double scalar = double(input_samples--) / double(output_samples--); //convert lengths to upper bounds
double sindex = 0.0;
  for(uint x = 0; x <= output_samples; x++) {
  uint32 y0 = input[bind_range(0, input_samples, uint32(sindex) + 0)];
  uint32 y1 = input[bind_range(0, input_samples, uint32(sindex) + 1)];
  double mu = sindex - uint(sindex); //calculate fractional portion of step
  uint16 yl = interpolate_cosine(mu, int16(y0 >>  0), int16(y1 >>  0));
  uint16 yr = interpolate_cosine(mu, int16(y0 >> 16), int16(y1 >> 16));
    output[x] = (yl << 0) + (yr << 16);
    sindex += scalar;
  }
}

void Audio::resample_cubic(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
double scalar = double(input_samples--) / double(output_samples--); //convert lengths to upper bounds
double sindex = 0.0;
  for(uint x = 0; x <= output_samples; x++) {
  uint32 y0 = input[bind_range(0, input_samples, uint32(sindex) - 1)];
  uint32 y1 = input[bind_range(0, input_samples, uint32(sindex) + 0)];
  uint32 y2 = input[bind_range(0, input_samples, uint32(sindex) + 1)];
  uint32 y3 = input[bind_range(0, input_samples, uint32(sindex) + 2)];
  double mu = sindex - uint(sindex); //calculate fractional portion of step
  uint16 yl = sclamp<16>( interpolate_cubic(mu, int16(y0 >>  0), int16(y1 >>  0), int16(y2 >>  0), int16(y3 >>  0)) );
  uint16 yr = sclamp<16>( interpolate_cubic(mu, int16(y0 >> 16), int16(y1 >> 16), int16(y2 >> 16), int16(y3 >> 16)) );
    output[x] = (yl << 0) + (yr << 16);
    sindex += scalar;
  }
}

void Audio::resample_hermite(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
double scalar = double(input_samples--) / double(output_samples--); //convert lengths to upper bounds
double sindex = 0.0;
  for(uint x = 0; x <= output_samples; x++) {
  uint32 y0 = input[bind_range(0, input_samples, uint32(sindex) - 1)];
  uint32 y1 = input[bind_range(0, input_samples, uint32(sindex) + 0)];
  uint32 y2 = input[bind_range(0, input_samples, uint32(sindex) + 1)];
  uint32 y3 = input[bind_range(0, input_samples, uint32(sindex) + 2)];
  double mu = sindex - uint(sindex); //calculate fractional portion of step
  uint16 yl = sclamp<16>( interpolate_hermite(mu, 0.0, 0.0, int16(y0 >>  0), int16(y1 >>  0), int16(y2 >>  0), int16(y3 >>  0)) );
  uint16 yr = sclamp<16>( interpolate_hermite(mu, 0.0, 0.0, int16(y0 >> 16), int16(y1 >> 16), int16(y2 >> 16), int16(y3 >> 16)) );
    output[x] = (yl << 0) + (yr << 16);
    sindex += scalar;
  }
}

*/

//

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
