#pragma once
#include "classgenerator.h"
#include "QProcess"
#include "projectfilesoperations.h"

class Turquoise2DEditor;
class ClassOperations
{
public:
    ClassOperations();
    static void createObjectFactoryClass(Turquoise2DEditor *mainWindow);
    static void createNewSceneClass(QString className,QString classFolder,QString scenePath,Turquoise2DEditor *mainWindow);

    static QStringList getProjectObjectClasses(Turquoise2DEditor *mainWindow,bool enableRelative=true);
    static QStringList getProjectClasses(Turquoise2DEditor *mainWindow,bool enableRelative=true);
    static QString getClassTypeName(QString classPath);

};

