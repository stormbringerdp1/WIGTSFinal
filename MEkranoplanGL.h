#ifndef MEKRANOPLANGLWIDGET_H
#define MEKRANOPLANGLWIDGET_H


#include <QGLWidget>
#include <QEvent>
#include <QMoveEvent>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>

#include "MEkranoplan.h"

//============================================================================================
class MEkranoplanGL : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MEkranoplanGL(QWidget *parent = 0);
  virtual ~MEkranoplanGL();


protected:
  virtual bool event(QEvent* event);

  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();


protected:
  void InitShaders();
  void InitTextures();

private:


  float PitchAngle;     float RollAngle;
  QOpenGLShaderProgram ShaderProgram;
  MEkranoplan Ekranoplan;

    GLuint texture;

    QMatrix4x4 projection;
    QQuaternion RotationQuaternion;
  QMatrix4x4 RotationMatrix4x4;

public:
  void SetQuaternion(QQuaternion* pQuaternion);

signals:

public slots:
};
//============================================================================================
#endif // MEKRANOPLANGLWIDGET_H
