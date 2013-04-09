#ifndef NALL_MATRIX_HPP
#define NALL_MATRIX_HPP

namespace nall {

namespace Matrix {

template<typename T>
inline void Multiply(T *output, const T *xdata, unsigned xrows, unsigned xcols, const T *ydata, unsigned yrows, unsigned ycols) {
  if(xcols != yrows) return;

  for(unsigned y = 0; y < xrows; y++) {
    for(unsigned x = 0; x < ycols; x++) {
      T sum = 0;
      for(unsigned z = 0; z < xcols; z++) {
        sum += xdata[y * xcols + z] * ydata[z * ycols + x];
      }
      *output++ = sum;
    }
  }
}

template<typename T>
inline vector<T> Multiply(const T *xdata, unsigned xrows, unsigned xcols, const T *ydata, unsigned yrows, unsigned ycols) {
  vector<T> output;
  output.resize(xrows * ycols);
  Multiply(output.data(), xdata, xrows, xcols, ydata, yrows, ycols);
  return output;
}

}

}

#endif
