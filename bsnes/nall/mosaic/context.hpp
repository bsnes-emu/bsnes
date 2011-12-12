#ifdef NALL_MOSAIC_INTERNAL_HPP

namespace nall {
namespace mosaic {

struct context {
  unsigned offset;
  unsigned width;
  unsigned height;
  unsigned count;

  bool endian;
  unsigned depth;

  unsigned blockWidth;
  unsigned blockHeight;
  unsigned blockStride;
  unsigned blockOffset;
  array<unsigned> block;

  unsigned tileWidth;
  unsigned tileHeight;
  unsigned tileStride;
  unsigned tileOffset;
  array<unsigned> tile;

  unsigned mosaicWidth;
  unsigned mosaicHeight;
  unsigned mosaicStride;
  unsigned mosaicOffset;
  array<unsigned> mosaic;

  unsigned paddingWidth;
  unsigned paddingHeight;
  unsigned paddingColor;
  array<unsigned> palette;

  inline unsigned objectWidth() const { return blockWidth * tileWidth * mosaicWidth + paddingWidth; }
  inline unsigned objectHeight() const { return blockHeight * tileHeight * mosaicHeight + paddingHeight; }
  inline unsigned objectSize() const {
    unsigned size = blockStride * tileWidth * tileHeight * mosaicWidth * mosaicHeight
                  + blockOffset * tileHeight * mosaicWidth * mosaicHeight
                  + tileStride * mosaicWidth * mosaicHeight
                  + tileOffset * mosaicHeight;
    return max(1u, size);
  }

  inline unsigned eval(const string &expression) {
    intmax_t result;
    if(fixedpoint::eval(expression, result) == false) return 0u;
    return result;
  }

  inline void eval(array<unsigned> &buffer, const string &expression) {
    lstring list = expression.split(",");
    for(auto &item : list) {
      item.trim();
      if(item.wildcard("f(?*) *")) {
        lstring part = item.split<1>(" ");
        part[0].trim("f(", ")");
        unsigned length = decimal(part[0].trim());
        for(unsigned n = 0; n < length; n++) {
          string fn = part[1];
          fn.replace("n", decimal(n));
          fn.replace("p", decimal(buffer.size()));
          buffer.append(eval(fn));
        }
      } else if(item != "") {
        buffer.append(eval(item));
      }
    }
  }

  inline void parse(const string &data) {
    reset();

    lstring lines = data.split("\n");
    for(auto &line : lines) {
      lstring part = line.split<1>(":");
      if(part.size() != 2) continue;
      part[0].trim();
      part[1].trim();

      if(part[0] == "offset") offset = eval(part[1]);
      if(part[0] == "width") width = eval(part[1]);
      if(part[0] == "height") height = eval(part[1]);
      if(part[0] == "count") count = eval(part[1]);

      if(part[0] == "endian") endian = eval(part[1]);
      if(part[0] == "depth") depth = eval(part[1]);

      if(part[0] == "blockWidth") blockWidth = eval(part[1]);
      if(part[0] == "blockHeight") blockHeight = eval(part[1]);
      if(part[0] == "blockStride") blockStride = eval(part[1]);
      if(part[0] == "blockOffset") blockOffset = eval(part[1]);
      if(part[0] == "block") eval(block, part[1]);

      if(part[0] == "tileWidth") tileWidth = eval(part[1]);
      if(part[0] == "tileHeight") tileHeight = eval(part[1]);
      if(part[0] == "tileStride") tileStride = eval(part[1]);
      if(part[0] == "tileOffset") tileOffset = eval(part[1]);
      if(part[0] == "tile") eval(tile, part[1]);

      if(part[0] == "mosaicWidth") mosaicWidth = eval(part[1]);
      if(part[0] == "mosaicHeight") mosaicHeight = eval(part[1]);
      if(part[0] == "mosaicStride") mosaicStride = eval(part[1]);
      if(part[0] == "mosaicOffset") mosaicOffset = eval(part[1]);
      if(part[0] == "mosaic") eval(mosaic, part[1]);

      if(part[0] == "paddingWidth") paddingWidth = eval(part[1]);
      if(part[0] == "paddingHeight") paddingHeight = eval(part[1]);
      if(part[0] == "paddingColor") paddingColor = eval(part[1]);
      if(part[0] == "palette") eval(palette, part[1]);
    }

    sanitize();
  }

  inline bool load(const string &filename) {
    string filedata;
    if(filedata.readfile(filename) == false) return false;
    parse(filedata);
    return true;
  }

  inline void sanitize() {
    if(depth < 1) depth = 1;
    if(depth > 24) depth = 24;

    if(blockWidth < 1) blockWidth = 1;
    if(blockHeight < 1) blockHeight = 1;

    if(tileWidth < 1) tileWidth = 1;
    if(tileHeight < 1) tileHeight = 1;

    if(mosaicWidth < 1) mosaicWidth = 1;
    if(mosaicHeight < 1) mosaicHeight = 1;
  }

  inline void reset() {
    offset = 0;
    width = 0;
    height = 0;
    count = 0;

    endian = 1;
    depth = 1;

    blockWidth = 1;
    blockHeight = 1;
    blockStride = 0;
    blockOffset = 0;
    block.reset();

    tileWidth = 1;
    tileHeight = 1;
    tileStride = 0;
    tileOffset = 0;
    tile.reset();

    mosaicWidth = 1;
    mosaicHeight = 1;
    mosaicStride = 0;
    mosaicOffset = 0;
    mosaic.reset();

    paddingWidth = 0;
    paddingHeight = 0;
    paddingColor = 0x000000;
    palette.reset();
  }

  inline context() {
    reset();
  }
};

}
}

#endif
