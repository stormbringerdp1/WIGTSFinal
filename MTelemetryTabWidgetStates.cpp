#include "MTelemetryTabWidgetStates.h"
#include "MTelemetryTabBarStates.h"
#include "MTelemetry.h"
//#include "MTelemetryTabState.h"
//#include "MTelemetryTabReport.h"
//#include "MTelemetryTabChannel.h"
#include "funcs.h"

#include <QList>
//#include <QSlider>
//#include <QGraphicsScene>
//#include <QGraphicsView>

//============================================================================================
MTelemetryTabWidgetStates::MTelemetryTabWidgetStates(QWidget *parent) //, MTelemetryTabWidgetChannels* pTabWidgetChannels)
                         : QTabWidget(parent)//, PTabWidgetChannels(pTabWidgetChannels)
{
  MTelemetryTabBarStates* pTabBar = new MTelemetryTabBarStates(this);
  setTabBar(pTabBar);

  CurrentIndex = 0; // IsNewTab = false;
  connect(this, SIGNAL(currentChanged(int)), SLOT(SlotIndexChanged(int)));

//  connect(this, SIGNAL(tabBarClicked(int)), SLOT(SlotTabBarClicked(int)));
}
//--------------------------------------------------------------------------------------------
/*
void MTelemetryTabWidgetStates::SlotTabBarClicked(int index)
{
//QMessageBox msgBox;  msgBox.setText("void MTelemetryTabWidgetReports::SlotTabBarClicked(int index)");   msgBox.exec();

//  IsActive = true;

//  MTelemetry* pTelemetry = qobject_cast<MTelemetry*>(parent()->parent()->parent()->parent());
//  Q_ASSERT(pTelemetry != NULL); //  Q_CHECK_PTR(pTelemetry);
//  pTelemetry->ActiveTabWidgetStates();
  GetPMainWnd()->GetPTelemetry()->ActivateTabWidgetStates();
}
*/
//--------------------------------------------------------------------------------------------
bool MTelemetryTabWidgetStates::event(QEvent* e)
{
  if(e->type() == QEvent::MouseButtonPress && !isEnabled()) {
//QMessageBox msgBox(QMessageBox::Warning, "Open Report File", "bool MTelemetryTabWidgetReports::event(QEvent* e)");  msgBox.exec();
    GetPMainWnd()->GetPTelemetry()->ActivateTabWidgetStates();
  }
  return QTabWidget::event(e);
}
//--------------------------------------------------------------------------------------------
QRect MTelemetryTabWidgetStates::AddTabs()
{
  QRect rectTelemetryStates;   QRect rect;
/*
//  MTelemetryTabBarChannels* pTabBar = qobject_cast<MTelemetryTabBarChannels*>(tabBar());
//  Q_ASSERT(pTabBar != NULL); // Q_CHECK_PTR(pTabBar);

  MTelemetryTabState* pTabState; //  MPlot* pPlot;  //QWidget* pTab;
  int count = GetPMainWnd()->GetPListParamsTelemetryStates()->count();
  for (int i = 0; i < count; i++) {  //    pTabChannel = new QWidget();
    pTabChannel = new MTelemetryTabState(this, &(*GetPMainWnd()->GetPListParamsTelemetryStates())[i]);
    rect = pTabState->GetRectState(); // pTab->setGeometry(rect);
    if(i == 0)  rectTelemetryStates = rect;
    else        rectTelemetryStates |= rect;
    addTab(pTabState, QIcon((*GetPMainWnd()->GetPListParamsTelemetryStates())[i][1]), (*GetPMainWnd()->GetPListParamsTelemetryStates())[i][0]);

//    connect(pTabChannel, SIGNAL(SignalUpdateTabBar()), pTabBar, SLOT(SlotUpdateTabBar()));
  }
//tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
//tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0));

  return rectTelemetryStates;
*/

  addTab(new QWidget(this), "State");
  return QRect(0,0, 0,0);
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabWidgetStates::SlotIndexChanged(int newIndex)
{
  tabBar()->setTabTextColor(CurrentIndex, QColor(0,0,0));
  tabBar()->setTabTextColor(newIndex, QColor(0,0,255));
  CurrentIndex = newIndex;
}
//--------------------------------------------------------------------------------------------
