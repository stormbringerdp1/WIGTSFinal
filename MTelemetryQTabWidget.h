#ifndef MTELEMETRYTABREPORT_H
#define MTELEMETRYTABREPORT_H

#include <QWidget>
#include <QMouseEvent>

//============================================================================================
class MTelemetryTabReport : public QWidget
{
    Q_OBJECT
public:
    explicit MTelemetryTabReport(QWidget *parent = 0);
  virtual ~MTelemetryTabReport();

protected:
  virtual void mousePressEvent(QMouseEvent* event);

signals:

public slots:

};
//============================================================================================

#endif // MTELEMETRYTABREPORT_H
