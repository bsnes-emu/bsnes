//2xSaI / Super 2xSaI / Super Eagle filter
//authors: kode54 and Kreed
//license: GPL

#include "2xsai.hpp"
#include "implementation.cpp"

//=====
//2xSaI
//=====

void _2xSaIFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height;

  if(width <= 256 && height <= 240) {
    outwidth  *= 2;
    outheight *= 2;
  }
}

void _2xSaIFilter::render(
  uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, input, pitch, width, height);
    return;
  }

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *line_in = (const uint16_t *) (((const uint8_t*)input) + pitch * y);
    uint32_t *line_out = temp + y * 256;
    for(unsigned x = 0; x < width; x++) {
      line_out[x] = colortable[line_in[x]];
    }
  }

  _2xSaI32( (unsigned char *) temp, 1024, 0, (unsigned char *) output, outpitch, width, height );
}

_2xSaIFilter::_2xSaIFilter() {
  temp = new uint32_t[256*240];
}

_2xSaIFilter::~_2xSaIFilter() {
  delete[] temp;
}

//===========
//Super 2xSaI
//===========

void Super2xSaIFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height;

  if(width <= 256 && height <= 240) {
    outwidth  *= 2;
    outheight *= 2;
  }
}

void Super2xSaIFilter::render(
  uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, input, pitch, width, height);
    return;
  }

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *line_in = (const uint16_t *) (((const uint8_t*)input) + pitch * y);
    uint32_t *line_out = temp + y * 256;
    for(unsigned x = 0; x < width; x++) {
      line_out[x] = colortable[line_in[x]];
    }
  }

  Super2xSaI32( (unsigned char *) temp, 1024, 0, (unsigned char *) output, outpitch, width, height );
}

Super2xSaIFilter::Super2xSaIFilter() {
  temp = new uint32_t[256*240];
}

Super2xSaIFilter::~Super2xSaIFilter() {
  delete[] temp;
}

//===========
//Super Eagle
//===========

void SuperEagleFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height;

  if(width <= 256 && height <= 240) {
    outwidth  *= 2;
    outheight *= 2;
  }
}

void SuperEagleFilter::render(
  uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, input, pitch, width, height);
    return;
  }

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *line_in = (const uint16_t *) (((const uint8_t*)input) + pitch * y);
    uint32_t *line_out = temp + y * 256;
    for(unsigned x = 0; x < width; x++) {
      line_out[x] = colortable[line_in[x]];
    }
  }

  SuperEagle32( (unsigned char *) temp, 1024, 0, (unsigned char *) output, outpitch, width, height );
}

SuperEagleFilter::SuperEagleFilter() {
  temp = new uint32_t[256*240];
}

SuperEagleFilter::~SuperEagleFilter() {
  delete[] temp;
}
