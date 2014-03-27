/*Class that initializes all the processes in Readiness UI*/
#include "MReadiness.h"
#include "mainwindow.h"
#include "funcs.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QIcon>
#include <QLabel>
#include <QListWidgetItem>
  #include <QMessageBox>
#include <QProgressBar>
#include <QRadialGradient>
#include <QScrollArea>
  #include <QStatusBar>
//============================================================================================
MReadiness::MReadiness(QWidget *parent) :
  QSplitter(Qt::Horizontal, parent)
{
  SMessageBoxTitle = QCoreApplication::instance()->applicationName()+":  Readiness";
}
//--------------------------------------------------------------------------------------------
MReadiness::~MReadiness()
{
  delete[] MRPoints;
  delete[] MAlphas;
  delete[] MkAlphas;
}
//--------------------------------------------------------------------------------------------
void MReadiness::Init()
{
//===####  I: Left (Ekranoplan):  ####===

  QGraphicsScene* PSceneEkranoplan = new QGraphicsScene(this);
  PSceneEkranoplan->addPixmap(QPixmap(":/lun2.png"));
//  PSceneEkranoplan->addPixmap(QPixmap(":/Ekranoplan.png"));
  PViewEkranoplan = new QGraphicsView(PSceneEkranoplan, this);

  for(int i = 0; i < GetPMainWnd()->GetPListTestRectsReadinessChannels()->count(); i++)
  {
    ListTestRectItems << PSceneEkranoplan->addRect((*GetPMainWnd()->GetPListTestRectsReadinessChannels())[i], QPen(), QBrush());
  }

  NRPoint = 5;
  MRPoints = new qreal[NRPoint];

  qreal rPoint1 = 0.5;  qreal rPoint2 = 1.0;
  qreal dRPoint = (rPoint2-rPoint1)/(NRPoint-1);

  for(int i = 0; i < NRPoint-1; i++) { MRPoints[i] = rPoint1 + i*dRPoint; }
  MRPoints[NRPoint-1] = rPoint2;

  Alpha1 = 100;   Alpha2 = 200;   DIncrementAlpha = 5;
  DAlpha = (Alpha2-Alpha1)/(NRPoint-1);
  MAlphas = new int[NRPoint];
  MkAlphas = new int[NRPoint];


//===####  II: 1) Right (Channels):  ####===

  QSplitter* pSplitterReadiness = new QSplitter(Qt::Vertical, this);

  QList<int> list;  list << 150 << 200;    setSizes(list);   list.clear();


//===  1) ReadinessTest:

  QScrollArea* pScrollAreaTest = new QScrollArea(pSplitterReadiness);
  pScrollAreaTest->setWidgetResizable(true);

  QWidget* pScrollAreaTestContents = new QWidget(pSplitterReadiness);
  pScrollAreaTestContents->setObjectName(QStringLiteral("ScrollAreaTestContents"));

  QRect rect = FillReadinessTest(pScrollAreaTestContents);
  pScrollAreaTestContents->setMinimumSize(rect.right() + 3,rect.bottom() + 3);

  pScrollAreaTest->setWidget(pScrollAreaTestContents);


//===  2) ReadinessStart:

  QScrollArea* pScrollAreaStart = new QScrollArea(pSplitterReadiness);
  pScrollAreaStart->setGeometry(QRect(40, 70, 611, 111));
  pScrollAreaStart->setWidgetResizable(true);

  QWidget* pScrollAreaStartContents = new QWidget(pSplitterReadiness);
  rect = FillReadinessStart(pScrollAreaStartContents);

  pScrollAreaStartContents->setMinimumSize(rect.right()+3,rect.bottom()+3);
  pScrollAreaStart->setWidget(pScrollAreaStartContents);

  list << 800 << 200;   pSplitterReadiness->setSizes(list);  list.clear();
}
//--------------------------------------------------------------------------------------------
//Fill the upper right rect
QRect MReadiness::FillReadinessTest(QWidget* pScrollAreaTestContents)
{
  QRect rectReadinessTest;    QRect rect;
  int yTopChannel = 0;

  int count = GetPMainWnd()->GetPListParamsReadinessChannels()->count();
  for(int i = 0; i < count; i++)
  {
    MReadinessTestChannel* pReadinessTestChannel = new MReadinessTestChannel(pScrollAreaTestContents, yTopChannel,
                                                                           &(*GetPMainWnd()->GetPListParamsReadinessChannels())[i]);
    ListReadinessTestChannels << pReadinessTestChannel;

    connect(pReadinessTestChannel,
            SIGNAL(SignalResultTestChannel(bool)),
            this,
            SLOT(SlotResultTestChannel(bool)));

    rect = pReadinessTestChannel->GetRectChannel();

    yTopChannel += rect.height() + 10;
    if(i == 0)  rectReadinessTest = rect;
    else        rectReadinessTest |= rect;
  }

  return rectReadinessTest;
}
//--------------------------------------------------------------------------------------------
//Fill the right bottom rect
QRect MReadiness::FillReadinessStart(QWidget* pScrollAreaStartContents)
{
  QRect rectReadinessStart;    QRect rect;

//= Button "START":
  PPushButtonStart = new QPushButton(pScrollAreaStartContents);
  rect = QRect(20, 30, 61, 51);   rectReadinessStart = rect;
  PPushButtonStart->setGeometry(rect);
  PPushButtonStart->setText(QApplication::translate("MainWindow", "START", 0));

  connect(PPushButtonStart,
          SIGNAL(clicked()),
          this,
          SLOT(SlotStart()));

//create three lists at the bottom
  rect = QRect(100, 30, 121, 101);   rectReadinessStart |= rect;
  CreateListWidget("Engines", PListWidgetEngines, pScrollAreaStartContents, rect, GetPMainWnd()->GetPListNamesEngines());
  rect = QRect(250, 30, 121, 101);   rectReadinessStart |= rect;
  CreateListWidget("Sensors", PListWidgetSensors, pScrollAreaStartContents, rect, GetPMainWnd()->GetPListNamesSensors());
  rect = QRect(400, 30, 121, 101);   rectReadinessStart |= rect;
  CreateListWidget("ServoMotors", PListWidgetServoMotors, pScrollAreaStartContents, rect, GetPMainWnd()->GetPListNamesServoMotors());

//= Button "Telemetry":
  PPushButtonTelemetry = new QPushButton(pScrollAreaStartContents);
  rect = QRect(535, 10, 60, 30);   rectReadinessStart |= rect;
  PPushButtonTelemetry->setGeometry(rect);
  PPushButtonTelemetry->setAutoDefault(true);
  PPushButtonTelemetry->setText(QApplication::translate("MainWindow", "Telemetry", 0));

  connect(PPushButtonTelemetry,
          SIGNAL(clicked()),
          GetPMainWnd(),
          SLOT(SlotSwitchToTelemetry()));

  return rectReadinessStart;
}
//--------------------------------------------------------------------------------------------
//creating and dispalying the members of the three lists at the bottom
void MReadiness::CreateListWidget(const char* nameList, QListWidget*& pListWidget, QWidget* pParent, QRect rectList,
                                          QList<QString>* pListNames)
{
  QRect rect;  rect.setRect(rectList.left() + 18, rectList.top() - 20, 80, 16);
  QLabel* pNameList = new QLabel(pParent);
  pNameList->setGeometry(rect);
  pNameList->setText(QApplication::translate("MainWindow", nameList, 0));

  pListWidget = new QListWidget(pParent);
  pListWidget->setGeometry(rectList);
  pListWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  pListWidget->setResizeMode(QListView::Adjust);
  pListWidget->setFrameShape(QFrame::WinPanel);
  pListWidget->setFrameShadow(QFrame::Sunken);
  pListWidget->setSelectionMode(QAbstractItemView::NoSelection);

  QListWidgetItem* pItem;
  int count = pListNames->count();
  for(int i = 0; i < count; i++)
  {
    pItem = new QListWidgetItem(QIcon(QString::fromUtf8(":/StartCircleEmpty.png")), (*pListNames)[i], pListWidget);
  }
}
//--------------------------------------------------------------------------------------------
void MReadiness::SlotStart()
{
  PPushButtonStart->setEnabled(false);
  PPushButtonTelemetry->setEnabled(false);

  emit SignalSetTypeModeReadiness();

  BResultTests = true;
  ITestChannel = -1;

  for(int i = 0; i < ListReadinessTestChannels.count(); i++)
  {
    ListTestRectItems[i]->setBrush(QBrush());
    ListReadinessTestChannels[i]->InitState();
    GetListWidgetItem(ListReadinessTestChannels[i]->GetNameChannel())->setIcon(QIcon(QString::fromUtf8(":/StartCircleEmpty.png")));
  }

  StartNextTestChannel();
}
//--------------------------------------------------------------------------------------------
void MReadiness::StartNextTestChannel()
{
  ITestChannel += 1;

  if(ITestChannel < ListReadinessTestChannels.count())
  {
    ListReadinessTestChannels[ITestChannel]->Test();

    for(int i = 0; i < NRPoint; i++)
    {
      MAlphas[i] = Alpha1+i*DAlpha;
      MkAlphas[i] = -DIncrementAlpha;
    }
    MkAlphas[0] = DIncrementAlpha;

    TimerIDTestRectItems = startTimer(30);

    PViewEkranoplan->ensureVisible(ListTestRectItems[ITestChannel], 10,10); // int xmargin = 50, int ymargin = 50)
  }
  else
  {
    PPushButtonStart->setEnabled(true);
    PPushButtonTelemetry->setEnabled(true);
  }
}
//--------------------------------------------------------------------------------------------
void MReadiness::timerEvent(QTimerEvent* event)
{
  QSplitter::timerEvent(event);
//--
  QRadialGradient radialGradTestSpot(QPointF(0.5, 0.5), 0.71);
  radialGradTestSpot.setCoordinateMode(QGradient::ObjectBoundingMode);

  for(int i = 0; i < NRPoint; i++)
  {

    MAlphas[i] = MAlphas[i] + MkAlphas[i];
    if(MAlphas[i] < Alpha1 || Alpha2 < MAlphas[i])
    {
      MkAlphas[i] = -MkAlphas[i];   MAlphas[i] = MAlphas[i] + 2*MkAlphas[i];
    }
    radialGradTestSpot.setColorAt(MRPoints[i], QColor(255, 225, 0, MAlphas[i])); // 255
  }

  ListTestRectItems[ITestChannel]->setBrush(QBrush(radialGradTestSpot));
}
//--------------------------------------------------------------------------------------------
//displaying the leftmost mark for passed or not passed
void MReadiness::SlotResultTestChannel(bool bResult)
{
  BResultTests &= bResult;

  QListWidgetItem* pItem = GetListWidgetItem(ListReadinessTestChannels[ITestChannel]->GetNameChannel());
  QColor colorTestRect;
  if(bResult)
  {
    pItem->setIcon(QIcon(QString::fromUtf8(":/StartPassed.png")));
    colorTestRect = QColor(0, 255, 0, 155);
  }
  else
  {
    pItem->setIcon(QIcon(QString::fromUtf8(":/StartNotPassed.png")));
    colorTestRect = QColor(255, 0, 0, 155);
  }

  killTimer(TimerIDTestRectItems);
  ListTestRectItems[ITestChannel]->setBrush(QBrush(colorTestRect));

//QMessageBox msgBox(QMessageBox::Critical, SMessageBoxTitle, "=====  TROUBLE!  =====");   msgBox.exec();
  StartNextTestChannel();
}
//--------------------------------------------------------------------------------------------
QListWidgetItem* MReadiness::GetListWidgetItem(QString nameItem)
{
  QListWidgetItem* pItem;
  pItem = GetListWidgetItem(PListWidgetEngines, nameItem);
  if(pItem != NULL)  return pItem;
  pItem = GetListWidgetItem(PListWidgetSensors, nameItem);
  if(pItem != NULL)  return pItem;
  pItem = GetListWidgetItem(PListWidgetServoMotors, nameItem);
  if(pItem == NULL) {
    QMessageBox msgBox(QMessageBox::Warning, SMessageBoxTitle, "GetListWidgetItem == NULL");   msgBox.exec();
  }
  return pItem;
}
//--------------------------------------------------------------------------------------------
QListWidgetItem* MReadiness::GetListWidgetItem(QListWidget* pListWidget, QString nameItem)
{
  QListWidgetItem* pItem;
  int count = pListWidget->count();

  for(int i = 0; i < count; i++)
  {
    pItem = pListWidget->item(i);
    if(pItem->text() == nameItem) return pItem;
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------
