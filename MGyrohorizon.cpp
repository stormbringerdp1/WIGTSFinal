#include "MGyrohorizon.h"
#include "funcs.h"

#include <QGraphicsView>
#include <QPainter>
#include <QLinearGradient>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QBitmap>

//============================================================================================
void DrawText(QPainter& painter, const qreal xoff, const qreal yoff, const qreal angle)
{
  QString text;  text.sprintf("%.0f",angle);
  QRectF rectF = painter.boundingRect(QRectF(0.0,0.0, 500.0,500.0), text, QTextOption(Qt::AlignCenter));
  painter.drawText(QRectF(-rectF.width()/2.0-0.5 + xoff, -rectF.height()/2.0-0.5 + yoff, rectF.width(),rectF.height()), text, QTextOption(Qt::AlignCenter));
}
//============================================================================================
MGyrohorizon::MGyrohorizon(QWidget *parent)
            : QWidget(parent)
{
  Width = 320.0;
  Height = 320.0;
  rRoll = 125.0;

  WidthHalf = Width/2.0;
  HeightHalf = Height/2.0;
  AngleHeightHalf = 40.0; //40 градусов
  AngleHeightHalfVisible = 25.0;//проримовка до 25
  HOneDegreeAngle = HeightHalf/AngleHeightHalf;

  MaxDim = qMax(Width,Height)*qSqrt(2.0);//нет белых квадратов при повороте
  MaxDimHalf = MaxDim/2.0;
  HeightHalfVisible = HeightHalf/AngleHeightHalf*AngleHeightHalfVisible;

  H1 = HOneDegreeAngle*AngleHeightHalf;// в пикселях от середины до верха
  H2 = HOneDegreeAngle*(90.0-AngleHeightHalf);//от вертикали до нашего угла, не видимый при старте


  ColorSky2 = QColor(0,0,80, 255);
  ColorSky1 = QColor(0,0,100, 255);
  ColorSky0 = QColor(80,180,255, 255);
  ColorGround0 = QColor(90,170,110, 255);
  ColorGround1 = QColor(90,17,11, 255);
  ColorGround2 = QColor(0,17,11, 255);

  ColorPitch = QColor(255,255,255, 255);
  ColorRoll = QColor(255,255,255, 255);

  ColorStaticBalance0 = QColor(255,255,0, 255);
  ColorStaticBalance1 = QColor(88,88,0, 255);
  ColorStaticTriangle = QColor(255,255,0, 255);
  ColorStaticBalanceOutline = QColor(0,0,0, 255);

  PitchAngle = 0.0;
  RollAngle = 0.0;
}
//--------------------------------------------------------------------------------------------
MGyrohorizon::~MGyrohorizon()
{
}
//--------------------------------------------------------------------------------------------
void MGyrohorizon::paintEvent(QPaintEvent * event)
{
//--
  QImage imgagePaint(size(), QImage::Format_ARGB32_Premultiplied);
  QPainter painter;
  painter.begin(&imgagePaint);
//--

  painter.setRenderHint(QPainter::Antialiasing, true);

  QLinearGradient linearGrad;
  QPointF mpointsF[5];
  qreal yh;

//=============================

  bool isUpDawn = false;
  while(PitchAngle < -90.0)
  {
    PitchAngle += 180.0;
    isUpDawn = !isUpDawn; //true;
  }
  while(PitchAngle > 90.0)
  {
    PitchAngle -= 180.0;
    isUpDawn = !isUpDawn; //true;
  }


  if(isUpDawn)
  {
      RollAngle += 180.0;

  }
  while(RollAngle < -180.0)
  {
    RollAngle += 360.0;
  }
  while(RollAngle > 180.0)
  {
     RollAngle -= 360.0;
  }


  qreal hPitchAngle = HeightHalf/AngleHeightHalf*PitchAngle;//здвиг по пикселям в соответсвии с градусами
  if(isUpDawn) {
    hPitchAngle = -hPitchAngle;
  }


  painter.translate(WidthHalf,HeightHalf);//переместили цент с 0,0 на центр
  painter.rotate(-RollAngle);

//=====  Pitch:  =====
  painter.setPen(Qt::NoPen);


//-Sky:

// 0:
  yh = hPitchAngle;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh-H1);
  linearGrad.setColorAt(0, ColorSky0);
  linearGrad.setColorAt(1, ColorSky1);
  QBrush brushSky1(linearGrad);   painter.setBrush(brushSky1);
  painter.drawRect(-MaxDimHalf,yh+0.5, MaxDim,-H1-2.0);//первый верхний четерехугольник

  yh -= H1;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh-H2);
  linearGrad.setColorAt(0, ColorSky1);
  linearGrad.setColorAt(1, ColorSky2);
  QBrush brushSky2(linearGrad);   painter.setBrush(brushSky2);
  painter.drawRect(QRectF(-MaxDimHalf,yh, MaxDim,-H2-2.0));

