#pragma once

namespace nall { namespace mosaic {

struct parser {
  //export from bitstream to canvas
  auto load(bitstream& stream, uint64 offset, context& ctx, uint width, uint height) -> void {
    canvas.allocate(width, height);
    canvas.fill(ctx.paddingColor);
    parse(1, stream, offset, ctx, width, height);
  }

  //import from canvas to bitstream
  auto save(bitstream& stream, uint64_t offset, context& ctx) -> bool {
    if(stream.readonly) return false;
    parse(0, stream, offset, ctx, canvas.width(), canvas.height());
    return true;
  }

private:
  auto read(uint x, uint y) const -> uint32 {
    uint addr = y * canvas.width() + x;
    if(addr >= canvas.width() * canvas.height()) return 0u;
    auto buffer = (uint32*)canvas.data();
    return buffer[addr];
  }

  auto write(uint x, uint y, uint32_t data) -> void {
    uint addr = y * canvas.width() + x;
    if(addr >= canvas.width() * canvas.height()) return;
    auto buffer = (uint32*)canvas.data();
    buffer[addr] = data;
  }

  auto parse(bool load, bitstream& stream, uint64 offset, context& ctx, uint width, uint height) -> void {
    stream.endian = ctx.endian;
    uint canvasWidth = width / (ctx.mosaicWidth * ctx.tileWidth * ctx.blockWidth + ctx.paddingWidth);
    uint canvasHeight = height / (ctx.mosaicHeight * ctx.tileHeight * ctx.blockHeight + ctx.paddingHeight);
    uint bitsPerBlock = ctx.depth * ctx.blockWidth * ctx.blockHeight;

    uint objectOffset = 0;
    for(uint objectY = 0; objectY < canvasHeight; objectY++) {
      for(uint objectX = 0; objectX < canvasWidth; objectX++) {
        if(objectOffset >= ctx.count && ctx.count > 0) break;
        uint objectIX = objectX * ctx.objectWidth();
        uint objectIY = objectY * ctx.objectHeight();
        objectOffset++;

        uint mosaicOffset = 0;
        for(uint mosaicY = 0; mosaicY < ctx.mosaicHeight; mosaicY++) {
          for(uint mosaicX = 0; mosaicX < ctx.mosaicWidth; mosaicX++) {
            uint mosaicData = ctx.mosaic(mosaicOffset, mosaicOffset);
            uint mosaicIX = (mosaicData % ctx.mosaicWidth) * (ctx.tileWidth * ctx.blockWidth);
            uint mosaicIY = (mosaicData / ctx.mosaicWidth) * (ctx.tileHeight * ctx.blockHeight);
            mosaicOffset++;

            uint tileOffset = 0;
            for(uint tileY = 0; tileY < ctx.tileHeight; tileY++) {
              for(uint tileX = 0; tileX < ctx.tileWidth; tileX++) {
                uint tileData = ctx.tile(tileOffset, tileOffset);
                uint tileIX = (tileData % ctx.tileWidth) * ctx.blockWidth;
                uint tileIY = (tileData / ctx.tileWidth) * ctx.blockHeight;
                tileOffset++;

                uint blockOffset = 0;
                for(uint blockY = 0; blockY < ctx.blockHeight; blockY++) {
                  for(uint blockX = 0; blockX < ctx.blockWidth; blockX++) {
                    if(load) {
                      uint palette = 0;
                      for(uint n = 0; n < ctx.depth; n++) {
                        uint index = blockOffset++;
                        if(ctx.order == 1) index = (index % ctx.depth) * ctx.blockWidth * ctx.blockHeight + (index / ctx.depth);
                        palette |= stream.read(offset + ctx.block(index, index)) << n;
                      }

                      write(
                        objectIX + mosaicIX + tileIX + blockX,
                        objectIY + mosaicIY + tileIY + blockY,
                        ctx.palette(palette, palette)
                      );
                    } else /* save */ {
                      uint32_t palette = read(
                        objectIX + mosaicIX + tileIX + blockX,
                        objectIY + mosaicIY + tileIY + blockY
                      );

                      for(uint n = 0; n < ctx.depth; n++) {
                        uint index = blockOffset++;
                        if(ctx.order == 1) index = (index % ctx.depth) * ctx.blockWidth * ctx.blockHeight + (index / ctx.depth);
                        stream.write(offset + ctx.block(index, index), palette & 1);
                        palette >>= 1;
                      }
                    }
                  }  //blockX
                }  //blockY

                offset += ctx.blockStride;
              }  //tileX

              offset += ctx.blockOffset;
            }  //tileY

            offset += ctx.tileStride;
          }  //mosaicX

          offset += ctx.tileOffset;
        }  //mosaicY

        offset += ctx.mosaicStride;
      }  //objectX

      offset += ctx.mosaicOffset;
    }  //objectY
  }

  image canvas;
};

}}
