#include "customtitlebar.h"
#include "ui_customtitlebar.h"

CustomTitleBar::CustomTitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomTitleBar)
{
    ui->setupUi(this);
    ui->title->setText(parent->windowTitle());
    this->setStyleSheet(" background-color:#252525;");
    this->layout()->setContentsMargins(0,0,0,15);
}

CustomTitleBar::~CustomTitleBar()
{
    delete ui;
}
