#include "MTelemetryTabBarChannels.h"
#include "MTelemetryTabWidgetChannels.h"
#include "MTelemetryTabChannel.h"

#include <QPainter>
#include <QList>
  #include <QMessageBox>
#include <QtGlobal>

//============================================================================================
MTelemetryTabBarChannels::MTelemetryTabBarChannels(QWidget *parent)
                        : QTabBar(parent)
{
  setElideMode(Qt::ElideNone);//Qt::ElideRight);

  CurrentIndex = 0;//-1;

  //tab bar clikced, send signal to paint text in blue
  connect(this,
          SIGNAL(currentChanged(int)),
          SLOT(SlotCurrentChanged(int)));
  //tab bar is clicked, based on index
  connect(this,
          SIGNAL(tabBarClicked(int)),
          SLOT(SlotTabBarClicked(int)));
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabBarChannels::SlotTabBarClicked(int clickedIndex)
{
  MTelemetryTabWidgetChannels* pTabWidgetChannels = qobject_cast<MTelemetryTabWidgetChannels*>(parent());
  Q_ASSERT(pTabWidgetChannels != NULL); // Q_CHECK_PTR(pTabWidgetChannels);

  MTelemetryTabChannel* pTabChannel = qobject_cast<MTelemetryTabChannel*>(pTabWidgetChannels->widget(clickedIndex));
  Q_ASSERT(pTabChannel != NULL); // Q_CHECK_PTR(pTabChannel);

  //if tab flickers, then upon cliking, flickering deasapers
  if(pTabChannel->GetIsFlicker())  pTabChannel->StopFlicker();
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabBarChannels::SlotCurrentChanged(int newIndex)
{
  //when tab is cliked, text is displayed in blue, making it active
  setTabTextColor(CurrentIndex, QColor(0,0,0));
  setTabTextColor(newIndex, QColor(0,0,255));
  CurrentIndex = newIndex;
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabBarChannels::paintEvent(QPaintEvent* event)
{
  QTabBar::paintEvent(event);
//--
  QPainter painter(this);

  painter.setPen(Qt::NoPen);

  MTelemetryTabWidgetChannels* pTabWidgetChannels = qobject_cast<MTelemetryTabWidgetChannels*>(parent());
  Q_ASSERT(pTabWidgetChannels != NULL); // Q_CHECK_PTR(pTabWidget);

  MTelemetryTabChannel* pTabChannel;  //  QColor* pColorBrushTab;

  bool isColorBrushFlicker;  QList<bool>* pListIsNonNormalMinValue;  QList<bool>* pListIsNonNormalMaxValue;

  for(int i = 0; i < pTabWidgetChannels->count(); i++)
  {
    pTabChannel = qobject_cast<MTelemetryTabChannel*>(pTabWidgetChannels->widget(i));
    Q_ASSERT(pTabChannel != NULL); // Q_CHECK_PTR(pTabChannel);

    if(pTabChannel->GetIsFlicker(&isColorBrushFlicker, &pListIsNonNormalMinValue, &pListIsNonNormalMaxValue))
    {
      if(isColorBrushFlicker)
      {
        painter.setBrush(QBrush(QColor(255,0,0, 100)));
        QRect rect = tabRect(i);
        if(currentIndex() != i)
        {
          rect.setTopLeft(rect.topLeft()+= QPoint(1,2));
          rect.setBottomRight(rect.bottomRight() += QPoint(-1,0));
        }
        else
        {
          rect.setTopLeft(rect.topLeft() += QPoint(-1,0));
          rect.setBottomRight(rect.bottomRight() += QPoint(1,0));
        }

        int count = pListIsNonNormalMinValue->count();
        int w = rect.width()/count;  int h = rect.height()/2;
        QList<QRect> listRectsFlicker;
        for(int j = 0; j < count; j++)
        {
          if((*pListIsNonNormalMaxValue)[j])
          {
            listRectsFlicker << QRect(rect.left() + j*w, rect.top(), w,h);
          }
          if((*pListIsNonNormalMinValue)[j])
          {
            listRectsFlicker << QRect(rect.left() + j*w, rect.top() + h, w,h);
          }
        }
        for(int k = 0; k < listRectsFlicker.count(); k++)
        {
          painter.drawRect(listRectsFlicker[k]);
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabBarChannels::SlotUpdateTabBar()
{
  update();
}
//--------------------------------------------------------------------------------------------
