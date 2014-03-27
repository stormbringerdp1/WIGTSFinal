#ifndef MTELEMETRYTABBARREPORTS_H
#define MTELEMETRYTABBARREPORTS_H

#include <QTabBar>
#include <QMouseEvent>

//============================================================================================
class MTelemetryTabBarReports : public QTabBar
{
    Q_OBJECT
public:
    explicit MTelemetryTabBarReports(QWidget *parent = 0);

private:
public:
protected:
  virtual void mousePressEvent(QMouseEvent* event);

signals:

public slots:

};

//============================================================================================
#endif // MTELEMETRYTABBARREPORTS_H
