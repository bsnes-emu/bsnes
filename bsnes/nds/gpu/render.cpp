
void GPU::updateDrawList(int y, DrawList& list, bool sorted) {
  // Blah, why are polys with numVerts==0 making it in?

  // Remove polygons that have finished rendering
  while(list.first < list.last && (!list.polys[list.first].numVerts
    || 192-list.polys[list.first].v[list.polys[list.first].numVerts-1]->y/0x400000 < y))
      list.first++;
  
  // Add primitives matching Y as their first line
  unsigned pi = list.nextPrim;
  while(pi < list.lastPrim) {
    auto *prim = &renderList->prims[renderList->order[pi]];
    if(sorted && y < prim->minY-1)
      break;
    
    if(!sorted || y == prim->minY-1) {
      auto *v = &renderList->verts[prim->firstVert];
      
      if(prim->drawMode & Prim::triStrip) {
        for(unsigned n = 2; n < prim->numVerts; n += 1, v += 1)
          setupPoly(list.polys[list.last++], prim, v, 3);
      }
      else if(prim->drawMode & Prim::quadStrip) {
        for(unsigned n = 2; n < prim->numVerts; n += 2, v += 2)
          setupPoly(list.polys[list.last++], prim, v, 4);
      }
      else {
        setupPoly(list.polys[list.last++], prim, v, prim->numVerts);
      }
      
      if(list.nextPrim == pi)
        list.nextPrim++;
    }
    pi++;
  }
}

void GPU::setupPoly(ActivePoly& poly, Prim* prim, Vertex *vs, unsigned numVerts) {
  for(unsigned n = 0; n < numVerts; n++)
    poly.v[n] = &vs[n];
  
  // Sort vertices by Y, then X if on same line
  std::sort(poly.v, poly.v+numVerts, [](Vertex *a, Vertex *b)
    { return a->y != b->y? a->y > b->y : a->x < b->x; });
  
  poly.numVerts          = numVerts;
  if(!numVerts) return;
  
  poly.p                 = prim;
  poly.lv = poly.next_lv = 0;  // start at top vertex
  poly.rv = poly.next_rv = 0;
  poly.side[0]           = 0;  // top and bottom are
  poly.side[numVerts-1]  = 0;  //   considered to be on both sides.
  
  // Find out which side each vertex is on. This is based on the observation
  // that with a convex polygon, edges always curve inwards, towards the other
  // side. So we switch sides every time the curve changes direction.
  for(int n = 1; n < numVerts-1; n++) {
    int64 dx0 = poly.v[n]->x - poly.v[n-1]->x;
    int64 dx1 = poly.v[n+1]->x - poly.v[n]->x;
    int64 dy0 = -(poly.v[n]->y - poly.v[n-1]->y);
    int64 dy1 = -(poly.v[n+1]->y - poly.v[n]->y);
    
    poly.side[n] = poly.side[n-1];  // special case for straight or co-linear edges
    
    if(!dy0) dy0 = 1;
    if(!dy1) dy1 = 1;
    
    // compare dx1/dy1 <=> dx0/dy0
    // - both sides have been multiplied by dy0*dy1 to avoid division by 0
    if(dx1*dy0 > dx0*dy1) poly.side[n] = -1;  // curves to the right, so must be on the left
    if(dx1*dy0 < dx0*dy1) poly.side[n] = +1;  // curves to the left, so must be on the right
  }
  setupNextEdge(poly, poly.lv, poly.next_lv, -1, poly.lnext, poly.dl_dv);
  setupNextEdge(poly, poly.rv, poly.next_rv, +1, poly.rnext, poly.dr_dv);
  
  poly.prev_lx = poly.lnext.x;
  poly.prev_rx = poly.rnext.x;
}

void GPU::setupNextEdge(ActivePoly& poly, unsigned& vi, unsigned& next,
  int side, Interpolants& edge, Interpolants& gradient)
{
  vi = next++;
  while(poly.side[next]*side < 0)
    next++;
  
  Interpolants endpoint;
  assignEdge(edge,      poly.v[vi]);    edge.x     &= ~0x3fffff;
  assignEdge(endpoint,  poly.v[next]);  endpoint.x &= ~0x3fffff;
  
  int32 y0 = 192 - poly.v[vi]->y/0x400000;
  int32 y1 = 192 - poly.v[next]->y/0x400000;
  int32 numLines = y1 - y0;
  if(numLines) {
    for(unsigned j = 0; j < Interpolants::count; j++)
      gradient[j] = (endpoint[j] - edge[j]) / numLines;
  }
}

