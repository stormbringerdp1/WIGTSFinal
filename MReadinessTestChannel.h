#ifndef MREADINESSTESTCHANNEL_H
#define MREADINESSTESTCHANNEL_H

#include "MSyntaxHighlighterParamsTest.h"

#include <QWidget>
#include <QString>
#include <QList>
#include <QLineEdit>
#include <QProgressBar>
#include <QTextEdit>
#include <QLabel>

//============================================================================================

class MReadinessTestChannel : public QWidget
{
    Q_OBJECT
public:
 MReadinessTestChannel(QWidget* pParent, const int yTopChannel, // const char* namePixmapFileChannel,
                       QList<QString>* pListParamsChannel);
  virtual ~MReadinessTestChannel();

  QString GetNameChannel() { return (*PListParamsChannel)[0]; } //  PLineEditNameChannel->text(); }
  QRect GetRectChannel() { return RectChannel; }

  void Test();
  void InitState();

private:

  QRect RectChannel;

  bool BResult;
  int  NTestInChannel;
  int  IListItem;
  int  PosParam;

  QList<QString>* PListParamsChannel;

  MSyntaxHighlighterParamsTest* PSyntaxHighlighterParamsTest;

  QString SColorNeutral;  QString SColorQuery;
  QString SColorReady;    QString SColorUnready;

  QProgressBar* PProgressBarTest1;
  QProgressBar* PProgressBarTest2;
  QProgressBar* PProgressBarStatus;

  QTextEdit* PTextEditTest;

  QLabel* PLabelStatus;
  QLabel* PLabelStatusPixmap;

  void StartTest(int nTestInChannel);
  void TestPassed(int nTestInChannel);
  void TestNotPassed(int nTestInChannel);

signals:
  void SignalGetReadinessTestValue(QString* psNameChannel, QString* psInputTestName, QString* psInputTestValue, QString* psOutputTestName);
  void SignalResultTestChannel(bool bResult);

public slots:
  void SlotTakeReadinessTestValue(float result);
};
//============================================================================================

#endif // MREADINESSTESTCHANNEL_H
