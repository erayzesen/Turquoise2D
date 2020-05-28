#pragma once

#include "QList"
#include "QVariantMap"
#include "QTextStream"
#include "QDebug"
#include "QFile"
#include "QRegularExpression"
#include "QString"
#include "QProcess"
#include "QDir"

class Turquoise2DEditor;
class ProjectFilesOperations
{
public:
    ProjectFilesOperations();
    static void addFilesToXcodeProject(QStringList relativeFilePathList,QString relativeParentFolderPath,Turquoise2DEditor *mainWindow);
    static void addFolderToXcodeProject(QString folderPath,QString relativeParentFolderPath,Turquoise2DEditor *mainWindow);
    static void removeFolderFromXcodeProject(QString relativeFolderPath,Turquoise2DEditor *mainWindow);
    static void removeFilesFromXcodeProject(QStringList relativeFilePathList,Turquoise2DEditor *mainWindow);
    static void updateXcodeProjectFiles(Turquoise2DEditor *mainWindow);

    static QString getXcodeFilePath(Turquoise2DEditor *mainWindow);
    static QString getXcodeFilePath(QString projectPath);
    static QString checkDirForFrameworks(QString dir);
    static QStringList getFrameworkPaths(Turquoise2DEditor *mainWindow,QString frameworkName="");
};
