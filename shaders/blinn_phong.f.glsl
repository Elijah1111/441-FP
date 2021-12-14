#version 410 core
precision mediump float;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float angle;
    float falloff;
    float constant;
    float linear;
    float quadratic; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool visible;
};

uniform Material material;
uniform Light skyLight;
uniform Light pointLight;
uniform Light spotLight;

uniform vec3 viewPos;

in VertexOut {
    vec3 fNorm;
    vec3 fPos;
} vertexOut;

out vec4 fColor;

vec3 calcDirLight(Light light, vec3 normalVec, vec3 eyeVec) {
    //calculate light direction vector
    vec3 lightDir = -normalize(light.direction);
    
    //calculate ambient color
    vec3 ambient  = light.ambient * material.ambient;

    //calculate diffuse color
    float diff = max(dot(lightDir, normalVec), 0.0);
    vec3 diffuse  = light.diffuse * material.diffuse * diff;

    //calculate specular color
    vec3 halfwayDir = normalize(lightDir + eyeVec);
    float spec = pow(max(dot(eyeVec, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 normalVec, vec3 posVec, vec3 eyeVec) {
    //calculate light direction vector
    vec3 lightDir = normalize(light.position - posVec);

    //calculate attenuation
    float distance    = length(light.position - posVec);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //calculate ambient color
    vec3 ambient  = light.ambient * material.ambient * attenuation;
    
    //calculate diffuse color
    float diff = max(dot(lightDir, normalVec), 0.0);
    vec3 diffuse  = light.diffuse * material.diffuse * diff * attenuation;

    //calculate specular color
    vec3 halfwayDir = normalize(lightDir + eyeVec);
    float spec = pow(max(dot(eyeVec, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec * attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Light light, vec3 normalVec, vec3 posVec, vec3 eyeVec) {
    //spotlight specific variable calculations
    vec3 lightDir = normalize(light.position - posVec);
    float theta = dot(normalize(lightDir), -light.direction);
    float intensity = pow(max(theta - light.angle, 0), light.falloff);

    //calculate attenuation
    float distance = length(light.position - posVec);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //calculate ambient color
    vec3 ambient  = light.ambient * material.ambient * attenuation;

    //calculate diffuse color
    float diff = max(dot(lightDir, normalVec), 0.0);
    vec3 diffuse  = light.diffuse * material.diffuse * diff * intensity * attenuation;

    //calculate specular color
    vec3 halfwayDir = normalize(lightDir + eyeVec);
    float spec = pow(max(dot(eyeVec, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec * intensity * attenuation;

    return (ambient + diffuse + specular);
}

void main() {
    vec3 eyeVec = normalize(viewPos - vertexOut.fPos);

    vec3 color = vec3(0.0);

    if(skyLight.visible) {
        color += calcDirLight(skyLight, vertexOut.fNorm, eyeVec);
    }
    if(pointLight.visible) {
        color += calcPointLight(pointLight, vertexOut.fNorm, vertexOut.fPos, eyeVec);
    }
    if(spotLight.visible) {
        color += calcSpotLight(spotLight, vertexOut.fNorm, vertexOut.fPos, eyeVec);
    }
    
    fColor = vec4(color, 1.0);
}