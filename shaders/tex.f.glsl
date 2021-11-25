#version 410 core

// uniform inputs
uniform samplerCube texMap;

// varying inputs
in vec3 texOut;

// fragment outputs
out vec4 fragColor;

void main() {
    vec4 value = texture(texMap, texOut);
    fragColor = value;
}
