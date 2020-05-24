#ifndef TURQUOISE2DEDITOR_H
#define TURQUOISE2DEDITOR_H


#include <QMainWindow>
#include <QListView>
#include <Collections/collectionbrowser.h>
#include <QTimer>
#include <QCoreApplication>
#include <customtitlebar.h>
#include <Canvas/scenepanel.h>
#include <Properties/propertiespanel.h>
#include "commands.h"
#include "QShortcut"
#include "QLineEdit"
#include "Hierarchy/hierarchypanel.h"
#include "fileoperations.h"
#include "ProjectConsole/projectconsole.h"
#include "HomeLauncher/homewidget.h"
#include "appsettings.h"
#include "QApplication"

#include "DialogsAndMenus/filemenu.h"
#include "DialogsAndMenus/projectmenu.h"
#include "DialogsAndMenus/windowmenu.h"

struct Cocos2dxSettings{
    QString projectLanguage="c++";
    QString macProjectPath="/proj.ios_mac";
    QString androidProjectPath="/proj.android";
    QString windows32ProjectPath="/proj.win32";
    QString resourcesPath="/Resources/res";
    QString classesPath="/Classes";
    QString frameworkIncludeClass="cocos2d.h";
    QString objectBaseTypeName="Node";
    QString objectLinkedTypeName="cocos2d::Node";
    QString macProjectFileToClasses="../Classes";
    QString androidProjectFileToClasses="/../../Classes";
    QString win32ProjectFileToClasses="../Classes";
};

namespace Ui {
class Turquoise2DEditor;
}
class GraphicsScene;
class PropertiesPanel;
class ScenePanel;
class HierarchyPanel;
class FileMenu;
class WindowMenu;
class ProjectMenu;
enum class SceneEditModes;
class HomeWidget;
class Turquoise2DEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Turquoise2DEditor(QWidget *parent = 0);
    ~Turquoise2DEditor();
    void OpenProject(QString projectFolder);
    void CloseProject();
    ScenePanel *currentScene=NULL;
    HierarchyPanel *currentHierarcyPanel=NULL;
    PropertiesPanel *propertiesPanel;

    //Home Launcher Tab
    HomeWidget *homeTab;
    void closeHomeTab();
    void openHomeTab();


    //Project Framework Properties
    QString projectFolderPath;
    QString projectName;
    QString frameworkName;
    QString projectLanguage;
    QString macProjectPath;
    QString androidProjectPath;
    QString windows32ProjectPath;
    QString resourcesPath;
    QString classesPath;
    QString objectBaseTypeName;
    QString objectLinkedTypeName;
    QString frameworkIncludeClass;
    QString macProjectFileToClasses;
    QString androidProjectFileToClasses;
    QString win32ProjectFileToClasses;
    QString frameworkLibraryPath="";
    void initProjectProperties(QString _projectFolderPath);
    void checkAndInitProjectFiles();
    void clearCollectionPanel();
    void initCollectionPanel();

    //Collection Browser operations
    CollectionBrowser *objectsBrowser=NULL;
    CollectionBrowser *spritesBrowser=NULL;
    CollectionBrowser *soundsBrowser=NULL;
    CollectionBrowser *scenesBrowser=NULL;
    CollectionBrowser *classesBrowser=NULL;

    //Docks and Tabs
    QDockWidget *propDock; //properties
    QDockWidget *hierDock; // hierarcy
    QDockWidget *collDock; // collection
    QTabWidget *tabWidget; //tabWidge


    //Scene operations
    QList<ScenePanel*> openedScenes;
    void addNewSceneTab(QString sceneFilePath);
    void showHierarchyList(HierarchyPanel *hPanel);
    void clearHierarchyPanel();
    ScenePanel* isSceneAlreadyOpened(QString sceneFilePath);
    void renameScaneTab(ScenePanel *scenePanel,QString newName="");
    bool checkUnsavedScenes();
    bool closeAllScenes();

    //Main Menu
    void initMenus();
    FileMenu *fileMenu;
    WindowMenu *windowMenu;
    ProjectMenu *projectMenu;
    QMenu *editMenu;
    void reCreateEditMenu();

    //Scene Toolbar
    QActionGroup *sceneToolsGroup;
    SceneEditModes lastSelectedEditMode;
    void initToolsBar();
    void selectToolWithEditMode(SceneEditModes targetEditMode);
    QAction *selectTool;
    QAction *panTool;
    QAction *zoomTool;
    QAction *drawTool;

    //Project Console
    QWidget *consoleWindow;
    QTextEdit *consoleText;


    //Copy-Paste operations
    QList<QJsonObject> copiedItems;
    QAction *copyAction;
    QAction *pasteAction;
    void copyItems();
    void pasteItems();
    void updateCopyPasteActions();

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent *e);

private slots:


    void on_pc_tabs_tabBarClicked(int index);
    void on_press_Backspace();
    void on_press_Enter();

private:
    Ui::Turquoise2DEditor *ui;
};

#endif // TURQUOISE2DEDITOR_H
