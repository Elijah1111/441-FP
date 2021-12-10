#version 410 core

// uniform inputs
uniform sampler2D texMap;
uniform sampler2D norMap;

uniform vec3 lPos;
uniform vec3 vPos;

// varying inputs
in vec3 fragPos;
in vec2 texCoord;
in vec3 tLPos;
in vec3 tVPos;
in vec3 tFPos;

// fragment outputs
out vec4 fragColor;

void main() {
    vec3 normal = texture(norMap, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 color = texture(texMap, texCoord).rgb;

    vec3 ambient = 0.3 * color;

    vec3 lDir = normalize(tLPos - tFPos);
    float diff = max(dot(lDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 vDir = normalize(tVPos - tFPos);
    vec3 hDir = normalize(lDir + vDir);
    float spec = max(dot(normal, hDir), 0.0);

    vec3 specular = vec3(0.2) * spec;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
    //fragColor = vec4(ambient, 1.0);
}
