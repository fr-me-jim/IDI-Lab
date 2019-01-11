                                               /*COSAS CHACHIS DE CÁMARA*/

void MyGLWidget::ini_camera(){
    projectTransform();
    OBS = glm::vec3(x,y,z);
    VRP = glm::vec3(x,y,z);
    UP = glm::vec3(x,y,z); // GIRAR LA CAMARA DE LADO SOBRE SI MISMA Z NO VALE UNA MIERDA
    viewTransform();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (perspectiva){
    Proj = glm::perspective(FOV, AR, ZNEAR, ZFAR);
    VRP = glm::vec3(x,y,z);
  }else
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, radiEsc, 3.0f*radiEsc);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

/*VARS(LOS NÚMEROS SON EJEMPLOS)
radiEsc = sqrt(5);
FOV = float(M_PI/3.0);
AR = 1.0f;
ZNEAR = radiEsc;
ZFAR = 4.0f*radiEsc;*/


//////////////////////////////////////////////////////////////////////////////////////////////////
void MyGLWidget::viewTransform ()
{
  //glm::mat4 View;  // Matriu de posició i orientació
  glm::mat4 View = glm::lookAt (OBS,VRP, UP);
  //View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

//LOS ROTATE AFECTAN AL GIRO DE LA ESCENA CON RATÓN








                                       /*COSAS CHACHIS DE TECLADO-RATÓN*/
  /*LAS FUNCIONES DE CADA CASE DEL SWITCH SON EJEMPLOS, CAMBIO DE ÓPTICA PARA TECLADO Y GIRAR CON EULER PARA RATÓN*/


//////////////////////////////////////////////////////////////////////////////////////////////////

//VARS(LOS NÚMEROS SON EJEMPLOS)
xClick = yClick = 0;
angleY = 0.0;
angleX = 0.0;
DoingInteractive = NONE;

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica
      perspectiva = !perspectiva;
      projectTransform ();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;
    ini_camera ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}




                                        /*HACER CAJITAS COMO EN E.S <3*/

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)

      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 2.0/(maxy-miny);  // - > cambiando el 2 cmabiamos el tamaño del modelo entero
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
  centrePatr2[0] = 5; centrePatr2[1] = 5; centrePatr2[2] = 5;
}




                                        /*IN-OUT VARS Y UNIFORMS*/

 /*var_main = glGetAttribLocation (program->programId(), "var_vertex_shader"); //LINKAR VARIABLES
 transLoc = glGetUniformLocation (program->programId(), "TG");  -> els noms entre "" han de ser els mateixos
 projLoc = glGetUniformLocation (program->programId(), "proj");    que al vertex shader.
 viewLoc = glGetUniformLocation (program->programId(), "view");

 Se declaran los uniforms en el VS, y el orden de trabajo es:
     1.Se linkan las invars y los uniforms del VS en el MYGlWidget.cpp
     2.Actualizas los valores de 1.(se pasan al VS al estar linkados)
     3.Usas los valores de 2. necesarios para el VS (gl_Position = proj * view * TG * vec4 (vertex, 1.0);),
            el resto se pasan a las outvars del VS y se pasan al FS para trabajar con ellos



                                          /*COSAS CHACHIS DE TG*/

 glm::mat4 TG(1.f);  // Matriu de transformació
 TG = glm::translate(TG,glm::vec3(x,y,z));
 TG = glm::scale(TG, glm::vec3(escala, escala, escala));
 TG = glm::rotate(TG,(float)radiants, glm::vec3(x,y,z));
 glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);


//SIEMPRE QUE HAYA QUE ROTAR SOBRE EL PORPIO VAO ASEGURARSE DE CENTRAR CON TG = glm::translate(TG,-centrePatr); EN modelTransform();
                                          /*COSAS CHACHIS DE QT*/

 Linkar función en qt entre elementos(tanto el slot de entrada como el de salida tienen que recibir/pasar los mismos types)
 Definir función en los public slots del .hh
 Iniciar función con makeCurrent();(si no no chuta) y finalizarla con update();(si no hay que clicar la pantalla para actualizar)





                                        /* ESCALAR */

 void MyGLWidget::modelTransformPatricio ()
 {
   glm::mat4 TG(1.f);  // Matriu de transformació
   TG = glm::translate(TG, glm::vec3(-4, 0, -4));
   TG = glm::scale(TG, glm::vec3(1.5*escalaPat, 1.5*escalaPat, 1.5*escalaPat)); // Hacer mas grande el Patricio
   TG = glm::rotate(TG, (float) M_PI/2, glm::vec3(-1, 0, 0));
   TG = glm::translate(TG, -centreBasePat);
   glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
 }



 escalaPat ES UNA VARIABLE QUE VALE 1.0/TAMAÑO_DEL_PATRICIO SI HACEMOS ESTO Y LO MULTIPLICAMOS X 1.5 ESTAMOS CONSIGUIENDO QUE EL PATRICIO
 TENGA UN TAMAÑO DE 1.5 AHORA (SE PUEDE CAMBIAR EL 1.5 POR CUALQUIER DOUBLE)



 
-----------------------------------------------NOTAS BLOQUE 3-----------------------------------------------
 
