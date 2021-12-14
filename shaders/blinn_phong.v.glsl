#version 410 core


uniform mat4 mvpMtx;
uniform mat4 modelMtx;
uniform mat4 viewMtx;
uniform mat4 projMtx;
uniform mat3 normMtx;
uniform vec3 viewPos;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
layout(location = 1) in vec3 vNorm;      // the position of this specific vertex in object space
layout(location = 2) in vec2 vTexCoords;

// vertex outputs;
out VertexOut {
    vec3 fNorm;
    vec3 fPos;
} vertexOut;

void main() {
    gl_Position = mvpMtx * vec4(vPos, 1.0);
    
    vertexOut.fPos = (modelMtx * vec4(vPos, 1.0)).xyz;
    vertexOut.fNorm = normalize(normMtx * vNorm);
}
