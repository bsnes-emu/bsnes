inline uint16 bPPU::addsub_pixels(uint32 cdest, uint32 csrc) {
  if(!regs.color_mode) {
  //add
    cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
    csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
    cdest += csrc;

    if(regs.color_halve) {
      cdest >>= 1;
    } else {
      if(cdest & 0x04000000)cdest |= 0x03e00000;
      if(cdest & 0x00008000)cdest |= 0x00007c00;
      if(cdest & 0x00000020)cdest |= 0x0000001f;
    }

    cdest &= 0x03e07c1f;
    return (cdest >> 16) | cdest;
  } else {
  //subtract
    if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
    else cdest -= (csrc & 0x7c00);
    if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
    else cdest -= (csrc & 0x03e0);
    if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
    else cdest -= (csrc & 0x001f);

    return (!regs.color_halve) ? cdest : ((cdest & 0x7bde) >> 1);
  }
}

inline uint16 bPPU::addsub_pixel(uint32 cdest) {
uint32 csrc = regs.color_rgb;
  if(!regs.color_mode) {
  //add
    cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
    csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
    cdest += csrc;

    if(regs.color_halve && !regs.addsub_mode) {
      cdest >>= 1;
    } else {
      if(cdest & 0x04000000)cdest |= 0x03e00000;
      if(cdest & 0x00008000)cdest |= 0x00007c00;
      if(cdest & 0x00000020)cdest |= 0x0000001f;
    }

    cdest &= 0x03e07c1f;
    return (cdest >> 16) | cdest;
  } else {
  //subtract
    if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
    else cdest -= (csrc & 0x7c00);
    if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
    else cdest -= (csrc & 0x03e0);
    if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
    else cdest -= (csrc & 0x001f);

    if(regs.color_halve && !regs.addsub_mode) {
      return (cdest & 0x7bde) >> 1;
    }

    return cdest;
  }
}