****EXTRA: patri.load() == CARGAMOS EL PATRICIO
 
**** AMB = ES EL COLOR DEL AMBIENTE (ES EL COLOR DEL OBJETO SUELO)
**** SHIN = CUANTO VA A BRILLAR EL OBJETO SUELO
**** ESPEC = PUNTO EN EL CUAL EL OBJETO VA A BRILLAR MAS, TODOS LOS FOCOS APUNTAN A ESE PUNTO
**** DIFF = RGB
 
 
 
----------------------------------------------BLOQUE 3-------------------------------------------
 
 
//Esta version es para que se vea iluminado desde la posicion inicial.
 
 
//VertexShader Main
 
     vec4 VertexSCO = view* TG * vec4(vertex,1.0);
     vec3 normalSCO = normalize(inverse(transpose(mat3(view*TG))) * normal);
     vec4 posFocusSCO = view * vec4(posFocus,1.0);
         // vec4 posFocusSCO = vec4(posFocus,1.0);
    //Podemos dejar el PosFocus sin multiplicar por la view cosa que hara que por todos los       
            //    lados que veamos se vea iluminado 
     vec3 L = normalize(vec3(posFocusSCO - VertexSCO));
     fcolor = Phong(normalSCO,L,VertexSCO);
     gl_Position = proj * view * TG * vec4 (vertex, 1.0);
 
 
 
// MyGLWidget.cpp 
void MyGLWidget::FocusTransform(){
 
    glUniform3fv (posFocusLoc, 1, &posFocus[0]);
    glUniform3fv (colFocusLoc, 1, &colFocus[0]);
    glUniform3fv (llumAmbientLoc, 1, &llumAmbient[0]);
 
}
//  !!!!! IMPORTANTE !!!!!! //
// Anadir llamada a esta funcion cuando cargamos todo porque sino no le pasara lso valores inciales que tienen los atributos de la camara y saldra la escena negra. 
 
 
//MyGL Widget.h
 
    //Declaracion de la funcion anterior
            void FocusTransform();
 
    //Variables
    GLuint PosFocusLoc,ColorinFocus,LuzFocus;
    glm::vec3 colFocus = glm::vec3(0.8, 0.8, 0.8);
    glm::vec3 llumAmbient = glm::vec3(0.2, 0.2, 0.2);
    glm::vec3 posFocus = glm::vec3(1, 0, 1);  // en SCA
 
 
// Si queremos mover la camara por el eje de las x
 
 
// En la funcion de : void MyGLWidget::keyPressEvent(QKeyEvent* event)
         //Anadimos un caso para cambiar el valor de posFocus.
 
    case Qt::Key_T: { // canvia òptica entre perspectiva i axonomètrica
      posFocus[0] += 0.5; // Para el caso de querer moverlo para  +x 
    posFocus[0] -= 0.5;  // moverlo para -x.
 
      FocusTransform ();
      break;
    }
 
 
 
---------------------------------- TEMA ILUMINACION ---/*  SHADERS  */
 
mat3 nMatrix   = inverse(transpose(mat3(view * TG)));
vec3 NormSCO   = normalize(nMatrix * normal);
vec4 vertexSCO = view * TG * vec4(vertex, 1.0);
vec4 focusSCO  = view * vec4(posFocus, 1.0);
vec4 L         = focusSCO - vertexSCO;
vec3 Lxyz      = normalize(L.xyz);
fcolor         = Lambert (NormSCO, Lxyz); (LAMBERT O PHONG, EL QUE USEMOS)
 
FUNCIONES PHONG Y LAMBERT CALCULAN EL FOCO DE LUZ, SE LE TIENEN QUE PASAR COORDENADAS DEL OBSERVADOR
 
 
// Valors per als components que necessitem dels focus de llum
vec3 colFocus = vec3(1, 1, 1);            // COLOR DEL FOCO
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);         // LUZ AMBIENTE
vec3 posFocus = vec3(5, 3, 5);  // en SCA       // POSICION DONDE ESTA LA LUZ
 
******************* FUNCIONES DE LAMBERT Y DE PHONG (EL LEÑADOR Y EL CHINO)
 
vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
 
    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb;
 
    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
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
 
    if ((dot(R, V) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine);
}
 
 
 
 
 
//--------------------------------------------Resize-------------------------------------------------------------
 
 
void MyGLWidget::resizeGL (int w, int h)
{
  ra = float (w) / float (h);
  switch (camera) {
    case PERSPECTIVE : {
      if (ra < 1) FOV = 2.0 * atan(tan(FOVi/2.0)/ra);
      break;
    } //perspectiva
    case ORTHO : {
      if (ra > 1.0) {
        top         = R/ra;
        bottom      = -R/ra;
      }
      if (ra < 1.0) {
        left        = -R*ra;
        right       = R*ra;
      }
      break;
    }
  }
  projectTransform();
  glViewport(0, 0, w, h);
}





//---------------------------radi




R= (sqrt(pow(puntMaxE.x-puntMinE.x,2)+pow(puntMaxE.y-puntMinE.y,2)+pow(puntMaxE.z-puntMinE.z,2)))/2;


