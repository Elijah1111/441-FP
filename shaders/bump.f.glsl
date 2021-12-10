#version 410 core

// uniform inputs
//Texture Color Map
uniform sampler2D texMap;
//Texture Normal Map
uniform sampler2D norMap;

//Light Position
uniform vec3 lPos;
//View Position
uniform vec3 vPos;

// varying inputs
//Position in the frament
in vec3 fragPos;
//Texture Coordinate
in vec2 texCoord;
//lPos in tangent space
in vec3 tLPos;
//vPos in Tangent space
in vec3 tVPos;
//fPos in tangent space
in vec3 tFPos;

// fragment outputs
out vec4 fragColor;

void main() {
    //Get normal from map
    vec3 normal = texture(norMap, texCoord).rgb;
    //Convert to tangent space
    normal = normalize(normal * 2.0 - 1.0);

    //Get the color for the fragment
    vec3 color = texture(texMap, texCoord).rgb;

    //Basic Blinn-Phong model
    vec3 ambient = 0.1 * color;

    vec3 lDir = normalize(tLPos - tFPos);
    float diff = max(dot(lDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 vDir = normalize(tVPos - tFPos);
    vec3 hDir = normalize(lDir + vDir);
    float spec = pow(max(dot(normal, hDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
