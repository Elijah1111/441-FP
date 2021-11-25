#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 model;

uniform mat3 normMat;

uniform vec3 lDir;                      //Directional Light Dir
uniform vec3 lCol;                      //Directional Light Color

uniform vec3 pointPos;                  //PointLight
uniform vec3 pointCol;                  //PointLight Col

uniform vec3 spotPos;                   //Spotlight position
uniform vec3 spotCol;                   //Spotlight color
uniform vec3 spotDir;                   //Spotlight Direction

uniform vec3 materialColor;             // the material color for our vertex (& whole object)

uniform vec3 viewPos;//where we are viewing from

uniform int dirBool;                    //Do we render the directional light?

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space

in vec3 vNorm; //should this be a layout?


// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

//PhongLigh helper
// Light Direction, Light Color, world pos, intensity, lenght of direction, attenuation variables
vec3 phongLight(vec3 ld, vec3 lc, vec3 wPos, float inten, float d, float b, float c) {
    float mag = length(ld);
    vec3 ldNorm = vec3(-ld[0]/mag, -ld[1]/mag, -ld[2]/mag);
    vec3 norm = normalize(normMat * vNorm);

    vec3 viewDir = normalize(viewPos - wPos);

    vec3 refl = reflect(-ld, norm);
    vec3 spec = inten* max(dot(viewDir, refl), 0) * lc;
    vec3 ambi = lc * 2*inten;
    vec3 diffu = lc * max(dot(ldNorm, norm), 0);

    return (diffu + ambi + spec) * (1 / (1 + b * d + c * c * d));
}

void main() {
    	// transform & output the vertex in clip space
    	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	    vec4 tmp = model * vec4(vPos,1.0);
	    vec3 pos = tmp.xyz;

        //If the coordinate is in the spotlight cone, use spotlight too.
        vec3 spotPhong;
        vec3 spotLDir = pos - spotPos;
        float theta = dot(spotDir, normalize(spotLDir));
	    if(abs(theta) < 0.91){
            spotPhong = vec3(0,0,0);
	    
        } else{
            spotPhong = phongLight(spotLDir, spotCol,pos, 0.5, 0, 0, 0);
	    }
	
        vec3 pointDir = pos - pointPos;
        vec3 dirPhong = phongLight(lDir, lCol, pos, 0.05, 0, 0, 0);
        vec3 pointPhong = phongLight(normalize(pointDir), pointCol, pos, 0.4, length(pointDir), .28, 0.02);
        if(dirBool == 1){
    	    color = (dirPhong + pointPhong + spotPhong) * materialColor;
        } 
        else {
    	    color = (pointPhong + spotPhong) * materialColor;
        }
}
