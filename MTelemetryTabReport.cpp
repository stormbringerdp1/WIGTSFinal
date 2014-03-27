#include "MTelemetryTabReport.h"
#include "MTelemetry.h"
#include "MTelemetryTabChannel.h"
#include "funcs.h"

#include <QFileDialog>
#include <QtGlobal>
#include <QMessageBox>

extern const char* ReportFileVersion;

//============================================================================================
MTelemetryTabReport::MTelemetryTabReport(QWidget* parent) //, MTelemetryTabWidgetChannels *pTabWidgetChannels) :
                   : QScrollArea(parent)
{
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabReport::AddPlot(MPlot* pPlot)
{
  ListPlots << pPlot;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabReport::Init()
{
  int countParamsTelemetryChannels = 0;
  QList<QString>        listNamesChannels;
  QList<int>            listCountPlotsInChannels;
  QList<QList<QString>> listNameParamsInChannels;

  MTelemetryTabWidgetChannels* pTabWidget;

  pTabWidget = GetPMainWnd()->GetPTelemetry()->GetPTabWidgetStates();
  FormListsParams(pTabWidget, countParamsTelemetryChannels, listNamesChannels, listCountPlotsInChannels, listNameParamsInChannels);

  pTabWidget = GetPMainWnd()->GetPTelemetry()->GetPTabWidgetChannels();
  FormListsParams(pTabWidget, countParamsTelemetryChannels, listNamesChannels, listCountPlotsInChannels, listNameParamsInChannels);

  QRect rectReport = FillTabReport(countParamsTelemetryChannels, listNamesChannels, listCountPlotsInChannels, listNameParamsInChannels, true);//isAssignCorrespondingReportPlot)
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabReport::FormListsParams(MTelemetryTabWidgetChannels* pTabWidget, int& countParamsTelemetryChannels,
                                          QList<QString>& listNamesChannels, QList<int>& listCountPlotsInChannels,
                                          QList<QList<QString>>& listNameParamsInChannels)
{
  int countParams = pTabWidget->count();
  countParamsTelemetryChannels += countParams;

  MTelemetryTabChannel* pTabChannel;

  QString sNameChannel;           QString sNameParamChannel;
  QString sMinParamYNormalValue;  QString sMaxParamYNormalValue;
  QList<QString> list; //  int m;

  for(int n = 0; n < countParams; n++)
  {
    pTabChannel = qobject_cast<MTelemetryTabChannel*>(pTabWidget->widget(n));
    Q_ASSERT(pTabChannel != NULL); //   Q_CHECK_PTR(pTabChannel);
    sNameChannel = *pTabChannel->GetPNameChannel();
    listNamesChannels << sNameChannel;
    listCountPlotsInChannels << pTabChannel->GetPListPlots()->count();

    for(int j = 0; j < pTabChannel->GetPListPlots()->count(); j++)
    {
      sNameParamChannel = *pTabChannel->GetPNameParamInChannel(j);
      sMinParamYNormalValue = *pTabChannel->GetPStringMinNormalValue(j);
      sMaxParamYNormalValue = *pTabChannel->GetPStringMaxNormalValue(j);

      list << sNameParamChannel << sMinParamYNormalValue << sMaxParamYNormalValue;
    }
    listNameParamsInChannels << list;  list.clear();
  }
}
//--------------------------------------------------------------------------------------------
QRect MTelemetryTabReport::FillTabReport(int countParamsTelemetryChannels, QList<QString>& listNamesChannels,
                                         QList<int>& listCountPlotsInChannels, QList<QList<QString>>& listNameParamsInChannels,
                                         bool isAssignCorrespondingReportPlot)
{

  QWidget* pScrollAreaReportContents = new QWidget(this);
  pScrollAreaReportContents->setObjectName(QStringLiteral("ScrollAreaReportContents"));

  MTelemetryTabWidgetChannels* pTabWidgetStates = GetPMainWnd()->GetPTelemetry()->GetPTabWidgetStates();
  MTelemetryTabWidgetChannels* pTabWidgetChannels = GetPMainWnd()->GetPTelemetry()->GetPTabWidgetChannels();

  int countTabsStates = pTabWidgetStates->count();

  MTelemetryTabChannel* pTabChannel;// = NULL;

  QString sNameChannel;  QString sNameParam;
  QRect rectReport;  int yTopChannel = -5;//0;
  QLineEdit* pLineEditNameChannel;  QRect rect;
  MPlot* pPlot;   QRect rectPlot(9,10, 321,321);


  int n = 0;
  while(n < countParamsTelemetryChannels)
  {

    yTopChannel += 10;

    pLineEditNameChannel = new QLineEdit(pScrollAreaReportContents);
    pLineEditNameChannel->setObjectName(QStringLiteral("LineEditNameChannel"));
    rect = QRect(285, yTopChannel, 100, 20);
    pLineEditNameChannel->setGeometry(rect);
    pLineEditNameChannel->setFrame(false);
    pLineEditNameChannel->setReadOnly(true);
    pLineEditNameChannel->setStyleSheet(QLatin1String("background: transparent;"));
    pLineEditNameChannel->setAlignment(Qt::AlignCenter);
    sNameChannel = listNamesChannels[n];
    pLineEditNameChannel->setText(sNameChannel);

    if(n == 0)  rectReport = rect;
    else        rectReport |= rect;

    yTopChannel += 30;
    rectPlot.moveTop(yTopChannel);

    if(isAssignCorrespondingReportPlot)
    {
      if(n < countTabsStates)  pTabChannel = qobject_cast<MTelemetryTabChannel*>(pTabWidgetStates->widget(n));
      else                     pTabChannel = qobject_cast<MTelemetryTabChannel*>(pTabWidgetChannels->widget(n-countTabsStates));
      Q_ASSERT(pTabChannel != NULL); // Q_CHECK_PTR(pTabChannel);
    }

    int j = 0;
    while(j < listCountPlotsInChannels[n])
    {
      Q_ASSERT(listNameParamsInChannels[n].count() == 3*2);
      sNameParam = listNameParamsInChannels[n][j*3]; //GetPMainWnd()->GetDeltaNItemListParamsTelemetry()];

      float* pminValue = NULL;
      if(listNameParamsInChannels[n][j*3+1] != "N/A")
      {
        pminValue = new float;   *pminValue = listNameParamsInChannels[n][j*3+1].toFloat();
      }
      float* pmaxValue = NULL;
      if(listNameParamsInChannels[n][j*3+2] != "N/A")
      {
        pmaxValue = new float;  *pmaxValue = listNameParamsInChannels[n][j*3+2].toFloat();
      }
      pPlot = new MPlot(pScrollAreaReportContents, true, sNameParam.toStdString().c_str(), pminValue, pmaxValue);
      delete pminValue;  delete pmaxValue;
      pPlot->setGeometry(rectPlot);
      if(isAssignCorrespondingReportPlot)  pTabChannel->AddAssignCorrespondingReportPlot(pPlot);
      else                                 AddPlot(pPlot);
      rectReport |= rectPlot;
      rectPlot.translate(rectPlot.width()+10,0);
      j += 1;
    }

    yTopChannel += rectPlot.height()+20;
    rectPlot.moveTopLeft(QPoint(10,yTopChannel));

    n += 1;
  }

  rectReport.setTopLeft(QPoint(0,0));
  rectReport.setBottomRight(QPoint(rectReport.right()-3,// + 15,
                                   rectReport.bottom()));// + 4));// + PTabWidgetStates->tabBar()->rect().height()));

  setGeometry(QRect(0,0, rectReport.right()+38,rectPlot.height()+70));
  pScrollAreaReportContents->setMinimumSize(rectReport.right()+5,rectReport.bottom()+3);
  setWidget(pScrollAreaReportContents);
  return rectReport;
}
//--------------------------------------------------------------------------------------------
bool MTelemetryTabReport::InsertDataFromFileReport(const QString& pathNameFileReport)
{
  int iResult = 0; // OK
  bool bMaybeFileCorrupted = false;

  QString sReportFileVersion;
  int countTelemetryChannels = 0;
  QList<QString>        listNamesChannels;
  QList<QString>        listMinMaxNormalValues;
  QList<int>            listCountPlotsInChannels;       int countPlotsInChannel;
  QList<QList<QString>> listNameParamsInChannelsTemp;   QList<QList<QString>> listNameParamsInChannels;
  QList<QString> list;

  QString sNameChannel;  QString sNameParam;  QString s;

  QFile fileReport(pathNameFileReport);
  if(!fileReport.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox msgBox; msgBox.setText("Problem file \""+pathNameFileReport+"\"");   msgBox.exec();
  }
  QTextStream streamFileReport(&fileReport);

  QString line;  int lineLength;  int pos1,pos2;
  int countAllPlots;  int indexAllPlots;
  QRegExp rex("[+.0-9]");  QRegExp rey("N/A|[-+.0-9]");
  QString sValue;  qreal fx,fy;

  while(!streamFileReport.atEnd())
  {
    line = streamFileReport.readLine();
    if(line.length() == 0)  continue;

//=Check ReportFileVersion:
    if(sReportFileVersion.length() == 0)
    {
      sReportFileVersion = ReportFileVersion;
      if(line.mid(0, sReportFileVersion.length()) != sReportFileVersion)  { iResult = 2;  break; }  // Unsupported version of file
      continue;
    }


    pos1 = 0;  pos2 = 0;

    if(listNamesChannels.isEmpty())
    {
      lineLength = line.length();
//= Reading the Channel Names:
      while(pos2 >= 0)
      {
        pos2 = line.indexOf('|', pos1);
        if(pos2 >= 0)
        {
          sNameChannel = line.mid(pos1, pos2-pos1);  pos1 = pos2+1;
          listNamesChannels << sNameChannel.trimmed();
          countTelemetryChannels += 1;
        }
      }
      sNameChannel = line.mid(pos1);
      listNamesChannels << sNameChannel.trimmed();
      countTelemetryChannels += 1;
      continue;
    }

    if(listCountPlotsInChannels.isEmpty())
    {
      if(line.length() != lineLength)  { iResult = 1;  break; }  // corrupted file
//= Reading the Channel Params Names:
      int pos2r, posm;
      QString sParamX = "T(sec)";   countPlotsInChannel = 0;
      pos2 = line.indexOf(sParamX, pos1);

      while(pos2 < line.length())
      {
        countPlotsInChannel += 1;
        pos1 = pos2 + sParamX.length();
        pos2 = line.indexOf(sParamX, pos1);
        pos2r = line.indexOf("|", pos1);
        if(pos2 < 0)  pos2 = line.length();

        posm = pos2r > 0 ? qMin(pos2,pos2r) : pos2;
        sNameParam = line.mid(pos1, posm-pos1);
        list << sNameParam.trimmed();
        if((pos2r >= 0 && pos2r < pos2) || (pos2r < 0 && pos2 == line.length()))
        {
          listCountPlotsInChannels << countPlotsInChannel;   countPlotsInChannel = 0;
          listNameParamsInChannelsTemp << list;  list.clear();
        }
      }

      countAllPlots = 0;
      for(int n = 0; n < listCountPlotsInChannels.count(); n++)
      {
        countAllPlots += listCountPlotsInChannels[n];
      }
      continue;
    }

    if(listMinMaxNormalValues.isEmpty())
    {
      if(line.length() != lineLength)  { iResult = 1;  break; }  // corrupted file
//= Reading the Min and Max Normal Values:
//MinNormalValue:
      QString sMinNormalValue;  QString sMaxNormalValue;
      while(1)
      {
        pos1 = line.indexOf(rey, pos2);
        if(pos1 < 0) break;
        pos2 = line.indexOf(' ', pos1);
        sMinNormalValue = line.mid(pos1, pos2-pos1);
        listMinMaxNormalValues << sMinNormalValue;
//MaxNormalValue:
        pos2 += 4; //  " -- "
        pos1 = line.indexOf(rey, pos2);
        if(pos1 < 0) break;
        pos2 = line.indexOf(' ', pos1);
        sMaxNormalValue = line.mid(pos1, pos2-pos1);
        listMinMaxNormalValues << sMaxNormalValue;
      }

      int m = 0;
      for(int n = 0; n < listNameParamsInChannelsTemp.count(); n++)
      {
        for(int j = 0; j < listNameParamsInChannelsTemp[n].count(); j++)
        {
          list << listNameParamsInChannelsTemp[n][j];
          list << listMinMaxNormalValues[m] << listMinMaxNormalValues[m+1];
          m += 2;
        }
        listNameParamsInChannels << list;  list.clear();
      }

      FillTabReport(countTelemetryChannels, listNamesChannels, listCountPlotsInChannels, listNameParamsInChannels, false);//bool isAssignCorrespondingReportPlot)

      continue;
    }

//= Reading the Params Values:
    if(bMaybeFileCorrupted)          { iResult = 1;      break; }  // corrupted file
    if(line.length() != lineLength)  { bMaybeFileCorrupted = true;  }
    indexAllPlots = 0;
    while(1)
    {
//dt:
      pos1 = line.indexOf(rex, pos2);
      if(pos1 < 0) break;
      pos2 = line.indexOf(' ', pos1);
      sValue = line.mid(pos1, pos2-pos1);  fx = sValue.toFloat();//  pos1 = pos2;
//param:
      pos1 = line.indexOf(rey, pos2);
      if(pos1 < 0) break;
      pos2 = line.indexOf(' ', pos1);
      sValue = line.mid(pos1, pos2-pos1);  fy = sValue.toFloat();

      GetPPlot(indexAllPlots)->AddValue(fx,fy);
      indexAllPlots += 1;
    }
  }
  fileReport.close();

  if(iResult == 1 || iResult == 2) { // Not OK
    QString sMessage;
    if(iResult == 1)  sMessage = "Selected file:\n\"" + pathNameFileReport + "\"\nis corrupted.";
    else              sMessage = "Unsupported version of selected file:\n\"" + pathNameFileReport + "\".";
    QMessageBox msgBox(QMessageBox::Critical, "Open Report File", sMessage);  msgBox.exec();
  }

  return iResult == 0 ? true : false;
}
//--------------------------------------------------------------------------------------------
