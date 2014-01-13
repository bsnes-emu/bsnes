#ifndef NALL_IMAGE_UTILITY_HPP
#define NALL_IMAGE_UTILITY_HPP

namespace nall {

bool image::crop(unsigned outputX, unsigned outputY, unsigned outputWidth, unsigned outputHeight) {
  if(outputX + outputWidth > width) return false;
  if(outputY + outputHeight > height) return false;

  uint8_t* outputData = allocate(outputWidth, outputHeight, stride);
  unsigned outputPitch = outputWidth * stride;

  #pragma omp parallel for
  for(unsigned y = 0; y < outputHeight; y++) {
    const uint8_t* sp = data + pitch * (outputY + y) + stride * outputX;
    uint8_t* dp = outputData + outputPitch * y;
    for(unsigned x = 0; x < outputWidth; x++) {
      write(dp, read(sp));
      sp += stride;
      dp += stride;
    }
  }

  delete[] data;
  data = outputData;
  width = outputWidth;
  height = outputHeight;
  pitch = outputPitch;
  size = width * pitch;
  return true;
}

void image::alphaBlend(uint64_t alphaColor) {
  uint64_t alphaR = (alphaColor & red.mask  ) >> red.shift;
  uint64_t alphaG = (alphaColor & green.mask) >> green.shift;
  uint64_t alphaB = (alphaColor & blue.mask ) >> blue.shift;

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    uint8_t* dp = data + pitch * y;
    for(unsigned x = 0; x < width; x++) {
      uint64_t color = read(dp);

      uint64_t colorA = (color & alpha.mask) >> alpha.shift;
      uint64_t colorR = (color & red.mask  ) >> red.shift;
      uint64_t colorG = (color & green.mask) >> green.shift;
      uint64_t colorB = (color & blue.mask ) >> blue.shift;
      double alphaScale = (double)colorA / (double)((1 << alpha.depth) - 1);

      colorA = (1 << alpha.depth) - 1;
      colorR = (colorR * alphaScale) + (alphaR * (1.0 - alphaScale));
      colorG = (colorG * alphaScale) + (alphaG * (1.0 - alphaScale));
      colorB = (colorB * alphaScale) + (alphaB * (1.0 - alphaScale));

      write(dp, (colorA << alpha.shift) | (colorR << red.shift) | (colorG << green.shift) | (colorB << blue.shift));
      dp += stride;
    }
  }
}

void image::transform(bool outputEndian, unsigned outputDepth, uint64_t outputAlphaMask, uint64_t outputRedMask, uint64_t outputGreenMask, uint64_t outputBlueMask) {
  if(endian == outputEndian && depth == outputDepth && alpha.mask == outputAlphaMask && red.mask == outputRedMask && green.mask == outputGreenMask && blue.mask == outputBlueMask) return;

  image output(outputEndian, outputDepth, outputAlphaMask, outputRedMask, outputGreenMask, outputBlueMask);
  output.allocate(width, height);

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    const uint8_t* sp = data + pitch * y;
    uint8_t* dp = output.data + output.pitch * y;
    for(unsigned x = 0; x < width; x++) {
      uint64_t color = read(sp);
      sp += stride;

      uint64_t a = (color & alpha.mask) >> alpha.shift;
      uint64_t r = (color & red.mask) >> red.shift;
      uint64_t g = (color & green.mask) >> green.shift;
      uint64_t b = (color & blue.mask) >> blue.shift;

      a = normalize(a, alpha.depth, output.alpha.depth);
      r = normalize(r, red.depth, output.red.depth);
      g = normalize(g, green.depth, output.green.depth);
      b = normalize(b, blue.depth, output.blue.depth);

      output.write(dp, (a << output.alpha.shift) | (r << output.red.shift) | (g << output.green.shift) | (b << output.blue.shift));
      dp += output.stride;
    }
  }

  operator=(std::move(output));
}

}

#endif
