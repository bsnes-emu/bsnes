FilterInterface filter;

void FilterInterface::set(FilterInterface::FilterType type) {
  active_filter = type;
}

void FilterInterface::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  switch(active_filter) { default:
    case Direct: return filter_direct.size(outwidth, outheight, width, height);
    case Scanline: return filter_scanline.size(outwidth, outheight, width, height);
    case Scale2x: return filter_scale2x.size(outwidth, outheight, width, height);
    case LQ2x: return filter_lq2x.size(outwidth, outheight, width, height);
    case HQ2x: return filter_hq2x.size(outwidth, outheight, width, height);
    case NTSC: return filter_ntsc.size(outwidth, outheight, width, height);
  }
}

void FilterInterface::render(
  uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
  unsigned *line, unsigned width, unsigned height
) {
  switch(active_filter) { default:
    case Direct: return filter_direct.render(output, outpitch, input, pitch, line, width, height);
    case Scanline: return filter_scanline.render(output, outpitch, input, pitch, line, width, height);
    case Scale2x: return filter_scale2x.render(output, outpitch, input, pitch, line, width, height);
    case LQ2x: return filter_lq2x.render(output, outpitch, input, pitch, line, width, height);
    case HQ2x: return filter_hq2x.render(output, outpitch, input, pitch, line, width, height);
    case NTSC: return filter_ntsc.render(output, outpitch, input, pitch, line, width, height);
  }
}

FilterInterface::FilterInterface() : active_filter(FilterInterface::Direct) {}
