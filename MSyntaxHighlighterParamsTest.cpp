#include "MSyntaxHighlighterParamsTest.h"

  #include <QMessageBox>
//============================================================================================

MSyntaxHighlighterParamsTest::MSyntaxHighlighterParamsTest(QTextDocument* parent, QList<QString>* pListParamsChannel,
                                                           int iFirstItemListParamsChannel, int deltaNItemListParamsChannel) // QList<QString> *pListParamsNameTest, QList<int> *pListParamsRangeTest)
                            : QSyntaxHighlighter(parent), PListParamsChannel(pListParamsChannel),
                              IFirstItemListParamsChannel(iFirstItemListParamsChannel), DeltaNItemListParamsChannel(deltaNItemListParamsChannel)
{
  InitBegin();

}
//--------------------------------------------------------------------------------------------
MSyntaxHighlighterParamsTest::~MSyntaxHighlighterParamsTest()
{
}
//--------------------------------------------------------------------------------------------
void MSyntaxHighlighterParamsTest::InitBegin()
{
  IListItem = IFirstItemListParamsChannel;
}
//--------------------------------------------------------------------------------------------
void MSyntaxHighlighterParamsTest::highlightBlock(const QString& text)
{
  int pos = text.indexOf((*PListParamsChannel)[IListItem], 0, Qt::CaseSensitive);
  if(pos >= 0)
  {
    pos += (*PListParamsChannel)[IListItem].length();
    QString s = text.mid(pos);
    if(!s.isEmpty())
    {
      float param = s.toFloat();
      QColor color;
      if((*PListParamsChannel)[IListItem+1].toFloat() <= param && param <= (*PListParamsChannel)[IListItem+2].toFloat())
        color = QColor(0,255,0);
      else
        color = QColor(255,0,0);
      setFormat(pos, s.length(), color);

      IListItem += DeltaNItemListParamsChannel;
    }
  }
 }
 //--------------------------------------------------------------------------------------------
