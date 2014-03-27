#ifndef MARDUINOCONNECTOR_H
#define MARDUINOCONNECTOR_H

#include "MTelemetryTabChannel.h"

#include <QObject>
#include <QList>
#include <QString>
#include <QtSerialPort/QtSerialPort>

//============================================================================================
class MArduinoConnector : public QObject
{
    Q_OBJECT
public:
    explicit MArduinoConnector(QObject *parent = 0);
  virtual ~MArduinoConnector();

private:

  QList<MTelemetryTabChannel*>  ListPTelemetryChannels;
  QList<bool>                   ListIsTelemetryArduinoValues;

  QSerialPort* PPort;

  QString* PStringMessage;

  bool IsTypeModeReadiness;
  bool IsTypeModeTelemetry;

  bool IsTakeTelemetryStrValues;
  void InitPort();

public:

signals:
  void SignalMessage(QString* pStr);
  void SignalTakeReadinessTestValue(float result);
  void SignalTakeTelemetryStrValues(MTelemetryTabChannel* pTelemetryTabChannel, const char* pCharsValues);

public slots:
  void SlotPortError(QSerialPort::SerialPortError ierror);
  void SlotAddSlotTelemetryTabChannel(MTelemetryTabChannel* pTelemetryTabChannel, bool isdArduinoValues);

  void SlotSetTypeModeReadiness();
  void SlotSetTypeModeTelemetry();

  void SlotStopTakingData();

  void SlotGetReadinessTestValue(QString* psNameChannel, QString* psInputTestName, QString* psInputTestValue, QString* psOutputTestName);
  void SlotGetAllTelemetryValues();

  void SlotReadyRead();
};
//============================================================================================

#endif // MARDUINOCONNECTOR_H
