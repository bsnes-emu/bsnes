auto VDP::Sprite::scanline() -> void {
  state.x = 0;
  state.y = vdp.io.vcounter;
  objects.reset();

  bool large = vdp.io.extendedHeight;
  uint14 attributeAddress = vdp.io.spriteAttributeTableAddress << 8;
  for(uint index : range(64)) {
    uint8 y = vdp.vram[attributeAddress + index];
    uint8 x = vdp.vram[attributeAddress + 0x80 + (index << 1)];
    uint8 pattern = vdp.vram[attributeAddress + 0x81 + (index << 1)];
    if(vdp.vlines() == 192 && y == 0xd0) break;

    if(vdp.io.spriteShift) x -= 8;
    y += 1;
    if(state.y < y) continue;
    if(state.y > y + (large ? 15 : 7)) continue;

    if(large) pattern.bit(0) = 0;

    objects.append({x, y, pattern});
    if(objects.size() == 8) {
      vdp.io.spriteOverflow = 1;
      break;
    }
  }
}

auto VDP::Sprite::run() -> void {
  output.color = 0;

  if(state.y >= vdp.vlines()) return;

  bool large = vdp.io.extendedHeight;
  for(auto& o : objects) {
    if(state.x < o.x) continue;
    if(state.x > o.x + 7) continue;

    uint x = o.x - state.x;
    uint y = o.y - state.y;

    uint14 address = vdp.io.spritePatternTableAddress << 13;
    address += o.pattern << 5;
    address += (y & (large ? 15 : 7)) << 2;

    uint4 color;
    color.bit(0) = vdp.vram[address + 0].bit(x & 7);
    color.bit(1) = vdp.vram[address + 1].bit(x & 7);
    color.bit(2) = vdp.vram[address + 2].bit(x & 7);
    color.bit(3) = vdp.vram[address + 3].bit(x & 7);
    if(color == 0) continue;

    if(output.color) {
      vdp.io.spriteCollision = true;
      break;
    }

    output.color = color;
  }

  state.x++;
}

auto VDP::Sprite::power() -> void {
}

auto VDP::Sprite::reset() -> void {
  memory::fill(&state, sizeof(State));
  memory::fill(&output, sizeof(Output));
}
