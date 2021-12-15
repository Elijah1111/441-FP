#version 410 core

uniform sampler2D image;

in vec2 texCoordsGeo;

out vec4 fragColorOut;

void main() {
    fragColorOut = texture(image, texCoordsGeo);
}
