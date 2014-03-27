#ifndef MREADINESS_H
#define MREADINESS_H

#include "MReadinessTestChannel.h"
#include <QSplitter>
#include <QGraphicsView>
#include <QList>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QString>
#include <QListWidget>

class MainWindow;
//============================================================================================

class MReadiness : public QSplitter
{
    Q_OBJECT
public:
    explicit MReadiness(QWidget *parent = 0);
  virtual ~MReadiness();

private:
  QString SMessageBoxTitle;

  QGraphicsView* PViewEkranoplan;

  QList<MReadinessTestChannel*> ListReadinessTestChannels;

  QList<QGraphicsRectItem*>     ListTestRectItems;
  int TimerIDTestRectItems;
  int NRPoint;    qreal* MRPoints;
  int Alpha1;  int Alpha2;   int DAlpha;  int DIncrementAlpha;
  int* MAlphas;  int* MkAlphas;



  QPushButton* PPushButtonStart;

  QListWidget* PListWidgetEngines;
  QListWidget* PListWidgetSensors;
  QListWidget* PListWidgetServoMotors;
  QPushButton* PPushButtonTelemetry;

  bool BResultTests;
  int  ITestChannel;


  QRect FillReadinessTest(QWidget* pScrollAreaTestContents);
  QRect FillReadinessStart(QWidget* pScrollAreaStartContents);
  void CreateListWidget(const char* nameList, QListWidget*& pListWidget, QWidget* pParent, QRect rectList, QList<QString>* pListNames);

  QListWidgetItem* GetListWidgetItem(QString nameItem);
  QListWidgetItem* GetListWidgetItem(QListWidget* pListWidget, QString nameItem);

  void StartNextTestChannel();

public:
  void Init();

protected:
  virtual void timerEvent(QTimerEvent* event);

signals:
  void SignalSetTypeModeReadiness();

public slots:
  void SlotStart();
  void SlotResultTestChannel(bool bResult);
};
//============================================================================================
#endif // MREADINESS_H