//90
  yh -= H2;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh-H2);
  linearGrad.setColorAt(0, ColorSky2);
  linearGrad.setColorAt(1, ColorSky1);
  QBrush brushSky3(linearGrad);   painter.setBrush(brushSky3);
  painter.drawRect(QRectF(-MaxDimHalf,yh, MaxDim,-H2-2.0));

  yh -= H2;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh-H1);
  linearGrad.setColorAt(0, ColorSky1);
  linearGrad.setColorAt(1, ColorSky0);
  QBrush brushSky4(linearGrad);   painter.setBrush(brushSky4);
  painter.drawRect(QRectF(-MaxDimHalf,yh+0.5, MaxDim,-H1-2.0));

//180

//-Ground:

// 0:
  yh = hPitchAngle;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh+H1);
  linearGrad.setColorAt(0, ColorGround0);
  linearGrad.setColorAt(1, ColorGround1);
  QBrush brushGround1(linearGrad);   painter.setBrush(brushGround1);
  painter.drawRect(QRectF(-MaxDimHalf,yh-0.5, MaxDim,H1+2.0));

  yh += H1;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh+H2);
  linearGrad.setColorAt(0, ColorGround1);
  linearGrad.setColorAt(1, ColorGround2);
  QBrush brushGround2(linearGrad);   painter.setBrush(brushGround2);
  painter.drawRect(QRectF(-MaxDimHalf,yh, MaxDim,H2+2.0));

//90:
  yh += H2;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh+H2);
  linearGrad.setColorAt(0, ColorGround2);
  linearGrad.setColorAt(1, ColorGround1);
  QBrush brushGround3(linearGrad);   painter.setBrush(brushGround3);
  painter.drawRect(QRectF(-MaxDimHalf,yh, MaxDim,H2+2.0));

  yh += H2;
  linearGrad.setStart(0.0,     yh);
  linearGrad.setFinalStop(0.0, yh+H1);
  linearGrad.setColorAt(0, ColorGround1);
  linearGrad.setColorAt(1, ColorGround0);
  QBrush brushGround4(linearGrad);   painter.setBrush(brushGround4);
  painter.drawRect(QRectF(-MaxDimHalf,yh, MaxDim,H1+2));
//180

//== Pitch Scale:

  QPen pen(ColorPitch);  pen.setWidthF(1.0);
  painter.setPen(pen);

  QVector<QLineF> lines;

//- (2)-degrees:
  qreal y;  bool isHas = false;
  for(int k = -1; k <= 1; k += 2)
  {
    y = hPitchAngle;
    while(y >=   2.0*(H1+H2)-2.0*HeightHalfVisible)  { y -= 2.0*(H1+H2); }
    while(y <= -(2.0*(H1+H2)-2.0*HeightHalfVisible)) { y += 2.0*(H1+H2); }
    for(int i = 1; i <= 4; i++)
    {
      y += -2.0*HOneDegreeAngle*k;
      if(qAbs(y) <= HeightHalfVisible)
      {
        isHas = true;
        lines << QLineF(-8.0,y, 8.0,y);
      }
    }
  }
  if(isHas)  painter.drawLines(lines);
  lines.clear();

  pen.setWidthF(1.5);   painter.setPen(pen);

  qreal xoff, yoff;
  qreal angle;

//- 10-degrees:
  mpointsF[0].setX(-24.0);
  mpointsF[1].setX(-24.0);
  mpointsF[2].setX(24.0);
  mpointsF[3].setX(24.0);
  for(int k = -1; k <= 1; k += 2)
  {
    for(int i = 0; i <= 9; i++)
    {
      angle = 10.0*i;
      y = hPitchAngle - angle*HOneDegreeAngle*k;
      while(y >=   2.0*(H1+H2)-2.0*HeightHalfVisible)  { y -= 2.0*(H1+H2); }
      while(y <= -(2.0*(H1+H2)-2.0*HeightHalfVisible)) { y += 2.0*(H1+H2); }
      if(qAbs(y) <= HeightHalfVisible)
      {
        if(i == 0)
        {
          painter.drawLine(QLineF(-48.0,y, 48.0,y));
        }
        else if(i < 9)
        {
          mpointsF[0].setY(y + 5.0*k);
          mpointsF[1].setY(y);
          mpointsF[2].setY(y);
          mpointsF[3].setY(y + 5.0*k);
          painter.drawPolyline(mpointsF, 4); //int pointCount)
          DrawText(painter, -24.0-12.0+0.5, y+5.0/2.0*k, angle);
          DrawText(painter,  24.0+12.0+1.0, y+5.0/2.0*k, angle);
        }
        else
        {
          lines << QLineF(-36.0,y-7.0, -36.0,y+7.0);
          lines << QLineF(-36.0,y,      36.0,y);
          lines << QLineF( 36.0,y-7.0,  36.0,y+7.0);
          painter.drawLines(lines);  lines.clear();
          DrawText(painter, -36.0-12.0+1.0, y, angle);
          DrawText(painter,  36.0+12.0+1.0, y, angle);
        }
      }
    }

  }
