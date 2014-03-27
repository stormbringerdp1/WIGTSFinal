#ifndef MPLOT_H
#define MPLOT_H

#include "MPlot.h"

#include <QFrame>
#include <QEvent>
#include <QList>
#include <QScrollBar>

//============================================================================================
class MPlot : public QFrame
{
    Q_OBJECT
public:
    explicit MPlot(QWidget* pParent, const bool isReport, const char* cYAxisLabel, float* pminYNormalValue, float* pmaxYNormalValue);

  virtual ~MPlot();                                                                  //          2: Plot moves to the left

private:
  bool IsReport;
  QString SYAxisLabel; // QString SXAxisLabel;
  float* PMinYNormalValue;  float* PMaxYNormalValue;

  qreal XStep;

  int NYStroke, NXStroke;


  int  IxyOff, IxyOffStroke, IxyStrokeLength, IxyArrow;

  int NItemVisibleLeft;
  QScrollBar*   PScrollBar;   int  HScrollBar;
  int  PosSB;

  QRectF  RectCurve;

  QList<QPointF>  ListVisibleValues;   int NMaxVisibleCount;
  QList<QPointF>  ListAllValues;       int NAllCount;
  qreal YMinAllValue, YMaxAllValue;


  void Init();

  void Draw(QPainter& painter);

protected:
  virtual bool event(QEvent* event);
  virtual void paintEvent(QPaintEvent* event);

public:
  void AddValue(const qreal &fx, const qreal &fy);
  void Clear();

signals:

public slots:
  void SliderMoved(int pos);
};
//============================================================================================
#endif // MPLOT_H
