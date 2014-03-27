#include "MPlot.h"
#include "funcs.h"

#include <QPainter>
#include <QMessageBox>
#include <QStatusBar>

//============================================================================================

extern int TimerInterval; // msec

//============================================================================================
MPlot::MPlot(QWidget* pParent, const bool isReport, const char* cYAxisLabel, float* pminYNormalValue, float* pmaxYNormalValue)
      : QFrame(pParent),
        IsReport(isReport), SYAxisLabel(cYAxisLabel)//, MinYNormalValue(minYNormalValue), MaxYNormalValue(maxYNormalValue)
{
  PMinYNormalValue = NULL;
  if(pminYNormalValue)
  {
    PMinYNormalValue = new float;   *PMinYNormalValue = *pminYNormalValue;
  }

  PMaxYNormalValue = NULL;
  if(pmaxYNormalValue)
  {
    PMaxYNormalValue = new float;   *PMaxYNormalValue = *pmaxYNormalValue;
  }

  XStep = TimerInterval/1000.0; //1.0;  //  [sec]

  IxyOff = 5;
  IxyOffStroke = 3;    IxyStrokeLength = 7;  IxyArrow = 15;

  NYStroke = 11;  NXStroke = 7;

  NMaxVisibleCount = 31;

  RectCurve = QRectF(0.0,0.0,1.0,1.0);

  if(IsReport)
  {
    PScrollBar = new QScrollBar(this);
    PScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
    PScrollBar->setOrientation(Qt::Horizontal);

    connect(PScrollBar,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(SliderMoved(int)));
  }

  Init();
}
//--------------------------------------------------------------------------------------------
MPlot::~MPlot()
{
  delete PMinYNormalValue;
  delete PMaxYNormalValue;
}
//--------------------------------------------------------------------------------------------
void MPlot::Init()
{
  YMinAllValue = std::numeric_limits<qreal>::max();  YMaxAllValue = std::numeric_limits<qreal>::min();
  PosSB = 0;
  if(IsReport)  PScrollBar->setRange(0,0);
}
//--------------------------------------------------------------------------------------------
bool MPlot::event(QEvent* event)
{
  bool b = QFrame::event(event); //catch different events
//--
  if(event->type() == QEvent::Polish)
  {//Polish is caught
    setObjectName(QStringLiteral("frame"));
    setFrameShape(QFrame::Box);


    if(!IsReport)  HScrollBar = 0;
    else
    {         HScrollBar = 13;
      QRect rect = geometry();
      PScrollBar->setGeometry(QRect(1, rect.height()-HScrollBar-1, rect.width()-1-1, HScrollBar));
    }

  }
  return b;
}
//--------------------------------------------------------------------------------------------
void MPlot::SliderMoved(int pos)
{
  PosSB = pos;
  update();
}
//--------------------------------------------------------------------------------------------
void MPlot::paintEvent(QPaintEvent* event)
{
  QFrame::paintEvent(event);

  //create a QPainter and pass a pointer to the device.
  //A paint device can be a QWidget, a QPixmap or a QImage
  QPainter painter(this);

  Draw(painter);
}
//--------------------------------------------------------------------------------------------
void MPlot::Draw(QPainter& painter)
{
  painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
  QFont font(font());  font.setStyleStrategy(QFont::NoAntialias);
  painter.setFont(font);

//f - величины функциональные, нет f - величины пиксельные
  qreal fyMinValue, fyMaxValue; // fxMinValue, fxMaxValue,

  QList<QPointF>* pListValues = !IsReport ? &ListVisibleValues : &ListAllValues;

  int count = pListValues->count();
  if(count == 0)
  {
      fyMinValue = 0.0;  fyMaxValue = 0.0;  /*fxMinValue = 0.0;  fxMaxValue = 0.0;*/   return; }
  else
  {
    NItemVisibleLeft = 0;
    if(IsReport && ListAllValues.count() > NMaxVisibleCount) {
      qreal dgx = RectCurve.width()/(NMaxVisibleCount-1);//delta x, distance between points
      int sbMax = dgx*(ListAllValues.count()-NMaxVisibleCount) + 0.5;//scrool bar definition
      PScrollBar->setMaximum(sbMax);
      PScrollBar->setPageStep(RectCurve.width()+0.5);
      NItemVisibleLeft = PosSB/dgx;// + 0.5;
    }

    QPointF fp;
    fyMinValue = std::numeric_limits<qreal>::max();  fyMaxValue = std::numeric_limits<qreal>::min();
    for(int i = NItemVisibleLeft; i < qMin(NItemVisibleLeft+NMaxVisibleCount, count); i++)
    {
      fp = (*pListValues)[i];
      if(fp.y() < fyMinValue)  fyMinValue = fp.y();
      if(fp.y() > fyMaxValue)  fyMaxValue = fp.y();
    }

    if(IsReport)
    {
      YMinAllValue = qMin(fyMinValue, YMinAllValue);   fyMinValue = YMinAllValue;
      YMaxAllValue = qMax(fyMaxValue, YMaxAllValue);   fyMaxValue = YMaxAllValue;
    }
  }

  qreal dfYMax = 0.0;
  if(PMaxYNormalValue)
  {
    if(fyMaxValue < *PMaxYNormalValue)  fyMaxValue = *PMaxYNormalValue;
    dfYMax = fyMaxValue - *PMaxYNormalValue;
  }
  qreal dfYMin = 0.0;
  if(PMinYNormalValue)
  {
    if(*PMinYNormalValue < fyMinValue)  fyMinValue = *PMinYNormalValue;
    dfYMin = *PMinYNormalValue - fyMinValue;
  }


//====  Calc Offs:  ====

  QString s;  QRectF rectFLabel;   qreal gwMaxLabel = 0.0;  qreal ghMaxLabel = 0.0;

  qreal dfy = (fyMaxValue-fyMinValue)/(NYStroke-1);
  //рисуем y ось с числами
  for(int i = 0; i < NYStroke; i++) {
    s.sprintf("%.3g", fyMinValue + i*dfy);
    rectFLabel = painter.boundingRect(QRectF(0,0, 500,500), s, QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
    gwMaxLabel = qMax(gwMaxLabel, rectFLabel.width());
    ghMaxLabel = qMax(ghMaxLabel, rectFLabel.height());
  }
  //здвиги осей
  qreal gxOffAxisY = IxyOff + gwMaxLabel + IxyOffStroke + IxyStrokeLength/2.0;
  qreal gyOffAxisX = IxyOff + ghMaxLabel + IxyOffStroke + IxyStrokeLength/2.0 + HScrollBar;

  QRect rectPlot = geometry();
  qreal gy0Top = IxyOff+IxyArrow;  qreal gy0Bottom = rectPlot.height() - gyOffAxisX;
  qreal gx0Left = gxOffAxisY;      qreal gx0Right = rectPlot.width() - IxyOff - IxyArrow;

  RectCurve = QRectF(QPointF(gxOffAxisY, gy0Top), QPointF(gx0Right, gy0Bottom));
  QPointF gxy0LeftBottom(gx0Left, gy0Bottom);
  qreal gxWidth = RectCurve.width();
  qreal gyHeight = RectCurve.height();

  painter.drawText(QRectF(gxOffAxisY+10, IxyOff-2, 300,30), SYAxisLabel, QTextOption(Qt::AlignLeft | Qt::AlignTop));
  painter.drawText(QRectF(QPoint(0,0), QPoint(rectPlot.width()-1-IxyOff, gy0Bottom-5)), "T", QTextOption(Qt::AlignRight | Qt::AlignBottom));


//====  Draw Axes, Grid:  ====

  QPen penAxis(QBrush(QColor(0,0,0)), 1, Qt::SolidLine, Qt::RoundCap);
  QPen penGrid(QBrush(QColor(89,89,89)), 0.25, Qt::SolidLine); //Qt::DotLine);

//=  Y Axis:
  QPointF pxy2(gx0Left, IxyOff);
  painter.setPen(penAxis);    painter.drawLine(gxy0LeftBottom, pxy2);
//=  Y Axis' Arrow:
  QPointF p(pxy2);  p += QPointF(-3, 10);   painter.drawLine(pxy2, p);
  p = pxy2;         p += QPointF(3, 10);    painter.drawLine(pxy2, p);
//=  Y Axis' Labels, Strokes, Grid:
  qreal gy;  qreal fy;
  qreal dgy = gyHeight/(NYStroke-1);
  qreal gx1Stroke = gx0Left - IxyStrokeLength/2.0;
  qreal gx2Stroke = gx0Left + IxyStrokeLength/2.0;
  for(int i = 0; i < NYStroke; i++)
  {
    gy = gy0Bottom - i*dgy;
    painter.setPen(penAxis);    painter.drawLine(QPointF(gx1Stroke, gy), QPointF(gx2Stroke, gy));
    painter.setPen(penGrid);    painter.drawLine(QPointF(gx2Stroke, gy), QPointF(gx0Right, gy));

    fy = fyMinValue + i*dfy;  s.sprintf("%.3g", fy);
    painter.drawText(QRectF(IxyOff, gy-ghMaxLabel/2.0, gwMaxLabel, ghMaxLabel),
                     s, QTextOption(Qt::AlignRight | Qt::AlignVCenter));
  }

//=  X Axis:
  pxy2 = QPointF(rectPlot.width()-IxyOff , gy0Bottom);
  painter.setPen(penAxis);
  painter.drawLine(gxy0LeftBottom, pxy2);

//=  X Axis' Arrow:
  p = pxy2;  p += QPointF(-10, -3);   painter.drawLine(pxy2, p);
  p = pxy2;  p += QPointF(-10, 3);    painter.drawLine(pxy2, p);

//=  X Axis' Labels, Strokes, Grid:
  painter.setPen(penGrid);    painter.drawLine(QPointF(gx0Right, gy0Bottom), QPointF(gx0Right, gy0Top));  // <<--- Right DotLine
  qreal dgx = gxWidth/(NXStroke-1);
  int j = PosSB/dgx;
  int delLeft = (-PosSB/dgx + j)*dgx;
  qreal gy2Stroke = gy0Bottom - IxyStrokeLength/2.0;
  qreal gy1Stroke = gy0Bottom + IxyStrokeLength/2.0;
  qreal dfx = (XStep*(NMaxVisibleCount-1))/(NXStroke-1);
  qreal gx;  qreal fx;

  for(int i = 0; i < NXStroke; i++)
  {
    gx = gx0Left + delLeft + i*dgx;
    if(gx < gx0Left)  continue;
    painter.setPen(penAxis);    painter.drawLine(QPointF(gx, gy1Stroke), QPointF(gx, gy2Stroke));
    painter.setPen(penGrid);    painter.drawLine(QPointF(gx, gy2Stroke), QPointF(gx, gy0Top));
    fx = (*pListValues)[0].x() + (j+i)*dfx;  s.sprintf("%.1f", fx);
    rectFLabel = painter.boundingRect(QRectF(0,0, 500,500), s, QTextOption());
    painter.drawText(QRectF(gx-rectFLabel.width()/2.0, rectPlot.height()-IxyOff-HScrollBar-rectFLabel.height(),
                            rectFLabel.width(),        rectFLabel.height()),
                     s, QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
  }


//===  Y Normal Range:  ===//рисуем две линии красные

  qreal ky = 0.0;  qreal d;
  if((d = fyMaxValue-fyMinValue) != 0.0)  ky = RectCurve.height()/d;

  QPen penRange(QBrush(QColor(255,0,0)), 1, Qt::SolidLine, Qt::RoundCap);
  painter.setPen(penRange);

  qreal gyMax = 0.0;
  if(PMaxYNormalValue)
  {
    gyMax = gy0Top + ky*(fyMaxValue - *PMaxYNormalValue);
    painter.drawLine(QPointF(gx0Left+1.0, gyMax), QPointF(gx0Right-1.0, gyMax));
  }
  qreal gyMin = 0.0;
  if(PMinYNormalValue) {
    gyMin = gy0Top + ky*(fyMaxValue - *PMinYNormalValue);
    painter.drawLine(QPointF(gx0Left+1.0, gyMin), QPointF(gx0Right-1.0, gyMin));
  }


//====  Draw Curve:  ====

  if(count == 0)  return;

  QPen penPoint(QBrush(QColor(0,0,0)), 6, Qt::SolidLine, Qt::RoundCap); //  QColor(0,0,0) <-- here any
  QPen penLine(QBrush(QColor(0,0,250)), 1, Qt::DotLine);

  QPointF fp;
  QPointF gp1,gp2;

  dgx = gxWidth/(NMaxVisibleCount-1);
  j = PosSB/dgx;
  delLeft = (-PosSB/dgx + j)*dgx;   bool isNornalValue;
  for(int i = NItemVisibleLeft; i < qMin(NItemVisibleLeft+NMaxVisibleCount+1, count); i++)
  {
    gx = gx0Left + delLeft + (i-NItemVisibleLeft)*dgx;
    fp = (*pListValues)[i];

    isNornalValue = true;
    if(PMinYNormalValue)
    {
      if(fp.y() < *PMinYNormalValue)  isNornalValue = false;
    }
    if(PMaxYNormalValue)
    {
      if(*PMaxYNormalValue < fp.y())  isNornalValue = false;
    }
    if(isNornalValue)  penPoint.setBrush(QColor(0,200,60));
    else               penPoint.setBrush(QColor(200,0,60));

    gy = gy0Top + ky*(fyMaxValue-fp.y());
    if(i == NItemVisibleLeft)
    {
        gp1 = QPointF(gx,gy);
      if(gx >= gx0Left && gx <= gx0Right)
      {
        painter.setPen(penPoint);   painter.drawPoint(gp1);
      }
    }
    else
    {   gp2 = QPointF(gx,gy);
      painter.setPen(penLine);
      if(gp1.x() < gx0Left)
      {
        qreal x1 = gp1.x();
        gp1.setX(gx0Left);
        gp1.setY(gp1.y()+(gx0Left-x1)*(gp2.y()-gp1.y())/(gp2.x()-x1));
      }
      if(gp2.x() > gx0Right)
      {
        qreal x2 = gp2.x();
        gp2.setX(gx0Right);
        gp2.setY(gp1.y()+(gx0Right-gp1.x())*(gp2.y()-gp1.y())/(x2-gp1.x()));
      }
      painter.drawLine(gp1, gp2);

      gp1 = gp2;
      if(gx >= gx0Left && gx <= gx0Right)
      {
        painter.setPen(penPoint);   painter.drawPoint(gp2);
      }
    }
  }

//===  Y UnNormal Ranges:  === рисуем красную зону

  if(dfYMax > 0.0 || dfYMin > 0.0)
  {
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(255,100,100, 50)));
    if(dfYMax > 0.0)  painter.drawRect(QRectF(gx0Left,gy0Top, gxWidth+1.0,gyMax-gy0Top));
    if(dfYMin > 0.0)  painter.drawRect(QRectF(gx0Left-1.0,gyMin, gxWidth+2.0,gy0Bottom-gyMin));
  }
}
//--------------------------------------------------------------------------------------------
void MPlot::AddValue(const qreal& fx, const qreal& fy)
{
  int count = ListVisibleValues.count();
  if(count >= NMaxVisibleCount)
  {
    ListVisibleValues.removeFirst();
  }
  QPointF fp(fx,fy);
  ListVisibleValues << fp;
  if(IsReport)
  {
    ListAllValues << fp;
  }

  update();
}
//--------------------------------------------------------------------------------------------
void MPlot::Clear()
{
  ListVisibleValues.clear();
  if(IsReport)  ListAllValues.clear();

  Init();

  update();
}
//--------------------------------------------------------------------------------------------
