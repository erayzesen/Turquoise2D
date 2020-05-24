#include "turquoise2deditor.h"
#include "UI_turquoise2deditor.h"

Turquoise2DEditor::Turquoise2DEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Turquoise2DEditor)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Icons/windowLogo.png"));
    //Setting Project Framework Properties
    //initProjectProperties("/Users/erayzesen/Desktop/SampleProject");

    //Set default selected Tool
    lastSelectedEditMode=SceneEditModes::GameItemsEdit;
    //Initialize Top Menu
    initMenus();

    initToolsBar();

    tabWidget=ui->scenesTab;

    //HOME TAB
    homeTab=new HomeWidget(this);
    ui->scenesTab->addTab(homeTab,"Home");

    //CONSOLE WINDOW
    consoleText=new QTextEdit;
    consoleWindow=ProjectConsole::CreateConsoleWindow(consoleText);

    //PROPERTIES PANEL
    ui->properties->setTitleBarWidget(new CustomTitleBar(ui->properties));
    propertiesPanel=ui->propPanel;
    propDock=ui->properties;

    //HIERARCHY DOC
    ui->scene_hierarchy->setTitleBarWidget(new CustomTitleBar(ui->scene_hierarchy));
    hierDock=ui->scene_hierarchy;


    //COLLECTION PANEL
    ui->project_collection->setTitleBarWidget(new CustomTitleBar(ui->project_collection));
    collDock=ui->project_collection;

    //SCENES TAB
    ui->scenesTab->tabBar()->setExpanding(true);
    //If tab changed
    connect(ui->scenesTab,&QTabWidget::currentChanged,[=](int index){
        qDebug()<<"degisti scene";
        qDebug()<<"index num:"<<index<<" scenes count:"<<openedScenes.count();
         if(index!=-1){
             //Checking if the tab type is ScenePanel.
             ScenePanel *selectedScenePanel=dynamic_cast<ScenePanel*>(ui->scenesTab->widget(index));
             HomeWidget *selectedHomeWidget=dynamic_cast<HomeWidget*>(ui->scenesTab->widget(index));
             if(selectedScenePanel){
                 //Setting currentScene
                 currentScene=selectedScenePanel;
                 //Clearing propertiesPanel
                 propertiesPanel->ClearGameProperties();
                 //Adding selected properties item to propertiesPanel
                 currentScene->scene->resetPropItem();
                 //Clearing hierarchyPanel container and Showing selected hierarcyPanel
                 showHierarchyList(currentScene->scene->hierarchyPanel);
                 //Setting scene edit mode according to the selected tool.
                 currentScene->scene->setEditMode(lastSelectedEditMode);
                 currentScene->scene->view->setFocus(Qt::OtherFocusReason);
                 //Updating edit menu actions
                 reCreateEditMenu();
                 //Updating copy paste actions
                 this->updateCopyPasteActions();
             }else if(selectedHomeWidget){
                 //Setting currentScene
                 currentScene=NULL;
                 //Clearing propertiesPanel
                 propertiesPanel->ClearGameProperties();
                 //Clearing hierarchyPanel
                 clearHierarchyPanel();
                 //Updating edit menu actions
                 reCreateEditMenu();
             }

        }else{
             currentScene=NULL;
             clearHierarchyPanel();
             this->updateCopyPasteActions();
         }
    });
    // If tab closed
    connect(ui->scenesTab,&QTabWidget::tabCloseRequested,[=](int index){
        //Trying ScenePanel type for target tab.
        ScenePanel *targetScene=dynamic_cast<ScenePanel*>(ui->scenesTab->widget(index));
        HomeWidget *targetHomeWidget=dynamic_cast<HomeWidget*>(ui->scenesTab->widget(index));

        //Checking if the tab type is ScenePanel.
        if(targetScene){
            bool permission=true;
            if(targetScene->isSceneChanged==true){
                ui->scenesTab->setCurrentIndex(index);
                QString message="Save changes to the scene document \""+targetScene->sceneFileName+"\" before closing?";
                QMessageBox::StandardButton resBtn = QMessageBox::question( this,"Turquoise2D Editor",message,QMessageBox::Cancel | QMessageBox::Discard | QMessageBox::Save);
                if(resBtn==QMessageBox::Cancel){
                    permission=false;
                }else if(resBtn==QMessageBox::Save){
                    fileMenu->SaveScene();
                }
            }

            if(permission==true){
                //Closing tab
                ui->scenesTab->removeTab(index);
                qDebug()<<"The tab closed!";
                openedScenes.removeAt(openedScenes.indexOf(targetScene));
                delete targetScene;
                qDebug()<<"The scene deleted!";
            }

        }else if(targetHomeWidget){
            ui->scenesTab->removeTab(index);
        }

    });
    //


    //CANVAS GRAPHICS VIEW

    //Create menus
    editMenu = this->menuBar()->addMenu(tr("&Edit"));
    reCreateEditMenu();

    //SHORTCUTS
    new QShortcut(QKeySequence(Qt::Key_Backspace ),this,SLOT(on_press_Backspace()) );
    new QShortcut(QKeySequence(Qt::Key_Return ),this,SLOT(on_press_Enter()) );
    //Setting mini shortcuts
    QShortcut *shortCut;
    //Draw Tool
    shortCut=new QShortcut(QKeySequence(Qt::Key_B ),this);
    connect(shortCut,&QShortcut::activated,[=]{
        selectToolWithEditMode(SceneEditModes::DrawEdit);
    });
    //Move/Transform Tool
    shortCut=new QShortcut(QKeySequence(Qt::Key_V),this);
    connect(shortCut,&QShortcut::activated,[=]{
        selectToolWithEditMode(SceneEditModes::GameItemsEdit);
    });
    //Pan Tool
    shortCut=new QShortcut(QKeySequence(Qt::Key_H),this);
    connect(shortCut,&QShortcut::activated,[=]{
        selectToolWithEditMode(SceneEditModes::PanEdit);
    });
    //Zoom Tool
    shortCut=new QShortcut(QKeySequence(Qt::Key_Z),this);
    connect(shortCut,&QShortcut::activated,[=]{
        selectToolWithEditMode(SceneEditModes::ZoomEdit);
    });
    //Set zoom 100%
    shortCut=new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1),this);
    connect(shortCut,&QShortcut::activated,[=]{
        if(currentScene){
            currentScene->scene->resetZoomLevel();
        }
    });

    //G
    //this->setAttribute(Qt::WA_ShowWithoutActivating);
    //this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
}

