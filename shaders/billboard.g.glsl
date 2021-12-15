#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projMtx;
uniform float size;

out vec2 texCoordsGeo;

void main() {
    // DONE #C output point in clip space
    gl_Position = projMtx * (gl_in[0].gl_Position + vec4(-size,-size,size,size));
    texCoordsGeo = vec2(0,0);
    // DONE #D emit
    EmitVertex();
    
    // DONE #F make another point
    gl_Position = projMtx * (gl_in[0].gl_Position + vec4(-size,size,size,size));
    texCoordsGeo = vec2(0,1);
    EmitVertex();

    // DONE #G make another point
    gl_Position = projMtx * (gl_in[0].gl_Position + vec4(size,-size,size,size));
    texCoordsGeo = vec2(1,0);
    EmitVertex();

    // DONE #H make another point
    gl_Position = projMtx * (gl_in[0].gl_Position + vec4(size,size,size,size));
    texCoordsGeo = vec2(1,1);
    EmitVertex();

    // DONE #E and end!
    EndPrimitive();
}