#ifndef MTELEMETRYTABWIDGETREPORTS_H
#define MTELEMETRYTABWIDGETREPORTS_H

#include "MTelemetryTabReport.h"

#include <QTabWidget>
#include <QEvent>

//============================================================================================
class MTelemetryTabWidgetReports : public QTabWidget
{
    Q_OBJECT
public:
    explicit MTelemetryTabWidgetReports(QWidget *parent); //, MTelemetryTabWidgetChannels *pTabWidgetChannels);

private:
  int CurrentIndex; // bool IsNewTab;

  bool InsertDataFromFileReport(MTelemetryTabReport* pTab, const QString& pathNameFileReport);

public:
  QRect AddTabs();
  void CloseTab(QWidget* pTab);

protected:
  virtual bool event(QEvent* e);
signals:

public slots:
  void SlotIndexChanged(int newIndex);
};
//============================================================================================

#endif // MTELEMETRYTABWIDGETREPORTS_H
