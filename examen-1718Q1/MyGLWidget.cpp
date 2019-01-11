#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleX = 0.0;
  angleY = M_PI / 4; //y declarada per l'enunciat
  DoingInteractive = NONE;
  radiEsc = 0.0;
  maxEsc = glm::vec3(10.0,10.5,10.0);
  minEsc = glm::vec3(-10.0,0.0,-10.0);
  VRP = (maxEsc+minEsc)/2.0f; // centre de l'escena : (maxe = (X.MAX,Y.MAX,Z.MAX) + mine = (X.MIN,Y.MIN,Z.MIN)) / 2
  OBS = glm::vec3(10,5.25,-10); //obs indicat a l'enunciat
  UP = glm::vec3(0,1,0);
  FOV = 0.0f;
  FOVi = 0.0f;
  ra = 1.0f;
  colorLlum = glm::vec3(0.8f,0.8f,0.8f);
  angleR = 0.0;
  canviCam = false;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffersModel();
  createBuffersTerra();
  projectTransform ();
  viewTransform ();
  ini_camera();
  glUniform3fv(colorLoc,1,&colorLlum[0]);
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer i el depth-buffer

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformIdent ();
  // pintem terra
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Pat 1
  glBindVertexArray (VAO_Model);
  modelTransformModel ();
  // Pintem Model Pat 1
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  // Pat 2
  glBindVertexArray (VAO_Model);
  modelTransformModel2 ();
  // Pintem Model Pat 2
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  // Pat 3
  glBindVertexArray (VAO_Model);
  modelTransformModel3 ();
  // Pintem Model Pat 3
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  // Pat 4
  glBindVertexArray (VAO_Model);
  modelTransformModel4 ();
  // Pintem Model Pat 4
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
    ra = float (w) / float (h);
    if (ra < 1) FOV = 2.0 * atan(tan(FOVi/2.0)/ra);
    projectTransform();
    glViewport(0, 0, w, h);
}


void MyGLWidget::ini_camera(){
    radiEsc = glm::distance(maxEsc,minEsc)/2;
    FOV = FOVi = 2.0 * asin(radiEsc/(radiEsc*2));
    projectTransform();
    viewTransform();
}

void MyGLWidget::modelTransformModel ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG,angleR, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0.0,0.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG,angleR, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(-2.25,2.75,0.0));
  TG = glm::translate(TG, glm::vec3(0.0,0.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModel3 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG,angleR, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(2.25,2.75,0.0));
  TG = glm::translate(TG, glm::vec3(0.0,0.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModel4 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG,angleR, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0.0,5.5,0.0));
  TG = glm::translate(TG, glm::vec3(0.0,0.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformIdent ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if(not canviCam){
    Proj = glm::perspective(FOV, ra, radiEsc, 3.0f*radiEsc);
   }
  else {
      Proj = glm::perspective(((float)M_PI/2.0f), ra, 1.0f,30.0f);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;  // Matriu de posició i orientació
  if(not canviCam){
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -VRP);
 }
  else {
      View = glm::lookAt (OBS,VRP, UP);
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 5.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::createBuffersModel ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  m.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (m, escalaModel, centreBaseModel);
  
  // Creació del Vertex Array Object del Model
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_ModelPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_ModelNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_ModelMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_ModelMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_ModelMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_ModelMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3, m.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::createBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[6] = {
        glm::vec3(-10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0,  10.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 normt (0,1,0);
  glm::vec3 normterra[6] = {
	normt, normt, normt, normt, normt, normt
  };

  // Definim el material del terra
  /*glm::vec3 amb(0.1,0.1,0);
  glm::vec3 diff(0.6,0.6,0.0);*/
  glm::vec3 amb(red,green,blue);
  glm::vec3 diff(red,green,blue);
  glm::vec3 spec(1.0,1.0,1.0);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[6] = {
	amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[6] = {
	diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[6] = {
	spec, spec, spec, spec, spec, spec
  };
  float matshinterra[6] = {
	shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  colorLoc = glGetUniformLocation(program->programId(), "colFocus");
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_H: { // help de l'aplicació
      std::cout << std::endl << "Ajuda: Tecles actives a l'aplicació" << std::endl;
      std::cout << "   H: Mostra aquest mateix help." << std::endl;
      break;
    }
     case Qt::Key_C: { // canvia òptica entre perspectiva i axonomètrica
        canviCam = !canviCam;
        projectTransform();
        viewTransform();
        break;
    }
    default: event->ignore(); break;
  }
  update();
}

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

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
    if (DoingInteractive == ROTATE)
    {
      // Fem la rotació
      angleY += (e->x() - xClick) * M_PI / 180.0;
      angleX += (e->y() - yClick) * M_PI / 180.0;
      viewTransform ();
    }

    xClick = e->x();
    yClick = e->y();

    update ();
}

void MyGLWidget::rotarPatricio(int angle)
{
 makeCurrent();
 angleR = float((angle*M_PI)/180);
 update();
}

void MyGLWidget::canviCam1()
{
    makeCurrent();
    if(canviCam) canviCam = false;
    projectTransform();
    viewTransform();
    update();
}
void MyGLWidget::canviCam2()
{

    makeCurrent();
    if(not canviCam)canviCam = true;
    projectTransform();
    viewTransform();
    update();
}
void MyGLWidget::canviColorTerra(int r, int g, int b)
{
    makeCurrent();
    red = (float) r/255;
    green = (float) g/255;
    blue = (float) b/255;
    createBuffersTerra ();
    update();
}