//- (15)-degrees:
  for(int k = -1; k <= 1; k += 2) {
    y = hPitchAngle - 10.0*HOneDegreeAngle*(1.0+0.5)*k;
    while(y >=   2.0*(H1+H2)-2.0*HeightHalfVisible)  { y -= 2.0*(H1+H2); }
    while(y <= -(2.0*(H1+H2)-2.0*HeightHalfVisible)) { y += 2.0*(H1+H2); }
    if(qAbs(y) <= HeightHalfVisible)
      painter.drawLine(QLineF(-16.0,y, 16.0,y));
  }

//=====  Roll:  =====
  painter.setBrush(QBrush(ColorRoll));

//- Triangle:
  painter.setPen(Qt::NoPen);
  mpointsF[0].setX(0.0);   mpointsF[0].setY(-rRoll);
  mpointsF[1].setX(-6.5);  mpointsF[1].setY(-rRoll - 11.258); //  11.258 = sqrt(3.0)/2.0 * 13.0
  mpointsF[2].setX(6.5);   mpointsF[2].setY(-rRoll - 11.258);
  painter.drawPolygon(mpointsF,3);

//- Arc:
  pen.setColor(ColorRoll);  pen.setWidthF(1.5);   painter.setPen(pen);
  painter.drawArc(QRectF(-rRoll, -rRoll, 2.0*rRoll, 2.0*rRoll), 30*16, 120*16);

//для крючочков
  qreal hs1 = 5.0;
  qreal hs2 = 12.0;  qreal ws2 = 5.0;
  qreal hs3 = 10.0;
  yoff = -rRoll - hs2 - 10.0; //  - 5.0
  angle = 0.0;

  qreal dopangle;  // QString text;

  for(int k = -1; k <= 1; k += 2)
  {
    xoff = ws2/2.0*k;
//- (5), (10):
    dopangle = 10.0*k;  painter.rotate(dopangle);
    for(int i = 1; i <= 2; i++)
    {
      painter.drawLine(QPointF(0.0,-rRoll), QPointF(0.0,-rRoll-hs1));
      painter.rotate(dopangle);
    }

//- 30, (45), 60, (75):
    dopangle = 15.0*k;  angle = 15.0;
    for(int i = 1; i <= 2; i++)
    {
// 30, 60:
      angle += dopangle*k;  //  text.sprintf(("%.0f"),angle);
      DrawText(painter, xoff, yoff, angle); //text);
      mpointsF[0].setX(0.0);  mpointsF[0].setY(-rRoll);
      mpointsF[1].setX(0.0);  mpointsF[1].setY(-rRoll-hs2);
      mpointsF[2].setX(ws2*k);  mpointsF[2].setY(-rRoll-hs2);
      painter.drawPolyline(mpointsF, 3);
// (45), (75):
      painter.rotate(dopangle);
      painter.drawLine(QPointF(0.0,-rRoll), QPointF(0.0,-rRoll-hs3));

      painter.rotate(dopangle);
      angle += dopangle*k;
    }

//- 90:
    xoff = 1.0;
    angle += dopangle*k;  //  text.sprintf(("%.0f"),angle);
    DrawText(painter, xoff, yoff, angle); //text);
    lines << QLineF(0.0,-rRoll, 0.0,-rRoll-hs2);
    lines << QLineF(-4.0,-rRoll-hs2, 4.0,-rRoll-hs2);
    painter.drawLines(lines);  lines.clear();

//- 120, 150:
    xoff = -ws2/2.0*k;
    dopangle = 30.0*k;    painter.rotate(dopangle);
    for(int i = 1; i <= 2; i++)
    {
      angle += dopangle*k;  //  text.sprintf(("%.0f"),angle);
      DrawText(painter, xoff, yoff, angle); //text);
      mpointsF[0].setX(0.0);  mpointsF[0].setY(-rRoll);
      mpointsF[1].setX(0.0);  mpointsF[1].setY(-rRoll-hs2);
      mpointsF[2].setX(-ws2*k);  mpointsF[2].setY(-rRoll-hs2);
      painter.drawPolyline(mpointsF, 3);

      painter.rotate(dopangle);
    }

    if(k == -1)  painter.rotate(180.0);
  }

