#include "filemenu.h"
#include "turquoise2deditor.h"

FileMenu::FileMenu(Turquoise2DEditor *_mainWindow,QWidget *parent) : QMenu(parent)
{
    this->setTitle("File");
    mainWindow=_mainWindow;
    //actions
    newProject=new QAction("New Project",this);
    this->addAction(newProject);
    openProject=new QAction("Open Project",this);
    this->addAction(openProject);
    newScene=new QAction("New Scene",this);
    newScene->setShortcut(QKeySequence::New);
    this->addAction(newScene);
    openScene=new QAction("Open Scene",this);
    openScene->setShortcut(QKeySequence::Open);
    this->addAction(openScene);
    saveScene=new QAction("Save Scene",this);
    saveScene->setShortcut(QKeySequence::Save);
    this->addAction(saveScene);
    //events
    connect(newProject,&QAction::triggered,this,&FileMenu::NewProject);
    connect(openProject,&QAction::triggered,this,&FileMenu::OpenProject);
    connect(newScene,&QAction::triggered,this,&FileMenu::NewScene);
    connect(openScene,&QAction::triggered,this,&FileMenu::OpenScene);
    connect(saveScene,&QAction::triggered,this,&FileMenu::SaveScene);
}

QJsonDocument FileMenu::generateNewSceneJson()
{
    QJsonObject sceneProperties{
        {"type","Turquoise2D Scene"},
        {"gridSize",QJsonArray{64,64}},
        {"isSnapMode",false},
        {"snapStyle","withCenter"}
    };
    QJsonObject sceneJson{
        {"sceneProperties",sceneProperties},
        {"objects",QJsonArray()},
        {"sprites",QJsonArray()}
    };
    QJsonDocument jsonDoc(sceneJson);
    return jsonDoc;
}

QHBoxLayout* FileMenu::createProjectInfo(QString projectLocation,QString frameworkName)
{
    QDir dir(projectLocation);
    //
    QString projectName,iconImagePath;
    if(frameworkName=="Cocos2d-x"){
        QFileInfo xcodeFile(ProjectFilesOperations::getXcodeFilePath(projectLocation));
        projectName=xcodeFile.baseName();
        iconImagePath=":/Icons/fwIcons_cocos2dx_128x128.png";
    }
    QHBoxLayout *mlayout=new QHBoxLayout;
    QFormLayout *rLayout=new QFormLayout;
    rLayout->setFormAlignment(Qt::AlignLeft);
    rLayout->setLabelAlignment(Qt::AlignLeft);
    QVBoxLayout *lLayout=new QVBoxLayout;

    QLabel *iconLabel=new QLabel;
    iconLabel->setMaximumSize(64,64);
    QPixmap imagePix(iconImagePath);
    imagePix=imagePix.scaled(64,64);
    iconLabel->setPixmap(imagePix);
    lLayout->addWidget(iconLabel);
    lLayout->setAlignment(Qt::AlignTop);

    mlayout->addLayout(lLayout);

    QLabel *nameField=new QLabel(projectName);
    nameField->setEnabled(false);
    rLayout->addRow("Project Name:",nameField);

    QLabel *frameworkField=new QLabel(frameworkName);
    frameworkField->setEnabled(false);
    rLayout->addRow("Framework:",frameworkField);
    rLayout->setAlignment(Qt::AlignLeft);

    mlayout->addLayout(rLayout);


    return mlayout;
}

void FileMenu::clearLayout(QLayout *ly)
{
    if(ly){
        while(ly->count()>0){
            QLayoutItem *item=ly->takeAt(0);
            if(item->layout()){
                clearLayout(item->layout());
            }
            if(item->widget()){
                delete item->widget();
                delete item;
            }
        }
    }
}

