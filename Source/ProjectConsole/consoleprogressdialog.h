#ifndef CONSOLEPROGRESSDIALOG_H
#define CONSOLEPROGRESSDIALOG_H
#include "QDialog"
#include "QTextEdit"
#include "QWidget"
#include "QLayout"
#include "QLabel"
#include "QProgressBar"
#include "QToolBox"
#include "QPushButton"

class Turquoise2DEditor;
class ConsoleProgressDialog : public QDialog
{
public:
    Turquoise2DEditor *mainWindow;
    QTextEdit *textField;
    explicit ConsoleProgressDialog(QString title,Turquoise2DEditor *_mainWindow,QWidget *parent=0);
    QLabel *progressText;
    QPushButton *showDetailsBtn;
    void setProgressText(QString text);
};

#endif // CONSOLEPROGRESSDIALOG_H