Turquoise2DEditor::~Turquoise2DEditor()
{
    delete ui;
}

void Turquoise2DEditor::OpenProject(QString projectFolder)
{
    if(closeAllScenes()==false){
        return;
    }
    initProjectProperties(projectFolder);
    checkAndInitProjectFiles();
    initCollectionPanel();
    this->setWindowTitle(projectName+" - Turquoise2D Editor");
    copiedItems.clear();
    this->updateCopyPasteActions();
    AppSettings::addRecentProject(projectFolder,frameworkName);
    homeTab->updateRecentList();
}

void Turquoise2DEditor::CloseProject()
{

}

void Turquoise2DEditor::closeHomeTab()
{
    int index=ui->scenesTab->indexOf(homeTab);
    if(index>-1){
        ui->scenesTab->removeTab(index);
    }
}

void Turquoise2DEditor::openHomeTab()
{
    int index=ui->scenesTab->indexOf(homeTab);
    if(index==-1){
        index=ui->scenesTab->addTab(homeTab,"Home");
    }
    ui->scenesTab->setCurrentIndex(index);
}

void Turquoise2DEditor::reCreateEditMenu()
{
    editMenu->clear();
    //UNDO REDO ACTIONS
    QAction *undoAction,*redoAction;
    if(currentScene){
        undoAction = currentScene->scene->stack->createUndoAction(this, tr("&Undo"));
        undoAction->setShortcuts(QKeySequence::Undo);
        redoAction = currentScene->scene->stack->createRedoAction(this, tr("&Redo"));
        redoAction->setShortcuts(QKeySequence::Redo);
    }else{
        undoAction=new QAction("Undo",editMenu);
        undoAction->setEnabled(false);
        redoAction=new QAction("Redo",editMenu);
        redoAction->setEnabled(false);
    }

    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    editMenu->addSeparator();

    //COPY-PASTE ACTIONS
    copyAction=new QAction("Copy",editMenu);
    copyAction->setShortcuts(QKeySequence::Copy);
    editMenu->addAction(copyAction);
    copyAction->setEnabled(false);
    if(currentScene){
        if(currentScene->scene->lastTransformItem){
            copyAction->setEnabled(true);
        }
    }
    pasteAction=new QAction("Paste",editMenu);
    pasteAction->setShortcuts(QKeySequence::Paste);
    editMenu->addAction(pasteAction);
    if(this->copiedItems.count()==0 && currentScene==NULL){
        pasteAction->setEnabled(false);
    }
    connect(copyAction,&QAction::triggered,[=](bool toggle){
        copyItems();
    });
    connect(pasteAction,&QAction::triggered,[=](bool toggle){
        pasteItems();
    });

}

