#include "MTelemetryTabWidgetChannels.h"
#include "MTelemetryTabBarChannels.h"
#include "MTelemetryTabChannel.h"
#include "funcs.h"

//============================================================================================
MTelemetryTabWidgetChannels::MTelemetryTabWidgetChannels(QWidget *parent, QList<QList<QString>>* pListParamsTelemetryChannels,
                                                                          int iFirstItemListParamsTelemetry)
                           : QTabWidget(parent),
                             PListParamsTelemetryChannels(pListParamsTelemetryChannels),
                             IFirstItemListParamsTelemetry(iFirstItemListParamsTelemetry)
{
  //initialize TabBar
  MTelemetryTabBarChannels* pTabBar = new MTelemetryTabBarChannels(this);
  pTabBar->setIconSize(QSize(16,11));

  setTabBar(pTabBar);
}
//--------------------------------------------------------------------------------------------
QRect MTelemetryTabWidgetChannels::AddTabs()
{
  QRect rectTelemetryChannels;  QRect rect;

  MTelemetryTabBarChannels* pTabBar = qobject_cast<MTelemetryTabBarChannels*>(tabBar());
  Q_ASSERT(pTabBar != NULL); // Q_CHECK_PTR(pTabBar);

  MTelemetryTabChannel* pTabChannel; //  MPlot* pPlot;  //QWidget* pTab;
  int count = PListParamsTelemetryChannels->count();
  for (int i = 0; i < count; i++)
  {
    pTabChannel = new MTelemetryTabChannel(this, &(*PListParamsTelemetryChannels)[i], IFirstItemListParamsTelemetry);
    rect = pTabChannel->GetRectChannel();
    if(i == 0)  rectTelemetryChannels = rect;
    else        rectTelemetryChannels |= rect;
    addTab(pTabChannel, QIcon((*PListParamsTelemetryChannels)[i][1]), (*PListParamsTelemetryChannels)[i][0]);

    connect(pTabChannel,
            SIGNAL(SignalUpdateTabBar()),
            pTabBar,
            SLOT(SlotUpdateTabBar()));
  }
  return rectTelemetryChannels;
}
//--------------------------------------------------------------------------------------------
//= For TabWidgetStates:
bool MTelemetryTabWidgetChannels::event(QEvent* e)
{
  if(e->type() == QEvent::MouseButtonPress && !isEnabled())
  {
    GetPMainWnd()->GetPTelemetry()->ActivateTabWidgetStates();
  }
  return QTabWidget::event(e);
}
//--------------------------------------------------------------------------------------------

