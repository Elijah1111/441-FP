#version 410 core

// uniform inputs
uniform mat4 model;
uniform mat4 mvpMatrix;

//Light pos, view pos
uniform vec3 pPos;
uniform vec3 sPos;
uniform vec3 vPos;


// attribute inputs
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
//Tangent for tangent space
layout(location = 3) in vec3 aTangent;

// varying outputs
out vec3 fragPos;
out vec2 texCoord;
//lPos, vPos, fPos in tangent space.
out vec3 tPPos;
out vec3 tSPos;
out vec3 tVPos;
out vec3 tFPos;
out vec3 tDPos;


void main() {
    //Calculate position of fragment
    fragPos = vec3(model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    
    //Calculate TNB space
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N, T);

    //Convert lPos, vPos, fPos to tangent space
    mat3 TBN = transpose(mat3(T, B, N));
    tPPos = TBN * pPos;
    tVPos = TBN * vPos;
    tFPos = TBN * fragPos;
    tSPos = TBN * sPos;
    tDPos = TBN * vec3(-1.0f, 0.0f, 0.0f);
    
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
}
