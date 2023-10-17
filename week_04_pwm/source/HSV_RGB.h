/*
 * HSV_RGB.hpp
 *
 *  Created on: Jan 25, 2021
 *      Author: danie
 */

#ifndef HSV_RGB_H_
#define HSV_RGB_H_
#include <math.h>
#include <stdint.h>

typedef struct RgbColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RgbColor;

RgbColor HSVtoRGB(int H, int S, int V) {

  float s = S / 100.;
  float v = V / 100.;
  float C = s * v;
  float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
  float m = v - C;
  float r, g, b;
  if (H >= 0 && H < 60) {
    r = C, g = X, b = 0;
  } else if (H >= 60 && H < 120) {
    r = X, g = C, b = 0;
  } else if (H >= 120 && H < 180) {
    r = 0, g = C, b = X;
  } else if (H >= 180 && H < 240) {
    r = 0, g = X, b = C;
  } else if (H >= 240 && H < 300) {
    r = X, g = 0, b = C;
  } else {
    r = C, g = 0, b = X;
  }
  RgbColor ret;
  ret.r = (r + m) * 255;
  ret.g = (g + m) * 255;
  ret.b = (b + m) * 255;
  return ret;
}

#endif /* HSV_RGB_H_ */
