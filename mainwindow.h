#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MReadiness.h"
#include "MTelemetry.h"
#include "MArduinoConnector.h"

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QThread>

  #include <QTimer>

//============================================================================================

namespace Ui {
class MainWindow;
}

//============================================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
//--
private:
  MReadiness*           PReadiness;
  MTelemetry*           PTelemetry;

  QList<QString>        ListNamesEngines;
  QList<QString>        ListNamesSensors;
  QList<QString>        ListNamesServoMotors;

  QList<QList<QString>> ListParamsReadinessChannels;  int IFirstItemListParamsReadiness;  int DeltaNItemListParamsReadiness;
  QList<QRectF>         ListTestRectsReadinessChannels;
  QList<QList<QString>> ListParamsTelemetryChannels;  int IFirstItemListParamsTelemetry;//  int DeltaNItemListParamsTelemetry;
                                                      int ILongeronsTelemetryChannel;
  QList<QList<QString>> ListParamsTelemetryStates;    int IFirstItemListParamsStates;

  QTimer Timer;
  qint64 TimeStartTelemetry;

  void CreateArduinoConnector();  void DeleteArduinoConnector();
  void InitLists();
  void SwitchSplitterTo(QWidget* pWidget);


  MArduinoConnector* PArduinoConnector;
  QThread            ThreadArduinoConnector;

public:

  void Init();

  QList<QString>* GetPListNamesEngines() { return &ListNamesEngines; }
  QList<QString>* GetPListNamesSensors() { return &ListNamesSensors; }
  QList<QString>* GetPListNamesServoMotors() { return &ListNamesServoMotors; }

  QList<QList<QString>>* GetPListParamsReadinessChannels() { return &ListParamsReadinessChannels; }
  const int GetIFirstItemListParamsReadiness() { return IFirstItemListParamsReadiness; }
  const int GetDeltaNItemListParamsReadiness() { return DeltaNItemListParamsReadiness; }
  QList<QRectF>* GetPListTestRectsReadinessChannels() { return &ListTestRectsReadinessChannels; }


  QList<QList<QString>>* GetPListParamsTelemetryChannels() { return &ListParamsTelemetryChannels; }
  const int GetIFirstItemListParamsTelemetry() { return IFirstItemListParamsTelemetry; }
  const int GetILongeronsTelemetryChannel() { return ILongeronsTelemetryChannel; }

  QList<QList<QString>>* GetPListParamsTelemetryStates() { return &ListParamsTelemetryStates; }
  const int GetIFirstItemListParamsStates() { return IFirstItemListParamsStates; }

  MArduinoConnector* GetPArduinoConnector() { return PArduinoConnector; }

  MTelemetry* GetPTelemetry() { return PTelemetry; }

  QTimer& GetTimer() { return Timer; }

  void  SetTimeStartTelemetry(const qint64 timeStartTelemetry) { TimeStartTelemetry = timeStartTelemetry; }
  const qint64 GetTimeStartTelemetry()                          { return TimeStartTelemetry; }


protected:
  virtual bool event(QEvent* event);

public:

signals:

public slots:
  void SlotMessage(QString* pStr);
  void SlotSwitchToReadiness();
  void SlotSwitchToTelemetry();
};
//============================================================================================
#endif // MAINWINDOW_H
