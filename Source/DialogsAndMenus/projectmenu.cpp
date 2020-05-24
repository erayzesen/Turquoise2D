#include "projectmenu.h"

ProjectMenu::ProjectMenu(Turquoise2DEditor *_mainWindow,QWidget *parent) : QMenu(parent)
{
    mainWindow=_mainWindow;
    this->setTitle("Run Project");

    runProject=new QAction("New Project",this);
    this->addAction(runProject);
}

