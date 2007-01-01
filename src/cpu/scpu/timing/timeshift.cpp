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
    htime += status.prev_line_clocks - clocks;
    if(vtime > 0) {
      vtime--;
    } else {
      vtime = status.prev_field_lines - 1;
    }
  }
}
