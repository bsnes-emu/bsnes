#if USE_AVX2 == 1
__attribute__((__target__("avx2")))
auto PPU::Line::renderMode7HD_AVX2(
  PPU::IO::Background& self, uint8 source,
  Pixel* above, Pixel* below,
  bool* windowAbove, bool* windowBelow,
  float originX, float a,
  float originY, float c
) -> void {
  const bool extbg = source == Source::BG2;
  const uint scale = ppu.hdScale();

  //8 pixels at the time, one int32 per pixel
  for (int x : range(256 * scale / 8)) {
    //ensure everything has the expected types.
    //some accesses are outside the buffers; the unwanted bytes are masked out,
    //but it's still undefined behavior, and it will crash if those bytes are unmapped or if compiler becomes too clever.
    static_assert(std::is_same_v<decltype(ppu.vram), uint16_t[32768]>);
    static_assert(std::is_same_v<decltype(cgram), uint16_t[256]>);
    static_assert(std::is_same_v<decltype(self.priority), uint8_t[2]>);
    static_assert(sizeof(Pixel) == 4);
    static_assert(offsetof(Pixel, source) == 0);
    static_assert(std::is_same_v<decltype(Pixel::source), uint8_t>);
    static_assert(offsetof(Pixel, priority) == 1);
    static_assert(std::is_same_v<decltype(Pixel::priority), uint8_t>);
    static_assert(offsetof(Pixel, color) == 2);
    static_assert(std::is_same_v<decltype(Pixel::color), uint16_t>);

    __m256 offset_f32 = _mm256_setr_ps(0, 1, 2, 3, 4, 5, 6, 7);
    __m256 posi_f32 = _mm256_add_ps(_mm256_set1_ps(x * 8), offset_f32);
    __m256 pos_f32 = _mm256_div_ps(posi_f32, _mm256_set1_ps(scale));

    __m256i doAbove = _mm256_setzero_si256(); //0x00000000 - ignore, 0xFFFFFFFF - render
    if(self.aboveEnable) {
      doAbove = _mm256_i32gather_epi32((int*)windowAbove, _mm256_cvtps_epi32(pos_f32), 1);
      doAbove = _mm256_and_si256(doAbove, _mm256_set1_epi32(0x00000001));
      doAbove = _mm256_sub_epi32(doAbove, _mm256_set1_epi32(0x00000001));
    }
    __m256i doBelow = _mm256_setzero_si256();
    if(self.belowEnable) {
      doBelow = _mm256_i32gather_epi32((int*)windowBelow, _mm256_cvtps_epi32(pos_f32), 1);
      doBelow = _mm256_and_si256(doBelow, _mm256_set1_epi32(0x00000001));
      doBelow = _mm256_sub_epi32(doBelow, _mm256_set1_epi32(0x00000001));
    }

    __m256 xf = _mm256_add_ps(pos_f32, _mm256_set1_ps(-0.5));
    if(io.mode7.hflip) xf = _mm256_sub_ps(_mm256_set1_ps(255), xf);

    __m256i pixelXx256 = _mm256_cvtps_epi32(_mm256_add_ps(_mm256_set1_ps(originX), _mm256_mul_ps(_mm256_set1_ps(a), xf)));
    __m256i pixelYx256 = _mm256_cvtps_epi32(_mm256_add_ps(_mm256_set1_ps(originY), _mm256_mul_ps(_mm256_set1_ps(c), xf)));

    __m256i tile_addr = _mm256_or_si256(_mm256_and_si256(_mm256_srai_epi32(pixelYx256,  4), _mm256_set1_epi32(127 << 7)),
                                        _mm256_and_si256(_mm256_srai_epi32(pixelXx256, 11), _mm256_set1_epi32(127)));
    __m256i tile = _mm256_and_si256(_mm256_i32gather_epi32((int*)ppu.vram, tile_addr, sizeof(uint16_t)), _mm256_set1_epi32(0x000000FF));
    __m256i outOfBounds = _mm256_cmpeq_epi32(_mm256_and_si256(_mm256_or_si256(pixelXx256, pixelYx256), _mm256_set1_epi32((~1023) * 256)), _mm256_setzero_si256());
    if(io.mode7.repeat == 3)
      tile = _mm256_blendv_epi8(_mm256_setzero_si256(), tile, outOfBounds);
    __m256i palette_addr = _mm256_or_si256(_mm256_or_si256(
                                           _mm256_and_si256(_mm256_srli_epi32(pixelYx256, 5), _mm256_set1_epi32(7 << 3)),
                                           _mm256_and_si256(_mm256_srli_epi32(pixelXx256, 8), _mm256_set1_epi32(7))),
                                           _mm256_and_si256(_mm256_slli_epi32(tile, 6),       _mm256_set1_epi32(0xFF << 6)));
    __m256i palette = _mm256_and_si256(_mm256_srai_epi32(_mm256_i32gather_epi32((int*)ppu.vram, palette_addr, sizeof(uint16_t)), 8), _mm256_set1_epi32(0x000000FF));
    if(io.mode7.repeat == 2)
      palette = _mm256_blendv_epi8(_mm256_setzero_si256(), palette, outOfBounds);

    __m256i priority;
    if(!extbg) {
      priority = _mm256_set1_epi32(self.priority[0]);
    } else {
      priority = _mm256_blendv_epi8(_mm256_set1_epi32(self.priority[0]), _mm256_set1_epi32(self.priority[1]),
                                    _mm256_shuffle_epi8(palette, _mm256_set_epi32(0x00000000, 0x04040404, 0x08080808, 0x0C0C0C0C,
                                                                                  0x00000000, 0x04040404, 0x08080808, 0x0C0C0C0C)));
      palette = _mm256_and_si256(palette, _mm256_set1_epi32(0x0000007F));
    }

    __m256i color;
    if(io.col.directColor && !extbg) {
      color = _mm256_or_si256(_mm256_or_si256(
                              _mm256_and_si256(_mm256_slli_epi32(palette, 2 + 16), _mm256_set1_epi32(0x001C0000)),
                              _mm256_and_si256(_mm256_slli_epi32(palette, 4 + 16), _mm256_set1_epi32(0x03800000))),
                              _mm256_and_si256(_mm256_slli_epi32(palette, 7 + 16), _mm256_set1_epi32(0x60000000)));
    } else {
      //cgram-1 gives me a free <<16
      color = _mm256_and_si256(_mm256_i32gather_epi32((int*)(cgram - 1), palette, sizeof(uint16_t)), _mm256_set1_epi32(0xFFFF0000));
    }

    __m256i pixels = _mm256_or_si256(_mm256_or_si256(color, _mm256_set1_epi32(source)), _mm256_slli_epi32(priority, 8));
    __m256i prevAbove = _mm256_loadu_si256((__m256i*)above);
    __m256i prevBelow = _mm256_loadu_si256((__m256i*)below);

    if(extbg) {
      doAbove = _mm256_and_si256(doAbove, _mm256_shuffle_epi8(_mm256_cmpgt_epi8(pixels, prevAbove),
                                                              _mm256_set_epi32(0x01010101, 0x05050505, 0x09090909, 0x0D0D0D0D,
                                                                               0x01010101, 0x05050505, 0x09090909, 0x0D0D0D0D)));
      doBelow = _mm256_and_si256(doBelow, _mm256_shuffle_epi8(_mm256_cmpgt_epi8(pixels, prevBelow),
                                                              _mm256_set_epi32(0x01010101, 0x05050505, 0x09090909, 0x0D0D0D0D,
                                                                               0x01010101, 0x05050505, 0x09090909, 0x0D0D0D0D)));
    }

    _mm256_storeu_si256((__m256i*)above, _mm256_blendv_epi8(prevAbove, pixels, doAbove));
    _mm256_storeu_si256((__m256i*)below, _mm256_blendv_epi8(prevBelow, pixels, doBelow));

    above += 8;
    below += 8;
  }
}
#endif
