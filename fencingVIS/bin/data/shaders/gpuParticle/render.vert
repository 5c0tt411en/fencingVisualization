#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

#pragma include "util.frag"

//uniform vec2 screen;
uniform sampler2DRect u_posAndAgeTex;
uniform float pointSize;
uniform float resolution;

void main() {
    vec2 st = gl_MultiTexCoord0.xy;
    vec3 pos = texture2DRect( u_posAndAgeTex, st ).xyz;
    gl_PointSize = pointSize;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
    gl_FrontColor = gl_Color;
}
