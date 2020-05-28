#pragma once

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <turquoise2deditor.h>

class Turquoise2DEditor;
class ProjectListItem : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectListItem(Turquoise2DEditor *_mainWindow,QString frameworkName,QString projectName,QString projectPath,QWidget *parent = 0);
    Turquoise2DEditor *mainWindow;

signals:

public slots:
};

