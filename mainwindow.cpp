/*Class responsible for Readiness UI configuration*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

QString SNameTelemetryTabChannel_HS;
QString SNameTelemetryTabChannel_Angles;
QString SNameTelemetryTabChannel_TP;

//============================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{

if(PTelemetry->isWindow())
{
  QMessageBox msgBox;
  msgBox.setText("MainWindow::~MainWindow()\nPTelemetry->isWindow() == true   <<===  Correct!");
  msgBox.exec();
}
if(PReadiness->isWindow())
{
  QMessageBox msgBox;
  msgBox.setText("MainWindow::~MainWindow()\nPReadiness->isWindow() == true   <<===  Correct!");
  msgBox.exec();
}
  DeleteArduinoConnector();
//--
    delete ui;
}
//--------------------------------------------------------------------------------------------
bool MainWindow::event(QEvent* event)
{
  if(event->type() == QEvent::Close)
  {
    if(PReadiness->parentWidget() == NULL)  PReadiness->setParent(this); //  PReadiness->close();
    if(PTelemetry->parentWidget() == NULL)  PTelemetry->setParent(this); //  PTelemetry->close();
  }

return QMainWindow::event(event); //catch different events
}
//--------------------------------------------------------------------------------------------
void MainWindow::Init()
{
  CreateArduinoConnector();

  InitLists();

//===####  II: 1) Right (Readiness):  ####===

  PReadiness = new MReadiness();
  PReadiness->Init();


//===####  III: 1) Right (Telemetry):  ####===

  PTelemetry = new MTelemetry(this);
  PTelemetry->Init();

//===####    ####===

  SlotSwitchToReadiness();

//======
  connect(PReadiness,
          SIGNAL(SignalSetTypeModeReadiness()),
          PArduinoConnector,
          SLOT(SlotSetTypeModeReadiness()));

  connect(PTelemetry,
          SIGNAL(SignalSetTypeModeTelemetry()),
          PArduinoConnector,
          SLOT(SlotSetTypeModeTelemetry()));

  connect(PTelemetry,
          SIGNAL(SignalGetAllTelemetryValues()),
          PArduinoConnector,
          SLOT(SlotGetAllTelemetryValues()));

  connect(PTelemetry,
          SIGNAL(SignalStopTakingData()),
          PArduinoConnector,
          SLOT(SlotStopTakingData()));
}
//--------------------------------------------------------------------------------------------
void MainWindow::CreateArduinoConnector()
{
  PArduinoConnector = new MArduinoConnector();

  PArduinoConnector->moveToThread(&ThreadArduinoConnector);

  connect(&ThreadArduinoConnector,
          &QThread::finished,
          PArduinoConnector,
          &QObject::deleteLater);

  ThreadArduinoConnector.start();
}
//--------------------------------------------------------------------------------------------
void MainWindow::DeleteArduinoConnector()
{
  ThreadArduinoConnector.quit();   ThreadArduinoConnector.wait();
}
//--------------------------------------------------------------------------------------------
void MainWindow::SlotMessage(QString* pStr)
{
  QMessageBox msg;  msg.setText(*pStr);   msg.exec();
}
//--------------------------------------------------------------------------------------------
void MainWindow::InitLists()
{
  QString sTextTestTitle("   State of Execution\n");

  QString sNameChannel;     QString sNameChannelPixmapFile;
  QString sTextTest;
  QString sInputTestName1;  QString sInputTestValue1;  QString sOutputTestName1;
  QString sInputTestName2;  QString sInputTestValue2;  QString sOutputTestName2;
  QString sParam1;          QString sParam2;
  QList<QString> list;


// == 1) Engines:  ===

sTextTest = sTextTestTitle;
  sNameChannel = "Right Engine";  sNameChannelPixmapFile = ":/Engine.png";
  sInputTestName1 = "Min Voltage (V)";
  sTextTest += sInputTestName1 + ":\t\t";
  sInputTestValue1 = "5";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "RPM";
  sParam1 = "    ";  sParam1 += sOutputTestName1;  sParam1 += ":\t\t\t\t\t\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Voltage (V)";
  sTextTest += sInputTestName2 + ":\t\t"; //20\n";
  sInputTestValue2 = "20";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "RPM";
  sParam2 = "    " + sOutputTestName2 + ":\t\t\t\t\t\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
//        0               1                         2
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
 //       3                  4                   5                   6          7        8
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "8"   << "12"
//        9                  10                  11                  12         13       14
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "400" << "600";
  IFirstItemListParamsReadiness = 6;  //  number of "sParam1"
  DeltaNItemListParamsReadiness = 6;  // = 12-6: distance between "sParam1" and "sParam2"
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(318.0,111.0, 50.0,50.0);

//=For ChannelStart:
  ListNamesEngines << sNameChannel;

//=For Telemetry Channel:
//        0               1
  list << sNameChannel << sNameChannelPixmapFile
//        2                         3        4
       << "Rotation Speed (RPM)" << "0.5" << "99.5" //  "2.0 - 98.0"  is the the Range of the "Rotation Speed (RPM)"
//        5                          6        7
       << "Power Consumption (W)" << "0.5" << "99.5"; //  "2.0 - 98.0"  is the the Range of the "Power Consumption (W)"
  IFirstItemListParamsTelemetry = 2; // number of the first param
  ListParamsTelemetryChannels << list;   list.clear();


sTextTest = sTextTestTitle;
  sNameChannel = "Left Engine";  sNameChannelPixmapFile = ":/Engine.png";
  sInputTestName1 = "Min Voltage (V)";
  sTextTest += sInputTestName1 + ":\t\t"; //5\n";
  sInputTestValue1 = "5";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "RPM";
  sParam1 = "    " + sOutputTestName1 + ":\t\t\t\t\t\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Voltage (V)";
  sTextTest += sInputTestName2 + ":\t\t"; //20\n";
  sInputTestValue2 = "20";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "RPM";
  sParam2 = "    " + sOutputTestName2 + ":\t\t\t\t\t\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "8"   << "12"
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "400" << "600";
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(154.0, 111.0, 50.0, 50.0);

//=For ChannelStart:
  ListNamesEngines << sNameChannel;

//=For Telemetry Channel:
  list << sNameChannel << sNameChannelPixmapFile
       << "Rotation Speed (RPM)" << "0.5" << "99.5"
       << "Power Consumption (W)" << "0.5" << "99.5";
  ListParamsTelemetryChannels << list;   list.clear();


// == 2) Sensors:  ===

sTextTest = sTextTestTitle;
  sNameChannel = "Right Sensor";  sNameChannelPixmapFile = ":/sensor.png";
  sInputTestName1 = "Min Angle (°)";
  sTextTest += sInputTestName1 + ":\t\t"; //1\n";
  sInputTestValue1 = "1";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "Voltage (V)";
  sParam1 = "    " + sOutputTestName1 + ":\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Angle (°)";
  sTextTest += sInputTestName2 + ":\t\t"; //10\n";
  sInputTestValue2 = "10";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "Voltage (V)";
  sParam2 = "    " + sOutputTestName2 + ":\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "4.5" << "5.5"
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "19"  << "21";
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(300.0,345.0, 50.0,50.0);

//=For ChannelStart:
  ListNamesSensors << sNameChannel;

//=For Telemetry Channel:
  list << sNameChannel << sNameChannelPixmapFile
       << "Voltage (V)" << "20.0" << "80.0"
       << "Power Consumption (W)" << "0.5" << "99.5";
  ListParamsTelemetryChannels << list;   list.clear();


sTextTest = sTextTestTitle;
  sNameChannel = "Left Sensor";  sNameChannelPixmapFile = ":/sensor.png";
  sInputTestName1 = "Min Angle (°)";
  sTextTest += sInputTestName1 + ":\t\t"; //1\n";
  sInputTestValue1 = "1";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "Voltage (V)";
  sParam1 = "    " + sOutputTestName1 + ":\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Angle (°)";
  sTextTest += sInputTestName2 + ":\t\t"; //10\n";
  sInputTestValue2 = "10";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "Voltage (V)";
  sParam2 = "    " + sOutputTestName2 + ":\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "9.5" << "10.5"   //  "4.5" << "5.5"
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "9.5" << "10.5";  //  "19"  << "21";
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(167.0,345.0, 50.0,50.0);

//=For ChannelStart:
  ListNamesSensors << sNameChannel;

//=For Telemetry Channel:
  list << sNameChannel << sNameChannelPixmapFile
       << "Voltage (V)" << "0.5" << "99.5"
       << "Power Consumption (W)" << "0.5" << "99.5";
  ListParamsTelemetryChannels << list;   list.clear();


// == 3) ServoMotors:  ===

sTextTest = sTextTestTitle;
//  sNameChannel = "Right ServoMotor";  sNameChannelPixmapFile = ":/Engine.png";
  sNameChannel = "Right ServoMotor";  sNameChannelPixmapFile = ":/servos.png";
  sInputTestName1 = "Min Voltage (V)";
  sTextTest += sInputTestName1 +":\t\t"; //5\n";
  sInputTestValue1 = "5";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "Angle (°)";
  sParam1 = "    " + sOutputTestName1 + ":\t\t\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Voltage (V)";
  sTextTest += sInputTestName2 + ":\t\t"; //20\n";
  sInputTestValue2 = "20";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "Angle (°)";
  sParam2 = "    " + sOutputTestName2 + ":\t\t\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "0.8" << "1.2"
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "9.5" << "10.5";
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(278.0,712.0, 35.0,35.0);

//=For ChannelStart:
  ListNamesServoMotors << sNameChannel;

//=For Telemetry Channel:
  list << "Longerons" << sNameChannelPixmapFile
       << "Left Longeron Angle (°)" << "0.5" << "99.5"
       << "Right Longeron Angle (°)" << "0.5" << "99.5";
  ListParamsTelemetryChannels << list;   list.clear();
  ILongeronsTelemetryChannel = ListParamsTelemetryChannels.count() - 1;


sTextTest = sTextTestTitle;
//  sNameChannel = "Left ServoMotor";  sNameChannelPixmapFile = ":/Engine.png";
  sNameChannel = "Left ServoMotor";  sNameChannelPixmapFile = ":/servos.png";
  sInputTestName1 = "Min Voltage (V)";
  sTextTest += sInputTestName1 +":\t\t"; //5\n";
  sInputTestValue1 = "5";
  sTextTest += sInputTestValue1;  sTextTest += "\n";
  sOutputTestName1 = "Angle (°)";
  sParam1 = "    " + sOutputTestName1 + ":\t\t\t\t";
  sTextTest += sParam1;  sTextTest += "\n";

  sInputTestName2 = "Max Voltage (V)";
  sTextTest += sInputTestName2 + ":\t\t"; //20\n";
  sInputTestValue2 = "20";
  sTextTest += sInputTestValue2;  sTextTest += "\n";
  sOutputTestName2 = "Angle (°)";
  sParam2 = "    " + sOutputTestName2 + ":\t\t\t\t";
  sTextTest += sParam2;

//=For ChannelTest:
  list << sNameChannel << sNameChannelPixmapFile << sTextTest
       << sInputTestName1 << sInputTestValue1 << sOutputTestName1 << sParam1 << "0.8" << "1.2"
       << sInputTestName2 << sInputTestValue2 << sOutputTestName2 << sParam2 << "9.5" << "10.5";
  ListParamsReadinessChannels << list;   list.clear();
  ListTestRectsReadinessChannels << QRectF(205.0, 712.0, 35.0, 35.0);

//=For ChannelStart:
  ListNamesServoMotors << sNameChannel;

  SNameTelemetryTabChannel_HS = "H, S";
//        0           1
  list << SNameTelemetryTabChannel_HS << ":/Engine.png"
//        2           3       4
       << "Height (cm)" << "N/A" << "N/A"
//        5          6       7
       << "Speed (m/s)" << "N/A" << "N/A";
  ListParamsTelemetryStates << list;   list.clear();
  IFirstItemListParamsStates = 2;  //  number of the first param ("Height")

  SNameTelemetryTabChannel_Angles = "Angles";
  list << SNameTelemetryTabChannel_Angles << ":/Engine.png"
       << "Pitch (°)" << "N/A" << "N/A"
       << "Roll (°)" << "N/A" << "N/A";
  ListParamsTelemetryStates << list;   list.clear();

  list << "D, A" << ":/Engine.png"
       << "Distance Traveled (m)" << "N/A" << "N/A"
       << "Acceleration (m/s**2)" << "N/A" << "N/A";
  ListParamsTelemetryStates << list;   list.clear();

  SNameTelemetryTabChannel_TP = "T, P";
  list << SNameTelemetryTabChannel_TP << ":/Engine.png"
       << "Temperature (°C)" << "N/A" << "N/A"
       << "Pressure (Pa)" << "N/A" << "N/A";
  ListParamsTelemetryStates << list;   list.clear();
}
//--------------------------------------------------------------------------------------------
void MainWindow::SlotSwitchToReadiness()
{
  SwitchSplitterTo(PReadiness);
}
//--------------------------------------------------------------------------------------------
void MainWindow::SlotSwitchToTelemetry()
{
  SwitchSplitterTo(PTelemetry);
}
//--------------------------------------------------------------------------------------------
void MainWindow::SwitchSplitterTo(QWidget* pWidget)
{
  takeCentralWidget();
  setCentralWidget(pWidget);
}
//--------------------------------------------------------------------------------------------
