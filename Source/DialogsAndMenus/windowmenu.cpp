#include "windowmenu.h"

WindowMenu::WindowMenu(Turquoise2DEditor *_mainWindow,QWidget *parent) : QMenu(parent)
{
    this->setTitle("Window");
    mainWindow=_mainWindow;

    homeTab=new QAction("Home",this);
    homeTab->setCheckable(true);
    homeTab->setChecked(true);
    this->addAction(homeTab);
    hierarchyPanel=new QAction("Hierarchy",this);
    hierarchyPanel->setCheckable(true);
    hierarchyPanel->setChecked(true);
    this->addAction(hierarchyPanel);
    propertiesPanel=new QAction("Properties",this);
    propertiesPanel->setCheckable(true);
    propertiesPanel->setChecked(true);
    this->addAction(propertiesPanel);
    collectionPanel=new QAction("Collection",this);
    collectionPanel->setCheckable(true);
    collectionPanel->setChecked(true);
    this->addAction(collectionPanel);

    connect(homeTab,&QAction::triggered,[=](bool checked){
        if(checked){
            mainWindow->openHomeTab();
        }else{
            mainWindow->closeHomeTab();
        }
    });

    connect(hierarchyPanel,&QAction::triggered,[=](bool checked){
        if(checked){
            mainWindow->hierDock->show();
        }else{
            mainWindow->hierDock->hide();
        }
    });
    connect(propertiesPanel,&QAction::triggered,[=](bool checked){
        if(checked){
            mainWindow->propDock->show();
        }else{
            mainWindow->propDock->hide();
        }
    });
    connect(collectionPanel,&QAction::triggered,[=](bool checked){
        if(checked){
            mainWindow->collDock->show();
        }else{
            mainWindow->collDock->hide();
        }
    });
}

