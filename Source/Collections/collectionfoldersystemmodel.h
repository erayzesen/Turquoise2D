#pragma once 
#include "QFileSystemModel"
#include "QDebug"
#include "SpriteSheet/spritesheet.h"

class CollectionFolderSystemModel : public QFileSystemModel
{
public:
    CollectionFolderSystemModel();
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole )const;
};
