struct {
  uint16 v, hc;

  bool   interlace, interlace_field, overscan;
  uint16 line_cycles, frame_lines;

  bool   dram_refreshed;
  uint16 dram_refresh_pos;

  uint8  dma_counter;

  uint16 region_scanlines;
}time;

inline uint16 vcounter();
inline uint16 hcounter();
inline uint16 hcycles();
inline bool   interlace();
inline bool   interlace_field();
inline bool   overscan();
inline uint16 region_scanlines();

inline void   set_interlace(bool r);
inline void   set_overscan (bool r);

inline uint8  dma_counter();

inline void   inc_vcounter();
inline uint16 get_hcounter();
inline void   dram_refresh();
inline void   add_cycles(int cycles);
inline void   time_reset();
