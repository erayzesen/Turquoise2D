#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QLayout>
#include "QPushButton"
#include "QListWidget"
#include "QLabel"
#include "turquoise2deditor.h"
#include "QListWidgetItem"
#include "HomeLauncher/projectlistitem.h"
#include "QVariantMap"
#include "appsettings.h"
#include "projectfilesoperations.h"

class Turquoise2DEditor;
class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(Turquoise2DEditor *_mainWindow,QWidget *parent = 0);
    Turquoise2DEditor *mainWindow;
    void updateRecentList();
    QListWidget *recentProjectList;

signals:

public slots:
};

#endif // HOMEWIDGET_H
