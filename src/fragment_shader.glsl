#version 330 core

in vec4 fragment_position;

layout(location = 0) out vec4 out_color;

void main() {
  out_color = (fragment_position + vec4(1.0f, 1.0f, 0.0f, 1.0f))/2.0f;
}
