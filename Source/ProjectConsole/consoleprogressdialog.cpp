#include "consoleprogressdialog.h"

ConsoleProgressDialog::ConsoleProgressDialog(QString title,Turquoise2DEditor *_mainWindow,QWidget *parent) : QDialog(parent)
{
    mainWindow=_mainWindow;
    this->setWindowTitle(title);
    QVBoxLayout *mainLayout=new QVBoxLayout;

    progressText=new QLabel("Operation is in progress, please wait..");
    mainLayout->addWidget(progressText);

    QProgressBar *pBar=new QProgressBar;
    pBar->setMaximum(0);
    pBar->setMinimum(0);
    mainLayout->addWidget(pBar);

    showDetailsBtn=new QPushButton("Show Details");
    mainLayout->addWidget(showDetailsBtn);

    connect(showDetailsBtn,&QPushButton::clicked,[=]{
        textField->show();
        showDetailsBtn->hide();
    });

    textField=new QTextEdit;
    textField->setReadOnly(true);
    mainLayout->addWidget(textField);
    textField->hide();



    this->setLayout(mainLayout);
    this->setFixedWidth(400);

    this->setWindowModality(Qt::ApplicationModal);

}

void ConsoleProgressDialog::setProgressText(QString text)
{
    progressText->setText(text);
}

