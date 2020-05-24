#ifndef COLLECTIONFILESYSTEMMODEL_H
#define COLLECTIONFILESYSTEMMODEL_H
#include "QFileSystemModel"
#include "QDebug"
#include "SpriteSheet/spritesheet.h"

class CollectionFileSystemModel : public QFileSystemModel
{
public:
    CollectionFileSystemModel();
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole )const;
};

#endif // COLLECTIONFILESYSTEMMODEL_H
