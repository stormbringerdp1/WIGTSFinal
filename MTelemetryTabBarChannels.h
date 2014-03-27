#ifndef MTELEMETRYTABBARCHANNELS_H
#define MTELEMETRYTABBARCHANNELS_H

#include "MPlot.h"

#include <QTabBar>
#include <QPaintEvent>
//============================================================================================
class MTelemetryTabBarChannels : public QTabBar
{
    Q_OBJECT
public:
    explicit MTelemetryTabBarChannels(QWidget *parent = 0);

private:
  int CurrentIndex;

protected:
  virtual void paintEvent(QPaintEvent* event);

signals:

public slots:
  void SlotCurrentChanged(int newIndex);
  void SlotTabBarClicked(int clickedIndex);
  void SlotUpdateTabBar();
};
//============================================================================================

#endif // MTELEMETRYTABBARCHANNELS_H
