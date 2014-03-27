#ifndef FUNCS_H
#define FUNCS_H

#include "mainwindow.h"

#include <QLineEdit>
//============================================================================================

MainWindow* GetPMainWnd();

QLineEdit* CreateLineEditCurrentValue(QWidget* pParent, const QRect& rectNameValue, const QString& nameValue, const QRect& rectValue);

//============================================================================================
#endif // FUNCS_H
