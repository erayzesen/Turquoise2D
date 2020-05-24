#include "classoperations.h"
#include "turquoise2deditor.h"

ClassOperations::ClassOperations()
{
    //
}

void ClassOperations::createObjectFactoryClass(Turquoise2DEditor *mainWindow)
{
    //Generating ObjectFactory class in framework
    ClassGenerator *factoryClass=new ClassGenerator("ObjectFactory");
    factoryClass->setHeaderIncludes(QStringList()<<"stdio.h"<<mainWindow->frameworkIncludeClass<<getProjectObjectClasses(mainWindow));
    factoryClass->addNameSpace("std");
    factoryClass->defineNewVariable(ClassAccessTypes::Public,mainWindow->objectLinkedTypeName+"*","newNode","");
    factoryClass->defineNewFunction(ClassAccessTypes::Public,"","ObjectFactory","","");
    //create function codes
    QTextStream cfc(new QString());
    cfc<<mainWindow->objectLinkedTypeName+ "* object;\n";
    QStringList objectClasses=getProjectObjectClasses(mainWindow,false);
    for(int i=0;i<objectClasses.count();i++){
        QString classType=getClassTypeName(objectClasses.at(i));
        if(i>0){
            cfc<<"else ";
        }
        cfc<<"if(classType==\""<<classType<<"\"){\n";
        cfc<<"object=new "<<classType<<"();\n";
        cfc<<"}";
    }
    cfc<<"\n";
    cfc<<"return object;";
    factoryClass->defineNewFunction(ClassAccessTypes::PublicStatic,mainWindow->objectLinkedTypeName+"*","create","string classType",cfc.readAll());
    factoryClass->writeToFile("ObjectFactory",mainWindow->projectFolderPath+mainWindow->classesPath+"/Turquoise2DFramework",true);
}

void ClassOperations::createNewSceneClass(QString className,QString classFolder,QString scenePath, Turquoise2DEditor *mainWindow)
{
    ClassGenerator *factoryClass=new ClassGenerator(className,QStringList()<<"cocos2d::Layer");
    factoryClass->setHeaderIncludes(QStringList()<<mainWindow->frameworkIncludeClass<<"Turquoise2D.h");
    factoryClass->addMacroToCpp("USING_NS_CC;");
    QTextStream csFuncCode(new QString()); //createScene function Code
    csFuncCode<<"auto scene = Scene::createWithPhysics(); \n";
    csFuncCode<<"auto layer = "+className+"::create(); \n";
    csFuncCode<<"scene->addChild(layer); \n";
    csFuncCode<<"return scene; \n";
    factoryClass->defineNewFunction(ClassAccessTypes::PublicStatic,"cocos2d::Scene*","createScene","",csFuncCode.readAll());
    QTextStream initFuncCode(new QString());
    initFuncCode<<"if ( !Layer::init() )\n";
    initFuncCode<<"{\n";
    initFuncCode<<"return false;\n";
    initFuncCode<<"}\n\n";
    initFuncCode<<"//Import Turquoise2D Scene\n";
    initFuncCode<<"gameScene=Turquoise2D::import(\""+scenePath+"\",this); \n";
    initFuncCode<<"//(!)For getting object: gameScene->GetObject<objectType*>(\"objectName\") \n";
    initFuncCode<<"//(!)For getting sprite: gameScene->GetSprite(\"spriteName\") \n";
    initFuncCode<<"return true; \n";
    factoryClass->defineNewFunction(ClassAccessTypes::PublicVirtual,"bool","init","",initFuncCode.readAll());
    factoryClass->defineNewVariable(ClassAccessTypes::Public,"Turquoise2D*","gameScene","");
    factoryClass->defineNewVariable(ClassAccessTypes::Public,"","CREATE_FUNC("+className+")","");

    factoryClass->writeToFile(className,classFolder,false);

    qDebug()<<classFolder.split("/Classes/").at(1)+className+".h";
    qDebug()<<classFolder.split("/Classes/").at(1)+className+".cpp";
    QString headerPath=classFolder.split("/Classes/").at(1)+className+".h";
    QString cppPath=classFolder.split("/Classes/").at(1)+className+".cpp";
    //ProjectFilesOperations::addClassToXcodeProject(headerPath,mainWindow);
    ProjectFilesOperations::updateXcodeProjectFiles(mainWindow);


}

QStringList ClassOperations::getProjectClasses(Turquoise2DEditor *mainWindow,bool enableRelative)
{
    QString rootPath=mainWindow->projectFolderPath+mainWindow->classesPath;
    QStringList typeList;
    typeList<<"*.h";
    QStringList fileList;
    QDir dir(rootPath);
    QDirIterator it(dir.absolutePath(), typeList, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()){
        //Filtering files
        it.next();
        if(it.fileInfo().absoluteFilePath()=="")
            continue;
        QString nPath=it.fileInfo().absoluteFilePath();
        if(enableRelative){
            nPath=nPath.split("/Classes/").at(1);
        }
        fileList.append(nPath);
    }
    qSort(fileList.begin(),fileList.end());
    return fileList;
}

QString ClassOperations::getClassTypeName(QString classPath)
{
    QString result;
    QFile file(classPath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "error", file.errorString());
    }
    QString text(file.readAll());
    QRegularExpression re("class\\s+([\\w]+)\\s?\\:\\s?public");
    QRegularExpressionMatch match=re.match(text);
    if(match.hasMatch()){
        result=match.captured(1);
    }
    return result;
}

QStringList ClassOperations::getProjectObjectClasses(Turquoise2DEditor *mainWindow,bool enableRelative)
{
    QStringList result;
    QStringList classFiles=getProjectClasses(mainWindow,false);
    for(int i=0;i<classFiles.count();i++){
        QString filePath=classFiles.at(i);
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(0, "error", file.errorString());
        }
        QString text(file.readAll());
        text.simplified();
        if(text.indexOf(": public cocos2d::Node")>0){
            if(enableRelative){
                filePath=filePath.split("/Classes/").at(1);
            }
            result.append(filePath);
        }
    }
    return result;
}

