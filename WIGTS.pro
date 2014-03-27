#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T14:01:01
#
#-------------------------------------------------

CONFIG += static

QT += core gui widgets serialport

TARGET = WIGTS
TEMPLATE = app

SOURCES += main.cpp \
    MTelemetryTabWidgetReports.cpp \
    MTelemetryTabBarReports.cpp \
    MArduinoConnector.cpp

qtHaveModule(opengl) {
    QT += opengl

SOURCES += mainwindow.cpp \
    MSyntaxHighlighterParamsTest.cpp \
    MReadiness.cpp \
    MTelemetry.cpp \
    funcs.cpp \
    MTelemetryTabChannel.cpp \
    MPlot.cpp \
    MEkranoplan.cpp \
    MGyrohorizon.cpp \
    MEkranoplanGL.cpp \
    MReadinessTestChannel.cpp \
    MTelemetryTabReport.cpp \
    MLabelCloseTab.cpp \
    MTelemetryTabBarChannels.cpp \
    MTelemetryTabWidgetChannels.cpp

HEADERS  += mainwindow.h \
    MSyntaxHighlighterParamsTest.h \
    MReadiness.h \
    MTelemetry.h \
    funcs.h \
    MTelemetryTabChannel.h \
    MPlot.h \
    MEkranoplan.h \
    MGyrohorizon.h \
    MEkranoplanGL.h \
    MReadinessTestChannel.h \
    MTelemetryTabReport.h \
    MLabelCloseTab.h \
    MTelemetryTabBarChannels.h \
    MTelemetryTabWidgetChannels.h

RESOURCES += \
    Shaders.qrc \
    Texture.qrc \
    Pictures.qrc

FORMS    += mainwindow.ui

OTHER_FILES +=

}

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
#INSTALLS += target

RESOURCES += \
    Ekranoplan_3dsMax.qrc

HEADERS += \
    MTelemetryTabWidgetReports.h \
    MTelemetryTabBarReports.h \
    MArduinoConnector.h

FORMS +=







