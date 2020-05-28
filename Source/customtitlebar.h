#pragma once

#include <QWidget>
#include <QFile>
#include <QTextStream>

namespace Ui {
class CustomTitleBar;
}

class CustomTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = 0);
    ~CustomTitleBar();

private:
    Ui::CustomTitleBar *ui;
};


