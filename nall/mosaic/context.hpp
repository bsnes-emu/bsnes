#pragma once

namespace nall { namespace mosaic {

struct context {
  context() {
    reset();
  }

  auto objectWidth() const -> uint { return blockWidth * tileWidth * mosaicWidth + paddingWidth; }
  auto objectHeight() const -> uint { return blockHeight * tileHeight * mosaicHeight + paddingHeight; }
  auto objectSize() const -> uint {
    uint size = blockStride * tileWidth * tileHeight * mosaicWidth * mosaicHeight
              + blockOffset * tileHeight * mosaicWidth * mosaicHeight
              + tileStride * mosaicWidth * mosaicHeight
              + tileOffset * mosaicHeight;
    return max(1u, size);
  }

  auto reset() -> void {
    offset = 0;
    width = 0;
    height = 0;
    count = 0;

    endian = 1;
    order = 0;
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
    paddingColor = 0;
    palette.reset();
  }

  auto eval(const string& expression) -> uint {
    if(auto result = Eval::integer(expression)) return result();
    return 0u;
  }

  auto eval(vector<uint>& buffer, const string& expression_) -> void {
    string expression = expression_;
    bool function = false;
    for(auto& c : expression) {
      if(c == '(') function = true;
      if(c == ')') function = false;
      if(c == ',' && function == true) c = ';';
    }

    lstring list = expression.split(",");
    for(auto& item : list) {
      item.strip();
      if(item.match("f(?*) ?*")) {
        item.trimLeft("f(", 1L);
        lstring part = item.split(") ", 1L);
        lstring args = part[0].split(";", 3L).strip();

        uint length = eval(args(0, "0"));
        uint offset = eval(args(1, "0"));
        uint stride = eval(args(2, "0"));
        if(args.size() < 2) offset = buffer.size();
        if(args.size() < 3) stride = 1;

        for(uint n = 0; n < length; n++) {
          string fn = part[1];
          fn.replace("n", string{n});
          fn.replace("o", string{offset});
          fn.replace("p", string{buffer.size()});
          buffer.resize(offset + 1);
          buffer[offset] = eval(fn);
          offset += stride;
        }
      } else if(item.match("base64*")) {
        uint offset = 0;
        item.trimLeft("base64", 1L);
        if(item.match("(?*) *")) {
          item.trimLeft("(", 1L);
          lstring part = item.split(") ", 1L);
          offset = eval(part[0]);
          item = part(1, "");
        }
        item.strip();
        for(auto& c : item) {
          if(c >= 'A' && c <= 'Z') buffer.append(offset + c - 'A' +  0);
          if(c >= 'a' && c <= 'z') buffer.append(offset + c - 'a' + 26);
          if(c >= '0' && c <= '9') buffer.append(offset + c - '0' + 52);
          if(c == '-') buffer.append(offset + 62);
          if(c == '_') buffer.append(offset + 63);
        }
      } else if(item.match("file *")) {
        item.trimLeft("file ", 1L);
        item.strip();
        //...
      } else if(item) {
        buffer.append(eval(item));
      }
    }
  }

  auto parse(const string& data) -> void {
    reset();

    lstring lines = data.split("\n");
    for(auto& line : lines) {
      lstring part = line.split(":", 1L).strip();
      if(part.size() != 2) continue;

      if(part[0] == "offset") offset = eval(part[1]);
      if(part[0] == "width") width = eval(part[1]);
      if(part[0] == "height") height = eval(part[1]);
      if(part[0] == "count") count = eval(part[1]);

      if(part[0] == "endian") endian = eval(part[1]);
      if(part[0] == "order") order = eval(part[1]);
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

  auto load(const string& filename) -> bool {
    if(auto filedata = string::read(filename)) {
      parse(filedata);
      return true;
    }
    return false;
  }

  auto sanitize() -> void {
    if(depth < 1) depth = 1;
    if(depth > 24) depth = 24;

    if(blockWidth < 1) blockWidth = 1;
    if(blockHeight < 1) blockHeight = 1;

    if(tileWidth < 1) tileWidth = 1;
    if(tileHeight < 1) tileHeight = 1;

    if(mosaicWidth < 1) mosaicWidth = 1;
    if(mosaicHeight < 1) mosaicHeight = 1;

    //set alpha to full opacity
    paddingColor |= 255u << 24;
    for(auto& color : palette) color |= 255u << 24;
  }

  uint offset;
  uint width;
  uint height;
  uint count;

  bool endian;  //0 = lsb, 1 = msb
  bool order;   //0 = linear, 1 = planar
  uint depth;   //1 - 24bpp

  uint blockWidth;
  uint blockHeight;
  uint blockStride;
  uint blockOffset;
  vector<uint> block;

  uint tileWidth;
  uint tileHeight;
  uint tileStride;
  uint tileOffset;
  vector<uint> tile;

  uint mosaicWidth;
  uint mosaicHeight;
  uint mosaicStride;
  uint mosaicOffset;
  vector<uint> mosaic;

  uint paddingWidth;
  uint paddingHeight;
  uint paddingColor;
  vector<uint> palette;
};

}}
