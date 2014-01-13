#ifndef NALL_IMAGE_BLEND_HPP
#define NALL_IMAGE_BLEND_HPP

namespace nall {

void image::impose(blend mode, unsigned targetX, unsigned targetY, image source, unsigned sourceX, unsigned sourceY, unsigned sourceWidth, unsigned sourceHeight) {
  source.transform(endian, depth, alpha.mask, red.mask, green.mask, blue.mask);

  for(unsigned y = 0; y < sourceHeight; y++) {
    const uint8_t* sp = source.data + source.pitch * (sourceY + y) + source.stride * sourceX;
    uint8_t* dp = data + pitch * (targetY + y) + stride * targetX;
    for(unsigned x = 0; x < sourceWidth; x++) {
      uint64_t sourceColor = source.read(sp);
      uint64_t targetColor = read(dp);

      int64_t sa = (sourceColor & alpha.mask) >> alpha.shift;
      int64_t sr = (sourceColor & red.mask  ) >> red.shift;
      int64_t sg = (sourceColor & green.mask) >> green.shift;
      int64_t sb = (sourceColor & blue.mask ) >> blue.shift;

      int64_t da = (targetColor & alpha.mask) >> alpha.shift;
      int64_t dr = (targetColor & red.mask  ) >> red.shift;
      int64_t dg = (targetColor & green.mask) >> green.shift;
      int64_t db = (targetColor & blue.mask ) >> blue.shift;

      uint64_t a, r, g, b;

      switch(mode) {
      case blend::add:
        a = max(sa, da);
        r = min(red.mask   >> red.shift,   ((sr * sa) >> alpha.depth) + ((dr * da) >> alpha.depth));
        g = min(green.mask >> green.shift, ((sg * sa) >> alpha.depth) + ((dg * da) >> alpha.depth));
        b = min(blue.mask  >> blue.shift,  ((sb * sa) >> alpha.depth) + ((db * da) >> alpha.depth));
        break;

      case blend::sourceAlpha:
        a = max(sa, da);
        r = dr + (((sr - dr) * sa) >> alpha.depth);
        g = dg + (((sg - dg) * sa) >> alpha.depth);
        b = db + (((sb - db) * sa) >> alpha.depth);
        break;

      case blend::sourceColor:
        a = sa;
        r = sr;
        g = sg;
        b = sb;
        break;

      case blend::targetAlpha:
        a = max(sa, da);
        r = sr + (((dr - sr) * da) >> alpha.depth);
        g = sg + (((dg - sg) * da) >> alpha.depth);
        b = sb + (((db - sb) * da) >> alpha.depth);
        break;

      case blend::targetColor:
        a = da;
        r = dr;
        g = dg;
        b = db;
        break;
      }

      write(dp, (a << alpha.shift) | (r << red.shift) | (g << green.shift) | (b << blue.shift));
      sp += source.stride;
      dp += stride;
    }
  }
}

}

#endif