void Turquoise2DEditor::initProjectProperties(QString _projectFolderPath)
{
    projectFolderPath=_projectFolderPath;
    frameworkName=ProjectFilesOperations::checkDirForFrameworks(projectFolderPath);
    if(frameworkName=="Cocos2d-x"){
        Cocos2dxSettings settings;
        projectLanguage=settings.projectLanguage;
        macProjectPath=settings.macProjectPath;
        androidProjectPath=settings.androidProjectPath;
        windows32ProjectPath=settings.windows32ProjectPath;
        resourcesPath=settings.resourcesPath;
        classesPath=settings.classesPath;
        frameworkIncludeClass=settings.frameworkIncludeClass;
        objectBaseTypeName=settings.objectBaseTypeName;
        objectLinkedTypeName=settings.objectLinkedTypeName;
        macProjectFileToClasses=settings.macProjectFileToClasses;
        androidProjectFileToClasses=settings.androidProjectFileToClasses;
        win32ProjectFileToClasses=settings.win32ProjectFileToClasses;
        projectName=QFileInfo(ProjectFilesOperations::getXcodeFilePath(this)).baseName();
        QStringList frameworkPathAlternatives=ProjectFilesOperations::getFrameworkPaths(this,"Cocos2d-x");
        frameworkLibraryPath=frameworkPathAlternatives.count()>0 ? frameworkPathAlternatives.at(0):"";
    }
}

void Turquoise2DEditor::checkAndInitProjectFiles()
{
    //Checking resource folder needs
    QString spritesPath,scenesPath,classesPath;
    spritesPath=projectFolderPath+resourcesPath+"/Sprites";
    scenesPath=projectFolderPath+resourcesPath+"/Scenes";
    QDir spritesDir(spritesPath);
    if(spritesDir.exists()!=true){
        QDir().mkpath(spritesPath);
    }
    QDir scenesDir(scenesPath);
    if(scenesDir.exists()!=true){
        QDir().mkpath(scenesPath);
    }

    //Checking Turquoise2D Framework
    QString runtimeFrameworkPath=projectFolderPath+this->classesPath+"/Turquoise2DFramework";
    qDebug()<<"runtime path:"<<runtimeFrameworkPath;
    if(QDir(runtimeFrameworkPath).exists()!=true){
        QString originalRuntimePath;
        if(frameworkName=="Cocos2d-x"){
            originalRuntimePath=FileOperations::getAppRootPath()+"/FrameworkHelpers/Cocos2d-x/Turquoise2DFramework";
        }
        qDebug()<<"originalRuntimePath:"<<originalRuntimePath;
        FileOperations::copyFolder(originalRuntimePath,runtimeFrameworkPath);
    }
    ProjectFilesOperations::updateXcodeProjectFiles(this);

}

void Turquoise2DEditor::addNewSceneTab(QString sceneFilePath)
{
    ScenePanel* alreadyOpenedScene=isSceneAlreadyOpened(sceneFilePath);
    if(alreadyOpenedScene){
        //int index=ui->scenesTab->indexOf(alreadyOpenedScene);
        ui->scenesTab->setCurrentWidget(alreadyOpenedScene);
        return;
    }

    ScenePanel *nScenePanel=new ScenePanel(sceneFilePath,ui->propPanel,this,this);
    openedScenes<<nScenePanel;
    QString titleName=QFileInfo(sceneFilePath).fileName();
    int tabNum=ui->scenesTab->addTab(nScenePanel,titleName);
    ui->scenesTab->setCurrentIndex(tabNum);
}

