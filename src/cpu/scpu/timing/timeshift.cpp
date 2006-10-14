alwaysinline void sCPU::timeshift_forward(uint clocks, uint &vtime, uint &htime) {
  htime += clocks;

  if(htime >= status.line_clocks) {
    htime -= status.line_clocks;
    if(++vtime >= status.field_lines) {
      vtime = 0;
    }
  }
}

alwaysinline void sCPU::timeshift_backward(uint clocks, uint &vtime, uint &htime) {
  if(htime >= clocks) {
    htime -= clocks;
  } else {
    htime += 1364 - clocks;
    if(status.region == SNES::NTSC && status.vcounter == 241 &&
       status.interlace == false && status.interlace_field == 1)htime -= 4;
    if(vtime > 0) {
      vtime--;
    } else {
      vtime = status.region_scanlines;
      if(status.interlace == true && status.interlace_field == 1)vtime++;
    }
  }
}
