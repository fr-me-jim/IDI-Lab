#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  divide = false, rotation = false;
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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffersQuadrat();
}

void MyGLWidget::pintaQuadrat ()  // AQUEST MÈTODE NO ES POT MODIFICAR !!!
{
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
  // Pintem el quadrat
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MyGLWidget::paintGL ()
{
  if (!divide){
      glViewport (0, 0, width(), height());
      glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

      //Primer Quadrat
      modelTransformR1();
      pintaQuadrat ();

      //Segon
      modelTransformR2();
      pintaQuadrat ();
  }
  else {
      glViewport (0, height()/2, width()/2, height()/2);
      glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

      //Primer Quadrat
      modelTransformR1();
      pintaQuadrat ();

      //Segon
      modelTransformR2();
      pintaQuadrat ();

      rotation = !rotation; // Activem la rotació

      glViewport (width()/2, 0, width()/2, height()/2);
      //Primer Quadrat
      modelTransformR1();
      pintaQuadrat ();

      //Segon
      modelTransformR2();
      pintaQuadrat ();

      rotation = !rotation; // Desactivem rotació

  }

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformR1 () 
{
  // Codi per a la TG necessària per al rectangle R1
    glm::mat4 TG(1.f);  // Matriu de transformació
    if (divide && rotation){
        TG = glm::rotate(TG, float(M_PI), glm::vec3(0,0,1));
    }
    TG = glm::translate(TG,glm::vec3(-0.5,0.1,0));
    TG = glm::scale(TG,glm::vec3(0.2/0.4,1.0/0.4,1));

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformR2 () 
{
  // Codi per a la TG necessària per al rectangle R2
    glm::mat4 TG(1.f);  // Matriu de transformació
    if (divide && rotation){
        TG = glm::rotate(TG, float(M_PI), glm::vec3(0,0,1));
    }
    TG = glm::translate(TG,glm::vec3(-0.3,-0.5,0.0));
    TG = glm::scale(TG,glm::vec3(0.6/0.4,0.2/0.4,1.0));

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
  // Aquí anirà el codi que cal fer quan es redimensiona la finestra
  // no es demana en aquest exercici!
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_V: { // canvi entre un i dos viewports
      divide = !divide;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffersQuadrat ()  // AQUEST MÈTODE NO ES POT MODIFICAR !!!
{
  glm::vec3 Vertices[4];  // Quatre vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.2, -0.2, 0.0);
  Vertices[1] = glm::vec3(0.2, -0.2, 0.0);
  Vertices[2] = glm::vec3(-0.2, 0.2, 0.0);
  Vertices[3] = glm::vec3(0.2, 0.2, 0.0);
  
  glm::vec3 Colors[4];
  Colors[0] = glm::vec3(1.0, 1.0, 0.0);
  Colors[1] = glm::vec3(1.0, 0.0, 0.0);
  Colors[2] = glm::vec3(1.0, 1.0, 0.0);
  Colors[3] = glm::vec3(1.0, 0.0, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBOs[2];
  glGenBuffers(2, VBOs);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation (program -> programId(), "TG");
}