//- 180:
  xoff = -1.0;
  angle += dopangle;  //  text.sprintf(("%.0f"),angle);
  DrawText(painter, xoff, yoff, angle); //text);
  lines << QLineF(0.0,-rRoll, 0.0,-rRoll-hs2);
  lines << QLineF(-3.0,-rRoll-hs2+4.0, 3.0,-rRoll-hs2+4.0);
  lines << QLineF(-6.0,-rRoll-hs2, 6.0,-rRoll-hs2);
  painter.drawLines(lines);  lines.clear();
  painter.rotate(-180.0);

//=====  Static:  =====

  painter.rotate(RollAngle);

//==Balance:

  pen.setColor(ColorStaticBalanceOutline);  pen.setWidthF(0.5);   painter.setPen(pen);

  linearGrad.setStart(0.0,    -3.5+1.5);
  linearGrad.setFinalStop(0.0, 3.5);
  linearGrad.setColorAt(0, ColorStaticBalance0);
  linearGrad.setColorAt(1, ColorStaticBalance1);
  QBrush brushBalance(linearGrad);   painter.setBrush(brushBalance);

//-Left:
  mpointsF[0].setX(-48.0-1.0);           mpointsF[0].setY(0.0);
  mpointsF[1].setX(-48.0-1.0-8.0);       mpointsF[1].setY(-3.5);
  mpointsF[2].setX(-48.0-1.0-8.0-24.0);  mpointsF[2].setY(-3.5);
  mpointsF[3].setX(-48.0-1.0-8.0-24.0);  mpointsF[3].setY(3.5);
  mpointsF[4].setX(-48.0-1.0-8.0);       mpointsF[4].setY(3.5);
  painter.drawPolygon(mpointsF,5);

//-Right:
  mpointsF[0].setX(48.0+1.0);
  mpointsF[1].setX(48.0+1.0+8.0);
  mpointsF[2].setX(48.0+1.0+8.0+24.0);
  mpointsF[3].setX(48.0+1.0+8.0+24.0);
  mpointsF[4].setX(48.0+1.0+8.0);
  painter.drawPolygon(mpointsF,5);

//-Center:
  linearGrad.setStart(0.0,    0.0);
  linearGrad.setFinalStop(0.0, 12.0+6.0);
  linearGrad.setColorAt(0, ColorStaticBalance0);
  linearGrad.setColorAt(1, ColorStaticBalance1);
  QBrush brushBalanceCenter(linearGrad);   painter.setBrush(brushBalanceCenter);
  mpointsF[0].setX(0.0);            mpointsF[0].setY(0.0);
  mpointsF[1].setX(-30.0);          mpointsF[1].setY(12.0);
  mpointsF[2].setX(0.0);            mpointsF[2].setY(6.0);
  mpointsF[3].setX(30.0);           mpointsF[3].setY(12.0);
  painter.drawPolygon(mpointsF,4);

//- Triangle:
  painter.setBrush(QBrush(ColorStaticTriangle));
  mpointsF[0].setX(0.0);   mpointsF[0].setY(-rRoll);
  mpointsF[1].setX(6.5);   mpointsF[1].setY(-rRoll + 11.258); //  11.258 = sqrt(3.0)/2.0 * 13.0
  mpointsF[2].setX(-6.5);  mpointsF[2].setY(-rRoll + 11.258);
  painter.drawPolygon(mpointsF,3);


//=============================
  painter.setOpacity(0.6);

  QPixmap pixmap = PEkranoplanGL->renderPixmap(); //80,80);//,false);
  pixmap.setMask(pixmap.createMaskFromColor(QColor(0,0,0, 255)));
  painter.drawPixmap(-WidthHalf+1,HeightHalf-pixmap.rect().height()-1, pixmap);
//--
  painter.end();
  QPainter paint(this);
  paint.drawImage(0,0, imgagePaint);
}
//--------------------------------------------------------------------------------------------
void MGyrohorizon::SetPitchRollAngles(float pitch, float roll)
{
  if((int)(PitchAngle*100) != (int)(pitch*100) || (int)(RollAngle*100) != (int)(roll*100))
  {
    PitchAngle = pitch;
    RollAngle = roll;

    update();
  }
}
//--------------------------------------------------------------------------------------------
void MGyrohorizon::SetQuaternion(QQuaternion* pQuaternion)
{
  PEkranoplanGL->SetQuaternion(pQuaternion);
}
