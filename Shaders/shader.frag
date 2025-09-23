#version 330 core

in vec4 v_color;
in vec2 tex_coord;
out vec4 colour;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 tex1 = texture(texture1, tex_coord);
    vec4 tex2 = texture(texture2, tex_coord);

    colour = mix(tex1, tex2, tex2.a) * v_color;
}
