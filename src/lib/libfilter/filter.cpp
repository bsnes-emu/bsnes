FilterInterface filter;

void FilterInterface::set(FilterInterface::FilterType type) {
  active_filter = type;
}

void FilterInterface::render(
  uint32_t *output, unsigned outpitch, unsigned &outwidth, unsigned &outheight,
  uint16_t *input, unsigned pitch, unsigned *line, unsigned width, unsigned height
) {
  switch(active_filter) { default:
    case Direct: {
      filter_direct.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    } break;

    case Scanline: {
      filter_scanline.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    } break;

    case Scale2x: {
      filter_scale2x.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    } break;

    case HQ2x: {
      filter_hq2x.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    } break;

    case NTSC: {
      filter_ntsc.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    } break;
  }
}

FilterInterface::FilterInterface() : active_filter(FilterInterface::Direct) {}
