#ifndef MGYROHORIZON_H
#define MGYROHORIZON_H

#include <QWidget>
#include <QPaintEvent>
#include "MEkranoplanGL.h"

//============================================================================================
class MGyrohorizon : public QWidget
{
    Q_OBJECT
public:
  explicit MGyrohorizon(QWidget *parent);
  virtual ~MGyrohorizon();

protected:
  virtual void paintEvent(QPaintEvent * event);

private:

  MEkranoplanGL* PEkranoplanGL;
  qreal Width;  qreal Height;
  qreal rRoll;

  qreal WidthHalf;
  qreal HeightHalf;  qreal AngleHeightHalf;  qreal AngleHeightHalfVisible;
  qreal HOneDegreeAngle;

  qreal MaxDim;  qreal MaxDimHalf;  qreal HeightHalfVisible;
  qreal H1;  qreal H2;


  QColor ColorSky0;            QColor ColorSky1;            QColor ColorSky2;
  QColor ColorGround0;         QColor ColorGround1;         QColor ColorGround2;
  QColor ColorPitch;           QColor ColorRoll;
  QColor ColorStaticBalance0;  QColor ColorStaticBalance1;  QColor ColorStaticTriangle;  QColor ColorStaticBalanceOutline;

  qreal PitchAngle;
  qreal RollAngle;

public:
  void SetPEkranoplanGL(MEkranoplanGL* pEkranoplanGL) { PEkranoplanGL = pEkranoplanGL; }
  void SetQuaternion(QQuaternion* pQuaternion);
  void SetPitchRollAngles(float pitch, float roll);

signals:

public slots:
};
//============================================================================================
#endif // MGYROHORIZON_H
