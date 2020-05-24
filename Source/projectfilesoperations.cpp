#include "projectfilesoperations.h"
#include "turquoise2deditor.h"

ProjectFilesOperations::ProjectFilesOperations()
{

}
/*
 * Relative path should came from *.xcodeproj file. Samples => "../Classes/class1.h" "../Classes/Turquoise2DFramework"
 * Default path should be physical path. Samples => "/Users/username/Desktop/project/Classes/class1.h"
 */
void ProjectFilesOperations::addFilesToXcodeProject(QStringList relativeFilePathList,QString relativeParentFolderPath,Turquoise2DEditor *mainWindow)
{
    QDir helperDir(QApplication::applicationDirPath());
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cd("ProjectHelpers");
    helperDir.cd("xcode");
    QFileInfo helperFile(helperDir.absolutePath()+"/xcodemod.py");

    QString filesArg;
    for(int i=0;i<relativeFilePathList.count();i++){
        filesArg+=relativeFilePathList.at(i);
        if(i!=relativeFilePathList.count()-1)
            filesArg+=",";
    }

    QProcess *proc=new QProcess();
    proc->setWorkingDirectory(helperFile.absolutePath());
    QStringList comm;
    comm<<"xcodemod.py"<<"-addfiles"<<"-xcp"<<getXcodeFilePath(mainWindow)<<"-rfp"<<filesArg<<"-rpfp"<<relativeParentFolderPath;
    proc->start("python",comm);
    qDebug()<<"Added Folder from Xcode Project => "<<filesArg;
}

void ProjectFilesOperations::addFolderToXcodeProject(QString folderPath,QString relativeParentFolderPath,Turquoise2DEditor *mainWindow)
{
    QDir helperDir(QApplication::applicationDirPath());
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cd("ProjectHelpers");
    helperDir.cd("xcode");
    QFileInfo helperFile(helperDir.absolutePath()+"/xcodemod.py");

    QProcess *proc=new QProcess();
    proc->setWorkingDirectory(helperFile.absolutePath());
    QStringList comm;
    comm<<"xcodemod.py"<<"-addfolder"<<"-xcp"<<getXcodeFilePath(mainWindow)<<"-fp"<<folderPath;
    if(relativeParentFolderPath!=""){
        comm<<"-rpfp"<<relativeParentFolderPath;
    }
    proc->start("python",comm);
    qDebug()<<"Added Folder from Xcode Project => "<<folderPath;
}

void ProjectFilesOperations::removeFolderFromXcodeProject(QString relativeFolderPath,Turquoise2DEditor *mainWindow)
{
    QDir helperDir(QApplication::applicationDirPath());
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cd("ProjectHelpers");
    helperDir.cd("xcode");
    QFileInfo helperFile(helperDir.absolutePath()+"/xcodemod.py");

    QProcess *proc=new QProcess();
    proc->setWorkingDirectory(helperFile.absolutePath());
    QStringList comm;
    comm<<"xcodemod.py"<<"-rmfolder"<<"-xcp"<<getXcodeFilePath(mainWindow)<<"-rfp"<<relativeFolderPath;
    proc->start("python",comm);
    qDebug()<<"Deleted Folder from Xcode Project =>"<<relativeFolderPath;
}

void ProjectFilesOperations::removeFilesFromXcodeProject(QStringList relativeFilePathList, Turquoise2DEditor *mainWindow)
{
    QDir helperDir(QApplication::applicationDirPath());
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cd("ProjectHelpers");
    helperDir.cd("xcode");
    QFileInfo helperFile(helperDir.absolutePath()+"/xcodemod.py");

    QString filesArg;
    for(int i=0;i<relativeFilePathList.count();i++){
        filesArg+=relativeFilePathList.at(i);
        if(i!=relativeFilePathList.count()-1)
            filesArg+=",";
    }

    QProcess *proc=new QProcess();
    proc->setWorkingDirectory(helperFile.absolutePath());
    QStringList comm;
    comm<<"xcodemod.py"<<"-rmfiles"<<"-xcp"<<getXcodeFilePath(mainWindow)<<"-rfp"<<filesArg;
    proc->start("python",comm);
    qDebug()<<"Deleted Files from Xcode Project =>"<<filesArg;
}

