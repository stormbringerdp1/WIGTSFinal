#ifndef MLABELCLOSETAB_H
#define MLABELCLOSETAB_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>

//============================================================================================
class MLabelCloseTab : public QLabel
{
    Q_OBJECT
public:
    explicit MLabelCloseTab(QWidget* parent, QWidget* pTab);

private:
  QWidget* PWidgetReports;
  QWidget* PTab;

protected:
  virtual bool event(QEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);


signals:

public slots:

};
//============================================================================================

#endif // MLABELCLOSETAB_H
