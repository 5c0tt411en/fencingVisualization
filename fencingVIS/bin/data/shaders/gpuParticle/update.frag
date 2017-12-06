#version 120

#extension GL_ARB_texture_rectangle : enable
#extension GL_ARB_draw_buffers : enable


#pragma include "util.frag"
#pragma include "noise4D.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;
uniform float u_time;
uniform float u_timestep;
uniform float u_scale;
uniform vec3 u_emitterPos;
uniform vec3 u_prevEmitterPos;
uniform float ptSpXY;
uniform float ptSpZ;
//uniform float emitterPosNoise;

void main(void){
    vec2 st = gl_TexCoord[0].st;
   
    vec4 posAndAge = texture2DRect(u_posAndAgeTex,st);
    
    vec4 velAndMaxAge = texture2DRect(u_velAndMaxAgeTex,st);
    
    vec3 pos = posAndAge.xyz;
    vec3 vel = velAndMaxAge.xyz;
    
    float age = posAndAge.w;
    float maxAge = velAndMaxAge.w;

    age++;
    
    
    if(age >= maxAge){
        age = 0;
        maxAge = 50.0 + 250.0 * random(pos.xx);
        float theta = 2.0 * PI * random(pos.yy);
        float phi = PI * random(pos.zz);
        float r = 5.0 * random(pos.xy);
        vec3 startPos = u_prevEmitterPos + (u_emitterPos - u_prevEmitterPos) * random(pos.yz);
        pos = startPos + vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
        vel.xyz = vec3(normalize(startPos));
    }
    
   
    vel.x += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 0.1352 * u_time * u_timestep)) + ptSpXY;
    vel.y += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 1.2814 * u_time * u_timestep)) + ptSpXY;
    vel.z += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 2.5564 * u_time * u_timestep)) + ptSpZ;
    
    pos += vel;
    
    gl_FragData[0].rgba = vec4(pos, age);
    gl_FragData[1].rgba = vec4(vel, maxAge); 
}
