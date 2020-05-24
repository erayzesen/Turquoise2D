#ifndef FILESELECTORMODEL_H
#define FILESELECTORMODEL_H
#include "QStandardItemModel"
#include "QObject"
#include "QDirIterator"
#include "QDir"
#include "QDebug"
#include "SpriteSheet/spritesheet.h"
#include "QtAlgorithms"
#include "QFileIconProvider"
#include "QMessageBox"


class FileSelectorModel : public QStandardItemModel
{
public:
    explicit FileSelectorModel(QString type,QString targetFolderPath,bool noneOption=false,QObject *parent=0);
    QStringList CheckNodeClasses(QStringList classFiles);
};

#endif // FILESELECTORMODEL_H
