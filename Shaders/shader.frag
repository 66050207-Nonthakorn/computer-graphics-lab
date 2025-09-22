#version 330 core

in vec4 v_color;
in vec2 tex_coord;
out vec4 colour;

uniform sampler2D texture1;

void main()
{
    colour = texture(texture1, tex_coord);
}