/*
  libinterp : version 0.01 ~byuu (2007-02-03)
*/

#ifndef __LIBINTERP
#define __LIBINTERP

static inline
double interpolate_point(
  double mu,
  double y0, double y1
) {
  return mu < 0.5 ? y0 : y1;
}

static inline
double interpolate_linear(
  double mu,
  double y0, double y1
) {
  return y0 + mu * (y1 - y0);
}

static inline
double interpolate_cosine(
  double mu,
  double y0, double y1
) {
  mu = (1.0 - cos(mu * 3.14159265)) / 2.0;
  return y0 + mu * (y1 - y0);
}

static inline
double interpolate_cubic(
  double mu,
  double y0, double y1, double y2, double y3
) {
double a0 = y3 - y2 - y0 + y1;
double a1 = y0 - y1 - a0;
double a2 = y2 - y0;
double a3 = y1;
  return a0 * mu * mu * mu +
         a1 * mu * mu +
         a2 * mu +
         a3;
}

/*
  tension: (0 = normal, -1 = low, +1 = high)
  bias: (0 = even, -n = left, +n = right)
*/
static inline
double interpolate_hermite(
  double mu, double tension, double bias,
  double y0, double y1, double y2, double y3
) {
double mu0 = ( (y1 - y0) * (1.0 + bias) * (1.0 - tension) / 2.0 ) +
             ( (y2 - y1) * (1.0 - bias) * (1.0 - tension) / 2.0 );
double mu1 = ( (y2 - y1) * (1.0 + bias) * (1.0 - tension) / 2.0 ) +
             ( (y3 - y2) * (1.0 - bias) * (1.0 - tension) / 2.0 );
double mu2 = mu * mu;
double mu3 = mu * mu * mu;
double a0  =  2.0 * mu3 - 3.0 * mu2 + 1.0;
double a1  =        mu3 - 2.0 * mu2 + mu;
double a2  =        mu3 -       mu2;
double a3  = -2.0 * mu3 + 3.0 * mu2;
  return a0 * y1 +
         a1 * mu0 +
         a2 * mu1 +
         a3 * y2;
}

#endif
