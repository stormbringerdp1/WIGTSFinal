#include "MLabelCloseTab.h"
#include "MTelemetryTabWidgetReports.h"

//============================================================================================
MLabelCloseTab::MLabelCloseTab(QWidget* parent, QWidget* pTab) :
                               QLabel(parent), PWidgetReports(parent), PTab(pTab)
{
  setPixmap(QPixmap(QString::fromUtf8(":/RemoveTabInactive.ico")));
  setAttribute(Qt::WA_Hover, true);
}
//--------------------------------------------------------------------------------------------
bool MLabelCloseTab::event(QEvent* event)
{
  bool b = QLabel::event(event);
//--
  if(event->type() == QEvent::HoverEnter) {
    setPixmap(QPixmap(QString::fromUtf8(":/RemoveTabActive.ico")));
  }
  if(event->type() == QEvent::HoverLeave) {
    setPixmap(QPixmap(QString::fromUtf8(":/RemoveTabInactive.ico")));
  }

  return b;
}
//--------------------------------------------------------------------------------------------
void MLabelCloseTab::mousePressEvent(QMouseEvent* event)
{
  QLabel::mousePressEvent(event);
//--
  if(event->button() == Qt::LeftButton) {
    MTelemetryTabWidgetReports* pTabWidgetReports = qobject_cast<MTelemetryTabWidgetReports*>(parent()->parent());
    Q_ASSERT(pTabWidgetReports != NULL); //Q_CHECK_PTR(pTabWidgetReports != NULL);
    pTabWidgetReports->CloseTab(PTab);
  }
}
//--------------------------------------------------------------------------------------------
