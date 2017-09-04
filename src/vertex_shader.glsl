#version 330 core

layout(location = 0) in vec2 vertex_position;

out vec4 fragment_position;

void main() {
  gl_Position = fragment_position = vec4(vertex_position, 0.0, 1.0);
}