void GPU::assignEdge(Interpolants& edge, Vertex* v) {
  edge.r = v->r;  edge.u = v->u;
  edge.g = v->g;  edge.v = v->v;
  edge.b = v->b;  edge.z = v->z;
  
  edge.inv_w = 0x80000000000/max(1,v->w);
  edge.x     = v->x;
}



void GPU::renderScanline() {
  int y = renderedLines;
  auto *line  = 1 + pixels[y-0 & 3];
  auto *line1 = 1 + pixels[y-1 & 3];
  auto *line2 = 1 + pixels[y-2 & 3];
   
  updateDrawList(y, drawList[0], true);
  updateDrawList(y, drawList[1], renderList->alphaSort);
  
  unsigned backOffs       = 0x40000 + 512*(backY+y & 0xff);
  uint16  *backImageColor = &system.vmap.tex[backOffs+0x00000 >> 14][backOffs+0x00000];
  uint16  *backImageDepth = &system.vmap.tex[backOffs+0x20000 >> 14][backOffs+0x20000];
  
  for(int x = -1; x <= 256; x++) {
    auto &px = line[x];
    uint16 color = backImageColor[backX+x & 0xff];
    uint16 depth = backImageDepth[backX+x & 0xff];
    
    // Stencil is preserved, even between frames!
    // Transitioning to stencilMode=1 is what clears it.
    px.flags &= Pixel::stencil;
    px.flags &= ~Pixel::blends;
    
    if(renderControl.backImage) {
      px.a.r  = color<<1 & 62;    px.a.r += px.a.r > 0;
      px.a.g  = color>>4 & 62;    px.a.g += px.a.g > 0;
      px.a.b  = color>>9 & 62;    px.a.b += px.a.b > 0;
      px.a.a  = color>>15? 31 : 0;
      px.a.id = backId;
      px.az   = depth<<16 & 0x7fff0000;
      
      if(depth & 1<<15) px.flags |= Pixel::fog;
    }
    else {
      px.a.r  = backColor[0];
      px.a.g  = backColor[1];
      px.a.b  = backColor[2];
      px.a.a  = backAlpha;
      px.a.id = backId;
      px.az   = backZ<<16;
      
      if(backFog) px.flags |= Pixel::fog;
    }
    
    px.b.a  = 0;
    px.b.id = 0xff;
    px.bz   = 0x7fffffff;
    px.sz   = 0x7fffffff;
  }
  
  renderDrawList(y, drawList[0], false);
  renderDrawList(y, drawList[1], true);
  if(y < 1) return;
  
  y--;  // edge filter requires 1 line latency
  
  int32 fogZ = this->fogZ << 16;
  uint32 *__restrict dest = &output[256*y];
  
  for(int x = 0; x < 256; x++) {
    // Any remaining fragments are combined here... or maybe this should
    // be done between solid/alpha passes? I'm not sure if you can get a
    // transparent pixel between a solid and a smooth edge, and still have
    // it look right.
    auto &px = line1[x];
    auto &above = px.a;
    auto &below = px.b;
    uint8 r = above.r;
    uint8 g = above.g;
    uint8 b = above.b;
    uint8 a = above.a;
    uint8 id = above.id;
    int64 z = px.az;
    
    if(px.flags & Pixel::edge) {
      auto &up    = line2[x];
      auto &left  = line1[x-1], &right = line1[x+1];
      auto &down  = line[x];
      
      // Toon edge filter - need to verify logic here, might be &&, etc.
      if(  z < up.az   && id != up.a.id   || z < down.az  && id != down.a.id
        || z < left.az && id != left.a.id || z < right.az && id != right.a.id
      ) {
        Vector outline = edgeTable[id/8];
        
        if(renderControl.edgeMode & RC::toon) {
          // Replace RGB with values from the table.
          // Is there an option to disable this for some polygons?
          r = outline[0] / 0x1000;
          g = outline[1] / 0x1000;
          b = outline[2] / 0x1000;
          
          // In this case, alpha is also overwritten. Edges that don't
          // pass the filter remain antialiased, as in "smooth" mode.
          if(renderControl.edgeMode == RC::blended)
            a = 16;  // fixed value - or is there a register?
        }
      }
    }
    
    if(below.a /*not empty*/) {
      if(below.id == id && a < 31 && (px.flags & Pixel::blends)) {
        // Don't blend equal object IDs
        r = below.r;
        g = below.g;
        b = below.b;
        a = below.a;
      }
      else if(renderControl.alphaBlend || !(px.flags & Pixel::blends)) {
        // Z blends, too! You can see this if you set fogAlpha=0
        // with edge smoothing on.
        z = px.bz + (z - px.bz) * (a+1) / 32u;
        
        // They cheaped out here. To get correct Photoshop-style alpha
        // (non pre-multiplied) requires dividing by A. It's nasty.
        r = below.r + (r - below.r) * (a+1) / 32u;
        g = below.g + (g - below.g) * (a+1) / 32u;
        b = below.b + (b - below.b) * (a+1) / 32u;
        a = max(a, below.a);
      }
    }
    
    // Polygons can be fogged toward a constant RGBA, or optionally just
    // their alpha channel. Unlike most systems, this is a per-pixel effect
    // based on the Z buffer.
    if(px.flags & Pixel::fog) {
      // Z := 0..0x7fffffff
      int32 dist = z-fogZ >> 16-renderControl.fogLevel;
      dist = max(0, min(0x7fff, dist));
      
      int32 ifog = fogTable[dist/0x400u];
      ifog += (fogTable[dist/0x400u + 1] - ifog) * (dist & 0x3ff)/0x400u;
      
      if(renderControl.fogMode == RC::color) {
        r += (fogColor[0] - r) * (ifog+1)/0x80u;
        g += (fogColor[1] - g) * (ifog+1)/0x80u;
        b += (fogColor[2] - b) * (ifog+1)/0x80u;
      }
      a += (fogAlpha - a) * (ifog+1)/0x80u;
    }
    *dest++ = a<<18 | b<<12 | g<<6 | r<<0;
  }
}

