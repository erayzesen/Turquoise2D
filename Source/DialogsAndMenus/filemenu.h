#ifndef FILEMENU_H
#define FILEMENU_H
#include "QMenu"
#include "QLayout"
#include "QWidget"
#include "QDebug"
#include "ClassGenerator/classoperations.h"
#include "QListWidget"
#include "QStackedWidget"
#include "newprojectdialog.h"

class Turquoise2DEditor;
class FileMenu : public QMenu
{
public:
    FileMenu(Turquoise2DEditor *_mainWindow,QWidget *parent=0);
    Turquoise2DEditor *mainWindow;
    //Actions
    QAction *openProject;
    QAction *newProject;
    QAction *newScene;
    QAction *openScene;
    QAction *saveScene;
    //Helpers
    QJsonDocument generateNewSceneJson();
    QHBoxLayout* createProjectInfo(QString projectLocation,QString frameworkName);
    void clearLayout(QLayout * ly);
public slots:
    //Functions
    void OpenProject();
    void NewProject();
    void NewScene();
    void OpenScene();
    void SaveScene();
    void SaveAsScene();
};

#endif // FILEMENU_H
