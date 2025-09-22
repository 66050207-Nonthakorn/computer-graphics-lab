#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 a_tex_coord;

out vec4 v_color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    v_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    tex_coord = a_tex_coord;
}