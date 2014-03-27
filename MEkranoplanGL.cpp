#include "MEkranoplanGL.h"
  #include "funcs.h"
  #include <QStatusBar>

#include <QOpenGLShader>
#include <QImage>

#include <math.h>
#include <locale.h>

#include <QMessageBox>
  #include <QGraphicsOpacityEffect>

//============================================================================================
MEkranoplanGL::MEkranoplanGL(QWidget *parent)
            : QGLWidget(parent), //, 0, Qt::Widget)
              PitchAngle(0.0), RollAngle(0.0)//,
{
}
//--------------------------------------------------------------------------------------------
MEkranoplanGL::~MEkranoplanGL()
{
    deleteTexture(texture);
}
//--------------------------------------------------------------------------------------------
bool MEkranoplanGL::event(QEvent* event)
{
  bool b = QGLWidget::event(event); //catch different events
//--
  if(event->type() == QEvent::Polish)
  {//Polish is caught
    setObjectName(QStringLiteral("MEkranoplanGL"));
    setGeometry(QRect(0, 170, 150, 150));
  }
  return b;
}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::initializeGL()
{
  initializeOpenGLFunctions();
  qglClearColor(QColor(0,0,0, 255));

  InitShaders();
  InitTextures();

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // Enable back face culling
  glEnable(GL_CULL_FACE);

//-    geometries.init();
  Ekranoplan.Init();
}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::InitShaders()
{
  QMessageBox msgBox;// msgBox.setText("void MMEkranoplanGLWidget::InitShaders()\nBEGIN");   msgBox.exec();

    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader

  if(!ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl")) {
    close();
    msgBox.setText("void MMEkranoplanGLWidget::InitShaders()\nShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex) == FALSE");   msgBox.exec();
  }

    // Compile fragment shader
  if(!ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl")) {
    close();
    msgBox.setText("void MMEkranoplanGLWidget::InitShaders()\nShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment) == FALSE");   msgBox.exec();
  }

    // Link shader pipeline
  if(!ShaderProgram.link()) {
    close();
    msgBox.setText("void MMEkranoplanGLWidget::InitShaders()\nShaderProgram.link() == FALSE");   msgBox.exec();
  }

    // Bind shader pipeline for use
  if(!ShaderProgram.bind()) {
    close();
    msgBox.setText("void MMEkranoplanGLWidget::InitShaders()\nShaderProgram.bind() == FALSE");   msgBox.exec();
  }

    // Restore system locale
    setlocale(LC_ALL, "");

}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::InitTextures()
{
    // Load cube.png image
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/Texture.png"));

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, (GLint)w, (GLint)h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 28.0;//45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(RotationQuaternion);
    // Set modelview-projection matrix
    ShaderProgram.setUniformValue("mvp_matrix", projection * matrix);

//! [6]

    // Use texture unit 0 which contains cube.png
    ShaderProgram.setUniformValue("texture", 0);

  ShaderProgram.setUniformValue("rollAngle", RollAngle); // for Light


    // Draw cube geometry
    Ekranoplan.Draw(&ShaderProgram);

}
//--------------------------------------------------------------------------------------------
void MEkranoplanGL::SetQuaternion(QQuaternion* pQuaternion)
{
  RotationQuaternion = pQuaternion->normalized();

  updateGL();
}
//--------------------------------------------------------------------------------------------
