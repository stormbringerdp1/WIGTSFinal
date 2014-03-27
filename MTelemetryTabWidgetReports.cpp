#include "MTelemetryTabWidgetReports.h"
#include "MTelemetryTabBarReports.h"
#include "MTelemetryTabReport.h"
#include "MLabelCloseTab.h"
#include "funcs.h"

//============================================================================================
MTelemetryTabWidgetReports::MTelemetryTabWidgetReports(QWidget *parent) //, MTelemetryTabWidgetChannels* pTabWidgetChannels)
                          : QTabWidget(parent) //, PTabWidgetChannels(pTabWidgetChannels)
{
  setTabBar(new MTelemetryTabBarReports(this));

  CurrentIndex = 0; // IsNewTab = false;
  connect(this,
          SIGNAL(currentChanged(int)),
          SLOT(SlotIndexChanged(int)));

}
//--------------------------------------------------------------------------------------------
QRect MTelemetryTabWidgetReports::AddTabs()
{

  QRect rectTelemetryReports;
  MTelemetryTabReport* pTabReport = new MTelemetryTabReport(this);
  pTabReport->Init();
  addTab(pTabReport, "Report");//listNameTabs[i]);
  rectTelemetryReports = pTabReport->geometry(); //rectReport;

  QWidget* pTab = new QWidget(this);
  int index = addTab(pTab, "");//listNameTabs[i]);
  return rectTelemetryReports;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabWidgetReports::SlotIndexChanged(int newIndex)
{
  tabBar()->setTabTextColor(CurrentIndex, QColor(0,0,0));
  tabBar()->setTabTextColor(newIndex, QColor(0,0,255));
  CurrentIndex = newIndex;
}
//--------------------------------------------------------------------------------------------
bool MTelemetryTabWidgetReports::event(QEvent* e)
{
  if(e->type() == QEvent::MouseButtonPress && !isEnabled()) {
    GetPMainWnd()->GetPTelemetry()->ActivateTabWidgetReports();
  }
  return QTabWidget::event(e);
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabWidgetReports::CloseTab(QWidget* pTab)
{
  int index = indexOf(pTab);
  removeTab(index);
  if(index  == currentIndex())  setCurrentIndex(qMin(index,count()-2));
}
//--------------------------------------------------------------------------------------------
