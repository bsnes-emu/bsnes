
// Flips a 4x4 matrix along the diagonal:
//   |a b c d|    |a e i m|
//   |e f g h| -> |b f j n|
//   |i j k l|    |c g k o|
//   |m n o p|    |d h l p|
void GPU::Matrix::transpose() {
  auto &m = *this;
  std::swap(m(0,1), m(1,0));
  std::swap(m(0,2), m(2,0));
  std::swap(m(0,3), m(3,0));
  std::swap(m(1,2), m(2,1));
  std::swap(m(1,3), m(3,1));
  std::swap(m(2,3), m(3,2));
}

// Product of two matrices: M := M * T.
void GPU::transform(Matrix& m, Matrix t) {
  t.transpose();
  for(unsigned i = 0; i < 4; i++)
    transform(t, m(i));
}

// Product of 4x4 matrix and 4D vector: v := T * v.
void GPU::transform(const Matrix& t, int32* v) {
  int32 vector[] = { v[0], v[1], v[2], v[3] };
  for(unsigned i = 0; i < 4; i++)
    v[i] = dot(t(i), vector);
}

// Dot product of two 4D vectors: s := a . b.
int32 GPU::dot(const int32* a, const int32* b) {
  return ( (int64) a[0] * b[0]
         + (int64) a[1] * b[1]
         + (int64) a[2] * b[2]
         + (int64) a[3] * b[3] )
       / 0x1000;
}

// Given a 4D homogenous coordinate, returns a mask telling whether
// it's out of bounds. One bit per side of the clipping volume.
unsigned GPU::outcode(const ClipSpaceVertex &v) {
  const int32 &x = v.position[0], &y = v.position[1];
  const int32 &z = v.position[2], &w = v.position[3];
  
  unsigned code = 0;
  if(x < -w) code += 1;  if(x > +w) code += 2;  // left, right
  if(y < -w) code += 4;  if(y > +w) code += 8;  // bottom, top
  if(z < -w) code += 16; if(z > +w) code += 32; // near, far
  return code;
}

// Looks at three triangle vertices and returns a signed value:
//   negative (back face), zero (edge on), or positive (front face).
//
// AKA sign of Z from the cross product (v1-v0) x (v2-v1) in screen coordinates:
//   Z = (v1-v0).x (v2-v1).y - (v1-v0).y (v2-v1).x
int GPU::facing(const ClipSpaceVertex &v0, const ClipSpaceVertex &v1, const ClipSpaceVertex &v2) {
  // These are in homogenous space, so.. that's more like:
  //   Z = a + b - c, where
  //     a = (v0.x v1.y - v0.y v1.x) / (v0.w v1.w)
  //     b = (v1.x v2.y - v1.y v2.x) / (v1.w v2.w)
  //     c = (v0.x v2.y - v0.y v2.x) / (v0.w v2.w)
  // 
  // Since only the sign matters,
  //   S = (a+b-c <=> 0) = (a+b <=> c)
  //
  // and knowing w is positive within the clipping volume,
  //   p  = v0.w v1.w v2.w
  //   a' = a*p, b' = b*p, c' = c*p
  //
  // that makes the divisions disappear.
  //                    x     y               y     x        w 
  int64 a = ((int64) v0.position[0]*v1.position[1] - (int64) v0.position[1]*v1.position[0]) / 0x1000 * v2.position[3];
  int64 b = ((int64) v1.position[0]*v2.position[1] - (int64) v1.position[1]*v2.position[0]) / 0x1000 * v0.position[3];
  int64 c = ((int64) v0.position[0]*v2.position[1] - (int64) v0.position[1]*v2.position[0]) / 0x1000 * v1.position[3];
  
  int64 r = a + b - c;
  return r<0? -1 : r>0? +1 : 0;
}

// a is inside the clipping plane
// b is outside, and will be clipped to the
//   intersection between AB and the clip plane.
// axis controls which of X,Y,Z is tested.
// side affects the sign of the W comparand.
GPU::ClipSpaceVertex GPU::clipEdge(const ClipSpaceVertex &a, const ClipSpaceVertex &b, unsigned axis, int side) {
  // Normally, to find the intersection we would attempt to find the fraction
  // of the edge that remains unclipped:
  //   t = (1 - ax) / (bx - ax)
  //
  // The problem is that we haven't divided by W. We end up with this mess:
  //   t = (1 - ax/aw) / (bx/bw - ax/aw)
  //
  // Not very helpful. Besides which we still need W for perspective-correct
  // texturing.
  //
  // In homogenous space, the clipping volume is defined by:
  //   -w < {x,y,z} < +w
  //
  // So we expect the clipped vertex c to have:
  //   cx = +/-cw     ; cx,cy,cz depending on the plane
  //
  // And for each {x,y,z,w}:
  //   cx = ax + t*(bx - ax)
  //
  // Expanding both sides of the first equation:
  //   ax + t*(bx - ax) = aw + t*(bw - aw)
  //   t*(bx - ax) - t*(bw - aw) = aw - ax
  //   t = (aw - ax) / ((bx - ax) - (bw - aw))
  //
  // We end up with these two equations:
  //   t = (+aw - ax) / ((+aw - ax) - (+bw - bx))  ; cx=+cw
  //   t = (-aw - ax) / ((-aw - ax) - (-bw - bx))  ; cx=-cw
  //
  // or more simply:
  //   t = (ax - +aw) / ((ax - +aw) - (bx - +bw))  ; cx=+cw
  //   t = (ax - -aw) / ((ax - -aw) - (bx - -bw))  ; cx=-cw
  //
  //   t = (ax +/- aw) / ((ax +/- aw) - (bx +/- bw))
  //
  // So clipping requires one division per edge that crosses a clipping plane.
  // Quite pleasant, actually.
  ClipSpaceVertex c;
  int32 ax_aw = a.position[axis] - side*a.position[3];
  int32 bx_bw = b.position[axis] - side*b.position[3];
  
  if(ax_aw - bx_bw) {
    int64 t = 0x40000000ll * ax_aw / (ax_aw - bx_bw);
    
    // t should be between 0 and 1. Rather than being a linear function of X/Y/Z
    // it also includes W. But at this point it's just an interpolation.
    for(unsigned n = 0; n < 4; n++) {
      c.position[n] = a.position[n] + int64(b.position[n] - a.position[n]) * t/0x40000000;
      c.texCoord[n] = a.texCoord[n] + int64(b.texCoord[n] - a.texCoord[n]) * t/0x40000000;
      c.color[n]    = a.color[n]    + int64(b.color[n]    - a.color[n]   ) * t/0x40000000;
    }
    // Ensure vertex is exactly on the clipping plane,
    // so we don't try and clip it again.
    c.position[axis] = side*c.position[3];
  }
  return c;
}
