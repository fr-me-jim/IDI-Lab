#version 330 core

//in vec3 fcolor; No fa falta ya que el color es calcula a fong
out vec4 FragColor;
in vec3 colFocus2;
in vec3 llumAmbient2;
in vec4 VertexSCO;
in vec3 normalSCO;
//in vec4 posFocusSCO;
in vec3 L;
in vec3 matamb2;
in vec3 matdiff2;
in vec3 matspec2;
in float matshin2;

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient2 * matamb2;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus2 * matdiff2 * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin2 == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin2);
    return (colRes + matspec2 * colFocus2 * shine); 
}

void main()
{	
	FragColor = vec4(Phong(normalSCO,L,VertexSCO),1);	
}
