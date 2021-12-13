#version 410 core

// uniform inputs
//Texture Color Map
uniform sampler2D texMap;
//Texture Normal Map
uniform sampler2D norMap;

//Light Position
uniform vec3 pPos;
uniform vec3 sPos;
//View Position
uniform vec3 vPos;

uniform vec3 pCol;

uniform vec3 sCol;
uniform vec3 sDir;

// varying inputs
//Position in the frament
in vec3 fragPos;
//Texture Coordinate
in vec2 texCoord;
//fPos in tangent space
in vec3 tFPos;
//vPos in Tangent space
in vec3 tVPos;
//pPos in tangent space
in vec3 tPPos;
in vec3 tSPos;
in vec3 tDPos;

// fragment outputs
out vec4 fragColor;

vec3 bpHelper(vec3 ld, vec3 lc, vec3 normal, float inten, float d, float b, float c) {
    vec3 ambient = lc * 2 * inten;

    float diff = max(dot(ld, normal), 0.0);
    vec3 diffuce = diff*lc;

    vec3 vDir = normalize(tVPos - tFPos);
    vec3 hDir = normalize(ld + vDir);

    float spec = pow(max(dot(normal, hDir), 0.0), 32.0);

    vec3 specular = inten * spec * lc;

    return (diffuce + ambient + specular) * (1 / (1 + b * d + c * c * d));
}

void main() {
    //Get normal from map
    vec3 normal = texture(norMap, texCoord).rgb;
    //Convert to tangent space
    normal = normalize(normal * 2.0 - 1.0);

    //Get the color for the fragment
    vec3 color = texture(texMap, texCoord).rgb;

    //Basic Blinn-Phong model
    /*vec3 ambient = 0.1 * color;

    vec3 lDir = normalize(tLPos - tFPos);
    float diff = max(dot(lDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 vDir = normalize(tVPos - tFPos);
    vec3 hDir = normalize(lDir + vDir);
    float spec = pow(max(dot(normal, hDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec * color;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
    */

    //Manually Setting direction light dir and color
    vec3 dirCol = vec3(1.0, 1.0, 1.0f);

    vec3 dirBlinnPhong = bpHelper(normalize(tDPos), dirCol, normal, 0.2, 0, 0, 0);
    vec3 pBlinnPhong = bpHelper(normalize(tPPos - tFPos), pCol, normal, 0.5, length(tPPos - tFPos), 0.28, 0.02);

    fragColor = vec4((dirBlinnPhong + pBlinnPhong )* color, 1.0);
    //fragColor = vec4((pBlinnPhong )* color, 1.0);
}
