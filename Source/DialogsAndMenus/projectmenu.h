#ifndef PROJECTMENU_H
#define PROJECTMENU_H
#include "QMenu"
#include "QLayout"
#include "QWidget"
#include "QDebug"
#include "ClassGenerator/classoperations.h"
#include "QListWidget"
#include "QStackedWidget"
#include "newprojectdialog.h"

class Turquoise2DEditor;
class ProjectMenu: public QMenu
{
public:
    ProjectMenu(Turquoise2DEditor *_mainWindow,QWidget *parent=0);
    Turquoise2DEditor *mainWindow;

    //Actions
    QAction *runProject;
};

#endif // PROJECTMENU_H
