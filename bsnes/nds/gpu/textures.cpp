
void GPU::TexCache::reset() {
  for(auto &bucket : table) {
    for(auto &texture : bucket)
      delete texture;
    
    bucket.reset();
  }
  size = 0;
}

bool GPU::TexCache::checkDirty(Texture *texture, uint32 *range, VRAMMapping* map, uint32 mask) {
  uint32 addr = range[0]       & ~255 & mask;
  uint32 end  = range[1] + 255 & ~255 & mask;
  
  for(; addr != end; addr = addr+256 & mask)
    if(map[addr>>14].dirty(addr)) return true;
  
  return false;
}

void GPU::TexCache::flushDirty(unsigned bank) {
  // Called whenever VRAM is (re-)assigned as texture memory
  //   (ie. the game finished uploading and has locked it).
  bool palMem = bank >= 4;
  
  for(auto &bucket : table) {
    for(auto &texture : bucket) {
      if(!(!palMem && checkDirty(texture, texture->texelRange, system.vmap.tex,    0x7ffff)
        || !palMem && checkDirty(texture, texture->indexRange, system.vmap.tex,    0x7ffff)
        ||  palMem && checkDirty(texture, texture->colorRange, system.vmap.texpal, 0x1ffff) ))
          continue;
      
      delete texture;
      texture = nullptr;
    }
    // Flush all the nullptrs at once to avoid O(n^2) removal.
    unsigned count = 0;
    for(unsigned n = 0; n < bucket.size(); n++) {
      if(bucket[n]) bucket[count++] = bucket[n];
    }
    bucket.resize(count);
  }
}


void GPU::TexCache::add(Texture *t) {
  uint10 hash = t->key ^ t->key>>10 ^ t->key>>20 ^ t->key>>30;
  table[hash].append(t);
  size++;
}

GPU::Texture* GPU::TexCache::get(uint64 key) {
  uint10 hash = key ^ key>>10 ^ key>>20 ^ key>>30;
  
  for(auto texture : table[hash]) {
    if(texture->key == key)
      return texture;
  }
  return nullptr;
}



GPU::Texture *GPU::getTexture(uint32 texImage, uint16 texPalette) {
  //texCache.reset();
  
  uint64   key = (uint64) texImage ^ (uint64)texPalette<<30;
  Texture *tex = texCache.get(key);
  
  if(!tex) {
    int format = texImage>>26 & 7;
    if(!format) return nullptr;
    
    bool     colorKey  = texImage & 1<<29;
    unsigned uaxis     = texImage>>16 & 5;
    unsigned vaxis     = texImage>>17 & 5;
    unsigned imgwidth  = 8 << (texImage>>20 & 7);
    unsigned imgheight = 8 << (texImage>>23 & 7); 
    
    tex = new Texture(key, imgwidth, imgheight);
    
    tex->colorBase = 8*(texPalette & 0x3fff);
    tex->imageBase = 8*(texImage   & 0xffff);
    tex->indexBase = 0x20000 + (tex->imageBase/2 & 0xffff);
    if(tex->imageBase >= 0x40000)
      tex->indexBase += 0x10000;
    
    // Mirroring implies repeat as well.
    if(uaxis == Prim::mirror) tex->width *= 2;
    if(vaxis == Prim::mirror) tex->height *= 2;
    if(uaxis &  Prim::repeat) tex->umask = tex->width-1;
    if(vaxis &  Prim::repeat) tex->vmask = tex->height-1;
    
    tex->image         = new uint32[tex->width * tex->height];
    tex->colorRange[0] = tex->colorRange[1] = tex->colorBase;
    tex->texelRange[0] = tex->texelRange[1] = tex->imageBase;
    tex->indexRange[0] = tex->indexRange[1] = tex->indexBase;
    
    switch(format) {
    case Prim::packed:   convertPacked(tex, imgwidth, imgheight); break;
    case Prim::I2:       convertI2(tex, imgwidth, imgheight, colorKey); break;
    case Prim::A5_I3:    convertI3(tex, imgwidth, imgheight); break;
    case Prim::I4:       convertI4(tex, imgwidth, imgheight, colorKey); break;
    case Prim::A3_I5:    convertI5(tex, imgwidth, imgheight); break;
    case Prim::I8:       convertI8(tex, imgwidth, imgheight, colorKey); break;
    case Prim::A1_RGB15: convertARGB(tex, imgwidth, imgheight); break;
    }
    mirrorTexture(tex, imgwidth, imgheight);
    texCache.add(tex);
  }
  return tex;
}