void Turquoise2DEditor::showHierarchyList(HierarchyPanel *hPanel)
{
    clearHierarchyPanel();
    ui->hierarchyLayout->layout()->addWidget(hPanel);
    hPanel->show();
    currentHierarcyPanel=hPanel;
}

void Turquoise2DEditor::clearHierarchyPanel()
{
    if(currentHierarcyPanel){
         ui->hierarchyLayout->layout()->removeWidget(currentHierarcyPanel);
         currentHierarcyPanel->hide();
         currentHierarcyPanel=NULL;
    }
}

ScenePanel* Turquoise2DEditor::isSceneAlreadyOpened(QString sceneFilePath)
{
    for(int i=0;i<openedScenes.count();i++){
        //sp: scenePanel
        ScenePanel* sp=openedScenes.at(i);
        if(sp->sceneFilePath==sceneFilePath){
            return sp;
        }
    }
    return NULL;
}

void Turquoise2DEditor::renameScaneTab(ScenePanel *scenePanel,QString newName)
{
    int index=ui->scenesTab->indexOf(scenePanel);
    qDebug()<<index;
    if(index!=-1){
        QString titleStr;
        if(newName==""){
            titleStr=QFileInfo(scenePanel->sceneFilePath).fileName();
            if(scenePanel->isSceneChanged){
                titleStr=titleStr+"*";
            }
        }else{
            titleStr=newName;
        }
        ui->scenesTab->setTabText(index,titleStr);
    }
}

bool Turquoise2DEditor::checkUnsavedScenes()
{
    for(int i=0;i<openedScenes.count();i++){
        ScenePanel *scn=openedScenes.at(i);
        if(scn->isSceneChanged){
            return true;
        }
    }
    return false;
}

bool Turquoise2DEditor::closeAllScenes()
{
    if(checkUnsavedScenes()){
        QString message="There are unsaved scenes in current project. Are you sure?";
        QMessageBox::StandardButton resBtn = QMessageBox::question( this,"Turquoise2D Editor",message,QMessageBox::Cancel | QMessageBox::Yes);
        if(resBtn== QMessageBox::Cancel){
            return false;
        }
    }
    while(openedScenes.count()>0){
        ScenePanel *scn=openedScenes.at(0);
        scn->isSceneChanged=false;
        int index=ui->scenesTab->indexOf(scn);
        qDebug()<<"index:"<<index;
        if(index!=-1){
            ui->scenesTab->tabCloseRequested(index);
        }
    }
    return true;
}

void Turquoise2DEditor::clearCollectionPanel()
{
    if(spritesBrowser)
        delete spritesBrowser;
    if(scenesBrowser)
        delete scenesBrowser;
    if(classesBrowser)
        delete classesBrowser;
}

void Turquoise2DEditor::initCollectionPanel()
{
    clearCollectionPanel();
    spritesBrowser=new CollectionBrowser(projectFolderPath+"/Resources/res/Sprites/",QStringList()<<"*.png"<<"*.jpg"<<"*.jpeg"<<"*.tga"<<"*.tiff"<<"*.plist",this);
    spritesBrowser->show();
    ui->pc_tabs->addTab(spritesBrowser,"Sprites");

    scenesBrowser=new CollectionBrowser(projectFolderPath+"/Resources/res/Scenes/",QStringList()<<"*.t2dscene",this);
    scenesBrowser->show();
    ui->pc_tabs->addTab(scenesBrowser,"Scenes");

    classesBrowser=new CollectionBrowser(projectFolderPath+"/Classes/",QStringList()<<"*.h"<<"*.cpp",this);
    classesBrowser->show();
    ui->pc_tabs->addTab(classesBrowser,"Classes");
}



void Turquoise2DEditor::initMenus()
{
    fileMenu=new FileMenu(this);
    this->menuBar()->addMenu(fileMenu);

    //We will add in the future
    //projectMenu=new ProjectMenu(this);
    //this->menuBar()->addMenu(projectMenu);

    windowMenu=new WindowMenu(this);
    this->menuBar()->addMenu(windowMenu);
}

