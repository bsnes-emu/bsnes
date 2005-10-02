inline uint16 bPPU::addsub_pixels(uint32 cdest, uint32 csrc) {
int r, g, b;
uint16 res;
  switch(regs.color_mode) {
  case 0: //COLORMODE_ADD:
    if(regs.color_halve == true) {
      cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
      csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
      cdest += csrc;
      cdest >>= 1;
      cdest &= 0x03e07c1f;
      return (cdest >> 16) | cdest;
    } else {
      cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
      csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
      cdest += csrc;
      if(cdest & 0x04000000)cdest |= 0x03e00000;
      if(cdest & 0x00008000)cdest |= 0x00007c00;
      if(cdest & 0x00000020)cdest |= 0x0000001f;
      cdest &= 0x03e07c1f;
      return (cdest >> 16) | cdest;
    }
    break;
  case 1: //COLORMODE_SUB:
    if(regs.color_halve == true) {
      if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
      else cdest -= (csrc & 0x7c00);
      if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
      else cdest -= (csrc & 0x03e0);
      if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
      else cdest -= (csrc & 0x001f);
      return (cdest & 0x7bde) >> 1;
    } else {
      if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
      else cdest -= (csrc & 0x7c00);
      if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
      else cdest -= (csrc & 0x03e0);
      if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
      else cdest -= (csrc & 0x001f);
      return cdest;
    }
    break;
  }
  return 0x0000; //prevent annoying warning message
}

inline uint16 bPPU::addsub_pixel(uint32 cdest) {
int r, g, b;
uint32 csrc  = (regs.color_r) | (regs.color_g << 5) | (regs.color_b << 10);
uint16 res;
  switch(regs.color_mode) {
  case 0: //COLORMODE_ADD:
    if(regs.color_halve == true && regs.addsub_mode == 0) {
      cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
      csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
      cdest += csrc;
      cdest >>= 1;
      cdest &= 0x03e07c1f;
      return (cdest >> 16) | cdest;
    } else {
      cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
      csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
      cdest += csrc;
      if(cdest & 0x04000000)cdest |= 0x03e00000;
      if(cdest & 0x00008000)cdest |= 0x00007c00;
      if(cdest & 0x00000020)cdest |= 0x0000001f;
      cdest &= 0x03e07c1f;
      return (cdest >> 16) | cdest;
    }
    break;
  case 1: //COLORMODE_SUB:
    if(regs.color_halve == true && regs.addsub_mode == 0) {
      if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
      else cdest -= (csrc & 0x7c00);
      if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
      else cdest -= (csrc & 0x03e0);
      if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
      else cdest -= (csrc & 0x001f);
      return (cdest & 0x7bde) >> 1;
    } else {
      if((cdest & 0x7c00) < (csrc & 0x7c00))cdest &= ~0x7c00;
      else cdest -= (csrc & 0x7c00);
      if((cdest & 0x03e0) < (csrc & 0x03e0))cdest &= ~0x03e0;
      else cdest -= (csrc & 0x03e0);
      if((cdest & 0x001f) < (csrc & 0x001f))cdest &= ~0x001f;
      else cdest -= (csrc & 0x001f);
      return cdest;
    }
    break;
  }
  return 0x0000; //prevent annoying warning message
}