void GPU::mirrorTexture(Texture *tex, int width, int height) {
  for(int y = 0; y < height; y++) {
    uint32 *row    = &tex->image[tex->width*y];
    uint32 *mirror = &tex->image[tex->width*(tex->height-1 - y)];
    
    if(tex->width > width) {
      for(int x = 0; x < width; x++)
        row[tex->width-1 - x] = row[x];
    }
    if(tex->height > height) {
      memcpy(mirror, row, tex->width*sizeof(*row));
    }
  }
}

void GPU::convertPacked(Texture *tex, int width, int height) {
  uint32 taddr = tex->imageBase;
  uint32 caddr = tex->indexBase;
  
  tex->texelRange[1] += (width/4)*(height/4)*4;
  tex->indexRange[1] += (width/4)*(height/4)*2;
  tex->colorRange[0] += 0x10000;
  
  for(unsigned v = 0; v < height; v += 4) {
    uint32 *dest = &tex->image[tex->width*v];
    
    for(unsigned u = 0; u < width; u += 4) {
      // Retrieve 4x4 block = 16 pixels x 2bpp = 32 bits
      auto   &page = system.vmap.tex[taddr>>14 & 31];
      uint32 block = page[taddr] | page[taddr+2]<<16;
      
      // Retrieve color selection and mode bits
      uint16 colorsel = system.vmap.tex[caddr>>14 & 31][caddr];
      uint32 paddr    = tex->colorBase + (4*colorsel & 0xfffc);
      uint32 fields   = 31<<20 | 31<<10 | 31<<0;
      uint32 gap      = 15;  // space apart to allow averaging
      
      // This could potentially span most of the palette space, but that would
      // complicate the game's texture allocation, so rather unlikely.
      tex->colorRange[0] = min(tex->colorRange[0], paddr+0);
      tex->colorRange[1] = max(tex->colorRange[1], paddr+8);
      
      uint32 alpha[4] = { 31, 31, 31, 31 };
      
      uint32 colors[4] = {
        uint15(system.vmap.texpal[paddr+0 >> 14 & 31][paddr+0]) * (1<<gap|1) & fields,
        uint15(system.vmap.texpal[paddr+2 >> 14 & 31][paddr+2]) * (1<<gap|1) & fields,
        uint15(system.vmap.texpal[paddr+4 >> 14 & 31][paddr+4]) * (1<<gap|1) & fields,
        uint15(system.vmap.texpal[paddr+6 >> 14 & 31][paddr+6]) * (1<<gap|1) & fields,
      };
      
      switch(colorsel >> 14) {
      case 3: // 2 colors (4-way mix)
        colors[2] = (5*colors[0] + 3*colors[1])/8 & fields;
        colors[3] = (3*colors[0] + 5*colors[1])/8 & fields;
      case 2: // 4 colors (separate)
        break;
        
      case 1: // 2 colors (3-way mix) + transparent
        colors[2] = (4*colors[0] + 4*colors[1])/8 & fields;
      case 0: // 3 colors (separate) + transarent
        alpha[3] = 0;
        break;
      }
      
      for(int i = 0; i < 4; i++)   {
        colors[i] |= colors[i] >> gap;
        colors[i] = alpha[i]<<15 | colors[i] & 0x7fff;
      }
      for(int sv = 0; sv < 4; sv++, dest += tex->width) {
        for(int su = 0; su < 4; su++, block >>= 2)
          dest[su] = colors[block & 3];
      }
      dest += 4 - 4*tex->width;
      taddr += 4;
      caddr += 2;
    }
  }
}

