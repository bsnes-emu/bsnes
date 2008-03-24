#ifdef SDSP_CPP

void sDSP::brr_decode(voice_t &v) {
  //state.t_brr_byte = ram[v.brr_addr + v.brr_offset] cached from previous clock cycle
  int nybbles = (state.t_brr_byte << 8) + ram[(uint16)(v.brr_addr + v.brr_offset + 1)];

  const int filter = (state.t_brr_header >> 2) & 3;
  const int scale  = (state.t_brr_header >> 4);

  //write to next four samples in circular buffer
  int *pos = &v.buf[v.buf_pos];
  v.buf_pos += 4;
  if(v.buf_pos >= brr_buf_size) v.buf_pos = 0;

  //decode four samples
  for(int *end = pos + 4; pos < end; pos++) {
    int s = sclip<4>(nybbles >> 12); //extract upper nybble and sign extend
    nybbles <<= 4;                   //advance nybble position
    if(scale <= 12) {
      s <<= scale;
      s >>= 1;
    } else {
      s &= ~0x7ff;
    }

    //apply IIR filter (2 is the most commonly used)
    const int p1 = pos[brr_buf_size - 1];
    const int p2 = pos[brr_buf_size - 2] >> 1;

    switch(filter) {
      case 0: break; //no filter

      case 1: {
        //s += p1 * 0.46875
        s += p1 >> 1;
        s += (-p1) >> 5;
      } break;

      case 2: {
        //s += p1 * 0.953125 - p2 * 0.46875
        s += p1;
        s -= p2;
        s += p2 >> 4;
        s += (p1 * -3) >> 6;
      } break;

      case 3: {
        //s += p1 * 0.8984375 - p2 * 0.40625
        s += p1;
        s -= p2;
        s += (p1 * -13) >> 7;
        s += (p2 * 3) >> 4;
      } break;
    }

    //adjust and write sample
    s = sclamp<16>(s);
    s = sclip<16>(s << 1);
    pos[brr_buf_size] = pos[0] = s; //second copy simplifies wrap-around
  }
}

#endif //ifdef SDSP_CPP
