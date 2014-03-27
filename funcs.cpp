#include "funcs.h"

#include <QMessageBox>
#include <QApplication>

//============================================================================================
MainWindow* GetPMainWnd()
{
  MainWindow* pMainWindow;
  foreach(QWidget* pwidget, QApplication::topLevelWidgets()) {
    pMainWindow = qobject_cast<MainWindow*>(pwidget);
    if(pMainWindow != NULL) break;
  }

//  MainWindow* pMainWindow = qobject_cast<MainWindow*>(parentWidget()->parentWidget());
  if(pMainWindow == NULL) {
//    QString s;  s.sprintf("ind = %d", ind);
    QMessageBox msgBox(QMessageBox::Critical, QCoreApplication::instance()->applicationName(), "GetPMainWnd() == NULL");   msgBox.exec();
  }
  return pMainWindow;
}
//--------------------------------------------------------------------------------------------
QLineEdit* CreateLineEditCurrentValue(QWidget* pParent, const QRect& rectNameValue, const QString& nameValue, const QRect& rectValue)
{
  QLineEdit* pLineEditNameParam = new QLineEdit(pParent);
//      pLineEditNameParam->setObjectName(QStringLiteral("LineEditNameParam"));
  pLineEditNameParam->setGeometry(rectNameValue);
  pLineEditNameParam->setFrame(false);
  pLineEditNameParam->setReadOnly(true);
  pLineEditNameParam->setStyleSheet(QLatin1String("background: transparent;"));
  pLineEditNameParam->setAlignment(Qt::AlignLeft);//Center);
  pLineEditNameParam->setText(nameValue);

  QLineEdit* pLineEditCurrentParam = new QLineEdit(pParent);
//      pLineEditCurrentParam->setObjectName(QStringLiteral("LineEditNameParam"));
  pLineEditCurrentParam->setGeometry(rectValue);
  pLineEditCurrentParam->setFrame(false);
  pLineEditCurrentParam->setReadOnly(true);
  pLineEditCurrentParam->setStyleSheet(QLatin1String("background: transparent;"));
  pLineEditCurrentParam->setAlignment(Qt::AlignRight);//Center);

  return pLineEditCurrentParam;
}
//--------------------------------------------------------------------------------------------