void Turquoise2DEditor::initToolsBar()
{

    //SCENE TOOLS
    QToolBar *sceneToolBar=this->addToolBar("Scene Tools");

    sceneToolsGroup=new QActionGroup(sceneToolBar);
    //Defining actions
    selectTool=new QAction(QIcon( QPixmap(":/Icons/toolicon_default.png") ),"Select",this);
    selectTool->setToolTip("Move/Transform Tool (V)");
    panTool=new QAction(QIcon( QPixmap(":/Icons/toolicon_pan.png") ),"Pan",this);
    panTool->setToolTip("Pan Tool (H)");
    zoomTool=new QAction(QIcon( QPixmap(":/Icons/toolicon_zoom.png") ),"Zoom",this);
    zoomTool->setToolTip("Zoom Tool (Z)");
    drawTool=new QAction(QIcon( QPixmap(":/Icons/toolicon_pencil.png") ),"Draw",this);
    drawTool->setToolTip("Draw Tool (B)");
    //Setting checkable
    selectTool->setCheckable(true);
    selectTool->setChecked(true);
    panTool->setCheckable(true);
    zoomTool->setCheckable(true);
    drawTool->setCheckable(true);
    //Adding actions to tools group
    sceneToolsGroup->addAction(selectTool);
    sceneToolsGroup->addAction(panTool);
    sceneToolsGroup->addAction(zoomTool);
    sceneToolsGroup->addAction(drawTool);
    sceneToolBar->addActions(sceneToolsGroup->actions());
    sceneToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    sceneToolBar->setMovable(false);

    connect(sceneToolsGroup,&QActionGroup::triggered,[=](QAction* checked){
        SceneEditModes selectedMode=SceneEditModes::GameItemsEdit;
        if(checked==panTool){
            selectedMode=SceneEditModes::PanEdit;
        }else if(checked==zoomTool){
            selectedMode=SceneEditModes::ZoomEdit;
        }else if(checked==drawTool){
            selectedMode=SceneEditModes::DrawEdit;
        }
        if(currentScene){
           currentScene->scene->setEditMode(selectedMode);
           currentScene->scene->views().at(0)->setFocus();
        }
        lastSelectedEditMode=selectedMode;
    });

    //PROJECT TOOLS
    QToolBar *projectToolBar=this->addToolBar("Project Tools");
    QAction *runProjectButton=new QAction(QIcon(":/Icons/toolicon_play.png"),"Run",this);
    projectToolBar->addAction(runProjectButton);
    QToolButton *runSettingsButton=new QToolButton;
    runSettingsButton->setIcon(QIcon(":/Icons/toolicon_playsettings.png"));
    runSettingsButton->setPopupMode(QToolButton::InstantPopup);
    projectToolBar->addWidget(runSettingsButton);
    projectToolBar->setMovable(true);

    connect(runProjectButton,&QAction::triggered,[=](){
        ProjectConsole::Run(this);
    });

    QMenu *runSettingsMenu=new QMenu(this);
    QMenu *frameworkVersion=new QMenu(runSettingsMenu);
    frameworkVersion->setTitle("Framework Version");
    runSettingsMenu->addMenu(frameworkVersion);
    QActionGroup *frameworkVersionsActions=new QActionGroup(frameworkVersion);

    connect(runSettingsMenu,&QMenu::aboutToShow,[=]{
        for(QAction *act:frameworkVersionsActions->actions()){
            frameworkVersionsActions->removeAction(act);
        }
        QStringList frameworkVersionOptions=ProjectFilesOperations::getFrameworkPaths(this);
        for(int i=0;i<frameworkVersionOptions.count();i++){
            QAction *act=new QAction(QFileInfo(frameworkVersionOptions.at(i)).fileName(),this);
            act->setData(frameworkVersionOptions.at(i));
            act->setCheckable(true);
            frameworkVersionsActions->addAction(act);
            if(this->frameworkLibraryPath==frameworkVersionOptions.at(i)){
                act->setChecked(true);
            }
        }
        frameworkVersion->clear();
        frameworkVersion->addActions(frameworkVersionsActions->actions());
    });

    connect(frameworkVersionsActions,&QActionGroup::triggered,[=](QAction *act){
        this->frameworkLibraryPath=act->data().toString();
        qDebug()<<frameworkLibraryPath;
    });
    runSettingsButton->setMenu(runSettingsMenu);

}

