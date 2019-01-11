#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform mat3 nMat;


// Valors per als components que necessitem dels focus de llum
//vec3 colFocus = vec3(0.8, 0.8, 0.8);
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

//out vec3 fcolor;
out vec3 NormSCO;
out vec4 vertexSCO;

out vec3 matamb2;
out vec3 matdiff2;
out vec3 matspec2;
out float matshin2;

void main()
{	

    matamb2 = matamb;
    matdiff2 = matdiff;
    matspec2 = matspec;
    matshin2 = matshin;
    NormSCO = normalize(nMat * normal);
    vertexSCO = view * TG * vec4(vertex, 1.0);
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
