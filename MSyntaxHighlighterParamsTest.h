#ifndef MSYNTAXHIGHLIGHTERPARAMSTEST_H
#define MSYNTAXHIGHLIGHTERPARAMSTEST_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QString>
#include <QList>

//============================================================================================

class MSyntaxHighlighterParamsTest : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit MSyntaxHighlighterParamsTest(QTextDocument* parent, QList<QString>* pListParamsChannel,
                                          int iFirstItemListParamsChannel, int deltaNItemListParamsChannel);
  virtual ~MSyntaxHighlighterParamsTest();

  void InitBegin();

private:
  int IFirstItemListParamsChannel;  int DeltaNItemListParamsChannel;

  int IListItem;
  QList<QString>* PListParamsChannel;

protected:
  virtual void highlightBlock(const QString& text);

signals:

public slots:

};
//============================================================================================
#endif // MSYNTAXHIGHLIGHTERPARAMSTEST_H
