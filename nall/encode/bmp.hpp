#pragma once

namespace nall { namespace Encode {

struct BMP {
  static auto create(const string& filename, const uint32_t* data, unsigned width, unsigned height, bool alpha) -> bool {
    file fp{filename, file::mode::write};
    if(!fp) return false;

    unsigned bitsPerPixel  = alpha ? 32 : 24;
    unsigned bytesPerPixel = bitsPerPixel / 8;
    unsigned alignedWidth  = width * bytesPerPixel;
    unsigned paddingLength = 0;
    unsigned imageSize     = alignedWidth * height;
    unsigned fileSize      = 0x36 + imageSize;
    while(alignedWidth % 4) alignedWidth++, paddingLength++;

    fp.writel(0x4d42, 2);        //signature
    fp.writel(fileSize, 4);      //file size
    fp.writel(0, 2);             //reserved
    fp.writel(0, 2);             //reserved
    fp.writel(0x36, 4);          //offset

    fp.writel(40, 4);            //DIB size
    fp.writel(width, 4);         //width
    fp.writel(-height, 4);       //height
    fp.writel(1, 2);             //color planes
    fp.writel(bitsPerPixel, 2);  //bits per pixel
    fp.writel(0, 4);             //compression method (BI_RGB)
    fp.writel(imageSize, 4);     //image data size
    fp.writel(3780, 4);          //horizontal resolution
    fp.writel(3780, 4);          //vertical resolution
    fp.writel(0, 4);             //palette size
    fp.writel(0, 4);             //important color count

    for(auto y : range(height)) {
      const uint32_t* p = data + y * width;
      for(auto x : range(width)) fp.writel(*p++, bytesPerPixel);
      if(paddingLength) fp.writel(0, paddingLength);
    }

    return true;
  }
};

}}
