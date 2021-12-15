#version 410 core

uniform mat4 mvMtx;

layout(location = 0) in vec3 vPos;

void main() {
    gl_Position = mvMtx * vec4(vPos, 1.0);
}