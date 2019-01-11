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
uniform vec3 colFocus;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(0, 10, 0);  // en SCA


out vec4 VertexSCO;
out vec3 normalSCO;
//out vec4 posFocusSCO;
//out vec3 fcolor; No fa falta ya que el color es calcula a fong
out vec3 matamb2;
out vec3 matdiff2;
out vec3 matspec2;
out float matshin2;
out vec3 colFocus2;
out vec3 llumAmbient2;

void main()
{
     matamb2 = matamb;
     matdiff2 = matdiff;
     matspec2 = matspec;
     matshin2 = matshin;
     colFocus2 = colFocus;
     llumAmbient2 = llumAmbient;
     VertexSCO = view* TG * vec4(vertex,1.0);
     normalSCO = normalize(inverse(transpose(mat3(view*TG))) * normal);
     //fcolor = Phong(normalSCO,L,VertexSCO); es calcula a phong
     gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
