#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QtWidgets>
#include <QLayout>
#include "FileSelector/fileselectormodel.h"
#include "FileSelector/spritesheetselectormodel.h"
#include "QStandardItem"

class FileSelectorDialog: public QWidget
{
public:
    FileSelectorDialog();
    static QString Sprite(QString targetFolderPath,QPoint pos=QPoint(0,0),QString targetPath="",bool noneOption=false,QWidget *parent=0);
    static QString Class(QString targetFolderPath,QPoint pos=QPoint(0,0),QString targetPath="",bool noneOption=false,QWidget *parent=0);
    static void setExplorerProps(QListView *listview,bool iconMode=true);
};

