
/*
 *Telemetry class responsible for initialization of telemetry
*/

#include "MTelemetry.h"
#include "funcs.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QScrollArea>
#include <QtGlobal>

//============================================================================================

int TimerInterval = 1000; // msec
const char* ReportFileVersion = "WIGTS Report Ver. 1.0";

//============================================================================================

MTelemetry::MTelemetry(QWidget *parent) :
  QSplitter(Qt::Horizontal, parent)
{
  //just a title for message error
  SMessageBoxTitle = QCoreApplication::instance()->applicationName()+":  Telemetry";

  CountAllTelemetryChannels = 0;

  PFileReport = NULL;
  PStreamFileReport = NULL;

  IsTelemetryStarted = false;
}
//--------------------------------------------------------------------------------------------
MTelemetry::~MTelemetry()
{
  CloseFileReport();
}
//--------------------------------------------------------------------------------------------
void MTelemetry::Init()
{
//===####  I: Left (Gyrohorizon + Params):  ####===

  //prepairung rect area for gyroscope and parameters
  QScrollArea* pScrollAreaGyrohorizon = new QScrollArea(this);
  pScrollAreaGyrohorizon->setWidgetResizable(true);

  QWidget* pScrollAreaGyrohorizonContents = new QWidget(this);

  QRect rectInfo = FillTelemetryGyrohorizon(pScrollAreaGyrohorizonContents);
  pScrollAreaGyrohorizon->setWidget(pScrollAreaGyrohorizonContents);

//===####  II: 1) Right (Channels):  ####===

  QSplitter* pSplitterTelemetry = new QSplitter(Qt::Vertical, this);//pSplitter);

  //set the sizes of main window vertically
  QList<int> list;  list << 97 << 200;    setSizes(list);   list.clear();


//===  1) TelemetryChannels:

  //prepairung rect area for right upper channels
  QScrollArea* pScrollAreaChannels = new QScrollArea(pSplitterTelemetry);
  pScrollAreaChannels->setWidgetResizable(true);

  QWidget* pScrollAreaChannelsContents = new QWidget(pSplitterTelemetry);

  QRect rect = FillTelemetryChannels(pScrollAreaChannelsContents);
  PTabWidgetChannels->setGeometry(rect);

  pScrollAreaChannels->setWidget(pScrollAreaChannelsContents);
  pScrollAreaChannelsContents->setMinimumSize(rect.right() + 5, rect.bottom() + 5);


//==  2) TelemetryStates + TelemetryReports:

  QScrollArea* pScrollAreaStatesReports = new QScrollArea(pSplitterTelemetry);
  pScrollAreaStatesReports->setWidgetResizable(true);

  QWidget* pScrollAreaStatesReportsContents = new QWidget(pSplitterTelemetry);

//= a) TelemetryStates:
  rect = FillTelemetryStates(pScrollAreaStatesReportsContents);

//= b) TelemetryReports:
  rect |= FillTelemetryReports(pScrollAreaStatesReportsContents);

  PTabWidgetStates->setGeometry(rect);

  rect.moveTo(0, PTabWidgetStates->tabBar()->geometry().height() - 10);
  rect.setBottom(rect.bottom() - 1);

  PTabWidgetReports->setGeometry(rect);

  PTabWidgetReports->setTabPosition(QTabWidget::South); //set the report tabs at bootom
  //PTabWidgetReports->tabBar()->setShape(QTabBar::RoundedSouth);


  pScrollAreaStatesReportsContents->setMinimumSize(rect.right(),rect.bottom()+3);
  pScrollAreaStatesReports->setWidget(pScrollAreaStatesReportsContents);


  for(int i = 0; i < CountAllTelemetryChannels; i++)
  {
    connect(GetCurrentPTabChannel(i),
            SIGNAL(SignalChannelTelemetryDoneParam(MTelemetryTabChannel*,qreal,float,bool,bool)),
            this,
            SLOT(SlotChannelTelemetryDoneParam(MTelemetryTabChannel*,qreal,float,bool,bool)));
  }
//==

  rectInfo |= AddInfoPanel(pScrollAreaGyrohorizonContents);//adds bootom part which is telemetry parameters readings

//- Button "START":
  PPushButtonStartTelemetry = new QPushButton(pScrollAreaGyrohorizonContents);
  rect.setRect(rectInfo.left() + 20, rectInfo.bottom() + 40, 60, 20);
  PPushButtonStartTelemetry->setGeometry(rect);
  PPushButtonStartTelemetry->setText(QApplication::translate("MainWindow", "START", 0));
  PPushButtonStartTelemetry->setEnabled(true);

  connect(PPushButtonStartTelemetry,
          SIGNAL(clicked()),
          this,
          SLOT(SlotStartTelemetry()));

  rectInfo |= rect;

//- Button "STOP":
  PPushButtonStopTelemetry = new QPushButton(pScrollAreaGyrohorizonContents);
  rect.setRect(rect.left() + 240, rect.top(), 60, 20);
  PPushButtonStopTelemetry->setGeometry(rect);
  PPushButtonStopTelemetry->setText(QApplication::translate("MainWindow", "STOP", 0));
  PPushButtonStopTelemetry->setEnabled(false);

  connect(PPushButtonStopTelemetry,
          SIGNAL(clicked()),
          this,
          SLOT(SlotStopTelemetry()));

  rectInfo |= rect;

//- Button "Readiness":
  PPushButtonReadiness = new QPushButton(pScrollAreaGyrohorizonContents);//pScrollAreaStateContents);
  rect.setRect(rect.left(), rect.top() + 60, 60, 30);
  PPushButtonReadiness->setGeometry(rect); //QRect(685, 110, 60, 30));
  PPushButtonReadiness->setAutoDefault(true);
  PPushButtonReadiness->setText(QApplication::translate("MainWindow", "Readiness", 0));

  connect(PPushButtonReadiness,
          SIGNAL(clicked()),
          GetPMainWnd(),
          SLOT(SlotSwitchToReadiness()));

  rectInfo |= rect;

//- sets minimum sizes before scroll activates
  pScrollAreaGyrohorizonContents->setMinimumSize(rectInfo.right() + 3, rectInfo.bottom() + 3);

//==

  ActivateTabWidgetStates();

//=====
  //set the sizes of right sides to 490 upper rect and 510 lower rect
  list << 490 << 510;    pSplitterTelemetry->setSizes(list);  list.clear();
//=====
}
//--------------------------------------------------------------------------------------------
QRect MTelemetry::FillTelemetryGyrohorizon(QWidget* pScrollAreaGyrohorizonContents)
{
  //creating the rect for gyrohorizon
  QGraphicsScene* pSceneGyrohorizon = new QGraphicsScene(pScrollAreaGyrohorizonContents);//pTab);
  QGraphicsView* pViewGyrohorizon = new QGraphicsView(pSceneGyrohorizon, pScrollAreaGyrohorizonContents);//pTab);
  QRect rect(15,15, 320, 320);
  pViewGyrohorizon->setGeometry(rect);

//=================

  PGyrohorizon = new MGyrohorizon(pScrollAreaGyrohorizonContents);
  PGyrohorizon->setGeometry(rect);//15,15, 320,320);
  PEkranoplanGL = new MEkranoplanGL(pViewGyrohorizon); // !

  PGyrohorizon->SetPEkranoplanGL(PEkranoplanGL);

  return QRect(0, 0, PGyrohorizon->geometry().right() + 12, PGyrohorizon->geometry().bottom());
}
//--------------------------------------------------------------------------------------------
//Channels refer to tab bars for readiness
QRect MTelemetry::FillTelemetryChannels(QWidget* pScrollAreaChannelsContents)
{

  PTabWidgetChannels = new MTelemetryTabWidgetChannels(pScrollAreaChannelsContents,
                                                       GetPMainWnd()->GetPListParamsTelemetryChannels(),
                                                       GetPMainWnd()->GetIFirstItemListParamsTelemetry());

  QRect rectTelemetryChannels = PTabWidgetChannels->AddTabs();
  CountAllTelemetryChannels += PTabWidgetChannels->count();


  rectTelemetryChannels.setTopLeft(QPoint(0,0));
  rectTelemetryChannels.setBottomRight(QPoint(rectTelemetryChannels.right() + 15,
                                              rectTelemetryChannels.bottom() + 4 + PTabWidgetChannels->tabBar()->rect().height()));
  return rectTelemetryChannels;
}
//--------------------------------------------------------------------------------------------
//States refer to tab bars for telemetry
QRect MTelemetry::FillTelemetryStates(QWidget* pScrollAreaStatesContents)
{
  PTabWidgetStates = new MTelemetryTabWidgetChannels(pScrollAreaStatesContents,
                                                     GetPMainWnd()->GetPListParamsTelemetryStates(),
                                                     GetPMainWnd()->GetIFirstItemListParamsStates());

  QRect rectTelemetryStates = PTabWidgetStates->AddTabs();
  CountAllTelemetryChannels += PTabWidgetStates->count();

  return rectTelemetryStates;
}
//--------------------------------------------------------------------------------------------
//Report refer to tab bars for reports at the bottom
QRect MTelemetry::FillTelemetryReports(QWidget* pScrollAreaReportsContents)
{
  PTabWidgetReports = new MTelemetryTabWidgetReports(pScrollAreaReportsContents);

  QRect rectTelemetryReports = PTabWidgetReports->AddTabs();

  return rectTelemetryReports;
}
//--------------------------------------------------------------------------------------------
//telemetry parameters values at left side
QRect MTelemetry::AddInfoPanel(QWidget* pScrollAreaGyrohorizonContents)
{
  int xLeftName = 20;  int yTopName = 370;
  QRect rectPanel(xLeftName, yTopName, 1, 1);   QRect rectName;  QRect rectValue;

  int countChannelsStates = GetPMainWnd()->GetPListParamsTelemetryStates()->count();
  int countParamsInChannel;
  QString sInfoParam;   QLineEdit* pLineEditInfoParam;

  for(int i = 0; i < countChannelsStates; i++)// 8 for now
  {
    countParamsInChannel = (*GetPMainWnd()->GetPListParamsTelemetryStates())[i].count();//2 params per channel
    for(int j = GetPMainWnd()->GetIFirstItemListParamsStates(); j < countParamsInChannel; j += 3)
    {
      sInfoParam = (*GetPMainWnd()->GetPListParamsTelemetryStates())[i][j];
      rectName.setRect(xLeftName, yTopName, 300, 20);
      rectValue.setRect(xLeftName + 245, yTopName, 50, 20);
      pLineEditInfoParam = CreateLineEditCurrentValue(pScrollAreaGyrohorizonContents,
                                                      rectName,
                                                      sInfoParam + ":",
                                                      rectValue);
      AssignCorrespondingChannelInfoParam(sInfoParam, pLineEditInfoParam);

      rectPanel |= rectName;
      rectPanel |= rectValue;
      yTopName += 25;
    }
  }
  //same for longeron values
  int i = GetPMainWnd()->GetILongeronsTelemetryChannel();
  countParamsInChannel = (*GetPMainWnd()->GetPListParamsTelemetryChannels())[i].count();
  for(int j = GetPMainWnd()->GetIFirstItemListParamsTelemetry(); j < countParamsInChannel; j += 3)
  {
    sInfoParam = (*GetPMainWnd()->GetPListParamsTelemetryChannels())[i][j];
    rectName.setRect(xLeftName, yTopName, 300, 20);
    rectValue.setRect(xLeftName + 245, yTopName, 50, 20);
    pLineEditInfoParam = CreateLineEditCurrentValue(pScrollAreaGyrohorizonContents,
                                                    rectName,
                                                    sInfoParam + ":",
                                                    rectValue);

    AssignCorrespondingChannelInfoParam(sInfoParam, pLineEditInfoParam);

    rectPanel |= rectName;
    rectPanel |= rectValue;
    yTopName += 25;
  }

  return rectPanel;
}
//--------------------------------------------------------------------------------------------
void MTelemetry::AssignCorrespondingChannelInfoParam(QString& sInfoParam, QLineEdit* pLineEditInfoParam)
{
  MTelemetryTabChannel* pTabChannel;
  for(int i = 0; i < CountAllTelemetryChannels; i++)
  {
    pTabChannel = GetCurrentPTabChannel(i);
    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j++)
    {
      if(*pTabChannel->GetPNameParamInChannel(j) == sInfoParam)
      {
        pTabChannel->AddAssignCorrespondingInfoParam(pLineEditInfoParam);
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------
void MTelemetry::ActivateTabWidgetStates()
{
  SwitchActiveTabWidgetFromTo(PTabWidgetReports, PTabWidgetStates);
}
//--------------------------------------------------------------------------------------------
void MTelemetry::ActivateTabWidgetReports()
{
  SwitchActiveTabWidgetFromTo(PTabWidgetStates, PTabWidgetReports);
}
//--------------------------------------------------------------------------------------------
void MTelemetry::SwitchActiveTabWidgetFromTo(QTabWidget* pTabWidgetFrom, QTabWidget* pTabWidgetTo)
{
  pTabWidgetFrom->stackUnder(pTabWidgetTo);//make tab go under theother tab widjet

  for(int i = 0; i < pTabWidgetFrom->count(); i++)  { pTabWidgetFrom->setEnabled(false); }
  for(int i = 0; i < pTabWidgetTo->count(); i++)    { pTabWidgetTo->setEnabled(true);    }
}
//--------------------------------------------------------------------------------------------
//start telemetry slot
void MTelemetry::SlotStartTelemetry()
{
  if(IsTelemetryStarted)  return;
  IsTelemetryStarted = true;

  PPushButtonStartTelemetry->setEnabled(false);
  PPushButtonStopTelemetry->setEnabled(true);

  PPushButtonReadiness->setEnabled(false);

  emit SignalSetTypeModeTelemetry();

  QDateTime currentDataTime = QDateTime::currentDateTime();

  qint64 timeStartTelemetry = currentDataTime.toMSecsSinceEpoch();
  GetPMainWnd()->SetTimeStartTelemetry(timeStartTelemetry);

  //connect timer (timeout signal) with timeout event
  connect(&GetPMainWnd()->GetTimer(),
          SIGNAL(timeout()),
          this,
          SLOT(SlotTimerSignal()));

  PrepareFileReport(currentDataTime);

  ICurrentAllChannel = 0; //-1;
  IsReadyNextCycle = true;

  MTelemetryTabChannel* pTabChannel;
  for(int n = 0; n < CountAllTelemetryChannels; n++)
  {
    pTabChannel = GetCurrentPTabChannel(n);
    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j++)
    {
      pTabChannel->ClearPlots();
    }
  }

  GetPMainWnd()->GetTimer().start(TimerInterval);//1000); // /12); // int msec
}
//--------------------------------------------------------------------------------------------
//stop telemetry slot event
void MTelemetry::SlotStopTelemetry()
{
  if(!IsTelemetryStarted)  return;
  IsTelemetryStarted = false;

  PPushButtonStartTelemetry->setEnabled(true);
  PPushButtonStopTelemetry->setEnabled(false);

  PPushButtonReadiness->setEnabled(true);

  emit SignalStopTakingData();

  if(!disconnect(&GetPMainWnd()->GetTimer(),
                 SIGNAL(timeout()),
                 this,
                 SLOT(SlotTimerSignal())))
  {
    QMessageBox msgBox(QMessageBox::Warning, SMessageBoxTitle,
                       "Problem::\n\ndisconnect(&GetPMainWnd()->GetTimer(), SIGNAL(timeout()), this, SLOT(SlotTimerSignal())) == false");
    msgBox.exec();
  }

  CloseFileReport();
}
//--------------------------------------------------------------------------------------------
//prepare file report function
void MTelemetry::PrepareFileReport(QDateTime& currentDataTime)
{
    QString sPathNameFileReport(QCoreApplication::applicationDirPath());  sPathNameFileReport += "/";
    NameFileReport = "Report - ";  NameFileReport += currentDataTime.toString("dd.MMM.yyyy - HH.mm.ss");  NameFileReport += ".wig";
    sPathNameFileReport += NameFileReport;
    PFileReport = new QFile(sPathNameFileReport);
    if(PFileReport->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      PStreamFileReport = new QTextStream(PFileReport);
    }
    else
    {
      QMessageBox msg; msg.setText("Problem file:\n\""+sPathNameFileReport+"\"");   msg.exec();
    }

  *PStreamFileReport << ReportFileVersion;   *PStreamFileReport << "\n\n";

  ListLengthAllParams.clear();
  MTelemetryTabChannel* pTabChannel;  QString sNameChannel;  QString sNameParamChannel;


//= Calculation of the Channel Lengths:
  QList<int> listLengthChannels;  int lengthChannel;   int lengthNameParamChannel;

  for(int n = 0; n < CountAllTelemetryChannels; n++)
  {
    lengthChannel = 0;
    pTabChannel = GetCurrentPTabChannel(n);

    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j ++)
    {
//1), 3)
      lengthChannel += 8; // "T(sec)  "; // = 8 chs
      ListLengthAllParams << 8;
//2), 4)

      sNameParamChannel = *pTabChannel->GetPNameParamInChannel(j);
      lengthNameParamChannel = qMax(sNameParamChannel.length(), 16);
      lengthChannel += lengthNameParamChannel; // "Rotation Speed (RPM)"
                                               // "Power Consumption (W)"
      if(j < pTabChannel->GetPListPlots()->count()-1)
      {
//2)
        lengthChannel += 4; // "    "
        ListLengthAllParams << lengthNameParamChannel + 4;
      }
      else
      {
//4)
        if(n < CountAllTelemetryChannels-1)
        {
          lengthChannel += 3; //3+1+3; // to "   |   ":  3+1+3 chs
          ListLengthAllParams << lengthNameParamChannel + 3; // to  "   |   ": 3+1+3;
        }
        else
        {
          ListLengthAllParams << lengthNameParamChannel;
        }
      }
    }
    listLengthChannels << lengthChannel;
  }

  NAllWritingParams = ListLengthAllParams.count();
  IWritingParamInChannel = 0;

//= Writing the Channel Names:

  for(int n = 0; n < CountAllTelemetryChannels; n++)
  {
    sNameChannel = *GetCurrentPTabChannel(n)->GetPNameChannel();
    sNameChannel = sNameChannel.leftJustified(listLengthChannels[n]);
    *PStreamFileReport << sNameChannel;
    if(n < CountAllTelemetryChannels-1)
    {
//5)
      *PStreamFileReport << "|   ";
    }
  }
  *PStreamFileReport << "\n";


//= Writing the Channel Params Names:

  int m = 0;  QString sParamX;  QString sParamY;

  for(int n = 0; n < CountAllTelemetryChannels; n++)
  {
    pTabChannel = GetCurrentPTabChannel(n);
    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j ++)
    {
      sParamX = "T(sec)";
      sParamX = sParamX.leftJustified(ListLengthAllParams[m]);  m += 1;
      *PStreamFileReport << sParamX;
      sParamY = *pTabChannel->GetPNameParamInChannel(j);
      sParamY = sParamY.leftJustified(ListLengthAllParams[m]);  m += 1;
      *PStreamFileReport << sParamY;
    }
    if(n < CountAllTelemetryChannels-1)
    {
//5)
      *PStreamFileReport << "|   ";
    }
  }
  *PStreamFileReport << "\n";


//= Writing the Min and Max Normal Values:

  QString sMinNormalValue;  QString sMaxNormalValue;
  m = 0;  QString sRange;

  for(int n = 0; n < CountAllTelemetryChannels; n++)
  {
    pTabChannel = GetCurrentPTabChannel(n);

    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j ++)
    {
      sRange = " ";  sRange = sRange.leftJustified(ListLengthAllParams[m]);  m += 1;
      *PStreamFileReport << sRange;
      sMinNormalValue = *pTabChannel->GetPStringMinNormalValue(j);
      sMaxNormalValue = *pTabChannel->GetPStringMaxNormalValue(j);

      sRange = sMinNormalValue + " -- " + sMaxNormalValue;
      sRange = sRange.leftJustified(ListLengthAllParams[m]);  m += 1;
      *PStreamFileReport << sRange;
    }
    if(n < CountAllTelemetryChannels-1)
    {
      *PStreamFileReport << "|   ";
    }
  }
  *PStreamFileReport << "\n";
  PStreamFileReport->flush();
}
//--------------------------------------------------------------------------------------------
void MTelemetry::CloseFileReport()
{
  WriteStrParamsFileLine();

  if(PFileReport)
  {
    if(PFileReport->isOpen()) PFileReport->close();
    delete PFileReport;  PFileReport = NULL;
  }

  if(PStreamFileReport)
  {
    delete PStreamFileReport;  PStreamFileReport = NULL;
  }

//NameFileReport.clear();
}
//--------------------------------------------------------------------------------------------
void MTelemetry::WriteStrParamsFileLine() //!!!!!!!
{
  bool isWriteFileLine = true;
  for(int n = 0; n < ListPStrParamsFileLines.count(); n++)
  {
    if(ListPStrParamsFileLines[n]->length() == 0)
    {
      isWriteFileLine = false;
      break;
    }
  }

  for(int n = 0; n < ListPStrParamsFileLines.count(); n++)
  {
    if(PStreamFileReport && isWriteFileLine)  { *PStreamFileReport << *ListPStrParamsFileLines[n]; }
    delete ListPStrParamsFileLines[n];
  }
  if(PStreamFileReport && isWriteFileLine)  PStreamFileReport->flush();
  ListPStrParamsFileLines.clear();
}
//--------------------------------------------------------------------------------------------
MTelemetryTabChannel* MTelemetry::GetCurrentPTabChannel(int indexAllChannel)
{
  MTelemetryTabChannel* pTabChannel;

  if(indexAllChannel < PTabWidgetStates->count())
    pTabChannel = qobject_cast<MTelemetryTabChannel*>(PTabWidgetStates->widget(indexAllChannel));
  else
    pTabChannel = qobject_cast<MTelemetryTabChannel*>(PTabWidgetChannels->widget(indexAllChannel - PTabWidgetStates->count()));

  Q_ASSERT(pTabChannel != NULL); // Q_CHECK_PTR(pTabChannel);
  return pTabChannel;
}
//............................................................................................
int MTelemetry::GetIndexAllTelemetryChannels(MTelemetryTabChannel* pTabChannel)
{
  int i;
  if((i = PTabWidgetStates->indexOf(pTabChannel)) >= 0)  return i;
  if((i = PTabWidgetChannels->indexOf(pTabChannel)) >= 0)  return PTabWidgetStates->count()+i;

  Q_ASSERT(false);
  return i;
}
//--------------------------------------------------------------------------------------------
void MTelemetry::SlotTimerSignal()
{
  if(!IsReadyNextCycle)  return;
  IsReadyNextCycle = false;

  emit SignalGetAllTelemetryValues();
}
//--------------------------------------------------------------------------------------------
void MTelemetry::SlotChannelTelemetryDoneParam(MTelemetryTabChannel* pTabChannel, qreal dt, float value, bool isNonNormal, bool isLastParamInChannel)
{
  if(ListPStrParamsFileLines.isEmpty())
  {
    for(int n = 0; n < CountAllTelemetryChannels; n++) { ListPStrParamsFileLines << new QString(); }
  }
  SaveTempStrParamsFileLine(GetIndexAllTelemetryChannels(pTabChannel), dt, value, isNonNormal, isLastParamInChannel);

  if(isLastParamInChannel)
  {
    ICurrentAllChannel += 1;
    if(ICurrentAllChannel >= CountAllTelemetryChannels)
    {

      WriteStrParamsFileLine();

      ICurrentAllChannel = 0;
      IsReadyNextCycle = true;
    }
  }
}
//--------------------------------------------------------------------------------------------
int MTelemetry::GetIWritingAllParams(int indexAllTelemetryChannels)
{
  int iWritingAllParams = 0;

  MTelemetryTabChannel* pTabChannel;
  for(int n = 0; n < indexAllTelemetryChannels; n++)
  {
    pTabChannel = GetCurrentPTabChannel(n);
    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j ++)
    {
      iWritingAllParams += 2;  // 2: "Time" and "Value"
    }
  }
  return iWritingAllParams;
}
//--------------------------------------------------------------------------------------------
void MTelemetry::SaveTempStrParamsFileLine(int indexAllTelemetryChannels, qreal dt, float value, bool isNonNormal, bool isLastParamInChannel)
{
  QString* pStrParamsFileLine = ListPStrParamsFileLines[indexAllTelemetryChannels];

  int iWritingAllParams = GetIWritingAllParams(indexAllTelemetryChannels) + IWritingParamInChannel;


  QString s;
  s.sprintf("%.3f", dt);
  s = s.leftJustified(ListLengthAllParams[iWritingAllParams]);
  *pStrParamsFileLine += s;

  iWritingAllParams += 1;
  s.sprintf("%.5f", value);
  if(isNonNormal)  s += " *";
  s = s.leftJustified(ListLengthAllParams[iWritingAllParams]);
  *pStrParamsFileLine += s;

  iWritingAllParams += 1;
  if(isLastParamInChannel)
  {
    if(iWritingAllParams < NAllWritingParams)
        s = "|   ";
    else
        s = "\n";

    *pStrParamsFileLine += s;

    IWritingParamInChannel = 0;

  }
  else
    IWritingParamInChannel += 2;  // 2: "Time" and "Value"
}
//--------------------------------------------------------------------------------------------
