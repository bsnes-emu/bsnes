
void PPU::renderObjs(unsigned y) {
  if(objEnable == false)
    return;
  
  for(unsigned n = 0; n < 128; n++) {
    objinfo& obj = this->obj[n];
    
    if(obj.renderMode == objinfo::hidden) continue;
    
    uint8 width  = 1 << obj.size;
    uint8 height = 1 << obj.size;
    uint8 row    = y - obj.y;
    
    bool wide = obj.shape == objinfo::wide;
    bool tall = obj.shape == objinfo::tall;
    bool affine = obj.renderMode & objinfo::affine;
    bool size = obj.renderMode == objinfo::affineDouble;
    
    if(obj.size <= 1) width <<= wide, height <<= tall;
    if(obj.size >= 1) width >>= tall, height >>= wide;
    if(wide && tall)  width = 1, height = 1;
    
    if(row   >= 8*(height << size)) continue;
    if(obj.x <= -8*(width << size)) continue;
    
    if(obj.kind == objinfo::bitmap) renderBitmapObj(obj, width, height, row);
    else                            renderTiledObj(obj, width, height, row);
  }
}

void PPU::renderTiledObj(objinfo& obj, unsigned objw, unsigned objh, unsigned y) {
  unsigned step = objTileMode?     0+objTileStep : 0;
  unsigned srow = objTileMode? objw << obj.depth : 32;
  unsigned tile = obj.index << step;
  
  uint32  zmask  = pxPriority*3u + pxLayer*7u;
  uint32  flags  = pxPriority*obj.priority + pxLayer*1u;
  bool    affine = obj.renderMode & objinfo::affine;
  bool    size   = obj.renderMode == objinfo::affineDouble;
  bool    hflip  = obj.transform & 8;
  bool    vflip  = obj.transform & 16;
  bool    ext    = obj.depth && objLargePal;
  bool    win    = obj.kind == objinfo::window;
  unsigned pal   = obj.depth? 256*obj.palette : 16*obj.palette;
  unsigned mask  = obj.depth? 0xff : 0x0f;
  
  if(blendAbove & 1<<4) flags |= pxBlendAbove;
  if(blendBelow & 1<<4) flags |= pxBlendBelow;
  
  if(obj.kind == objinfo::blend) flags |= pxBlendForce;
  
  affineparam tf = { 0x100, 0, 0, 0x100 };
  
  int32 cx = 8*objw/2, ox = 0 - (cx << size), dw = 8*objw << size;
  int32 cy = 8*objh/2, oy = y - (cy << size), dh = 8*objh << size;
  
  if(affine) {
    tf = objTransform[obj.transform];
  } else {
    if(hflip) ox++, tf.dx_dh *= -1;
    if(vflip) oy++, tf.dy_dv *= -1;
  }
  
  int32 fx = ox*tf.dx_dh + oy*tf.dx_dv;
  int32 fy = ox*tf.dy_dh + oy*tf.dy_dv;
  
  int x = obj.x + 8;
  for(unsigned t = 0; t < dw && x < 256+8; x++, t++) {
    uint32 mx = (fx >> 8) + cx;
    uint32 my = (fy >> 8) + cy;
    
    if(mx < 8*objw && my < 8*objh) {
      unsigned taddr = tile;
      
      taddr += (my/8)*srow;
      taddr += (mx/8) << obj.depth; taddr <<= 3;
      taddr += (my%8) << obj.depth; taddr <<= 2;
      taddr += (mx%8) >>!obj.depth;
      
      uint16 data = system.vmap.obj(which, taddr>>14)[taddr];
      
      if(obj.depth) data = data >> 8*(mx%2) & mask;
      else          data = data >> 4*(mx%4) & mask;
      
      if(x >= 0 && data) {
        if(win) {
          window[x] = min(window[x], 0x20+winArea[2]);
        }
        else {
          uint32 bgr = objPal[pal+data & 0xff];
          if(ext) {
            bgr = system.vmap.objpal[which][pal>>13][pal+data << 1];
            bgr = (bgr<<3 & 0760000) | (bgr<<2 & 0007600) | (bgr<<1 & 0000076);
          }
          if(flags < (objLayer[x] & zmask)) objLayer[x] = flags + bgr;
        }
      }
    }
    fx += tf.dx_dh;
    fy += tf.dy_dh;
  }
}

void PPU::renderBitmapObj(objinfo& obj, unsigned objw, unsigned objh, unsigned y) {
  unsigned srow = objBitmapMode>1? 16*objw : 256<<objBitmapMode;
  unsigned tile = obj.index;
  
  if(objBitmapMode>1) tile <<= 7+objBitmapStep;
  else                tile   = 8*srow*(obj.index>>5 & 31) + 16*(obj.index & 31);
  
  uint32  zmask  = pxPriority*3u + pxLayer*7u;
  uint32  flags  = pxPriority*obj.priority + pxLayer*1u + pxAlpha*2u*(obj.palette+1);
  bool    affine = obj.renderMode & objinfo::affine;
  bool    size   = obj.renderMode == objinfo::affineDouble;
  bool    hflip  = obj.transform & 8;
  bool    vflip  = obj.transform & 16;
  unsigned alpha = obj.palette;
  
  if(blendAbove & 1<<4) flags |= pxBlendAbove;
  if(blendBelow & 1<<4) flags |= pxBlendBelow;
  
  flags |= pxBlendForce + (2*alpha+1)*pxAlpha;
  
  affineparam tf = { 0x100, 0, 0, 0x100 };
  
  int32 cx = 8*objw/2, ox = 0 - (cx << size), dw = 8*objw << size;
  int32 cy = 8*objh/2, oy = y - (cy << size), dh = 8*objh << size;
  
  if(affine) {
    tf = objTransform[obj.transform];
  } else {
    if(hflip) ox++, tf.dx_dh *= -1;
    if(vflip) oy++, tf.dy_dv *= -1;
  }
  
  int32 fx = ox*tf.dx_dh + oy*tf.dx_dv;
  int32 fy = ox*tf.dy_dh + oy*tf.dy_dv;
  
  int x = obj.x + 8;
  for(unsigned t = 0; t < dw && x < 256+8; x++, t++) {
    uint32 mx = (fx >> 8) + cx;
    uint32 my = (fy >> 8) + cy;
    
    if(mx < 8*objw && my < 8*objh) {
      unsigned taddr = tile + srow*my + 2*mx;
      
      uint16 data = system.vmap.obj(which, taddr>>14)[taddr];
      
      if(x >= 0 && (data & 0x8000)) {
        uint5 b = data>>10, g = data>>5, r = data>>0;
        
        if(flags < (objLayer[x] & zmask)) objLayer[x] = flags | b<<13 | g<<7 | r<<1;
      }
    }
    fx += tf.dx_dh;
    fy += tf.dy_dh;
  }
}
