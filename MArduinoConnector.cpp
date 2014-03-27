#include "MArduinoConnector.h"
#include "funcs.h"

  #include <QThread>
  #include <QApplication>

//============================================================================================
//==== Arduino Params - the same as in "sketch.ino": ====

const char CharTypeModeReadiness = 'R';
const char CharTypeModeTelemetry = 'T';
//const char CharSendTelemetryNextCharsValues = 'V';
const char CharStopSendingData = 'S';

char CharWriteTelemetryModeOccasionally = 'o';

//= For Altitude and Speed TabChannel "H, S":
int   IndexTabChannel_HS = 5;
const char* CharsCodeHeightSpeed = "hs2 ";

//= For Pitch and Roll TabChannel "Angles":
int   IndexTabChannel_Angles = 6;
const char* CharsCodeQuaternion = "q4 ";

//= For Temperature and Pressure TabChannel "T, P":
int   IndexTabChannel_TP = 8;
const char* CharsCodeTemperaturePressure = "tp2 ";

char CharSeparator = ' ';

//====

const char* NameDevice = "Arduino Uno";

extern QString SNameTelemetryTabChannel_HS;
extern QString SNameTelemetryTabChannel_Angles;
extern QString SNameTelemetryTabChannel_TP;

//============================================================================================
MArduinoConnector::MArduinoConnector(QObject *parent) :
    QObject(parent)
{
  PPort = NULL;

  PStringMessage = new QString();

  IsTypeModeReadiness = false;
  IsTypeModeTelemetry = false;

  IsTakeTelemetryStrValues = false;

  connect(this,
          SIGNAL(SignalMessage(QString*)),
          GetPMainWnd(),
          SLOT(SlotMessage(QString*)));

  InitPort();
}
//--------------------------------------------------------------------------------------------
MArduinoConnector::~MArduinoConnector()
{
  if(PPort)
  {
    if(PPort->isOpen())
    {
      SlotStopTakingData();
      PPort->clear();  PPort->close();
    }
    delete PPort;  PPort = NULL;
  }

  delete PStringMessage;  PStringMessage = NULL;
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotAddSlotTelemetryTabChannel(MTelemetryTabChannel* pTelemetryTabChannel, bool isdArduinoValues)
{
  ListPTelemetryChannels << pTelemetryTabChannel;
  ListIsTelemetryArduinoValues << isdArduinoValues;
  connect(this,
          SIGNAL(SignalTakeTelemetryStrValues(MTelemetryTabChannel*,const char*)),
          pTelemetryTabChannel,
          SLOT(SlotTakeTelemetryStrValues(MTelemetryTabChannel*,const char*)));


  disconnect(pTelemetryTabChannel,
             SIGNAL(SignalAddSlotTelemetryTabChannel(MTelemetryTabChannel*,bool)),
             this,
             SLOT(SlotAddSlotTelemetryTabChannel(MTelemetryTabChannel*,bool)));
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::InitPort()
{
qsrand(1234578);
  PStringMessage->clear();

  QString portName;//("COM4");
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
  {
    if(info.description() == NameDevice)
    {
      portName = info.portName();
      *PStringMessage = info.description() + ":\n"
                      + QObject::tr("     Port: ") + info.portName() + "\n"
                      + QObject::tr("     Location: ") + info.systemLocation() + "\n"
                      + QObject::tr("     Manufacturer: ") + info.manufacturer() + "\n"
                      + QObject::tr("     Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                      + QObject::tr("     Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                      + QObject::tr("     Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
      break;
    }
  }

  if(portName.isEmpty())
  {
    *PStringMessage = NameDevice;  *PStringMessage += " is not connected.";
  }
  else
  {
    PPort = new QSerialPort(this);

    PPort->setPortName(portName);
    if(PPort->open(QIODevice::ReadWrite))
    {
      PPort->setBaudRate(QSerialPort::Baud9600); //38400);
      PPort->setDataBits(QSerialPort::Data8);
      PPort->setParity(QSerialPort::NoParity);
      PPort->setStopBits(QSerialPort::OneStop);
      PPort->setFlowControl(QSerialPort::NoFlowControl); // QSerialPort::SoftwareControl

      PPort->setReadBufferSize(64);
      *PStringMessage += "\nPort \""+portName+"\" is open.\n";

      PPort->clear();
      connect(PPort, SIGNAL(readyRead()), SLOT(SlotReadyRead()));
    }
    else
    {
      *PStringMessage += "\nPort \""+portName+"\" is not open.\n";

      delete PPort;  PPort = NULL;
    }
  }

  emit SignalMessage(PStringMessage);
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotSetTypeModeReadiness()
{
  IsTypeModeReadiness = true;
  IsTypeModeTelemetry = false;

  if(PPort)  PPort->write(&CharTypeModeReadiness, 1);
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotGetReadinessTestValue(QString* psNameChannel, QString* psInputTestName, QString* psInputTestValue, QString* psOutputTestName)
{
QThread::sleep(2); //unsigned long secs) [static]
  emit SignalTakeReadinessTestValue(10.0);
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotSetTypeModeTelemetry()
{
  IsTypeModeReadiness = false;
  IsTypeModeTelemetry = true;

  IsTakeTelemetryStrValues = true;

  if(PPort)  PPort->write(&CharTypeModeTelemetry, 1);
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotStopTakingData()
{
  if(PPort)  PPort->write(&CharStopSendingData, 1);

  IsTakeTelemetryStrValues = false;
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotGetAllTelemetryValues()
{
  if(!IsTakeTelemetryStrValues) return;

  QString strValues;  float value;  QString s;

  for(int i = 0; i < ListPTelemetryChannels.count(); i++)
  {

    if(ListIsTelemetryArduinoValues[i])
    {

      if(PPort == NULL)
      {

        if(*ListPTelemetryChannels[i]->GetPNameChannel() == SNameTelemetryTabChannel_HS)
        {
//== Altitude and Speed:
//= "5oas2 12 28.123\n"
          Q_ASSERT(i == IndexTabChannel_HS);
          strValues.sprintf("%d",IndexTabChannel_HS);                        // "5"
          strValues += CharWriteTelemetryModeOccasionally;                   // "o"
          strValues += CharsCodeHeightSpeed;                                 // "hs2 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;   strValues += CharSeparator;                      // "12 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;                                                    // "28.123"
        }
        else if(*ListPTelemetryChannels[i]->GetPNameChannel() == SNameTelemetryTabChannel_Angles)
        {
//== Angles (Quaternion):
//= "6oq4 0.123 -4.567 8.910 11.121\n"
          Q_ASSERT(i == IndexTabChannel_Angles);
          strValues.sprintf("%d",IndexTabChannel_Angles);                    // "6"
          strValues += CharWriteTelemetryModeOccasionally;                   // "o"
          strValues += CharsCodeQuaternion;                                  // "q4 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;   strValues += CharSeparator;                      // "0.123 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;   strValues += CharSeparator;                      // "-4.567 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;   strValues += CharSeparator;                      // "8.910 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;                                                    // "11.121"
        }
        else if(*ListPTelemetryChannels[i]->GetPNameChannel() == SNameTelemetryTabChannel_TP)
        {
//= Temperature and Pressure:
// "8otp2 25 101850"
          Q_ASSERT(i == IndexTabChannel_TP);
          strValues.sprintf("%d",IndexTabChannel_TP);                        // "8"
          strValues += CharWriteTelemetryModeOccasionally;                   // "o"
          strValues += CharsCodeTemperaturePressure;                         // "tp2 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;   strValues += CharSeparator;                      // "25 "
          value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
          strValues += s;                                                    // "101850"
        }
        else
        {
          *PStringMessage = "Непонятка.\n";
          *PStringMessage += "void MArduinoConnector::SlotGetAllTelemetryValues()\n";
          *PStringMessage += "Этого сообщения быть НЕ ДОЛЖНО!";
          emit SignalMessage(PStringMessage);
          return;
        }

        char* pCharsValues = new char[64];
        for(int j = 0; j < strValues.length(); j++) { pCharsValues[j] = strValues.toStdString().c_str()[j]; }
        pCharsValues[strValues.length()] = '\0';
        emit SignalTakeTelemetryStrValues(ListPTelemetryChannels[i], pCharsValues);
      }
    }

    else
    {
      value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
      strValues  = s;   strValues += CharSeparator;                      // "15.678 "
      value = ((qrand()*1.0)/RAND_MAX)*100.0;   s.sprintf("%.3f",value);
      strValues += s;                                                    // "10.185"
      char* pCharsValues = new char[64];
      for(int j = 0; j < strValues.length(); j++) { pCharsValues[j] = strValues.toStdString().c_str()[j]; }
      pCharsValues[strValues.length()] = '\0';

      emit SignalTakeTelemetryStrValues(ListPTelemetryChannels[i], pCharsValues);
    }
  }

  if(PPort)  PPort->write(&CharWriteTelemetryModeOccasionally, 1);
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotReadyRead()
{
  if(PPort->canReadLine()) {
    char* pCharsValues = new char[64];
    int count = (int)PPort->readLine(pCharsValues, 63); //PPort->bytesAvailable());
//=Read Data:

      if(IsTypeModeTelemetry)
      {

        emit SignalTakeTelemetryStrValues(ListPTelemetryChannels[QChar(pCharsValues[0]).digitValue()], pCharsValues);
      }

      else if(IsTypeModeReadiness)
      {
//== Readiness Data:
      }
  }
}
//--------------------------------------------------------------------------------------------
void MArduinoConnector::SlotPortError(QSerialPort::SerialPortError ierror)
{
  if(ierror == 0)  return;
  PStringMessage->sprintf("ierror = %d\n\n", ierror);  *PStringMessage += PPort->errorString();
  emit SignalMessage(PStringMessage);
}
//--------------------------------------------------------------------------------------------
