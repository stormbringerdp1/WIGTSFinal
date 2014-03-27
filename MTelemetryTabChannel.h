#ifndef MTELEMETRYTABCHANNEL_H
#define MTELEMETRYTABCHANNEL_H

#include "MPlot.h"

#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QTimer>

//============================================================================================
class MTelemetryTabChannel : public QWidget
{
    Q_OBJECT
public:
    explicit MTelemetryTabChannel(QWidget* parent, QList<QString>* pListParamsTelemetryChannel, int iFirstItemListParamsTelemetry);
  virtual ~MTelemetryTabChannel();

  QRect GetRectChannel() { return RectChannel; }

protected:
  virtual bool event(QEvent* pEvent);

private:
  QList<QString>* PListParamsTelemetryChannel;
  int             IFirstItemListParamsTelemetry;

  QList<float*> ListPMinNormalValues;   QList<float*> ListPMaxNormalValues;

  QRect RectChannel;

  QList<QLineEdit*> ListLineEditCurrentParams;

  QList<MPlot*> ListPlots;
  QList<MPlot*> ListCorrespondingReportPlots;

  QList<QLineEdit*> ListCorrespondingInfoParams;

  bool   IsArduinoValues;
  float* MValues;

  QTimer Timer;
  bool IsFlicker;            bool IsFlickerBrush;
  QList<bool> ListIsNonNormalMaxValue;  QList<bool> ListIsNonNormalMinValue;


  void UpdateTabBar();

  void GetTempValues(const char* pCharsValues, const char* charsCodeValues, const int nvalues, float* tempValues);


public:
  const QList<MPlot*>* GetPListPlots() { return &ListPlots; }

  QString* GetPNameChannel() { return &(*PListParamsTelemetryChannel)[0]; }
  QString* GetPNameParamInChannel(int j) { return &(*PListParamsTelemetryChannel)[IFirstItemListParamsTelemetry + j*3]; }

  QString* GetPStringMinNormalValue(int j) { return &(*PListParamsTelemetryChannel)[IFirstItemListParamsTelemetry + j*3 + 1]; }
  QString* GetPStringMaxNormalValue(int j) { return &(*PListParamsTelemetryChannel)[IFirstItemListParamsTelemetry + j*3 + 2]; }

  void AddAssignCorrespondingReportPlot(MPlot* pCorrespondingReportPlot);
  void AddAssignCorrespondingInfoParam(QLineEdit* pLineEditInfoParam);

  bool GetIsFlicker(bool* pIsColorBrushFlicker = NULL, QList<bool>** ppListIsNonNormalMinValue = NULL, QList<bool>** ppListIsNonNormalMaxValue = NULL);
  void StopFlicker();

  void ClearPlots();


signals:
  void SignalAddSlotTelemetryTabChannel(MTelemetryTabChannel* pTelemetryTabChannel, bool isdArduinoValues);
  void SignalChannelTelemetryDoneParam(MTelemetryTabChannel* pTabChannel, qreal dt, float value, bool isNonNormal, bool isLastParamInChannel);
  void SignalUpdateTabBar();

public slots:
  void SlotTimerSignal();
  void SlotTakeTelemetryStrValues(MTelemetryTabChannel* pTelemetryTabChannel, const char* pCharsValues);
};
//============================================================================================
#endif // MTELEMETRYTABCHANNEL_H
