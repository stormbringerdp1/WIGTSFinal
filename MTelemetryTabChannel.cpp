#include "MTelemetryTabChannel.h"
#include "MArduinoConnector.h"
#include "funcs.h"

#include <QTabWidget>
#include <QTabBar>
#include <QtGlobal>
#include <QDateTime>
#include <QMessageBox>

//===
extern char CharWriteTelemetryModeOccasionally; // = 'o';

//= For Altitude and Speed TabChannel "H, S":
extern int   IndexTabChannel_HS; // = 5;
extern const char* CharsCodeHeightSpeed; // = "hs2 ";

//= For Pitch and Roll TabChannel "Angles":
extern int   IndexTabChannel_Angles; // = 6;
extern const char* CharsCodeQuaternion; // = "q4 ";

//= For Temperature and Pressure TabChannel "T, P":
extern int   IndexTabChannel_TP; // = 8;
extern const char* CharsCodeTemperaturePressure; // = "tp2 ";

extern char CharSeparator; // = ' ';
//===

extern QString SNameTelemetryTabChannel_HS;
extern QString SNameTelemetryTabChannel_Angles;
extern QString SNameTelemetryTabChannel_TP;

//--------------------------------------------------------------------------------------------
void GetYawPitchRoll(float* ypr, float* q)
{
//---
  q[2] = -q[2];

//=Yaw:
  ypr[0] = atan2(-2.0*(q[1]*q[2]-q[0]*q[3]), 2.0*(q[1]*q[1]+q[0]*q[0])-1.0)*180.0/M_PI;

//=Roll:
  float qx = -2.0*(q[2]*q[3]-q[1]*q[0]);
  float qy = 2.0*(q[3]*q[3]+q[0]*q[0])-1.0;
  float q2 = atan2(qx, qy);
  ypr[2] = q2*180.0/M_PI;

//=Pitch:
  float q1 = 2.0*(q[1]*q[3]+q[2]*q[0]);
  if((45.0 <= ypr[2] && ypr[2] <= 135.0) || (-135.0 <= ypr[2] && ypr[2] <= -45.0)) {
    ypr[1] = atan2(q1, qx/sin(q2));
  }
  else {
    ypr[1] = atan2(q1, qy/cos(q2));
  }
  ypr[1] = ypr[1]*180.0/M_PI;

//---
  q[2] = -q[2];
  q[1] = -q[1];
}
//============================================================================================
MTelemetryTabChannel::MTelemetryTabChannel(QWidget* parent, QList<QString>* pListParamsTelemetryChannel, int iFirstItemListParamsTelemetry)
                    : QWidget(parent),
                      PListParamsTelemetryChannel(pListParamsTelemetryChannel), IFirstItemListParamsTelemetry(iFirstItemListParamsTelemetry)
{
  QTabWidget* pTabWidget = qobject_cast<QTabWidget*>(parent);
  Q_ASSERT(pTabWidget != NULL); // Q_CHECK_PTR(pTabWidgetChannels);
  QTabBar* pTabBar = pTabWidget->tabBar();
  Q_ASSERT(pTabBar != NULL); // Q_CHECK_PTR(pTabBarChannels);

  int yTopName = pTabBar->rect().height();
  QRect rectPlot(10,yTopName+6, 321,321); //initialize the reca area for the graph at specified position and dimensions
  MPlot* pPlot;

  for(int i = IFirstItemListParamsTelemetry; i < PListParamsTelemetryChannel->count(); i += 3)
  {
   float* pminValue = NULL;
    if((*PListParamsTelemetryChannel)[i+1] != "N/A")
    {
      pminValue = new float;   *pminValue = (*PListParamsTelemetryChannel)[i+1].toFloat();
    }
    ListPMinNormalValues << pminValue;

    float* pmaxValue = NULL;
    if((*PListParamsTelemetryChannel)[i+2] != "N/A")
    {
      pmaxValue = new float;  *pmaxValue = (*PListParamsTelemetryChannel)[i+2].toFloat();
    }
    ListPMaxNormalValues << pmaxValue;

    pPlot = new MPlot(this, false, (*PListParamsTelemetryChannel)[i].toStdString().c_str(), pminValue, pmaxValue);
    pPlot->setGeometry(rectPlot);

    ListPlots << pPlot;

    if(i == IFirstItemListParamsTelemetry)  RectChannel = rectPlot;
    else                                    RectChannel |= rectPlot;

    rectPlot.translate(rectPlot.width()+10,0); //moves the graph to the left 10 pixels
  }


  IsFlicker = false;   IsFlickerBrush = false;
  for(int i = 0; i < ListPlots.count(); i++)
  {
    ListIsNonNormalMaxValue << false;
    ListIsNonNormalMinValue << false;
  }

  MValues = new float[ListPlots.count()];

  IsArduinoValues = false;

  if(*GetPNameChannel() == SNameTelemetryTabChannel_HS ||
     *GetPNameChannel() == SNameTelemetryTabChannel_Angles ||
     *GetPNameChannel() == SNameTelemetryTabChannel_TP) {

    IsArduinoValues = true;

//QMessageBox msg; msg.setText(SNameTelemetryTabChannel_Angles);  msg.exec();
  }

  connect(this,
          SIGNAL(SignalAddSlotTelemetryTabChannel(MTelemetryTabChannel*,bool)),
          GetPMainWnd()->GetPArduinoConnector(),
          SLOT(SlotAddSlotTelemetryTabChannel(MTelemetryTabChannel*,bool)));

  emit SignalAddSlotTelemetryTabChannel(this, IsArduinoValues);

  connect(&Timer,
          SIGNAL(timeout()),
          this,
          SLOT(SlotTimerSignal()));
}
//--------------------------------------------------------------------------------------------
MTelemetryTabChannel::~MTelemetryTabChannel()
{
  for(int i = 0; i < ListPMinNormalValues.count(); i++) { delete ListPMinNormalValues[i]; }
  for(int i = 0; i < ListPMaxNormalValues.count(); i++) { delete ListPMaxNormalValues[i]; }

  delete[] MValues;  MValues = NULL;
}
//--------------------------------------------------------------------------------------------
bool MTelemetryTabChannel::event(QEvent* pEvent)
{
  bool b = QWidget::event(pEvent); //catch different events
//--
  if(pEvent->type() == QEvent::Polish) {//Polish is caught

    int xLeftName = 20;   QLineEdit* pLineEditCurrentParam;

    for(int i = 0; i < ListPlots.count(); i++)
    {

      pLineEditCurrentParam = CreateLineEditCurrentValue(ListPlots[i]->parentWidget(),
                                                         QRect(xLeftName,10, 300,20),
                                                         "Current Value of " + *GetPNameParamInChannel(i) + ":",
                                                         QRect(xLeftName+245,10, 50,20));
      ListLineEditCurrentParams << pLineEditCurrentParam;

      xLeftName += 330;
    }

  }
//QMessageBox msg; msg.setText("pEvent->type() == EventType_TakeStrValues");  msg.exec();
  return b;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::AddAssignCorrespondingReportPlot(MPlot* pCorrespondingReportPlot)
{
  ListCorrespondingReportPlots << pCorrespondingReportPlot;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::AddAssignCorrespondingInfoParam(QLineEdit* pLineEditInfoParam)
{
  ListCorrespondingInfoParams << pLineEditInfoParam;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::GetTempValues(const char* pCharsValues, const char* charsCodeValues, const int nvalues, float* tempValues)
{
  QString* pStrValues = new QString(pCharsValues);

  int pos1 = 0;  int pos2;

  if(charsCodeValues) {
    pos1 = pStrValues->indexOf(charsCodeValues); // "6oq4 0.123 -4.567 8.910 11.121"  or  "-0.123 4.567"  or  ...
    Q_ASSERT(pos1 >= 0);

    pos2 = pStrValues->indexOf(CharSeparator, pos1);
    int n = pStrValues->at(pos2-1).digitValue();
    Q_ASSERT(n == nvalues);
    pos1 = pos2 + 1;
  }

  int i = -1;
  while(pos1 < pStrValues->length())
  {
    i += 1;
    pos2 = pStrValues->indexOf(CharSeparator, pos1);
    if(pos2 < 0)  pos2 = pStrValues->length();
    tempValues[i] = pStrValues->mid(pos1, pos2-pos1).toFloat();
    pos1 = pos2+1;
  }

  Q_ASSERT(i+1 == nvalues);

  delete pStrValues;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::SlotTakeTelemetryStrValues(MTelemetryTabChannel* pTelemetryTabChannel, const char* pCharsValues)
{
  if(pTelemetryTabChannel != this)  return;

  if(IsArduinoValues)
  {

    if(*GetPNameChannel() == SNameTelemetryTabChannel_HS)
    {
//= Height and Speed:
// "5ohs2 12 28.123"

      Q_ASSERT(QChar(pCharsValues[0]).digitValue() == IndexTabChannel_HS);
      Q_ASSERT(pCharsValues[1] == CharWriteTelemetryModeOccasionally);


      GetTempValues(pCharsValues, CharsCodeHeightSpeed, 2, MValues);
    }
    else if(*GetPNameChannel() == SNameTelemetryTabChannel_Angles)
    {
//= Angles (Quaternion):
// "6oq4 0.123 -4.567 8.910 11.121"  or  "6q4 0.123 -4.567 8.910 11.121"

      Q_ASSERT(QChar(pCharsValues[0]).digitValue() == IndexTabChannel_Angles);

      float fquaternion[4];// = new float[*pnvalues];
      GetTempValues(pCharsValues, CharsCodeQuaternion, 4, fquaternion);
      float yawPitchRoll[3];// = new float[3];
      GetYawPitchRoll(yawPitchRoll, fquaternion);

      MValues[0] = yawPitchRoll[1];  // Pitch for ICurrentParam == 0
      MValues[1] = yawPitchRoll[2];  // Roll  for ICurrentParam == 1
      GetPMainWnd()->GetPTelemetry()->GetPGyrohorizon()->SetPitchRollAngles(MValues[0], MValues[1]);

      QQuaternion quaternion(fquaternion[0], fquaternion[1],fquaternion[3],fquaternion[2]);
      GetPMainWnd()->GetPTelemetry()->GetPGyrohorizon()->SetQuaternion(&quaternion);

      if(pCharsValues[1] != CharWriteTelemetryModeOccasionally)
      {
        delete[] pCharsValues;
        return;
      }

    }
    else if(*GetPNameChannel() == SNameTelemetryTabChannel_TP)
    {
//= Temperature and Pressure:
// "8otp2 25 101850"

      Q_ASSERT(QChar(pCharsValues[0]).digitValue() == IndexTabChannel_TP);
      Q_ASSERT(pCharsValues[1] == CharWriteTelemetryModeOccasionally);


      GetTempValues(pCharsValues, CharsCodeTemperaturePressure, 2, MValues);
    }
    else
    {
      QMessageBox msg(QMessageBox::Critical, "void MTelemetryTabChannel::SlotTakeTelemetryParamStrValues", "Unknown CharsCode");  msg.exec();
      delete[] pCharsValues; // IsReadyTakeValues = true;  // delete[] mValues;
      return;
    }

  }
  else
  {
// "-0.123 4.567"
    GetTempValues(pCharsValues, NULL, 2, MValues);
  }

  delete[] pCharsValues;

//--------------

  qreal dt = (QDateTime::currentDateTime().toMSecsSinceEpoch()-GetPMainWnd()->GetTimeStartTelemetry())/1000.0;

  float value;  QString s;
  bool isNonNormal;  bool isLastParamInChannel;

  for(int iCurrentParam = 0; iCurrentParam < ListPlots.count(); iCurrentParam++)
  {

    value = MValues[iCurrentParam];
    s.sprintf("%.1f",value);
    ListLineEditCurrentParams[iCurrentParam]->setText(s);

    if(iCurrentParam < ListCorrespondingInfoParams.count())
    {
      ListCorrespondingInfoParams[iCurrentParam]->setText(s);
    }
    else if(ListCorrespondingInfoParams.count() != 0)
    {
      QMessageBox msgBox(QMessageBox::Critical, "void MTelemetryTabChannel::SlotResultGetTelemetryValue(bool isOK, float* pFloatValues, int floatReadLength)",
                       "Problem::\n\nICurrentParam < ListCorrespondingInfoParams.count()\n\nДоделать Программу");
      msgBox.exec();
    }

    ListPlots[iCurrentParam]->AddValue(dt, value);
    Q_ASSERT(iCurrentParam < ListCorrespondingReportPlots.count());
    ListCorrespondingReportPlots[iCurrentParam]->AddValue(dt, value);

    isNonNormal = false;
    if(ListPMinNormalValues[iCurrentParam])
    {
      if(value < *ListPMinNormalValues[iCurrentParam])
      {
        isNonNormal = true;
        ListIsNonNormalMinValue.replace(iCurrentParam, true);
      }
    }
    if(ListPMaxNormalValues[iCurrentParam])
    {
      if(*ListPMaxNormalValues[iCurrentParam] < value)
      {
        isNonNormal = true;
        ListIsNonNormalMaxValue.replace(iCurrentParam, true);
      }
    }
    if(isNonNormal)
    {
      if(!Timer.isActive())
      {
        IsFlicker = true;  IsFlickerBrush = true; // !IsFlickerBrush;
       Timer.start(500); // int msec
        UpdateTabBar();
      }
    }

    if(iCurrentParam < ListPlots.count()-1)  isLastParamInChannel = false;
    else
    {
        isLastParamInChannel = true;
    }

    SignalChannelTelemetryDoneParam(this, dt, value, isNonNormal, isLastParamInChannel);
  }

}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::SlotTimerSignal()
{
  IsFlickerBrush = !IsFlickerBrush;
  UpdateTabBar();
}
//--------------------------------------------------------------------------------------------
bool MTelemetryTabChannel::GetIsFlicker(bool* pIsColorBrushFlicker/* = NULL*/, QList<bool>** ppListIsNonNormalMinValue/* = NULL*/, QList<bool>** ppListIsNonNormalMaxValue/* = NULL*/)
{
  if(IsFlicker)
  {
    if(pIsColorBrushFlicker)
    {
      if(IsFlickerBrush)  *pIsColorBrushFlicker = true; //&ColorBrushTabFlicker;
      else                *pIsColorBrushFlicker = false; //NULL;
    }
    if(ppListIsNonNormalMinValue) *ppListIsNonNormalMinValue = &ListIsNonNormalMinValue;
    if(ppListIsNonNormalMaxValue) *ppListIsNonNormalMaxValue = &ListIsNonNormalMaxValue;
  }
  return IsFlicker;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::StopFlicker()
{
  IsFlicker = false;   IsFlickerBrush = false;
  for(int i = 0; i < ListPlots.count(); i++)
  {
    ListIsNonNormalMaxValue.replace(i, false);
    ListIsNonNormalMinValue.replace(i, false);
  }

  if(Timer.isActive())  Timer.stop();
  UpdateTabBar();
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::UpdateTabBar()
{
  emit SignalUpdateTabBar();
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabChannel::ClearPlots()
{
  StopFlicker();

  for(int i = 0; i < ListPlots.count(); i++)                    { ListPlots[i]->Clear(); }
  for(int i = 0; i < ListCorrespondingReportPlots.count(); i++) { ListCorrespondingReportPlots[i]->Clear(); }
  for(int i = 0; i < ListCorrespondingInfoParams.count(); i++)  { ListCorrespondingInfoParams[i]->setText(""); }
}
//--------------------------------------------------------------------------------------------
