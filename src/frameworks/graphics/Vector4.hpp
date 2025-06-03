// nothing special, just a struct with 4 floats
#pragma once

struct Vector4 {
  float t = 0;
  float d = 0;
  float l = 0;
  float r = 0;

  Vector4() = default;
  Vector4(float padding) : t(padding), d(padding), l(padding), r(padding) {}
  Vector4(float padx, float pady) : t(pady), d(pady), l(padx), r(padx) {}
  Vector4(float t_, float d_, float l_, float r_) : t(t_), d(d_), l(l_), r(r_) {}
};