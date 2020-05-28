#pragma once

#include "turquoise2deditor.h"
#include "QWidget"
#include "QTextEdit"
#include "consoleprogressdialog.h"
#include "QTimer"

class ProjectConsole : public QWidget
{
public:
    ProjectConsole();
    static void Run(Turquoise2DEditor *mainWindow);
    static void NewProject(Turquoise2DEditor *mainWindow,QString frameworkName,QString frameworkPath,QString projectName,QString projectPath,bool portrait);
    static QWidget* CreateConsoleWindow(QTextEdit* consoleText,QWidget *parent=0);
};

