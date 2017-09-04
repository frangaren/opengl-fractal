#version 330 core

#define MAX_N 5000

in vec4 fragment_position;

layout(location = 0) out vec4 out_color;

int mandelbrot_iteration_count(vec2 c);
float modulus(vec2 v);
vec2 complex_product(vec2 a, vec2 b);

void main() {
  int n = mandelbrot_iteration_count(fragment_position.xy * 3.0f);
  float c;
  if (n == MAX_N) {
    c = 0.0f;
  } else {
    c = float(n)/15.0f;
  }
  out_color = vec4(c, 0.33f*c, 0.0f, 1.0f);
}

int mandelbrot_iteration_count(vec2 c) {
  vec2 z = c;
  int n = 0;
  while (modulus(z) < 4.0f && n < MAX_N) {
    z = complex_product(z, z) + c;
    n++;
  }
  return n;
}

float modulus(vec2 v) {
  vec2 w = v*v;
  return w.x + w.y;
}

vec2 complex_product(vec2 a, vec2 b) {
  return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}
