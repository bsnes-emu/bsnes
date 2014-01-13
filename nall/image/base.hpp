#ifndef NALL_IMAGE_BASE_HPP
#define NALL_IMAGE_BASE_HPP

namespace nall {

struct image {
  uint8_t* data   = nullptr;
  unsigned width  = 0;
  unsigned height = 0;
  unsigned pitch  = 0;
  unsigned size   = 0;

  bool endian     =  0;  //0 = lsb, 1 = msb
  unsigned depth  = 32;
  unsigned stride =  4;

  struct channel {
    uint64_t mask;
    unsigned depth;
    unsigned shift;

    inline bool operator==(const channel& source) {
      return mask == source.mask && depth == source.depth && shift == source.shift;
    }

    inline bool operator!=(const channel& source) {
      return !operator==(source);
    }
  };

  channel alpha = {255u << 24, 8u, 24u};
  channel red   = {255u << 16, 8u, 16u};
  channel green = {255u <<  8, 8u,  8u};
  channel blue  = {255u <<  0, 8u,  0u};

  enum class blend : unsigned {
    add,
    sourceAlpha,  //color = sourceColor * sourceAlpha + targetColor * (1 - sourceAlpha)
    sourceColor,  //color = sourceColor
    targetAlpha,  //color = targetColor * targetAlpha + sourceColor * (1 - targetAlpha)
    targetColor,  //color = targetColor
  };

  //static.hpp
  static inline unsigned bitDepth(uint64_t color);
  static inline unsigned bitShift(uint64_t color);
  static inline uint64_t normalize(uint64_t color, unsigned sourceDepth, unsigned targetDepth);

  //core.hpp
  inline bool operator==(const image& source);
  inline bool operator!=(const image& source);

  inline image& operator=(const image& source);
  inline image& operator=(image&& source);
  inline image(const image& source);
  inline image(image&& source);
  inline image(bool endian, unsigned depth, uint64_t alphaMask, uint64_t redMask, uint64_t greenMask, uint64_t blueMask);
  inline image(const string& filename);
  inline image(const uint8_t* data, unsigned size);
  inline image();
  inline ~image();

  inline uint64_t read(const uint8_t* data) const;
  inline void write(uint8_t* data, uint64_t value) const;

  inline void free();
  inline bool empty() const;
  inline bool load(const string& filename);
  inline void allocate(unsigned width, unsigned height);

  //fill.hpp
  inline void fill(uint64_t color = 0);
  inline void linearGradient(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
  inline void horizontalGradient(uint64_t a, uint64_t b);
  inline void verticalGradient(uint64_t a, uint64_t b);
  inline void radialGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY, signed centerX, signed centerY);
  inline void radialGradient(uint64_t a, uint64_t b, signed radiusX, signed radiusY);
  inline void radialGradient(uint64_t a, uint64_t b);

  //scale.hpp
  inline void scale(unsigned width, unsigned height, bool linear = true);

  //blend.hpp
  inline void impose(blend mode, unsigned targetX, unsigned targetY, image source, unsigned x, unsigned y, unsigned width, unsigned height);

  //utility.hpp
  inline bool crop(unsigned x, unsigned y, unsigned width, unsigned height);
  inline void alphaBlend(uint64_t alphaColor);
  inline void transform(bool endian, unsigned depth, uint64_t alphaMask, uint64_t redMask, uint64_t greenMask, uint64_t blueMask);

protected:
  //core.hpp
  inline uint8_t* allocate(unsigned width, unsigned height, unsigned stride);

  //scale.hpp
  inline void scaleLinearWidth(unsigned width);
  inline void scaleLinearHeight(unsigned height);
  inline void scaleLinear(unsigned width, unsigned height);
  inline void scaleNearest(unsigned width, unsigned height);

  //load.hpp
  inline bool loadBMP(const string& filename);
  inline bool loadPNG(const string& filename);
  inline bool loadPNG(const uint8_t* data, unsigned size);

  //interpolation.hpp
  alwaysinline void isplit(uint64_t* component, uint64_t color);
  alwaysinline uint64_t imerge(const uint64_t* component);
  alwaysinline uint64_t interpolate1f(uint64_t a, uint64_t b, double x);
  alwaysinline uint64_t interpolate1f(uint64_t a, uint64_t b, uint64_t c, uint64_t d, double x, double y);
  alwaysinline uint64_t interpolate1i(int64_t a, int64_t b, uint32_t x);
  alwaysinline uint64_t interpolate1i(int64_t a, int64_t b, int64_t c, int64_t d, uint32_t x, uint32_t y);
  inline uint64_t interpolate4f(uint64_t a, uint64_t b, double x);
  inline uint64_t interpolate4f(uint64_t a, uint64_t b, uint64_t c, uint64_t d, double x, double y);
  inline uint64_t interpolate4i(uint64_t a, uint64_t b, uint32_t x);
  inline uint64_t interpolate4i(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint32_t x, uint32_t y);
};

}

#endif