void GPU::renderDrawList(int y, DrawList& list, bool blend) {
  auto *line = 1 + pixels[y & 3];
  
  unsigned alphaTest = minAlpha * renderControl.alphaTest;
  
  for(unsigned n = list.first; n < list.last; n++) {
    auto &poly = list.polys[n];
    if(!poly.numVerts) continue;
    
    int y0 = 192-poly.v[0]->y/0x400000;
    int y1 = 192-poly.v[poly.numVerts-1]->y/0x400000; 
    
    if(y < y0-1 || y >= y1) continue;
    
    while(y == 192-poly.v[poly.next_lv]->y/0x400000-1 && poly.next_lv < poly.numVerts-1)
      setupNextEdge(poly, poly.lv, poly.next_lv, -1, poly.lnext, poly.dl_dv);
    
    while(y == 192-poly.v[poly.next_rv]->y/0x400000-1 && poly.next_rv < poly.numVerts-1)
      setupNextEdge(poly, poly.rv, poly.next_rv, +1, poly.rnext, poly.dr_dv);
    
    // X range for previous, current, and next line
    // We need this to determine "edge" cases (sorry) ->
    int px0 = poly.prev_lx / 0x400000;  // Top:    x in x0..prev_x0
    int px1 = poly.prev_rx / 0x400000;  //           or prev_x1..x1
    int x0  = poly.left.x  / 0x400000;  // Bottom: x in x0..next_x0
    int x1  = poly.right.x / 0x400000;  //           or next_x1..x1
    int nx0 = poly.lnext.x / 0x400000;  // Left:   x+0 == x0
    int nx1 = poly.rnext.x / 0x400000;  // Right:  x+1 == x1
    
    // Theory:
    // A pixel is marked "edge" on one of the above conditions. Translucent
    // (alpha < 31) polygons do not modify the edge flags. Smooth edges have
    // their alpha set to the fraction of pixel covered.
    
    // Test for dropout - DS doesn't have explicit line or point primitives;
    // to draw them, you simply use geometry thinner than 1px. The rasterizer
    // ALMOST never leaves holes.. even in extremely narrow triangles. Very
    // occasionally, degenerate triangles will have gaps.
    if(x0 > px1) x0 = px1;
    if(x1 < px0) x1 = px0;
    if(x1 <= x0) x1 = x0;
    
    if(y >= y0) {
      Texture *tex = nullptr;
      if(renderControl.texturing)
        tex = getTexture(poly.p->texImage, poly.p->texPalette);
      
      Interpolants i = poly.left;  // RGB, UVZW at current pixel
      Interpolants delta;          // Gradient from left to right edges
      
      if(x1 > x0)
        for(unsigned j = 0; j < Interpolants::count; j++)
          delta[j] = (poly.left[j] - poly.right[j]) / (x0-x1);
      
      uint8 id      = poly.p->id;
      int drawMode  = poly.p->drawMode;
      int shadeMode = drawMode & Prim::shadeMask;
      
      if(!renderControl.fogMode)
        drawMode &= ~Prim::fog;
     
      if((drawMode & Prim::shadeMask) == Prim::stencil) {
        if(id) {
          stencilMode = false;  // Draw to screen, using stencil bits as mask.
        }
        else if(stencilMode == false) {
          stencilMode = true;  // Draw to stencil buffer.
          
          // Toggling into stencil mode with ID=0 was observed to clear the
          // stencil buffer. This IS affected by polygon sorting so it will
          // not happen on every line, or even every frame, unless so arranged!
          //
          // The buffer holds data for only 2 lines, and if not cleared, then
          // old data is used. So shadow volumes must be closed to work.
          for(unsigned x = 0; x < 256; x++)
            line[x].flags &= ~Pixel::stencil;
        }
      }
      
      if(i.inv_w < 0x2000) i.inv_w = 0x2000;
      
      // Hrm. Too many variables here, this should be split up. Perhaps:
      //   pass 1: calculate edge coverage (a==31 only)
      //   pass 2: if z <= bz, calculate [argbf]z (modulate, decal, toon, stencil)
      //   pass 3: do atest, ztest (less, equal), merge with line buffer (a==31, a<31)?
      
      for(unsigned x = x0; x <= x1; x++) {
        int32 w = 0x80000000000ll / max(2ll, i.inv_w);
        int32 z = i.z + 0x40000000; // z/w
        
        // Should find some way to justify this - <<7 was Selected for
        // suitable fog in Mario Kart's Chocolate Mountain course..
        if(renderList->depthUseW)
          z = w<<7;
        
        // Unproject the interpolated values
        int32 u = ((int64) i.u * w)/0x40000000;
        int32 v = ((int64) i.v * w)/0x40000000;
        
        Vector color = {
          min(63, (int64) i.r * w/0x4000000),
          min(63, (int64) i.g * w/0x4000000),
          min(63, (int64) i.b * w/0x4000000),
          poly.p->alpha
        };
        
        i.u     += delta.u;     i.r += delta.r;
        i.v     += delta.v;     i.g += delta.g;
        i.z     += delta.z;     i.b += delta.b;
        i.inv_w += delta.inv_w;
        
        bool edge = false;
        
        if(blend == false) {
          edge = x0 <= x && (x <= px0 || x <= nx0)
            || (px1 <= x || nx1 <= x) && x <= x1
            || x == x0 || x == x1;
          
          if(renderControl.edgeMode & RC::smooth) {
            if(x == x0) color[3] = 31-(poly.left.x>>17 & 31);
            if(x == x1) color[3] =    (poly.right.x>>17 & 31);
          }
        }
        
        Vector texColor = { 63, 63, 63, 31 };
        if(tex) {
          u = max(0, min(tex->width-1,  u & tex->umask));
          v = max(0, min(tex->height-1, v & tex->vmask));
          
          uint32 abgr = tex->image[tex->width*v + u];
          texColor[0] = 2*(abgr>> 0 & 31) + 1;
          texColor[1] = 2*(abgr>> 5 & 31) + 1;
          texColor[2] = 2*(abgr>>10 & 31) + 1;
          texColor[3] = (abgr>>15);
        }
        
        auto &px = line[x];
        auto &above = px.a;
        auto &below = px.b;
        bool ztest_a = drawMode & Prim::zequal? z == px.az : z < px.az;
        bool ztest_b = drawMode & Prim::zequal? z == px.bz : z < px.bz;
        
        if(shadeMode == Prim::stencil) {
          if(stencilMode) {
            // Draw to stencil buffer, not the screen
            // Does texturing work here? Alpha, maybe?
            if(ztest_a == false) {
              px.flags |= Pixel::stencil;
              px.sz     = px.az;
            }
            continue;
          }
          // Mask polygons using the stencil buffer
          if(px.flags & Pixel::stencil) {
            // Succeeded - force Z onto the fragment being shadowed
            z = px.sz;
            if(id == above.id) {
              // FF IV surrounds characters in shadow volumes, and uses the ID
              // to prevent self-shadowing. So technically this fragment is on
              // top, yet the DS somehow draws it underneath the model. If not
              // done this way, edge smoothing would clash with the shadow.
              ztest_a = false;
              ztest_b = true;
              z = px.bz;
            }
          } else {
            // Failed stencil test
            continue;
          }
        }
        
        if(shadeMode == Prim::toon) {
          // Look up shade based on the red channel
          int a = color[3];
          color = toonTable[renderControl.toonShading][color[0]/2 & 31];
          color[0] /= 0x1000u;
          color[1] /= 0x1000u;
          color[2] /= 0x1000u;
          color[3] = a;
        }
        
        if(shadeMode == Prim::decal) {
          color[0] += (texColor[0] - color[0]) * (texColor[3]+1) / 32u;
          color[1] += (texColor[1] - color[1]) * (texColor[3]+1) / 32u;
          color[2] += (texColor[2] - color[2]) * (texColor[3]+1) / 32u;
        } else {
          color[0] = color[0] * texColor[0] / 64u;
          color[1] = color[1] * texColor[1] / 64u;
          color[2] = color[2] * texColor[2] / 64u;
          color[3] = (1 + color[3]) * texColor[3] / 32u;
        }
        
        if(color[3] <= renderControl.alphaTest)
          continue;
        
        if(blend) {
          if(ztest_a) {
            if(above.id == id) continue;
            
            if(!renderControl.alphaBlend) {
              // Mario Kart uses this on the car selection screen. The alpha
              // and coverage are still retained and used for PPU blending.
              above.r = color[0];
              above.g = color[1];
              above.b = color[2];
              above.a = max(color[3], px.a.a);
              above.id = id;
              
              if(!(drawMode & Prim::fog)) px.flags &= ~Pixel::fog;
              if(drawMode & Prim::zwrite) px.az = z;
              continue;
            }
            
            if(below.a /*not empty*/) {
              if(below.id != above.id || color[3] == 31 || !(px.flags & Pixel::blends)) {
                // Merge the top pixel down to make room
                px.bz  += int64(px.az - px.bz) * (above.a + 1)/32u;
                
                below.r += (above.r - below.r) * (above.a + 1)/32u;
                below.g += (above.g - below.g) * (above.a + 1)/32u;
                below.b += (above.b - below.b) * (above.a + 1)/32u;
                below.a  = max(above.a, below.a);
                below.id = above.id;
              }
            } else {
              // Bottom empty or top solid, simply push down
              below = above;
              px.bz = px.az;
            }
            // Then write the top one
            above.r = color[0];
            above.g = color[1];
            above.b = color[2];
            above.a = color[3];
            above.id = id;
            
            // AND with previous fog bit
            if(!(drawMode & Prim::fog)) px.flags &= ~Pixel::fog;
            if(drawMode & Prim::zwrite) px.az = z;
            
            px.flags |= Pixel::blends;
          }
          else if(ztest_b) {
            if(below.a /*not empty*/) {
              if(below.id == id && color[3] < 31) continue;
              
              if(!renderControl.alphaBlend) {
                if(drawMode & Prim::zwrite)
                  px.bz  = z;
                
                below.r = color[0];
                below.g = color[1];
                below.b = color[2];
              }
              else {
                // Blend into bottom pixel
                if(drawMode & Prim::zwrite)
                  px.bz  += int64(z - px.bz) * (color[3] + 1)/32u;
                
                below.r += (color[0] - below.r) * (color[3] + 1)/32u;
                below.g += (color[1] - below.g) * (color[3] + 1)/32u;
                below.b += (color[2] - below.b) * (color[3] + 1)/32u;
              }
              below.a  = max(color[3], below.a);
              below.id = id;
            } else {
              // Bottom empty, simply replace
              if(drawMode & Prim::zwrite)
                px.bz = z;
              
              below.r = color[0];
              below.g = color[1];
              below.b = color[2];
              below.a = color[3];
              below.id = id;
            }
          }
          continue;
        }
        
        if(ztest_a) {
          if(drawMode & Prim::fog) px.flags |=  Pixel::fog;
          else                     px.flags &= ~Pixel::fog;
          
          // Push the top pixel down. Anything beneath is lost.
          // Check first to avoid breaking backAlpha == 0.
          if(above.a /*not empty*/) {
            px.bz = px.az;
            below = above;
            below.a = 31;
          }
          above.r  = color[0];
          above.g  = color[1];
          above.b  = color[2];
          above.a  = color[3];
          above.id = id;
          px.az    = z;
          
          if(edge) px.flags |= Pixel::edge;
          else     px.flags &= ~Pixel::edge;
        }
        else if(ztest_b) {
          below.r  = color[0];
          below.g  = color[1];
          below.b  = color[2];
          below.a  = 31;
          below.id = id;
          px.bz    = z;
        }
      }
      poly.prev_lx = poly.left.x;
      poly.prev_rx = poly.right.x;
    }
    poly.left    = poly.lnext;
    poly.right   = poly.rnext;
    
    for(unsigned j = 0; j < Interpolants::count; j++) {
      poly.lnext[j] += poly.dl_dv[j];
      poly.rnext[j] += poly.dr_dv[j];
    }
  }
}