void Turquoise2DEditor::selectToolWithEditMode(SceneEditModes targetEditMode)
{
    if(targetEditMode==SceneEditModes::GameItemsEdit){
        selectTool->setChecked(true);

    }else if(targetEditMode==SceneEditModes::PanEdit){
        panTool->setChecked(true);

    }else if(targetEditMode==SceneEditModes::ZoomEdit){
        zoomTool->setChecked(true);

    }else if(targetEditMode==SceneEditModes::DrawEdit){
        drawTool->setChecked(true);
    }
    if(currentScene->scene->editMode!=targetEditMode){
        currentScene->scene->setEditMode(targetEditMode);
    }
    lastSelectedEditMode=targetEditMode;
}

void Turquoise2DEditor::copyItems()
{
    if(currentScene){
        if(currentScene->scene->lastTransformItem){
            copiedItems.clear();
            QList<QGraphicsItem*> items=currentScene->scene->lastTransformItem->items;
            for(int i=0;i<items.count();i++){
                GameItem *cgi=dynamic_cast<GameItem*>(items.at(i));
                if(cgi){
                    copiedItems<<cgi->toJsonObject();
                }
            }
            updateCopyPasteActions();
        }
    }
    qDebug()<<"Copied "<<copiedItems.count()<<" items";
}

void Turquoise2DEditor::pasteItems()
{
    if(currentScene){
        QUndoCommand *cmd=new AddJsonBasedItemCommand(currentScene->scene,copiedItems);
        currentScene->scene->stack->push(cmd);
    }
}

void Turquoise2DEditor::updateCopyPasteActions()
{
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
    if(currentScene!=NULL){
        if(currentScene->scene->lastTransformItem){
            copyAction->setEnabled(true);
        }
        if(this->copiedItems.count()>0){
            pasteAction->setEnabled(true);
        }
    }

}
//SHORTCUTS AND KEYBOARD INPUTS
void Turquoise2DEditor::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Space){
        if(currentScene){
            if(currentScene->scene->startSelectionPoint==NULL){
                if(currentScene->scene->editMode!=SceneEditModes::PanEdit){
                    currentScene->scene->setEditMode(SceneEditModes::PanEdit);
                }
            }
        }
        qDebug()<<"space";

    }else if(e->key()==Qt::Key_Alt){
        qDebug()<<"eyedropper";
        if(currentScene){
            if(currentScene->scene->editMode==SceneEditModes::DrawEdit){
                currentScene->scene->setEditMode(SceneEditModes::EyedropperEdit);
            }
        }
    }
}

void Turquoise2DEditor::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Space){
        if(currentScene){
            currentScene->scene->setEditMode(lastSelectedEditMode);
        }
    }else if(e->key()==Qt::Key_Alt){
        if(currentScene->scene->editMode==SceneEditModes::EyedropperEdit){
            if(currentScene){
                currentScene->scene->setEditMode(lastSelectedEditMode);
            }
        }
    }
}


void Turquoise2DEditor::on_pc_tabs_tabBarClicked(int index)
{
    //ON TAB BAR CLICKED
}

void Turquoise2DEditor::on_press_Backspace()
{
    qDebug()<<"test";
    if(currentScene!=NULL){
        if(currentScene->scene->lastTransformItem!=NULL){
            QUndoCommand *cmd=new RemoveItemCommand(currentScene->scene,currentScene->scene->lastTransformItem->items);
            currentScene->scene->stack->push(cmd);
        }
    }

}

void Turquoise2DEditor::on_press_Enter()
{
    QObject *focusedObj=qApp->focusObject();
    //Defining focused object types
    QLineEdit *lineEdit=dynamic_cast<QLineEdit*>(focusedObj);
    QSpinBox *spinbox=dynamic_cast<QSpinBox*>(focusedObj);
    QDoubleSpinBox *doublespinbox=dynamic_cast<QDoubleSpinBox*>(focusedObj);
    //If focused object type QlineEdit
    if(lineEdit || spinbox || doublespinbox ){
        this->setFocus();
    }
}


