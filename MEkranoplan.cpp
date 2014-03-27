#include "MEkranoplan.h"

#include <QVector3D>
  #include <QMessageBox>
#include <QList>
#include <QFile>
#include <QTextStream>

//============================================================================================
struct VertexData
{
    QVector3D position;
  QVector3D normal;//////
    QVector2D texCoord;
};
//============================================================================================
MEkranoplan::MEkranoplan()
{
}
//--------------------------------------------------------------------------------------------
MEkranoplan::~MEkranoplan()
{
  glDeleteBuffers(2, vboIds);
}
//--------------------------------------------------------------------------------------------
void MEkranoplan::Init()
{
  initializeOpenGLFunctions();///////

//! [0]
  glGenBuffers(2, vboIds);

//! [0]

  // Initializes cube geometry and transfers it to VBOs
  InitGeometry();//////
}
//--------------------------------------------------------------------------------------------
void MEkranoplan::InitGeometry()
{

  QString fileNameIn(":/Ekranoplan_3dsMax.obj");
  QList<QVector3D> listPositions;
  QList<QVector3D> listTextures;
  QList<QVector3D> listNormals;
  QList<int> listFacePositionIndices;
  QList<int> listFaceTextureIndices;
  QList<int> listFaceNormalIndices;

  QFile fileIn(fileNameIn);
  if(!fileIn.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QMessageBox msgBox; msgBox.setText("Problem file \""+fileNameIn+"\"");   msgBox.exec();
    return;
  }

QMessageBox msgBox;
  QTextStream in(&fileIn);  QString line;  QChar qc; char c;  int pos1,pos2;  QString s;  QVector3D v3D;
  while(!in.atEnd())
  {
    line = in.readLine();
    if(line.length() == 0)  continue;
    qc = line.at(0);
    switch(qc.toLatin1())
    {
      case 'v':
        c = line.at(1).toLatin1();
        if(c == ' ' || c == 't' || c == 'n')
        {  // vertices || textures || normals
          pos1 = 3;
          pos2 = line.indexOf(' ', pos1);  v3D.setX(line.mid(pos1, pos2-pos1).toFloat());  pos1 = pos2+1;
          pos2 = line.indexOf(' ', pos1);  v3D.setY(line.mid(pos1, pos2-pos1).toFloat());
                                           v3D.setZ(line.mid(pos2+1).toFloat());
          if(c == ' ')       listPositions << v3D;
          else if(c == 't')  listTextures << v3D;
          else               listNormals  << v3D;   //  c == 'n'
        }
        break;
      case 'f':
        if(line.at(1).toLatin1() == ' ')
        {  // indices
          pos1 = 2;
          if(listFaceTextureIndices.count() == 0 && listNormals.count() == 0)
          {
//:  f 5 34 85
                              pos2 = line.indexOf(' ', pos1);  listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf(' ', pos1);  listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.count();             listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
          }
          if((listTextures.count() > 0 && listNormals.count() == 0) || (listTextures.count() == 0 && listNormals.count() > 0))
          {
//:  f 5/6 34/67 85/2
                              pos2 = line.indexOf('/', pos1);  listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf(' ', pos1);
            if(listTextures.count() > 0)  listFaceTextureIndices << line.mid(pos1, pos2-pos1).toInt();
            else                          listFaceNormalIndices  << line.mid(pos1, pos2-pos1).toInt();

            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf(' ', pos1);
            if(listTextures.count() > 0)  listFaceTextureIndices << line.mid(pos1, pos2-pos1).toInt();
            else                          listFaceNormalIndices  << line.mid(pos1, pos2-pos1).toInt();

            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  listFacePositionIndices << line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.count();
            if(listTextures.count() > 0)  listFaceTextureIndices << line.mid(pos1, pos2-pos1).toInt();
            else                          listFaceNormalIndices  << line.mid(pos1, pos2-pos1).toInt();
          }
          else
          {
//:  f 5/6/4 34/67/18 85/2/16

            int  p1,p2,p3, t1,t2,t3, n1,n2,n3;
                              pos2 = line.indexOf('/', pos1);  p1 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  t1 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf(' ', pos1);  n1 = line.mid(pos1, pos2-pos1).toInt();

            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  p2 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  t2 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf(' ', pos1);  n2 = line.mid(pos1, pos2-pos1).toInt();

            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  p3 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.indexOf('/', pos1);  t3 = line.mid(pos1, pos2-pos1).toInt();
            pos1 = pos2 + 1;  pos2 = line.count();             n3 = line.mid(pos1, pos2-pos1).toInt();

            listFacePositionIndices << p1;   listFacePositionIndices << p2;   listFacePositionIndices << p3;
            listFaceTextureIndices  << t1;   listFaceTextureIndices <<  t2;   listFaceTextureIndices  << t3;
            listFaceNormalIndices   << n1;   listFaceNormalIndices <<   n2;   listFaceNormalIndices   << n3;
          }
        }
        break;
      default:
        ;
    }
  }
  fileIn.close();
  float fm = 0.0;
  for(int i=0; i < listPositions.count(); i++)
  {  //         fm = 359.85657
    fm = qMax(fm, qAbs(listPositions.at(i).x()));
    fm = qMax(fm, qAbs(listPositions.at(i).y()));
    fm = qMax(fm, qAbs(listPositions.at(i).z()));
  }
//openGL counts from 1; C++ from 0, therefore replace
  for(int i = 0; i < listFacePositionIndices.count(); i++) {
    listFacePositionIndices.replace(i, listFacePositionIndices.at(i)-1);
    listFaceTextureIndices.replace(i, listFaceTextureIndices.at(i)-1);
    listFaceNormalIndices.replace(i, listFaceNormalIndices.at(i)-1);
  }

  QList<int> listFacePositionIndicesDiff;
  QList<int> listFaceTextureIndicesDiff;
  QList<int> listFaceNormalIndicesDiff;
  listFacePositionIndicesDiff << listFacePositionIndices.at(0);
  listFaceTextureIndicesDiff << listFaceTextureIndices.at(0);
  listFaceNormalIndicesDiff << listFaceNormalIndices.at(0);

  QList<int> listVertexIndices;
  listVertexIndices << 0;
  int j;  bool isHas;
  for(int i = 1; i < listFacePositionIndices.count(); i++)
  {
    isHas = false;
    for(j = 0; j < listFacePositionIndicesDiff.count(); j++)
    {
      if(listFacePositionIndices.at(i) == listFacePositionIndicesDiff.at(j) &&
         listFaceTextureIndices.at(i) == listFaceTextureIndicesDiff.at(j) &&
         listFaceNormalIndices.at(i) == listFaceNormalIndicesDiff.at(j)) {
        isHas = true;
        break;
      }
    }
    if(!isHas)
    {
      listFacePositionIndicesDiff << listFacePositionIndices.at(i);
      listFaceTextureIndicesDiff << listFaceTextureIndices.at(i);
      listFaceNormalIndicesDiff << listFaceNormalIndices.at(i);
    }
    listVertexIndices << j;
  }
  CountVertexIndices = listVertexIndices.count();
  VertexData* vertices = new VertexData[listFacePositionIndicesDiff.count()]; //  countIndicesDiff == listFacePositionIndicesDiff.count()

  for(int i = 0; i < listFacePositionIndicesDiff.count(); i++)
  {
    vertices[i].position = listPositions.at(listFacePositionIndicesDiff.at(i)); //-1)
    vertices[i].position /= fm;
    vertices[i].normal = listNormals.at(listFaceNormalIndicesDiff.at(i)); //-1);
    vertices[i].texCoord.setX(qMin(listTextures.at(listFaceTextureIndicesDiff.at(i)).x(), 1.0f));
    vertices[i].texCoord.setY(qMin(listTextures.at(listFaceTextureIndicesDiff.at(i)).y(), 1.0f));
  }

  GLushort* indices = new GLushort[CountVertexIndices];
  for(int i=0; i < CountVertexIndices; i++) { indices[i] = listVertexIndices.at(i); }

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, listFacePositionIndicesDiff.count()*sizeof(VertexData), vertices, GL_STATIC_DRAW);

  delete[] vertices;


    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CountVertexIndices*sizeof(GLushort), indices, GL_STATIC_DRAW);

  delete[] indices;
}
//--------------------------------------------------------------------------------------------
void MEkranoplan::Draw(QOpenGLShaderProgram* pShaderProgram)
{
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = pShaderProgram->attributeLocation("a_position");
    pShaderProgram->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

//--
  offset += sizeof(QVector3D);

// Tell OpenGL programmable pipeline how to locate normal position data
  int normalLocation = pShaderProgram->attributeLocation("a_normal");
  pShaderProgram->enableAttributeArray(normalLocation);
  glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);
//--

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = pShaderProgram->attributeLocation("a_texcoord");
    pShaderProgram->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);
  glDrawElements(GL_TRIANGLES, CountVertexIndices, GL_UNSIGNED_SHORT, 0);
}
//--------------------------------------------------------------------------------------------
