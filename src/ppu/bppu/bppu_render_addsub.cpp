inline uint16 bPPU::addsub_pixels(uint32 x) {
uint32 cdest = pixel_cache[x].src_main,
       csrc  = pixel_cache[x].src_sub;
bool   halve = false;
  if(regs.color_halve && window_cache[COL].main[x]) {
    if(regs.addsub_mode && pixel_cache[x].bg_sub == BACK);
    else {
      halve = true;
    }
  }

  if(!regs.color_mode) {
  //add
    cdest = ((cdest << 16) | cdest) & 0x03e07c1f;
    csrc  = ((csrc  << 16) | csrc)  & 0x03e07c1f;
    cdest += csrc;

    if(!halve) {
      if(cdest & 0x04000000)cdest |= 0x03e00000;
      if(cdest & 0x00008000)cdest |= 0x00007c00;
      if(cdest & 0x00000020)cdest |= 0x0000001f;
    } else {
      cdest >>= 1;
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

    return (!halve) ? cdest : ((cdest & 0x7bde) >> 1);
  }
}
