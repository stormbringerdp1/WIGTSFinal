#ifndef MTELEMETRY_H
#define MTELEMETRY_H

#include "MTelemetryTabWidgetChannels.h"
#include "MTelemetryTabWidgetReports.h"
#include "MTelemetryTabChannel.h"
#include "MEkranoplanGL.h"
#include "MGyrohorizon.h"

#include <QSplitter>
#include <QTabWidget>
#include <QPushButton>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

class MainWindow;
//============================================================================================

class MTelemetry : public QSplitter
{
    Q_OBJECT
public:
    explicit MTelemetry(QWidget *parent = 0);
  virtual ~MTelemetry();

private:
  QString SMessageBoxTitle;

  MGyrohorizon* PGyrohorizon;

  MTelemetryTabWidgetChannels* PTabWidgetChannels;
  MTelemetryTabWidgetChannels* PTabWidgetStates;
  MTelemetryTabWidgetReports*  PTabWidgetReports;

  bool IsTelemetryStarted;
  int  CountAllTelemetryChannels;
  bool IsReadyNextCycle;
  int  ICurrentAllChannel;

  MEkranoplanGL* PEkranoplanGL;
  QPushButton* PPushButtonStartTelemetry;
  QPushButton* PPushButtonStopTelemetry;

  QPushButton* PPushButtonReadiness;

  QRect FillTelemetryGyrohorizon(QWidget* pScrollAreaGyrohorizonContents);
  QRect FillTelemetryChannels(QWidget* pScrollAreaChannelsContents);
  QRect FillTelemetryStates(QWidget* pScrollAreaStatesContents);
  QRect FillTelemetryReports(QWidget* pScrollAreaReportsContents);

  QRect AddInfoPanel(QWidget* pScrollAreaGyrohorizonContents);
  void  AssignCorrespondingChannelInfoParam(QString& sInfoParam, QLineEdit* pLineEditInfoParam);

  void SwitchActiveTabWidgetFromTo(QTabWidget* pTabWidgetFrom, QTabWidget* pTabWidgetTo);

  QString         NameFileReport;
  QFile*          PFileReport;
  QTextStream*    PStreamFileReport;
  QList<int>      ListLengthAllParams;
  QList<QString*> ListPStrParamsFileLines;

  int             NAllWritingParams;
  int             IWritingParamInChannel;

  void PrepareFileReport(QDateTime& currentDataTime);
  void SaveTempStrParamsFileLine(int indexAllTelemetryChannels, qreal dt, float value, bool isNonNormal, bool isLastParamInChannel);
  int  GetIWritingAllParams(int indexAllTelemetryChannels);
  void WriteStrParamsFileLine();
  void CloseFileReport();

  int GetIndexAllTelemetryChannels(MTelemetryTabChannel* pTabChannel);


public:
  void Init();

  void ActivateTabWidgetStates();
  void ActivateTabWidgetReports();

  MGyrohorizon* GetPGyrohorizon() { return PGyrohorizon; }

  MTelemetryTabChannel* GetCurrentPTabChannel(int indexAllChannel);

  MTelemetryTabWidgetChannels* GetPTabWidgetChannels() { return PTabWidgetChannels; }
  MTelemetryTabWidgetChannels* GetPTabWidgetStates()   { return PTabWidgetStates;   }

  const QString* GetCurrentPNameFileReport() { return &NameFileReport; }

signals:
  void SignalSetTypeModeTelemetry();
  void SignalGetAllTelemetryValues();
  void SignalStopTakingData();

public slots:
  void SlotStartTelemetry();
  void SlotStopTelemetry();
  void SlotTimerSignal();
  void SlotChannelTelemetryDoneParam(MTelemetryTabChannel* pTabChannel, qreal dt, float value, bool isNonNormal, bool isLastParamInChannel);
};
//============================================================================================
#endif // MTELEMETRY_H
