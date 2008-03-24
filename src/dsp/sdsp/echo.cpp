#ifdef SDSP_CPP

//current echo buffer pointer for left/right channel
#define ECHO_PTR(ch) (&ram[state.t_echo_ptr + ch * 2])

//sample in echo history buffer, where 0 is the oldest
#define ECHO_FIR(i) state.echo_hist_pos[i]

//calculate FIR point for left/right channel
#define CALC_FIR(i, ch) ((ECHO_FIR(i + 1)[ch] * (int8)REG(fir + i * 0x10)) >> 6)

void sDSP::echo_read(bool channel) {
  uint8 *in = ECHO_PTR(channel);
  int s = (int8)in[1] * 0x100 + in[0];
  //second copy simplifies wrap-around handling
  ECHO_FIR(0)[channel] = ECHO_FIR(8)[channel] = s >> 1;
}

int sDSP::echo_output(bool channel) {
  int output = sclip<16>((state.t_main_out[channel] * (int8)REG(mvoll + channel * 0x10)) >> 7)
             + sclip<16>((state.t_echo_in [channel] * (int8)REG(evoll + channel * 0x10)) >> 7);
  return sclamp<16>(output);
}

void sDSP::echo_write(bool channel) {
  if(!(state.t_echo_enabled & 0x20)) {
    uint8 *out = ECHO_PTR(channel);
    int s = state.t_echo_out[channel];
    out[0] = (uint8)s;
    out[1] = (uint8)(s >> 8);
  }

  state.t_echo_out[channel] = 0;
}

void sDSP::echo_22() {
  //history
  state.echo_hist_pos++;
  if(state.echo_hist_pos >= &state.echo_hist[8]) state.echo_hist_pos = state.echo_hist;

  state.t_echo_ptr = (uint16)((state.t_esa << 8) + state.echo_offset);
  echo_read(0);

  //FIR
  int l = CALC_FIR(0, 0);
  int r = CALC_FIR(0, 1);

  state.t_echo_in[0] = l;
  state.t_echo_in[1] = r;
}

void sDSP::echo_23() {
  int l = CALC_FIR(1, 0) + CALC_FIR(2, 0);
  int r = CALC_FIR(1, 1) + CALC_FIR(2, 1);

  state.t_echo_in[0] += l;
  state.t_echo_in[1] += r;

  echo_read(1);
}

void sDSP::echo_24() {
  int l = CALC_FIR(3, 0) + CALC_FIR(4, 0) + CALC_FIR(5, 0);
  int r = CALC_FIR(3, 1) + CALC_FIR(4, 1) + CALC_FIR(5, 1);

  state.t_echo_in[0] += l;
  state.t_echo_in[1] += r;
}

void sDSP::echo_25() {
  int l = state.t_echo_in[0] + CALC_FIR(6, 0);
  int r = state.t_echo_in[1] + CALC_FIR(6, 1);

  l = sclip<16>(l);
  r = sclip<16>(r);

  l += (int16)CALC_FIR(7, 0);
  r += (int16)CALC_FIR(7, 1);

  state.t_echo_in[0] = sclamp<16>(l) & ~1;
  state.t_echo_in[1] = sclamp<16>(r) & ~1;
}

void sDSP::echo_26() {
  //left output volumes
  //(save sample for next clock so we can output both together)
  state.t_main_out[0] = echo_output(0);

  //echo feedback
  int l = state.t_echo_out[0] + sclip<16>((state.t_echo_in[0] * (int8)REG(efb)) >> 7);
  int r = state.t_echo_out[1] + sclip<16>((state.t_echo_in[1] * (int8)REG(efb)) >> 7);

  state.t_echo_out[0] = sclamp<16>(l) & ~1;
  state.t_echo_out[1] = sclamp<16>(r) & ~1;
}

void sDSP::echo_27() {
  //output
  int outl = state.t_main_out[0];
  int outr = echo_output(1);
  state.t_main_out[0] = 0;
  state.t_main_out[1] = 0;

  //TODO: global muting isn't this simple
  //(turns DAC on and off or something, causing small ~37-sample pulse when first muted)
  if(REG(flg) & 0x40) {
    outl = 0;
    outr = 0;
  }

  //output sample to DAC
  snes.audio.update(outl, outr);
}

void sDSP::echo_28() {
  state.t_echo_enabled = REG(flg);
}

void sDSP::echo_29() {
  state.t_esa = REG(esa);

  if(!state.echo_offset) state.echo_length = (REG(edl) & 0x0f) << 11;

  state.echo_offset += 4;
  if(state.echo_offset >= state.echo_length) state.echo_offset = 0;

  //write left echo
  echo_write(0);

  state.t_echo_enabled = REG(flg);
}

void sDSP::echo_30() {
  //write right echo
  echo_write(1);
}

#endif //ifdef SDSP_CPP