void ProjectFilesOperations::updateXcodeProjectFiles(Turquoise2DEditor *mainWindow)
{
    QDir helperDir(QApplication::applicationDirPath());
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cdUp();
    helperDir.cd("ProjectHelpers");
    helperDir.cd("xcode");
    QFileInfo helperFile(helperDir.absolutePath()+"/xcodemod.py");

    qDebug()<<"HelperFile: "<<helperFile.absoluteFilePath();
    qDebug()<<"Folder Path: "<<mainWindow->projectFolderPath+mainWindow->classesPath;
    qDebug()<<"Xcode File Path: "<<getXcodeFilePath(mainWindow);

    QProcess *proc=new QProcess();
    proc->setWorkingDirectory(helperFile.absolutePath());
    QStringList comm;
    comm<<"xcodemod.py"<<"-updatefolder"<<"-xcp"<<getXcodeFilePath(mainWindow)<<"-fp"<<mainWindow->projectFolderPath+mainWindow->classesPath<<"-rfp"<<mainWindow->macProjectFileToClasses;
    proc->start("python",comm);
    qDebug()<<"Updated => "<<mainWindow->projectFolderPath+mainWindow->classesPath<<" folder from Xcode Project";
}

QString ProjectFilesOperations::getXcodeFilePath(Turquoise2DEditor *mainWindow)
{
    QDir dir(mainWindow->projectFolderPath+mainWindow->macProjectPath);
    QFileInfoList fileList=dir.entryInfoList();
    QString resultStr="";
    for(int i=0;i<fileList.count();i++){
        if(fileList.at(i).suffix()=="xcodeproj"){
            resultStr=fileList.at(i).absoluteFilePath();
        }
    }
    return resultStr;
}

QString ProjectFilesOperations::getXcodeFilePath(QString projectPath)
{
    QString resultStr="";
    QString frameworkName=checkDirForFrameworks(projectPath);
    if(frameworkName=="Cocos2d-x"){
        Cocos2dxSettings settings;
        QDir dir(projectPath+settings.macProjectPath);
        QFileInfoList fileList=dir.entryInfoList();
        for(int i=0;i<fileList.count();i++){
            if(fileList.at(i).suffix()=="xcodeproj"){
                resultStr=fileList.at(i).absoluteFilePath();
            }
        }
    }
    return resultStr;

}

QString ProjectFilesOperations::checkDirForFrameworks(QString dir)
{
    QDir directory(dir);
    bool cond1,cond2,cond3,cond4,cond5;
    //Cococs2d-x condinations
    cond1=QDir(directory.absolutePath()+"/proj.ios_mac").exists();
    cond2=QDir(directory.absolutePath()+"/Resources").exists();
    cond3=QDir(directory.absolutePath()+"/Classes").exists();
    if(cond1 && cond2 && cond3){
        return "Cocos2d-x";
    }
    return "";
}

QStringList ProjectFilesOperations::getFrameworkPaths(Turquoise2DEditor *mainWindow, QString frameworkName)
{
    //Setting framework folder
    QStringList resultList;
    QString frameworksPath=FileOperations::getAppRootPath()+"/Frameworks";

    QString fName=frameworkName;
    if(fName==""){
        fName=mainWindow->frameworkName;
    }
    if(fName=="Cocos2d-x"){
        frameworksPath=frameworksPath+"/Cocos2d-x";
        QDir frameworksDir(frameworksPath);
        QFileInfoList entryList=frameworksDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,QDir::Name);

        for(int i=0;i<entryList.count();i++){
            resultList<<entryList.at(i).absoluteFilePath();
        }
    }
    return resultList;
}

