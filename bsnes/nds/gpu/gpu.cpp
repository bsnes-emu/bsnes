#include <nds/nds.hpp>

namespace NintendoDS {

GPU gpu;


GPU::GPU() { }

void GPU::power() {
  powered        = true;
  swapArgument   = 0;
  uploadList     = &sceneList[0];
  renderList     = &sceneList[1];
  renderedLines  = 0;
  stencilMode    = false;
  sceneFinished  = false;
  commandBufIrq  = false;
  packedCommands = 0;
  numArgs        = 0;
  
  for(unsigned n = 0; n < 2; n++) {
    sceneList[n].numVerts = 0;
    sceneList[n].numPrims = 0;
    sceneList[n].firstAlpha = 0;
    sceneList[n].alphaSort = 1;
    sceneList[n].depthUseW = 0;
    
    memset(sceneList[n].verts, 0, sizeof sceneList[n].verts);
    memset(sceneList[n].prims, 0, sizeof sceneList[n].prims);
  }
  
  // Geometry state
  Matrix identity = {
     0x1000, 0,0,0,
    0, 0x1000, 0,0,
    0,0, 0x1000, 0,
    0,0,0, 0x1000,
  };
  clipMatrix    = identity;  projSP = viewSP = stackOverflow = 0;
  textureMatrix = identity;  matrixMode = mmProjection;
  projMatrix    = identity;  for(auto &m : projStack)  m = identity;
  viewMatrix    = identity;  for(auto &m : viewStack)  m = identity;
  lightMatrix   = identity;  for(auto &m : lightStack) m = identity;
  
  for(auto &l : lights) {
    l.enable     = false;
    l.color      = Vector{0,0,0,0};
    l.direction  = Vector{0,0,0,0};
    l.eyeVector  = Vector{0,0,0,0};
    l.halfVector = Vector{0,0,0,0};
  }
  diffuse  = Vector{0,0,0,0};
  ambient  = Vector{0,0,0,0};
  specular = Vector{0,0,0,0};
  emission = Vector{0,0,0,0};
  useShineTable = false;
  for(auto &e : shininess) e = 0;
  
  normal          = Vector{0,0,0,0};
  vertex.position = Vector{0,0,0,0x1000};
  vertex.texCoord = Vector{0,0,0,0x1000};
  vertex.color    = Vector{0x3f000,0x3f000,0x3f000,0};
  
  primitive.size         = 0;
  primitive.winding      = 0;
  primitive.type         = PS::tris;
  primitive.attributes   = attributes = 0;
  primitive.texImage     = texImage   = 0;
  primitive.texPalette   = texPalette = 0;
  primitive.texTransform = PS::ttNone;
  
  // Render state
  renderControl.texturing   = false;
  renderControl.toonShading = RC::darken;
  renderControl.edgeMode    = RC::solid;
  renderControl.fogMode     = 0;
  renderControl.fogLevel    = 0;
  renderControl.alphaTest   = false;
  renderControl.alphaBlend  = false;
  renderControl.backImage   = false;
  
  backColor[0] = 0;       fogColor[0]  = 0;
  backColor[1] = 0;       fogColor[1]  = 0;
  backColor[2] = 0;       fogColor[2]  = 0;
  backAlpha    = 0;       fogAlpha     = 0;
  backId       = 0;       fogZ         = 0;
  backX        = 0;       
  backY        = 0;       minAlpha = 0;
  backZ        = 0x7fff;
  
  for(auto &e : fogTable)     e = 0;
  for(auto &e : edgeTable)    e = Vector{0,0,0,0};
  for(auto &e : toonTable[0]) e = Vector{0,0,0,0};
  for(auto &e : toonTable[1]) e = Vector{0,0,0,0};
  
  for(unsigned n = 0; n < 2; n++) {
    drawList[n].first = 0;
    drawList[n].last = 0;
    drawList[n].nextPrim = 0;
    drawList[n].lastPrim = 0;
  }
}

void GPU::scanline() {
  uint8 renderTime = (video.line + 50) % 263;
  
  if(renderTime < 241 && renderedLines < 192+1) {
    // Rendering starts in advance. Draw time varies; however, up to 48 extra
    // scanlines can be cached, providing a buffer for complicated scenes.
    //
    // That isn't emulated at the moment. Scanline rendering is still needed,
    // however, to get stencilling and other edge cases right.
    renderScanline();
    renderedLines++;
  }
  
  if(renderTime == 242) {
    // Finished rendering - process the new scene list.
    // Games are free to load textures during this time.
    swapBuffers();
    renderedLines = -1;
    
    drawList[0].nextPrim = 0;
    drawList[0].lastPrim = renderList->firstAlpha;
    drawList[0].first    = 0;
    drawList[0].last     = 0;
    
    drawList[1].nextPrim = renderList->firstAlpha;
    drawList[1].lastPrim = renderList->numPrims;
    drawList[1].first    = 0;
    drawList[1].last     = 0;
    
    for(auto &line : pixels) for(auto &p : line) {
      p.a.a  = p.b.a  = 0;
      p.a.id = p.b.id = 0xff;
      p.az   = p.bz   = 0x7fffffff;
    }
  }
  
  if(commandBufIrq)
    arm9.interrupt.flags |= CPUCore::irqGeomBuf;
}

void GPU::swapBuffers() {
  if(sceneFinished) {
    std::swap(uploadList, renderList);
    
    // These settings apply to the NEW list, not the old one.
    uploadList->alphaSort = !(swapArgument >> 0);
    uploadList->depthUseW =   swapArgument >> 1;
    uploadList->numVerts  = 0;
    uploadList->numPrims  = 0;
    
    // Release the ARM9 if it was blocking on a command..
    sceneFinished = false;
    //texCache.reset();
    
    // Terminate the current strip, if any.
    if(renderList->numPrims < 2048 && primitive.type >= 2
      && primitive.size >= (primitive.type==PS::quadStrip? 4 : 3))
        renderList->numPrims++;
    
    while(renderList->numPrims) {
      auto &p = renderList->prims[renderList->numPrims-1];
      
      // Ensure the program submitted enough vertices..
      // Amusingly, this case not only crashes us, but the real GPU as well.
      // Should issue a diagnostic here.
      //
      // This check also catches the case where vertex RAM fills up,
      // leaving the last primitive unfinished.
      if(p.numVerts < 3 || p.numVerts < 4 && (p.drawMode & Prim::quadStrip)) {
        renderList->numPrims--;
      } else {
        break;
      }
    }
    primitive.size = 0;
    
    // Sort the new list
    auto count  = renderList->numPrims;
    auto &prims = renderList->prims;
    auto &verts = renderList->verts;
    
    renderList->firstAlpha = 0;
    
    for(unsigned n = 0; n < count; n++) {
      renderList->order[n] = n;
      
      // Is this per primitive or per polygon?
      prims[n].minY = 255;
      prims[n].maxY = 0;
      
      // Clip range is x/w, y/w := +/- 0x40000000
      for(unsigned k = 0; k < prims[n].numVerts; k++) {
        int32 y = 192 - verts[prims[n].firstVert + k].y/0x400000;
        
        prims[n].minY = min(prims[n].minY, y);
        prims[n].maxY = max(prims[n].maxY, y);
      }
      
      // Special case for wireframe... only edge pixels are drawn.
      if(prims[n].alpha == 0) {
        prims[n].alpha = 31;
        prims[n].wire = 1;
      }
      // Alpha is not the only thing that can send a primitive to the
      // translucent pile. Using textures with the alpha channel does it too.
      prims[n].blend = prims[n].alpha < 31
        || prims[n].texFormat == Prim::A3_I5
        || prims[n].texFormat == Prim::A5_I3;
      
      if(prims[n].blend == 0)
        renderList->firstAlpha++;
    }
    
    std::sort(renderList->order, renderList->order+count, [&](uint16 i0, uint16 i1) -> bool {
      auto &p0 = prims[i0], &p1 = prims[i1];
      
      if(p0.blend != p1.blend) {
        return p0.blend < p1.blend;  // separate solid + blend passes
      }
      if(p0.blend == false || renderList->alphaSort) {
        // Sort polygons towards the top of the screen first. They prolly
        //   do this to save time and avoid searching the entire list.
        if(p0.minY != p1.minY) return p0.minY < p1.minY;
        if(p0.maxY != p1.maxY) return p0.maxY < p1.maxY;
      }
      return i0 < i1;  // retain order in case of blending or ties
    });
  }
}



uint32 GPU::regRenderOptions() {
  return renderControl.texturing<<0 | renderControl.toonShading<<1
       | renderControl.alphaTest<<2 | renderControl.alphaBlend<<3
       | renderControl.edgeMode<<4 | renderControl.fogMode<<6
       | renderControl.fogLevel<<8 | renderControl.backImage<<14;
}

uint32 GPU::regRenderLoad() {
  return 48-2;
}

void GPU::regRenderOptions(uint32 data, uint32 mask) {
  if(mask & 0x00ff) {
    renderControl.texturing = data >> 0;
    renderControl.toonShading = data >> 1;
    renderControl.alphaTest = data >> 2;
    renderControl.alphaBlend = data >> 3;
    renderControl.edgeMode = data >> 4;
    renderControl.fogMode = data >> 6;
  }
  if(mask & 0x7f00) {
    renderControl.fogLevel = data >> 8;
    renderControl.backImage = data >> 14;
  }
}

void GPU::regRenderMinAlpha(uint32 data, uint32 mask) {
  minAlpha ^= (minAlpha ^ data) & mask;
}

void GPU::regRenderClearColor(uint32 data, uint32 mask) {
  if(mask & 0x00007fff) {
    backColor[0] = data>> 0 & 31;
    backColor[1] = data>> 5 & 31;
    backColor[2] = data>>10 & 31;
    backFog      = data>>15 & 1;
    
    for(unsigned n = 0; n < 3; n++)
      backColor[n] = 2*backColor[n] + (backColor[n] > 0);
  }
  if(mask & 0x001f0000) backAlpha = data>>16 & 31;
  if(mask & 0x3f000000) backId    = data>>24 & 63;
}

void GPU::regRenderClearCoord(uint32 data, uint32 mask) {
  if(mask & 0x00007fff) backZ = data>>0;
  if(mask & 0x00ff0000) backX = data>>16;
  if(mask & 0xff000000) backY = data>>24;
}

void GPU::regRenderFogColor(uint32 data, uint32 mask) {
  if(mask & 0x00007fff) {
    fogColor[0] = data>> 0 & 31;
    fogColor[1] = data>> 5 & 31;
    fogColor[2] = data>>10 & 31;
    for(unsigned n = 0; n < 3; n++)
      fogColor[n] = 2*fogColor[n] + (fogColor[n] > 0);
  }
  if(mask & 0x001f0000) {
    fogAlpha = data>>16 & 31;
  }
}

void GPU::regRenderFogCoord(uint32 data, uint32 mask) {
  if(mask & 0x00007fff) fogZ = data;
}

void GPU::regRenderFogTable(unsigned index, uint32 data, uint32 mask) {
  if(mask & 0x000000ff) fogTable[4*index+0] = data >> 0  & 0x7f;
  if(mask & 0x0000ff00) fogTable[4*index+1] = data >> 8  & 0x7f;
  if(mask & 0x00ff0000) fogTable[4*index+2] = data >> 16 & 0x7f;
  if(mask & 0xff000000) fogTable[4*index+3] = data >> 24 & 0x7f;
  
  fogTable[32] = fogTable[31];  // extra element for lerping
}

void GPU::regRenderEdgeTable(unsigned index, uint32 data, uint32 mask) {
  //uint6 r0 = 2*uint5(data>> 0), r1 = 2*uint5(data>>16);
  //uint6 g0 = 2*uint5(data>> 5), g1 = 2*uint5(data>>21);
  //uint6 b0 = 2*uint5(data>>10), b1 = 2*uint5(data>>26);
  //
  //if(r0) r0++;  if(r1) r1++;
  //if(g0) g0++;  if(g1) g1++;
  //if(b0) b0++;  if(b1) b1++;
  //
  //auto *edge = &edgeTable[2*index];
  //
  //if(mask & 0x0000ffff) edge[0] = b0<<12 | g0<<6 | r0<<0;
  //if(mask & 0xffff0000) edge[1] = b1<<12 | g1<<6 | r1<<0;
  if(mask & 0x0000ffff) unpackColor(edgeTable[2*index+0], data>>0);
  if(mask & 0xffff0000) unpackColor(edgeTable[2*index+1], data>>16);
}

void GPU::regRenderToonTable(unsigned index, uint32 data, uint32 mask) {
  auto &d0 = toonTable[0][2*index+0];
  auto &d1 = toonTable[0][2*index+1];
  auto &l0 = toonTable[1][2*index+0];
  auto &l1 = toonTable[1][2*index+1];
  
  if(mask & 0x0000ffff) unpackColor(d0, data>>0);
  if(mask & 0xffff0000) unpackColor(d1, data>>16);
  
  // Generate entries for lighten mode
  l0 = d0;          l1 = d1;
  l0[0] += 0x1000;  l1[0] += 0x1000;
  l0[1] += 0x1000;  l1[1] += 0x1000;
  l0[2] += 0x1000;  l1[2] += 0x1000;
}



void GPU::regGeomMaxPointDepth(uint32 data, uint32 mask) {
  
}

void GPU::regGeomStatus(uint32 data, uint32 mask) {
  data &= mask;
  if(data & 1<<15) {
    stackOverflow = false;
    projSP = 0;
    viewSP = 0;
  }
  
  if(mask & 0xff000000) {
    commandBufIrq = data>>30;
    if(commandBufIrq)
      arm9.interrupt.flags |= CPUCore::irqGeomBuf;
  }
}

uint32 GPU::regGeomStatus() {
  return boxResult<<1 | (viewSP % 32)<<8 | projSP<<13 | stackOverflow<<15
       | 1<<25 | 1<<26 | commandBufIrq<<30;
}

uint32 GPU::regGeomLoad() {
  return uploadList->numPrims<<0 | uploadList->numVerts<<16;
}

uint32 GPU::regGeomPosition(unsigned index) {
  return vertexResult[index];
}

uint32 GPU::regGeomNormal(unsigned index) {
  return uint16(int12(normalResult[index/2 + 0]))<<0
       | uint16(int12(normalResult[index/2 + 1]))<<16;
}

uint32 GPU::regClipMatrix(unsigned index) {
  uint2 col = index / 4;
  uint2 row = index % 4;
  
  return clipMatrix[4*row + col];
}

uint32 GPU::regLightMatrix(unsigned index) {
  uint2 col = index / 3;
  uint2 row = index % 3;
  
  return lightMatrix[4*row + col];
}



void GPU::sendGeomBuffered(uint32 data) {
  // On real hardware commands are buffered, and filling up this buffer
  // forces an indefinite waitstate until the buffer drains. A swapBuffers
  // command sits around 'til vblank, thereby throttling the game at 60fps.
  
  // Since we don't emulate that yet, this is the closest thing.
  while(sceneFinished) {
    arm9.step(16);
    if(arm7.clock < -255)
      co_switch(arm7.thread);
  }
  // Command bytes are packed 4 to a word..
  if(packedCommands) args[numArgs++] = data;
  else               packedCommands = data;
  
  // Take the first one and try to run it.
  // It succeeds when enough arguments are provided.
  //
  // It's important to run ALL possible commands here, since some of them
  // don't consume any arguments and the next invocation would erroneously
  // pass data to the wrong command.
  while(packedCommands && geomCommand(packedCommands)) {
    numArgs = 0;
    packedCommands >>= 8;
  }
  if(commandBufIrq)
    arm9.interrupt.flags |= CPUCore::irqGeomBuf;
}

void GPU::sendGeomImmediate(uint8 command, uint32 data) {
  while(sceneFinished) {
    arm9.step(16);
    if(arm7.clock < -255)
      co_switch(arm7.thread);
  }
  args[numArgs++] = data;
  if(geomCommand(command)) {
    numArgs = 0;
  }
  if(commandBufIrq)
    arm9.interrupt.flags |= CPUCore::irqGeomBuf;
}

bool GPU::geomCommand(uint8 command) {
  switch(command) {
  case 0x10: return gxMatrixMode();
  case 0x11: return gxMatrixPush();
  case 0x12: return gxMatrixPop();
  case 0x13: return gxMatrixStore();
  case 0x14: return gxMatrixRestore();
  case 0x15: return gxMatrixLoadIdentity();
  case 0x16: return gxMatrixLoad4x4();
  case 0x17: return gxMatrixLoad4x3();
  case 0x18: return gxMatrixMult4x4();
  case 0x19: return gxMatrixMult4x3();
  case 0x1a: return gxMatrixRotate();
  case 0x1b: return gxMatrixScale();
  case 0x1c: return gxMatrixTranslate();
  
  case 0x20: return gxColor();
  case 0x21: return gxNormal();
  case 0x22: return gxTexCoord();
  case 0x23: return gxVertex3i_16();
  case 0x24: return gxVertex3i_10();
  case 0x25: return gxVertex2i_XY();
  case 0x26: return gxVertex2i_XZ();
  case 0x27: return gxVertex2i_YZ();
  case 0x28: return gxVertex3i_Rel();
  case 0x29: return gxAttribute();
  case 0x2a: return gxTexImage();
  case 0x2b: return gxTexPalette();
  
  case 0x30: return gxLightDiffuseAmbient();
  case 0x31: return gxLightSpecularEmission();
  case 0x32: return gxLightDirection();
  case 0x33: return gxLightColor();
  case 0x34: return gxLightShininess();
  
  case 0x40: return gxBeginPrimitive();
  case 0x50: return gxSwapBuffers();
  case 0x60: return gxViewport();
  
  case 0x70: return gxCullTest();
  case 0x71: return gxPositionTest();
  case 0x72: return gxDirectionTest();
  }
  return true;
}


#include "math.cpp"
#include "commands.cpp"
#include "geometry.cpp"
#include "render.cpp"
#include "textures.cpp"

}