void FileMenu::OpenProject()
{
    QDialog *dialog=new QDialog();
    dialog->setWindowTitle("Open Project");
    QVBoxLayout *mLayout=new QVBoxLayout;
    mLayout->setAlignment(Qt::AlignTop);
    mLayout->setSizeConstraint(QLayout::SetFixedSize);
    dialog->setLayout(mLayout);

    QHBoxLayout *selectFolderLayout=new QHBoxLayout();
    selectFolderLayout->addWidget(new QLabel("Project Location"));
    QLineEdit *FolderField=new QLineEdit;
    FolderField->setEnabled(false);
    selectFolderLayout->addWidget(FolderField);
    QPushButton *selectFolderButton=new QPushButton("Select Folder");
    selectFolderLayout->addWidget(selectFolderButton);
    mLayout->addLayout(selectFolderLayout);

    QVBoxLayout *projectInfoLayout=new QVBoxLayout;
    mLayout->addLayout(projectInfoLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Open)->setEnabled(false);
    mLayout->addWidget(buttonBox);

    connect(buttonBox,&QDialogButtonBox::accepted,[=]{
        dialog->accept();
    });
    connect(buttonBox,&QDialogButtonBox::rejected,[=]{
        dialog->reject();
    });



    connect(selectFolderButton,&QPushButton::released,[=] {
        //Opening select folder dialog
        QFileDialog folderDialog;
        folderDialog.setFileMode(QFileDialog::Directory);
        folderDialog.setOption(QFileDialog::ShowDirsOnly);
        folderDialog.setDirectory(QDir::homePath());
        if(folderDialog.exec()){
            //We will check selected folder (if the folder is in project scene folder)
            QString nDirectory=folderDialog.directory().absolutePath();
            QString frameworkType=ProjectFilesOperations::checkDirForFrameworks(nDirectory);
            qDebug()<<projectInfoLayout->count();
            if(frameworkType==""){
                buttonBox->button(QDialogButtonBox::Open)->setEnabled(false);
                FolderField->setText("");
                clearLayout(projectInfoLayout);

            }else{
                clearLayout(projectInfoLayout);
                buttonBox->button(QDialogButtonBox::Open)->setEnabled(true);
                FolderField->setText(nDirectory);
                QHBoxLayout *infoLayout=createProjectInfo(nDirectory,frameworkType);
                projectInfoLayout->addLayout(infoLayout);
            }
        }
    });

    if(dialog->exec()){
        //Open Project Codes
        mainWindow->OpenProject(FolderField->text());
    }


}

void FileMenu::NewProject()
{
    if(mainWindow->closeAllScenes()){
        NewProjectDialog *dialog=new NewProjectDialog(mainWindow);
        dialog->exec();
    }

}

void FileMenu::NewScene()
{
    qDebug()<<"new scene trigerred";
    QString projectRoot=mainWindow->projectFolderPath;
    QString resourcesPath=mainWindow->resourcesPath;
    QString defaultTargetFolder=projectRoot+resourcesPath+"/Scenes";

    QDialog *newSceneDialog=new QDialog();
    newSceneDialog->setWindowTitle("Create New Scene");
    QVBoxLayout *dialogLayout=new QVBoxLayout;
    newSceneDialog->setLayout(dialogLayout);

    QHBoxLayout *formLayout=new QHBoxLayout;
    QLineEdit *sceneNameField=new QLineEdit;
    formLayout->addWidget(new QLabel("Scene Name:"));
    formLayout->addWidget(sceneNameField);
    dialogLayout->addLayout(formLayout);

    QHBoxLayout *folderSelectLayout=new QHBoxLayout;
    folderSelectLayout->addWidget(new QLabel("Target Folder:"));
    QLineEdit *targetFolderField=new QLineEdit(defaultTargetFolder);
    targetFolderField->setEnabled(false);
    folderSelectLayout->addWidget(targetFolderField);
    QPushButton *targetFolderSelectButton=new QPushButton("Select Folder");
    folderSelectLayout->addWidget(targetFolderSelectButton);

    dialogLayout->addLayout(folderSelectLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |     QDialogButtonBox::Cancel);
    dialogLayout->addWidget(buttonBox);

    connect(buttonBox,&QDialogButtonBox::accepted,[=]{
        newSceneDialog->accept();
    });
    connect(buttonBox,&QDialogButtonBox::rejected,[=]{
        newSceneDialog->reject();
    });

    connect(targetFolderSelectButton,&QPushButton::released,[=]{
        //Opening select folder dialog
        QFileDialog folderDialog;
        folderDialog.setFileMode(QFileDialog::Directory);
        folderDialog.setOption(QFileDialog::ShowDirsOnly);
        folderDialog.setDirectory(targetFolderField->text());
        if(folderDialog.exec()){
            //We will check selected folder (if the folder is in project scene folder)
            QString newDirectory=folderDialog.directory().absolutePath();
            targetFolderField->setText(newDirectory);
        }
    });

    if(newSceneDialog->exec()){
        qDebug()<<targetFolderField->text();
        QString sceneFilePath=targetFolderField->text()+"/"+sceneNameField->text()+".t2dscene";
        qDebug()<<sceneFilePath;
        QFile file(sceneFilePath);
        if(!file.open(QIODevice::WriteOnly)){
            qWarning()<<"Can't creating scene in '"<<sceneFilePath<<"' named folder!";
            return;
        }
        file.write(generateNewSceneJson().toJson());
        mainWindow->addNewSceneTab(sceneFilePath);
        QString insideFolder;
        QStringList parsedFolder=targetFolderField->text().split("/Scenes/");
        if(parsedFolder.count()<2){
            insideFolder="";
        }else{
            insideFolder=parsedFolder.at(1);
        }
        qDebug()<<"insideFolder:"<<insideFolder;
        QString classFolderPath=projectRoot+mainWindow->classesPath+"/"+insideFolder;
        ClassOperations::createNewSceneClass(sceneNameField->text(),classFolderPath,sceneFilePath.split("/res/").at(1),this->mainWindow);
    }


}

