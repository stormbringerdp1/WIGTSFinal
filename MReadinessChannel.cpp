#include "MReadinessChannel.h"

MReadinessChannel::MReadinessChannel(const int yTopChannel, const char* nameChannel, const char* namePixmapChannel,
                                     QString* pTextTest, QList<QString>* pListParamsNameTest, QList<int>* pListParamsRangeTest)
{
    QLabel* label = new QLabel(pScrollAreaTestContents);
    label->setObjectName(QStringLiteral("Engine_1"));
    label->setGeometry(QRect(10, 20, 101, 71));
    label->setPixmap(QPixmap(QString::fromUtf8("Engine.png")));

    QProgressBar* progressBar = new QProgressBar(pScrollAreaTestContents);
    progressBar->setObjectName(QStringLiteral("ProgressBar_1"));
    progressBar->setEnabled(true);
    progressBar->setGeometry(QRect(110, 40, 91, 6));
    progressBar->setMaximum(0);
    progressBar->setValue(-1);
    progressBar->setTextVisible(false);

    QProgressBar* progressBar_2 = new QProgressBar(pScrollAreaTestContents);
    progressBar_2->setObjectName(QStringLiteral("ProgressBar_2"));
    progressBar_2->setEnabled(true);
    progressBar_2->setGeometry(QRect(110, 70, 91, 6));
    progressBar_2->setMaximum(0);
    progressBar_2->setValue(-1);
    progressBar_2->setTextVisible(false);

    QTextEdit* textEdit = new QTextEdit(pScrollAreaTestContents);
    textEdit->setObjectName(QStringLiteral("textEdit"));
    textEdit->setGeometry(QRect(200, 10, 121, 91));
    textEdit->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(85, 255, 255);"));

    QProgressBar* progressBar_3 = new QProgressBar(pScrollAreaTestContents);
    progressBar_3->setObjectName(QStringLiteral("ProgressBar_3"));
    progressBar_3->setEnabled(true);
    progressBar_3->setGeometry(QRect(320, 60, 91, 6));
    progressBar_3->setMaximum(0);
    progressBar_3->setValue(-1);
    progressBar_3->setTextVisible(false);
    progressBar_3->setInvertedAppearance(true);

    QTextEdit* textEdit_2 = new QTextEdit(pScrollAreaTestContents);
    textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
    textEdit_2->setGeometry(QRect(410, 40, 121, 41));

    QLabel* label_2 = new QLabel(pScrollAreaTestContents);
    label_2->setObjectName(QStringLiteral("label_2"));
    label_2->setGeometry(QRect(550, 40, 41, 41));
    label_2->setPixmap(QPixmap(QString::fromUtf8("Engine.png")));
*/

}
