#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QWidget>
#include <QLayout>
#include "QDialogButtonBox"
#include "QPushButton"
#include "QLabel"
#include "QLineEdit"
#include "QGridLayout"
#include "QComboBox"
#include "projectfilesoperations.h"
#include "QFileInfo"
#include "QCheckBox"
#include "QApplication"
#include "QSettings"
#include "qfiledialog.h"
#include "ProjectConsole/projectconsole.h"


class Turquoise2DEditor;
class NewProjectDialog : public QDialog
{
public:
    NewProjectDialog(Turquoise2DEditor *parent=0);
    Turquoise2DEditor *mainWindow;
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    QWidget *cocos2dxContent;
    QWidget *Cocos2dxContent();
    bool checkForUniqueTargetPath(QString targetPath);
};

#endif // NEWPROJECTDIALOG_H
