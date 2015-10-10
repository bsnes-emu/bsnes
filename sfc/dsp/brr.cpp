#ifdef DSP_CPP

auto DSP::brrDecode(Voice& v) -> void {
  //state.t_brr_byte = ram[v.brr_addr + v.brr_offset] cached from previous clock cycle
  signed nybbles = (state._brrByte << 8) + smp.apuram[(uint16)(v.brrAddress + v.brrOffset + 1)];

  const signed filter = (state._brrHeader >> 2) & 3;
  const signed scale  = (state._brrHeader >> 4);

  //decode four samples
  for(auto n : range(4)) {
    //bits 12-15 = current nybble; sign extend, then shift right to 4-bit precision
    //result: s = 4-bit sign-extended sample value
    signed s = (int16)nybbles >> 12;
    nybbles <<= 4; //slide nybble so that on next loop iteration, bits 12-15 = current nybble

    if(scale <= 12) {
      s <<= scale;
      s >>= 1;
    } else {
      s &= ~0x7ff;
    }

    //apply IIR filter (2 is the most commonly used)
    const signed p1 = v.buffer[v.bufferOffset - 1];
    const signed p2 = v.buffer[v.bufferOffset - 2] >> 1;

    switch(filter) {
    case 0:
      break;

    case 1:
      //s += p1 * 0.46875
      s += p1 >> 1;
      s += (-p1) >> 5;
      break;

    case 2:
      //s += p1 * 0.953125 - p2 * 0.46875
      s += p1;
      s -= p2;
      s += p2 >> 4;
      s += (p1 * -3) >> 6;
      break;

    case 3:
      //s += p1 * 0.8984375 - p2 * 0.40625
      s += p1;
      s -= p2;
      s += (p1 * -13) >> 7;
      s += (p2 * 3) >> 4;
      break;
    }

    //adjust and write sample
    s = sclamp<16>(s);
    s = (int16)(s << 1);
    v.buffer.write(v.bufferOffset++, s);
    if(v.bufferOffset >= BrrBufferSize) v.bufferOffset = 0;
  }
}

#endif
