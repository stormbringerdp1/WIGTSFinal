#include "MReadinessTestChannel.h"
#include "funcs.h"

#include <QMessageBox>
//-------------------------------------------------------------------------------------------------------
MReadinessTestChannel::MReadinessTestChannel(QWidget* pParent, const int yTopChannel, // const char* namePixmapFileChannel,
                                             QList<QString>* pListParamsChannel)
                      : QWidget(pParent), PListParamsChannel(pListParamsChannel)
{

  SColorNeutral = QString("rgb(255,200,0)");
  SColorQuery = QString("rgb(255,225,0)");

  SColorReady = QString("rgb(0,255,0)");
  SColorUnready = QString("rgb(255,0,0)");

  int xLeft = 10;

  int widthLabelPixmapChannel = 101;   int heightLabelPixmapChannel = 71;
  int widthProgressBarTest = 91;       int heightProgressBarTest = 6;
  int widthTextEditTest = 121;         int heightTextEditTest = 75;
  int widthProgressBarStatus = 91;     int heightProgressBarStatus = 6;
  int widthLabelStatus = 121;          int heightLabelStatus = 41;

  QRect rect;

//== 2): progression bar under the first picture

  PProgressBarTest1 = new QProgressBar(pParent);
  PProgressBarTest1->setEnabled(true);
  rect = QRect(xLeft, yTopChannel + 49, widthLabelPixmapChannel + widthProgressBarTest, heightProgressBarTest);
  PProgressBarTest1->setGeometry(rect);
  PProgressBarTest1->setMinimum(0);
  PProgressBarTest1->setTextVisible(false);
  RectChannel = rect;

  PProgressBarTest2 = new QProgressBar(pParent);
  PProgressBarTest2->setEnabled(true);
  rect = QRect(xLeft, yTopChannel + 74, widthLabelPixmapChannel + widthProgressBarTest, heightProgressBarTest);
  PProgressBarTest2->setGeometry(rect);
  PProgressBarTest2->setMinimum(0);
  PProgressBarTest2->setTextVisible(false);
  RectChannel |= rect;

//== 1): leftmost picture

  QLineEdit* pLineEditNameChannel = new QLineEdit(pParent);
  rect = QRect(xLeft, yTopChannel + 4, widthLabelPixmapChannel, 20);
  pLineEditNameChannel->setGeometry(rect);
  pLineEditNameChannel->setFrame(false);
  pLineEditNameChannel->setReadOnly(true);
  pLineEditNameChannel->setStyleSheet(QLatin1String("background: transparent;"));
  pLineEditNameChannel->setText((*PListParamsChannel)[0]); //nameChannel);
  RectChannel |= rect;

  QLabel* pLabelPixmapChannel = new QLabel(pParent);
  rect = QRect(xLeft, yTopChannel + 24, widthLabelPixmapChannel, heightLabelPixmapChannel);
  pLabelPixmapChannel->setGeometry(rect);
  pLabelPixmapChannel->setScaledContents(true);
  pLabelPixmapChannel->setPixmap(QPixmap((*PListParamsChannel)[1]));
  RectChannel |= rect;


//== 4): progress bar second to the left

  xLeft += widthLabelPixmapChannel + widthProgressBarTest;

  PProgressBarStatus = new QProgressBar(pParent);
  PProgressBarStatus->setEnabled(true);
  rect = QRect(xLeft, yTopChannel + 62, widthTextEditTest+widthProgressBarStatus, heightProgressBarStatus);
  PProgressBarStatus->setGeometry(rect);
  PProgressBarStatus->setMinimum(0);
  PProgressBarStatus->setTextVisible(false);
  RectChannel |= rect;

//== 3): test console output window

  QLineEdit* pLineEditTest = new QLineEdit(pParent);
  rect = QRect(xLeft, yTopChannel, widthTextEditTest, 20);
  pLineEditTest->setGeometry(rect);
  pLineEditTest->setFrame(false);
  pLineEditTest->setReadOnly(true);
  pLineEditTest->setStyleSheet(QLatin1String("background: transparent;"));
  pLineEditTest->setAlignment(Qt::AlignCenter);
  pLineEditTest->setText(QStringLiteral("Test"));
  RectChannel |= rect;

  PTextEditTest = new QTextEdit(pParent);
  rect = QRect(xLeft, yTopChannel + 20, widthTextEditTest, heightTextEditTest);
  PTextEditTest->setGeometry(rect);
  PTextEditTest->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\ncolor: rgb(85, 255, 255);"));
  PTextEditTest->setTabStopWidth(8); // By default, this property contains a value of 80 pixels.
  PTextEditTest->setReadOnly(true);
  RectChannel |= rect;

  PSyntaxHighlighterParamsTest = new MSyntaxHighlighterParamsTest(PTextEditTest->document(), pListParamsChannel,
                                            GetPMainWnd()->GetIFirstItemListParamsReadiness(), GetPMainWnd()->GetDeltaNItemListParamsReadiness());

//== 5): status of check

  xLeft += widthTextEditTest + widthProgressBarStatus;

  QLineEdit* pLineEditStatus = new QLineEdit(pParent);
  rect = QRect(xLeft, yTopChannel + 24, widthLabelStatus, 20);
  pLineEditStatus->setGeometry(rect);
  pLineEditStatus->setFrame(false);
  pLineEditStatus->setReadOnly(true);
  pLineEditStatus->setStyleSheet(QLatin1String("background: transparent;"));
  pLineEditStatus->setAlignment(Qt::AlignCenter);
  pLineEditStatus->setText(QStringLiteral("Status"));
  RectChannel |= rect;

  PLabelStatus = new QLabel(pParent);
  PLabelStatus->setObjectName(QStringLiteral("LabelStatus"));
  rect = QRect(xLeft, yTopChannel + 44, widthLabelStatus, heightLabelStatus);
  PLabelStatus->setGeometry(rect);
  PLabelStatus->setAlignment(Qt::AlignCenter);
  PLabelStatus->setFrameShape(QFrame::StyledPanel);
  PLabelStatus->setFrameShadow(QFrame::Raised);
  RectChannel |= rect;

//== 6): icon for status

  xLeft += widthLabelStatus + 10;
  PLabelStatusPixmap = new QLabel(pParent);
  rect = QRect(xLeft, yTopChannel + 52, 25, 25);
  PLabelStatusPixmap->setGeometry(rect);
  RectChannel |= rect;

  InitState();
}
//--------------------------------------------------------------------------------------------
MReadinessTestChannel::~MReadinessTestChannel()
{
  PSyntaxHighlighterParamsTest->setDocument(0);  delete PSyntaxHighlighterParamsTest;
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::InitState()
{
  PProgressBarTest1->setMaximum(100);  PProgressBarTest1->setValue(100);
  PProgressBarTest1->setStyleSheet("QProgressBar::chunk { background-color: "+SColorNeutral+"; }");

  PProgressBarTest2->setMaximum(100);  PProgressBarTest2->setValue(100);
  PProgressBarTest2->setStyleSheet("QProgressBar::chunk { background-color: "+SColorNeutral+"; }");


  PSyntaxHighlighterParamsTest->InitBegin();
  PTextEditTest->setText((*PListParamsChannel)[2]);


  PProgressBarStatus->setMaximum(100);  PProgressBarStatus->setValue(100);
  PProgressBarStatus->setStyleSheet("QProgressBar::chunk { background-color: "+SColorNeutral+"; }");

  PLabelStatus->setStyleSheet(QLatin1String("background-color: transparent;"));
  PLabelStatus->setText("");

  PLabelStatusPixmap->setPixmap(QPixmap());//(QString::fromUtf8("Transparent.ico")));
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::Test()
{

  connect(this,
          SIGNAL(SignalGetReadinessTestValue(QString*,QString*,QString*,QString*)),
          GetPMainWnd()->GetPArduinoConnector(),
          SLOT(SlotGetReadinessTestValue(QString*,QString*,QString*,QString*)));


  connect(GetPMainWnd()->GetPArduinoConnector(),
          SIGNAL(SignalTakeReadinessTestValue(float)),
          this,
          SLOT(SlotTakeReadinessTestValue(float)));

  BResult = true;
  NTestInChannel = 1;
  IListItem = GetPMainWnd()->GetIFirstItemListParamsReadiness();
  PosParam = 0;

  StartTest(NTestInChannel);
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::StartTest(int nTestInChannel)
{
  if(nTestInChannel == 1)
  {
    PProgressBarTest1->setStyleSheet("QProgressBar::chunk { background-color: qlineargradient(spread:pad, x1: 0.0, y1: 0.5, x2: 1.0, y2: 0.5,"
    " stop: 0 rgb(127,127,127), stop: 0.1 "+SColorQuery+", stop: 0.9 "+SColorQuery+", stop: 1 rgb(127,127,127));"
    " width: 70.0px; margin: 1.5px }");
    PProgressBarTest1->setMaximum(0);
  }
  else if(nTestInChannel == 2)
  {
    PProgressBarTest2->setStyleSheet("QProgressBar::chunk { background-color: qlineargradient(spread:pad, x1: 0.0, y1: 0.5, x2: 1.0, y2: 0.5,"
    " stop: 0 rgb(127,127,127), stop: 0.1 "+SColorQuery+", stop: 0.9 "+SColorQuery+", stop: 1 rgb(127,127,127));"
    " width: 70.0px; margin: 1.5px }");
    PProgressBarTest2->setMaximum(0);
  }
  emit SignalGetReadinessTestValue(&(*PListParamsChannel)[0], &(*PListParamsChannel)[IListItem-3], &(*PListParamsChannel)[IListItem-2], &(*PListParamsChannel)[IListItem-1]);
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::SlotTakeReadinessTestValue(float result)
{
  if(IListItem < PListParamsChannel->count())
  {
//msgBox.setText("2");   msgBox.exec();
    QString s;   QString sTextTest;  QString sParam;

      s.sprintf("%.1f",result);
      sTextTest = PTextEditTest->document()->toPlainText(); //> >plainText();//  (*PListParamsChannel)[2];
      sParam = (*PListParamsChannel)[IListItem];
      PosParam = sTextTest.indexOf(sParam, PosParam, Qt::CaseSensitive) + sParam.length();
      sTextTest.insert(PosParam,s);
      PSyntaxHighlighterParamsTest->InitBegin();
      PTextEditTest->setText(sTextTest);

      if((*PListParamsChannel)[IListItem+1].toFloat() <= result && result <= (*PListParamsChannel)[IListItem+2].toFloat())
      {
        TestPassed(NTestInChannel);
        BResult &= true;
      }
      else
      {
        TestNotPassed(NTestInChannel);
        BResult &= false;
      }
      NTestInChannel += 1;
      IListItem += GetPMainWnd()->GetDeltaNItemListParamsReadiness();

      if(IListItem < PListParamsChannel->count())
      {
        StartTest(NTestInChannel);
//QMessageBox msgBox; msgBox.setText(s);   msgBox.exec();
        return;
      }
  }

  if(!disconnect(this,
                 SIGNAL(SignalGetReadinessTestValue(QString*,QString*,QString*,QString*)),
                 GetPMainWnd()->GetPArduinoConnector(),
                 SLOT(SlotGetReadinessTestValue(QString*,QString*,QString*,QString*))))
  {
    QMessageBox msgBox(QMessageBox::Warning, "void MReadinessTestChannel::SlotResultTest(float result)",
                       "Problem::\n\ndisconnect(this, SIGNAL(SignalGetReadinessTestValue(QString*,QString*,QString*,QString*)), GetPMainWnd()->GetPArduino(), SLOT(SlotGetReadinessTestValue(QString*,QString*,QString*,QString*))) == false");
      msgBox.exec();
  }

  if(!disconnect(GetPMainWnd()->GetPArduinoConnector(),
                 SIGNAL(SignalTakeReadinessTestValue(float)),
                 this,
                 SLOT(SlotTakeReadinessTestValue(float))))
  {
    QMessageBox msgBox(QMessageBox::Warning, "void MReadinessTestChannel::SlotResultTest(float result)",
                       "Problem::\n\ndisconnect(GetPMainWnd()->GetPArduino(), SIGNAL(SignalTakeReadinessTestValue(float)), this, SLOT(SlotTakeReadinessTestValue(float))) == false");
      msgBox.exec();
  }

  emit SignalResultTestChannel(BResult);
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::TestPassed(int nTestInChannel)
{
  if(nTestInChannel == 1)
  {
    PProgressBarTest1->setStyleSheet("QProgressBar::chunk { background-color: rgb(0,255,0);}");
    PProgressBarTest1->setMaximum(100);  PProgressBarTest1->setValue(100);
  }
  if(nTestInChannel == 2)
  {
    PProgressBarTest2->setStyleSheet("QProgressBar::chunk { background-color: rgb(0,255,0);}");
    PProgressBarTest2->setMaximum(100);  PProgressBarTest2->setValue(100);

    if(BResult)
    {
      PProgressBarStatus->setStyleSheet("QProgressBar::chunk { background-color: rgb(0,255,0);}");

      PLabelStatus->setStyleSheet(QLatin1String("background-color: rgb(0,255,0);\ncolor: rgb(0,0,0);"));
      PLabelStatus->setText("Ready");

      PLabelStatusPixmap->setPixmap(QPixmap(QString::fromUtf8(":/TestPassed.ico")));
    }
    else
    {
      PProgressBarStatus->setStyleSheet("QProgressBar::chunk { background-color: rgb(255,0,0);}");

      PLabelStatus->setStyleSheet(QLatin1String("background-color: rgb(255,0,0);\ncolor: rgb(0,0,0);"));
      PLabelStatus->setText("Unready");

      PLabelStatusPixmap->setPixmap(QPixmap(QString::fromUtf8(":/TestNotPassed.ico")));
    }
  }
}
//--------------------------------------------------------------------------------------------
void MReadinessTestChannel::TestNotPassed(int nTestInChannel)
{
  if(nTestInChannel == 1)
  {
    PProgressBarTest1->setStyleSheet("QProgressBar::chunk { background-color: rgb(255,0,0);}");
    PProgressBarTest1->setMaximum(100);  PProgressBarTest1->setValue(100);
  }
  if(nTestInChannel == 2)
  {
    PProgressBarTest2->setStyleSheet("QProgressBar::chunk { background-color: rgb(255,0,0);}");
    PProgressBarTest2->setMaximum(100);  PProgressBarTest2->setValue(100);

    PProgressBarStatus->setStyleSheet("QProgressBar::chunk { background-color: rgb(255,0,0);}");

    PLabelStatus->setStyleSheet(QLatin1String("background-color: rgb(255,0,0);\ncolor: rgb(0,0,0);"));
    PLabelStatus->setText("Unready");
    PLabelStatusPixmap->setPixmap(QPixmap(QString::fromUtf8(":/TestNotPassed.ico")));
  }
}
//--------------------------------------------------------------------------------------------