void GPU::convertI2(Texture *tex, int width, int height, bool colorKey) {
  tex->texelRange[1] += width*height/4;
  tex->colorRange[1] += 8;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + width/4*v;
    
    for(unsigned u = 0; u < width; u++, taddr += !(u%4)) {
      uint2  index = system.vmap.tex[taddr>>14 & 31][taddr] >> 2*(u&7);
      uint32 paddr = tex->colorBase + 2*index;
      uint16 bgr   = system.vmap.texpal[paddr>>14 & 31][paddr];
      uint5  a     = 31*(colorKey == false || index > 0);
      
      *dest++ = a<<15 | bgr;
    }
  }
}

void GPU::convertI3(Texture *tex, int width, int height) {
  tex->texelRange[1] += width*height/1;
  tex->colorRange[1] += 16;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + width*v;
    
    for(unsigned u = 0; u < width; u++, taddr++) {
      uint8  index = system.vmap.tex[taddr>>14 & 31][taddr] >> 8*(u&1);
      uint32 paddr = tex->colorBase + 2*(index & 0x07);
      uint16 bgr   = system.vmap.texpal[paddr>>14 & 31][paddr];
      uint5  a     = index>>3;
      
      *dest++ = a<<15 | bgr;
    }
  }
}

void GPU::convertI4(Texture *tex, int width, int height, bool colorKey) {
  tex->texelRange[1] += width*height/2;
  tex->colorRange[1] += 32;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + width/2*v;
    
    for(unsigned u = 0; u < width; u++, taddr += !(u%2)) {
      uint4  index = system.vmap.tex[taddr>>14 & 31][taddr] >> 4*(u&3);
      uint32 paddr = tex->colorBase + 2*index;
      uint16 bgr   = system.vmap.texpal[paddr>>14 & 31][paddr];
      uint5  a     = 31*(colorKey == false || index > 0);
      
      *dest++ = a<<15 | bgr;
    }
  }
}

void GPU::convertI5(Texture *tex, int width, int height) {
  tex->texelRange[1] += width*height/1;
  tex->colorRange[1] += 64;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + width*v;
    
    for(unsigned u = 0; u < width; u++, taddr++) {
      uint8  index = system.vmap.tex[taddr>>14 & 31][taddr] >> 8*(u&1);
      uint32 paddr = tex->colorBase + 2*(index & 0x1f);
      uint16 bgr   = system.vmap.texpal[paddr>>14 & 31][paddr];
      uint5  a     = (index>>5) * 9/2;
      
      *dest++ = a<<15 | bgr;
    }
  }
}

void GPU::convertI8(Texture *tex, int width, int height, bool colorKey) {
  tex->texelRange[1] += width*height/1;
  tex->colorRange[1] += 512;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + width*v;
    
    for(unsigned u = 0; u < width; u++, taddr++) {
      uint8  index = system.vmap.tex[taddr>>14 & 31][taddr] >> 8*(u&1);
      uint32 paddr = tex->colorBase + 2*index;
      uint16 bgr   = system.vmap.texpal[paddr>>14 & 31][paddr];
      uint5  a     = 31*(colorKey == false || index > 0);
      
      *dest++ = a<<15 | bgr;
    }
  }
}

void GPU::convertARGB(Texture *tex, int width, int height) {
  tex->texelRange[1] += width*height*2;
  
  for(unsigned v = 0; v < height; v++) {
    uint32 *dest = &tex->image[tex->width*v];
    uint32 taddr = tex->imageBase + 2*width*v;
    
    for(unsigned u = 0; u < width; u++, taddr += 2) {
      uint16 abgr = system.vmap.tex[taddr>>14 & 31][taddr];
      uint5  a    = 31*(abgr >> 15);
      
      *dest++ = a<<15 | abgr;
    }
  }
}