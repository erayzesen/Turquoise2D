#ifndef COLLECTIONSPRITESHEETMODEL_H
#define COLLECTIONSPRITESHEETMODEL_H
#include "QStandardItemModel"
#include "QStandardItem"
#include "SpriteSheet/spritesheet.h"
#include "QUrl"
#include "QMimeData"

class CollectionSpriteSheetModel: public QStandardItemModel
{
public:
    explicit CollectionSpriteSheetModel(QObject *parent=0);
    void setTarget(QString targetPath);
    QMimeData *mimeData(const QModelIndexList &indexes) const;
};


#endif // COLLECTIONSPRITESHEETMODEL_H
