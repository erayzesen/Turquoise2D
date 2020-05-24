#ifndef WINDOWMENU_H
#define WINDOWMENU_H
#include "QMenu"
#include "QLayout"
#include "QWidget"
#include "QDebug"
#include "ClassGenerator/classoperations.h"
#include "QListWidget"
#include "QStackedWidget"
#include "newprojectdialog.h"

class Turquoise2DEditor;
class WindowMenu: public QMenu
{
public:
    WindowMenu(Turquoise2DEditor *_mainWindow,QWidget *parent=0);
    Turquoise2DEditor *mainWindow;

    //Actions
    QAction *homeTab;
    QAction *hierarchyPanel;
    QAction *propertiesPanel;
    QAction *collectionPanel;
};

#endif // WINDOWMENU_H
