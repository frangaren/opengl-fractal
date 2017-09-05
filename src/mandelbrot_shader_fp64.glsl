#version 330 core
#extension GL_ARB_gpu_shader_fp64: enable

#define MAX_N 5000
#define M 15.0

uniform dvec2 resolution;
uniform dvec2 offset;
uniform double zoom;

in vec4 fragment_position;

layout(location = 0) out vec4 out_color;

dvec2 apply_transformations(dvec2 v);
int mandelbrot_iteration_count(dvec2 c);
double modulus(dvec2 v);
dvec2 complex_product(dvec2 a, dvec2 b);

void main() {
  dvec2 point = apply_transformations(fragment_position.xy);
  int n = mandelbrot_iteration_count(point);
  float c;
  if (n == MAX_N) {
    c = 0.0f;
  } else {
    c = float(n)/M;
  }
  out_color = vec4(c, 0.33f*c, 0.0f, 1.0f);
}

dvec2 apply_transformations(dvec2 v) {
  double ratio = resolution.x/resolution.y;
  return zoom * (v * dvec2(ratio, 1.0)) + offset;
}

int mandelbrot_iteration_count(dvec2 c) {
  dvec2 z = c;
  int n = 0;
  while (modulus(z) < 4.0f && n < MAX_N) {
    z = complex_product(z, z) + c;
    n++;
  }
  return n;
}

double modulus(dvec2 v) {
  dvec2 w = v*v;
  return w.x + w.y;
}

dvec2 complex_product(dvec2 a, dvec2 b) {
  return dvec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}
