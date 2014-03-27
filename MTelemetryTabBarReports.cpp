#include "MTelemetryTabBarReports.h"
#include "MTelemetryTabWidgetReports.h"
#include "MLabelCloseTab.h"
#include "funcs.h"

#include <QFileDialog>
#include <QMessageBox>

//============================================================================================
MTelemetryTabBarReports::MTelemetryTabBarReports(QWidget *parent) :
    QTabBar(parent)
{
}
//--------------------------------------------------------------------------------------------
void MTelemetryTabBarReports::mousePressEvent(QMouseEvent* event)
{
  if(event->button() == Qt::LeftButton && tabAt(event->pos()) == count()-1)
  {
    QFileDialog dialog(this, "Open File Report", "", "Files (*.wig)");  dialog.setAcceptMode(QFileDialog::AcceptOpen);//Save);

    if(dialog.exec() == QDialog::Accepted)
    {
//=Add Report Tab:
      QString pathNameFileReport(dialog.selectedFiles().at(0));
      QString nameFileReport(dialog.directory().relativeFilePath(pathNameFileReport));

      if(*GetPMainWnd()->GetPTelemetry()->GetCurrentPNameFileReport() != nameFileReport)
      {
        MTelemetryTabWidgetReports* pTabWidgetReports = qobject_cast<MTelemetryTabWidgetReports*>(parent());
        Q_ASSERT(pTabWidgetReports != NULL); // Q_CHECK_PTR(pTabWidgetState);

        MTelemetryTabReport* pTabReport = new MTelemetryTabReport(pTabWidgetReports);
        int index = pTabWidgetReports->insertTab(count()-1, pTabReport, nameFileReport.left(nameFileReport.length()-4));
        setTabButton(count()-2, QTabBar::RightSide, new MLabelCloseTab(this, pTabReport));

        if(pTabReport->InsertDataFromFileReport(pathNameFileReport))
        {
          setCurrentIndex(index);
        }
        else  pTabWidgetReports->CloseTab(pTabReport);
      }
      else
      {
        QMessageBox msgBox(QMessageBox::Warning, "Open Report File", "Selected file:\n\""+nameFileReport+"\"\nis the current report file.");  msgBox.exec();
      }
    }
  }
  else
  {
    QTabBar::mousePressEvent(event);
  }
}
