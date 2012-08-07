  
  struct Vector {
    int32 v[4];
    
    Vector() { }
    
    Vector(const std::initializer_list<int32>& elems) {
      unsigned n = 0;
      for(auto i : elems) {
        if(n == 4) break;
        v[n++] = i;
      }
    }
    inline int32& operator[](unsigned i) { return v[i]; }
    inline const int32& operator[](unsigned i) const { return v[i]; }
    inline operator int32*() { return v; }
  };
  
  struct Matrix {
    int32 m[4*4];
    
    Matrix() { }
    
    Matrix(const std::initializer_list<int32>& elems) {
      unsigned n = 0;
      for(auto i: elems)
        if(n < 4*4) m[n++] = i;
    }
    
    inline int32& operator[](unsigned i) { return m[i]; }
    inline int32& operator()(unsigned i, unsigned j) { return m[4*i + j]; }
    inline int32* operator()(unsigned i) { return &m[4*i]; }
    
    inline const int32& operator[](unsigned i) const { return m[i]; }
    inline const int32& operator()(unsigned i, unsigned j) const { return m[4*i + j]; }
    inline const int32* operator()(unsigned i) const { return &m[4*i]; }
    
    void transpose();
  };
  
  struct ClipSpaceVertex {
    Vector position;
    Vector texCoord;
    Vector color;
  };
  
  static void transform(Matrix& m, Matrix t);
  static void transform(const Matrix& t, int32* v);
  
  static int32 dot(const int32* a, const int32* b);
  static int facing(const ClipSpaceVertex &v0, const ClipSpaceVertex &v1, const ClipSpaceVertex &v2);
  static unsigned outcode(const ClipSpaceVertex &v);
  static ClipSpaceVertex clipEdge(const ClipSpaceVertex &b, const ClipSpaceVertex &a, unsigned axis, int side);
