#ifndef MTELEMETRYTABREPORT_H
#define MTELEMETRYTABREPORT_H

#include "MPlot.h"
#include "MTelemetryTabWidgetChannels.h"
#include <QScrollArea>
#include <QList>
#include <QEvent>

//============================================================================================
class MTelemetryTabReport : public QScrollArea //QWidget
{
    Q_OBJECT
public:
    explicit MTelemetryTabReport(QWidget* parent);

private:
  QList<MPlot*> ListPlots;
  void FormListsParams(MTelemetryTabWidgetChannels* pTabWidget, int& countParamsTelemetryChannels,
                       QList<QString>& listNamesChannels, QList<int>& listCountPlotsInChannels,
                       QList<QList<QString>>& listNameParamsInChannels);


protected:
public:
  void Init();

  QRect FillTabReport(int countParamsTelemetryChannels, QList<QString>& listNamesChannels,
                      QList<int>& listCountPlotsInChannels, QList<QList<QString>>& listNameParamsInChannels,
                      bool isAssignCorrespondingReportPlot);

  void AddPlot(MPlot* pPlot);
  MPlot* GetPPlot(int index) { return ListPlots[index]; }

  bool InsertDataFromFileReport(const QString& pathNameFileReport);

signals:

public slots:

};
//============================================================================================

#endif // MTELEMETRYTABREPORT_H
