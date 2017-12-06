#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect sparkTex;

void main() {
    gl_FragColor = gl_Color;
//    vec2 st = gl_TexCoord[0].st;
//    gl_FragColor = texture2DRect(sparkTex, st);
}