void FileMenu::OpenScene()
{
    QString projectRoot=mainWindow->projectFolderPath;
    QString resourcesPath=mainWindow->resourcesPath;
    QString defaultTargetFolder=projectRoot+resourcesPath+"Scenes/";

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Turquoise 2D Scenes (*.t2dscene)");
    //fileDialog.setOption(QFileDialog::ShowDirsOnly);
    fileDialog.setDirectory(defaultTargetFolder);
    if(fileDialog.exec()){
        QString selectedFilePath;
        if(fileDialog.selectedFiles().count()==1){
            //We will check selected folder (if the folder is in project scene folder)
            selectedFilePath=fileDialog.selectedFiles().at(0);
        }else{
            qDebug()<<"Can't valid selected file";
            return;
        }
        mainWindow->addNewSceneTab(selectedFilePath);
        qDebug()<<fileDialog.selectedFiles();
    }
}

void FileMenu::SaveScene()
{
    QString sceneFilePath=mainWindow->currentScene->sceneFilePath;
    QFile file(sceneFilePath);
    if(!file.open(QIODevice::WriteOnly)){
        qWarning()<<"Can't save scene in '" <<sceneFilePath << "' road!";
        return;
    }
    QJsonObject sceneJson=mainWindow->currentScene->scene->toJsonObject();
    QJsonDocument jsonDoc(sceneJson);
    file.write(jsonDoc.toJson());
    qDebug()<<"save scene trigerred";
    mainWindow->currentScene->isSceneChanged=false;
    mainWindow->renameScaneTab(mainWindow->currentScene);
    //Checking project files
    ClassOperations::createObjectFactoryClass(this->mainWindow);
    ProjectFilesOperations::updateXcodeProjectFiles(mainWindow);
}

void FileMenu::SaveAsScene()
{
    QString sceneName="deneme.t2dscene";
    QString projectRoot=mainWindow->projectFolderPath;
    QString resourcesPath=mainWindow->resourcesPath;
    QString targetFolder=projectRoot+resourcesPath+"Scenes";
    QString sceneFilePath=targetFolder+sceneName;
    QFile file(sceneFilePath);
    if(!file.open(QIODevice::WriteOnly)){
        qWarning()<<"Can't create scene in '" <<targetFolder << "' named folder!";
        return;
    }
    QJsonObject sceneJson=mainWindow->currentScene->scene->toJsonObject();
    QJsonDocument jsonDoc(sceneJson);
    file.write(jsonDoc.toJson());
    qDebug()<<"save scene trigerred";
    //burada scene'yi kaydeden kod gelecek
}
