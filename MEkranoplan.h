#ifndef MEKRANOPLAN_H
#define MEKRANOPLAN_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

//============================================================================================
class MEkranoplan : protected QOpenGLFunctions
//-                 protected QGLFunctions
{
public:
    MEkranoplan();
    virtual ~MEkranoplan();

  void Init();
  void Draw(QOpenGLShaderProgram* pShaderProgram);

private:
  void InitGeometry();

    GLuint vboIds[2];
//--
  GLsizei CountVertexIndices;
};
//============================================================================================
#endif // MEKRANOPLAN_H
