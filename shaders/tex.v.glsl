#version 410 core

// uniform inputs
uniform mat4 mvpMat;

// attribute inputs
layout(location = 0) in vec3 aPos;

// varying outputs
out vec3 texOut;

void main() {
    gl_Position = mvpMat * vec4(aPos, 1.0);

    texOut = aPos;
}
